import * as QueryString from 'query-string';
import { OAUTH_PROVIDER } from 'containers/constants';
import { HydraClient } from './hydra-oauth-provider';
import { Auth0Client } from './auth0-oauth-provider';

export type AuthCallbackMode = 'cli_get' | 'cli_token' | 'ui';

interface RedirectArgs {
  mode?: AuthCallbackMode;
  location?: string;
  org_name?: string;
  signup?: boolean;
  redirect_uri?: string;
}

const getRedirectURL = (isSignup: boolean) => {
  // Translate the old API parameters to new versions. In paricular:
  // local, (no redirect_url) -> cli_token
  // local -> cli_get
  // default: ui
  // We also translate the location parameters so redirects work as expected.
  // TODO(zasgar/michelle): When we finish porting everything to the new API this code
  // can be simplified.

  const redirectArgs: RedirectArgs = {};
  const parsed = QueryString.parse(window.location.search.substring(1));
  if (parsed.local_mode && !!parsed.local_mode) {
    if (parsed.redirect_uri) {
      // eslint-disable-next-line @typescript-eslint/camelcase
      redirectArgs.redirect_uri = typeof parsed.redirect_uri === 'string' && String(parsed.redirect_uri);
      redirectArgs.mode = 'cli_get';
    } else {
      redirectArgs.mode = 'cli_token';
    }
  } else {
    if (parsed.redirect_uri && typeof parsed.redirect_uri === 'string') {
      // eslint-disable-next-line @typescript-eslint/camelcase
      redirectArgs.redirect_uri = String(parsed.redirect_uri);
    }
    redirectArgs.mode = 'ui';
  }

  if (parsed.location && typeof parsed.location === 'string') {
    redirectArgs.location = parsed.location;
  }
  if (parsed.org_name && typeof parsed.org_name === 'string') {
    // eslint-disable-next-line @typescript-eslint/camelcase
    redirectArgs.org_name = parsed.org_name;
  }
  if (isSignup) {
    redirectArgs.signup = true;
  }
  const qs = QueryString.stringify(redirectArgs as any);
  const redirectURL = `${window.location.origin}/auth/callback?${qs}`;

  const segmentId = typeof parsed.tid === 'string' ? parsed.tid : '';
  if (segmentId) {
    analytics.alias(segmentId);
  }
  return redirectURL;
};

export const GetOAuthProvider = () => {
  if (OAUTH_PROVIDER === 'auth0') {
    return new Auth0Client(getRedirectURL);
  }
  if (OAUTH_PROVIDER === 'hydra') {
    return new HydraClient(getRedirectURL);
  }
  throw new Error(`OAUTH_PROVIDER ${OAUTH_PROVIDER} invalid. Expected hydra or auth0.`);
};

export const OAuthSignupRequest = () => GetOAuthProvider().signupRequest();
export const OAuthLoginRequest = () => GetOAuthProvider().loginRequest();
