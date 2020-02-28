import {VisualizationSpecMap} from 'components/vega/spec';

export interface ChartPosition {
  x: number;
  y: number;
  w: number;
  h: number;
}

export interface Layout extends ChartPosition {
  i: string;
}

export interface Chart {
  description: string;
  position: ChartPosition;
}

export interface Placement {
  [key: string]: Chart;
}

export const GRID_WIDTH = 10;

// Tiles a grid with the VegaSpec keys.
function layoutDefaultGrid(vegaSpec: VisualizationSpecMap): Placement {
  const placement = {};
  let curX: number = 0;
  let curY: number = 0;
  Object.keys(vegaSpec).forEach((key) => {
    // If we exceed the current width, move to the next row.
    if (curX >= GRID_WIDTH) {
      curX = 0;
      ++curY;
    }
    const pos: ChartPosition = { x: curX, y: curY, w: 1, h: 1 };
    // Create a chart with the default description.
    placement[key] = { description: '', position: pos };
    // Move the next position to the right.
    ++curX;
  });
  return placement;
}

// buildLayout compiles a layout given the inputs. If a placement isn't found for a vega spec,
// generate a new set of placement.
export const buildLayout = (vegaSpecMap: VisualizationSpecMap, placement: Placement): Placement => {
  for (const key of Object.keys(vegaSpecMap)) {
    if (key in placement) {
      continue;
    }
    return layoutDefaultGrid(vegaSpecMap);
  }
  return placement;
};

export function toLayout(placement: Placement) {
  return Object.keys(placement).map((key) => {
    const chart = placement[key];
    return {
      ...chart.position,
      i: key,
    };
  });
}

export function parsePlacement(json: string): Placement {
  try {
    const parsed = JSON.parse(json);
    if (typeof parsed === 'object') {
      return parsed as Placement;
    }
  } catch (e) {
    // noop. tslint doesn't allow empty blocks.
  }
  return {};
}

export function updatePositions(placement: Placement, layouts: Layout[]): Placement {
  const newPlacement = {};
  for (const layout of layouts) {
    const key = layout.i;
    const old = placement[key];
    newPlacement[key] = {
      ...(old || {}),
      position: {
        ...layout,
      },
    };
  }
  return newPlacement;
}
