// Code generated for package schema by go-bindata DO NOT EDIT. (@generated)
// sources:
// schema.graphql
package schema

import (
	"bytes"
	"compress/gzip"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
	"time"
)

func bindataRead(data []byte, name string) ([]byte, error) {
	gz, err := gzip.NewReader(bytes.NewBuffer(data))
	if err != nil {
		return nil, fmt.Errorf("Read %q: %v", name, err)
	}

	var buf bytes.Buffer
	_, err = io.Copy(&buf, gz)
	clErr := gz.Close()

	if err != nil {
		return nil, fmt.Errorf("Read %q: %v", name, err)
	}
	if clErr != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}

type asset struct {
	bytes []byte
	info  os.FileInfo
}

type bindataFileInfo struct {
	name    string
	size    int64
	mode    os.FileMode
	modTime time.Time
}

// Name return file name
func (fi bindataFileInfo) Name() string {
	return fi.name
}

// Size return file size
func (fi bindataFileInfo) Size() int64 {
	return fi.size
}

// Mode return file mode
func (fi bindataFileInfo) Mode() os.FileMode {
	return fi.mode
}

// Mode return file modify time
func (fi bindataFileInfo) ModTime() time.Time {
	return fi.modTime
}

// IsDir return file whether a directory
func (fi bindataFileInfo) IsDir() bool {
	return fi.mode&os.ModeDir != 0
}

// Sys return file is sys mode
func (fi bindataFileInfo) Sys() interface{} {
	return nil
}

var _schemaGraphql = []byte("\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\xff\xb4\x58\x41\x6f\xeb\xb8\x11\xbe\xeb\x57\x4c\x36\x87\x4d\x80\xc4\x87\xa2\xdd\x83\x4f\x55\x6d\xbf\x7d\x6a\x12\xc5\x8d\x9d\xbc\x6e\x17\x41\x40\x8b\x63\x8b\x88\x44\xea\x91\x94\x13\xb7\x78\xff\xbd\x18\x92\x92\x45\xdb\xd9\xc5\x5b\x60\x4f\x16\xc9\xe1\xcc\xc7\x99\xf9\x86\x43\x9f\xc3\xb2\x14\x06\xd6\xa2\x42\xe0\x68\x0a\x2d\x56\x68\xc0\x96\x08\xa6\x28\xb1\x66\xb0\xd6\xaa\x76\xe3\x74\x9e\x81\x41\xbd\x15\x05\x8e\x92\xf3\xe4\x1c\x32\xfb\xa3\x01\xa9\x2c\x08\x8e\xac\xba\x82\x55\x6b\xe1\x0d\x41\x22\x72\xb0\x0a\x6a\x26\x5b\x56\x55\x3b\xd8\xa0\x44\xcd\x2c\x82\xdd\x35\x68\x60\xad\xb4\xd3\xb7\xdc\x35\xb8\x28\xb4\x68\x2c\x3c\x66\xc9\x39\xbc\x95\x28\xc1\xf6\x60\x84\x81\xb6\xe1\xcc\x22\x1f\x79\x88\x05\x93\xb0\x42\xe0\x4a\x22\xac\x76\xa0\x5b\x29\x85\xdc\x8c\x93\x73\x80\x8d\x66\x4d\xf9\xb5\xba\xf6\x90\xaf\x9d\x1d\xaf\xb9\xb3\x7d\x6d\x4d\x38\xd0\x28\x08\xc3\xf5\xb5\x6a\x6d\xd3\xda\x6e\x9e\x8f\xac\x71\x30\x44\x51\xc2\x9b\xa8\xaa\x01\xf0\x12\x21\x08\x93\x6e\x0f\xd0\x96\xcc\x7a\xb9\x15\x42\x23\x8a\x57\xe4\xd0\x36\x04\x8d\xc4\x1f\xb3\x51\x12\x7c\x3b\xd0\xef\x76\x1a\x30\xa5\x6a\x2b\x0e\xf8\x2e\x8c\x05\x21\xbd\xbb\x59\x8d\xc0\x85\xc6\xc2\x2a\xbd\x03\x36\x0c\x42\x8f\x99\xb6\x8f\x92\x24\x84\xe6\x7f\x09\xc0\xd7\x16\xf5\x6e\x0c\xff\xa2\x9f\x04\xa0\x6e\x2d\xb3\x42\xc9\x31\xdc\x85\xaf\xe4\x5b\x92\x38\xd0\x8f\x06\x75\x26\xd7\xca\x6d\x13\x7c\x0c\xd9\xf4\x2c\x01\x90\xac\xc6\x31\x2c\xac\x16\x72\x43\x63\xac\x99\xa8\x86\x13\x8d\x28\x6c\xab\x23\x19\xa5\x37\x79\xb4\xed\x5b\x92\xa0\x6c\x6b\x48\xb5\x15\x6b\x56\x58\x8a\xad\xb3\x03\x90\x2e\x5f\x1e\xf3\x9b\xfc\xfe\x4b\xde\x0d\x6f\xb3\xfc\xf1\xdf\x2f\xe9\xdd\xf4\xa7\xbf\x76\x53\xd3\xf4\xe1\x4b\x96\xc7\x73\x93\xfb\x7c\x99\x66\xf9\xec\xe1\x65\x31\x5b\xbe\xfc\x92\xde\xdd\x2e\x4e\x2f\x0d\xf5\xf5\x40\x5a\xab\x0a\x55\x37\x15\x5a\x9c\x49\x2b\xec\x6e\x61\x29\x92\x84\x29\x9d\x2d\x06\x90\x68\x34\x9f\xe5\xd3\x2c\xff\x39\x8c\x1e\x1e\xf3\x7c\x3f\xfa\x94\x66\xb7\xb3\x69\x18\x2c\x67\x0f\x77\x59\x9e\x2e\x67\xd3\x93\x96\xd2\x82\x3c\xde\x1f\x3e\x8d\xce\x7e\x0e\xa9\x04\xe4\xc2\x02\x73\x62\xa0\x8a\xa2\xd5\x06\xc4\x1a\x18\xb4\x06\x35\x94\xcc\x40\xad\xb8\x58\x0b\x62\x50\x89\x20\xa4\x4b\x39\x7c\xb7\x94\x56\x42\x1a\xd4\x56\xc8\x0d\x28\x0d\x1c\x2b\x74\xdf\x45\xc9\x34\x2b\x2c\x6a\x33\x72\x46\x5c\xca\x09\x59\x54\x2d\x27\x22\xef\x1a\xb7\xc1\xe7\xd8\x2b\xee\x56\x8a\x69\x0e\x4c\x72\x68\x98\xf1\x0a\x54\x5d\x33\xc9\xdd\x76\x42\x3c\x9b\x66\x4b\x0f\x17\x0c\x56\x58\xec\xf1\xca\x6a\x77\x1a\x74\x51\x2a\x83\x12\x98\x04\x36\xf0\x06\x98\x76\xb3\x41\x43\x7b\x47\x1d\x2c\x2e\x0a\x66\x09\x97\x72\x26\x08\x54\xb4\xc5\x91\x4a\xd8\x8e\x21\xb5\xda\x7a\xf6\x91\xa9\x1f\x0d\x90\x6d\x2a\x1f\xca\x4d\x4a\x72\x0c\x6b\x1a\xad\x1a\x2d\x1c\x4f\xd9\xaa\x3b\xc5\x62\x76\x3b\x9b\x2c\x7f\x23\x1f\x6e\x84\xe4\x21\x1d\x6e\xa2\x74\xb8\x79\x99\xdf\x4f\xc3\xd7\xe2\x69\xd2\x7d\x4d\x1e\xb2\xf9\x32\x0c\xf2\xf4\x6e\xb6\x98\xa7\x93\x59\x4f\xae\x29\x36\x95\xda\xd5\x28\xed\x0d\xee\x0e\x18\xf6\x8a\xbb\x21\x79\x0a\x8d\x54\xd5\x52\x7b\x67\xc6\xf0\xa9\x52\xcc\xd2\x2c\xd5\xde\x88\x4f\x4e\xad\xa3\xb5\x53\x47\x0e\x18\xf7\x1c\x3e\xf3\xa1\xbe\x9f\xde\x5f\x50\x52\x68\x21\xd5\xe5\x18\xee\xd8\x2b\x42\x36\x05\x8d\x5f\x5b\xa1\x91\x83\x92\x05\x55\x22\xe7\x46\x03\x6a\x8b\xce\x75\x75\x5b\x59\x71\x5d\x54\xad\xb1\xa8\xc1\xb4\x4d\xa3\xb4\x25\xbf\x85\xa9\x0b\x0f\xfd\x72\x0c\x13\x3f\xd1\x59\x0c\xeb\x66\x0c\xbf\x0e\x57\x9e\xff\x54\x34\x13\x25\x25\xba\x04\x3c\xc2\xb5\x5f\xda\x23\x14\x5d\x09\xba\x60\x83\x5a\x34\x8e\x2a\x13\x69\xb8\xcd\xba\x19\xda\xd7\xc9\x9a\x7e\xd7\xb0\xbe\x5d\xee\xb7\x9b\xce\xd2\x30\x6b\x2f\x1c\x4f\x3b\xe9\xab\x90\xa5\x73\x65\xc6\x90\x49\x7b\x15\xf8\x33\xfe\xa0\x54\x5c\x75\x27\x7d\xcc\xa6\x43\x8b\x03\xe1\x07\x34\x6d\x65\x0f\xcd\x7e\x12\x58\xf1\x43\xdb\x6b\x9a\x0c\x47\x3e\x99\xf3\x57\xae\x88\x76\x41\x49\xf5\x86\x84\x29\xa4\xa7\xc5\x9f\x4f\xc3\x8b\xa4\x17\x3d\xcf\x9f\x13\x97\x0a\xfe\x5a\xaf\x37\x1a\x50\xf2\x46\x09\x69\xcd\x15\x68\x5c\xfb\x88\x73\x55\x50\x29\x80\xa2\x52\x2d\x67\x8d\x18\x35\x5a\xb9\x7a\x50\x89\x2d\x3e\x09\x7c\x23\x34\xb7\xe1\xfb\x0e\x2d\xe3\xcc\x32\x9f\x65\x9d\xc4\x44\x49\x8b\xd2\x9a\x90\x12\x67\x97\x63\xb8\x3d\x58\x22\x71\xdf\x04\x90\x3a\x8f\x28\x56\xe6\x57\x4f\xa8\x5a\x44\x0b\x67\xfe\x4c\x9e\xe1\x44\x66\xe3\xe8\x3a\xe0\x3b\x19\x88\x0a\x80\xd7\x1f\xc9\x0c\xd4\xc7\xa2\x3d\xd7\x8f\x03\xee\x88\x4f\x15\x1d\xa9\x69\xaa\x99\xb5\xc8\xc3\x9d\x20\xcc\xe0\x82\x30\x21\xf6\xbe\x75\xa1\x82\xbc\x42\x94\xd0\x30\x6d\x90\x77\x0d\x49\x5c\x66\x55\x5f\x8b\x7d\x1d\x66\xab\x85\x55\x0d\x34\xca\x08\x8a\xa3\xbb\x0c\x7a\x9b\xd9\x30\xc5\x9c\xfc\x97\x12\x6d\x89\xfa\x08\x03\xe1\x62\xb0\x65\x95\xe0\x57\x80\xef\x58\xb4\x96\xad\x2a\xec\xee\x18\xd2\x2a\xcc\xac\x9f\x1f\xc3\x3f\x94\xaa\x90\x49\x7f\xdf\x54\xd5\xe0\xca\xf0\x8d\x22\xb2\xa2\x04\xb5\x76\x86\x02\x48\x87\x8d\xbe\xf7\xa2\x63\xf8\x75\x39\x9c\x78\xee\x9d\x1a\x4d\x0f\xfc\x29\x24\xc7\xf7\x81\x62\x7f\xf1\xd8\x12\x0d\x46\x18\x98\x76\xbe\x0f\x26\x33\xda\xe5\x48\x1d\x79\xc1\x5f\x93\x74\x7c\x36\xd8\x1c\x1a\x5d\x8a\x14\x5b\x05\x83\xae\x5d\xac\xa9\x30\x92\xdd\xe0\x95\x81\xa3\xc8\xce\x7e\x94\xae\x2d\xea\x85\x53\x3e\xf4\x94\x89\x0e\xfe\x11\x11\x4f\xa5\xd5\x81\x2b\x5e\x85\xe4\x1f\x95\x89\x83\xce\x30\xdc\x50\xc4\x0b\x57\xca\xfa\xd9\x9a\xd9\xa2\xa4\x14\xe1\xf8\xee\xca\x48\x26\xed\x33\x81\xa4\x76\xeb\x94\x72\xd7\x87\xf5\xf7\x72\xa8\xe4\x34\xd9\x9a\x41\x7c\x38\xae\x19\x31\xc0\xa9\xa1\x6e\x43\x2a\x5b\x86\x04\x7b\x95\xea\x4d\x92\xa7\x9e\xfe\xf3\xb2\x88\x3b\x2c\xda\x1a\xb6\x18\x28\x91\x55\xb6\xdc\xd1\xee\x12\x99\xb6\x2b\x64\xd6\x47\x54\x63\x81\x62\xeb\xae\x24\xd0\xb8\x69\x2b\xa6\x41\x48\x8b\x7a\xcb\x2a\xe3\x9a\x23\x5b\x7a\x62\x74\xf7\x92\x30\xa0\xd1\x34\x4a\x72\x02\x61\x95\x2b\xa0\x68\xac\xd9\xe3\xf8\x3c\x4b\x6f\x97\x9f\x7f\x39\xc0\xe1\x1f\x12\xca\xd5\x3d\x61\x0a\x7f\x63\x11\x8d\x7d\xea\xfd\xfc\x30\x9f\x40\xd1\xdf\x63\xb0\xd2\xc8\x5e\xcd\xc8\x29\x28\x55\x83\x9e\xe8\xcc\xf6\xdd\x52\x07\xc8\xe9\x2d\x54\x8d\xb0\x62\xc5\x2b\xf5\x66\x42\xa2\x83\xae\xd1\xb4\x35\x65\x38\x04\x44\x1e\xc9\x1e\xe8\x34\x5b\x4c\xee\xf3\x7c\x36\x59\xba\xbe\xf6\xc0\x6b\xee\xd1\x45\x87\x0c\xef\x31\x1c\xfa\x20\xbc\x55\x1a\xad\x0a\x34\x86\xf8\xd3\x89\x0f\xe2\x31\x9f\xa6\x4b\xdf\x3f\x7b\xd5\x5b\xf1\x5f\xd1\x35\x8a\xdd\xf9\xfd\x6b\x91\xa6\xe8\x01\x69\x50\x5a\x60\x72\x07\xca\xf1\x69\xdd\x6a\xcf\x2b\x9f\x15\xfe\x19\x68\x80\xad\x54\xeb\x1d\xf1\x16\x88\x27\xec\x30\xce\x4a\x9f\x40\x73\x7c\xd2\x00\xe7\x8d\x5e\x58\x7a\x17\xc2\xe9\x6d\x78\x54\x6b\x26\x2a\xf4\x3d\xb2\x20\x7c\x6f\x74\x6c\x06\x2b\xc6\x0f\x3d\xe9\x8e\x3a\xeb\x9e\x08\x1d\xe3\x9e\x9c\x81\x89\x92\x6b\xb1\x71\x29\xdd\x30\x63\x6c\xa9\x55\xbb\x29\x67\x92\x88\xcd\xf7\x74\xee\x36\xd1\x6d\xc3\x84\x8c\xa8\x70\xf8\x34\x3b\xdd\x39\x06\xa6\xed\xc5\x6a\x34\x86\x6d\x86\xd4\xd5\xc8\xcc\x80\xb5\x9d\xcd\xb9\xe2\x7f\xc8\x5a\x6b\xbe\xcb\x1c\x50\xd8\xfd\xe1\x5c\xeb\x18\x9f\x94\xee\xca\xde\x09\xfb\xae\xf2\xa0\x87\xee\xcc\x46\xf5\xc2\x75\x04\xcc\xd8\xcf\x1d\xbb\x23\xa4\xdb\x41\x18\xc6\x51\x50\xf6\xab\x4f\xa8\x4d\x5c\xcf\x42\xb2\x7f\xb8\x90\xc7\x45\xb1\xd1\x68\xed\x6e\x72\x72\xed\xb8\x6f\x0a\xae\xd0\xaa\x9a\x57\x4c\x62\xef\x7f\x57\x38\xfb\x91\xef\x1e\x64\x5b\xe7\x8a\xa3\xef\x21\xc3\x44\x26\x8d\xd5\x2d\x75\x0e\xc8\x87\x8b\x07\xfe\x8b\xfb\x62\xef\xc9\x26\xe5\x5c\xa3\x89\x22\x67\xd5\x2b\xca\xe3\x47\x47\xf7\xcf\x81\xdb\x38\x71\x59\x10\x14\x47\x2f\x02\xf8\x3b\xc7\x46\x23\xf5\x10\xfc\xa2\x0b\xf9\x0f\x41\xc0\xd7\x59\xe2\x4e\x48\x23\xd8\x0a\x06\xcd\x7b\x68\x8a\x7e\xb8\x4c\x00\x1e\x1d\xe7\x86\x81\xb9\x08\x2e\x23\x8f\x65\xd3\xb3\xab\xdf\x62\xce\x65\xff\x75\xd6\xc3\x8c\x3a\xab\xa3\x46\x0b\x60\x4a\x8f\xe6\x58\x6a\xd0\x97\xf5\xea\x7a\x77\xee\x1f\x09\xe1\xcf\x8c\x56\x47\x7f\x8d\x00\x98\x92\xfd\xe5\x6f\x3f\x1d\xfb\x30\x7a\x2f\xf8\x08\x58\xac\xdd\x7d\x1d\x56\x9e\x8f\x64\x9d\xd8\x36\x4e\x3c\x47\xc4\x92\xc9\x0d\x56\x6a\x13\xc5\x4e\xd4\x68\x2c\xab\x9b\x41\xce\x7f\x4b\x92\x73\x78\xf8\x9d\x36\xdb\x99\x3c\xec\xae\x7f\xe7\x3f\xa1\xa3\xc7\xe9\x77\x9a\xe9\x5a\x69\x67\xa6\x0e\x36\xc7\x47\x28\xdc\xbf\x4d\xef\x55\x27\x3d\x44\xb0\x15\xe6\x9f\x8b\xfb\xfc\x8f\x80\x88\x5b\xff\xef\x3a\x29\xd0\x0d\xd5\xa1\x8c\x13\xe4\xbb\x8c\x7f\x70\xfe\x83\x47\x49\xa8\x0e\xf1\xd1\xbf\x25\xff\x0f\x00\x00\xff\xff\x11\xfa\x99\x13\xa2\x15\x00\x00")

func schemaGraphqlBytes() ([]byte, error) {
	return bindataRead(
		_schemaGraphql,
		"schema.graphql",
	)
}

func schemaGraphql() (*asset, error) {
	bytes, err := schemaGraphqlBytes()
	if err != nil {
		return nil, err
	}

	info := bindataFileInfo{name: "schema.graphql", size: 5538, mode: os.FileMode(436), modTime: time.Unix(1598381712, 0)}
	a := &asset{bytes: bytes, info: info}
	return a, nil
}

// Asset loads and returns the asset for the given name.
// It returns an error if the asset could not be found or
// could not be loaded.
func Asset(name string) ([]byte, error) {
	cannonicalName := strings.Replace(name, "\\", "/", -1)
	if f, ok := _bindata[cannonicalName]; ok {
		a, err := f()
		if err != nil {
			return nil, fmt.Errorf("Asset %s can't read by error: %v", name, err)
		}
		return a.bytes, nil
	}
	return nil, fmt.Errorf("Asset %s not found", name)
}

// MustAsset is like Asset but panics when Asset would return an error.
// It simplifies safe initialization of global variables.
func MustAsset(name string) []byte {
	a, err := Asset(name)
	if err != nil {
		panic("asset: Asset(" + name + "): " + err.Error())
	}

	return a
}

// AssetInfo loads and returns the asset info for the given name.
// It returns an error if the asset could not be found or
// could not be loaded.
func AssetInfo(name string) (os.FileInfo, error) {
	cannonicalName := strings.Replace(name, "\\", "/", -1)
	if f, ok := _bindata[cannonicalName]; ok {
		a, err := f()
		if err != nil {
			return nil, fmt.Errorf("AssetInfo %s can't read by error: %v", name, err)
		}
		return a.info, nil
	}
	return nil, fmt.Errorf("AssetInfo %s not found", name)
}

// AssetNames returns the names of the assets.
func AssetNames() []string {
	names := make([]string, 0, len(_bindata))
	for name := range _bindata {
		names = append(names, name)
	}
	return names
}

// _bindata is a table, holding each asset generator, mapped to its name.
var _bindata = map[string]func() (*asset, error){
	"schema.graphql": schemaGraphql,
}

// AssetDir returns the file names below a certain
// directory embedded in the file by go-bindata.
// For example if you run go-bindata on data/... and data contains the
// following hierarchy:
//     data/
//       foo.txt
//       img/
//         a.png
//         b.png
// then AssetDir("data") would return []string{"foo.txt", "img"}
// AssetDir("data/img") would return []string{"a.png", "b.png"}
// AssetDir("foo.txt") and AssetDir("notexist") would return an error
// AssetDir("") will return []string{"data"}.
func AssetDir(name string) ([]string, error) {
	node := _bintree
	if len(name) != 0 {
		cannonicalName := strings.Replace(name, "\\", "/", -1)
		pathList := strings.Split(cannonicalName, "/")
		for _, p := range pathList {
			node = node.Children[p]
			if node == nil {
				return nil, fmt.Errorf("Asset %s not found", name)
			}
		}
	}
	if node.Func != nil {
		return nil, fmt.Errorf("Asset %s not found", name)
	}
	rv := make([]string, 0, len(node.Children))
	for childName := range node.Children {
		rv = append(rv, childName)
	}
	return rv, nil
}

type bintree struct {
	Func     func() (*asset, error)
	Children map[string]*bintree
}

var _bintree = &bintree{nil, map[string]*bintree{
	"schema.graphql": &bintree{schemaGraphql, map[string]*bintree{}},
}}

// RestoreAsset restores an asset under the given directory
func RestoreAsset(dir, name string) error {
	data, err := Asset(name)
	if err != nil {
		return err
	}
	info, err := AssetInfo(name)
	if err != nil {
		return err
	}
	err = os.MkdirAll(_filePath(dir, filepath.Dir(name)), os.FileMode(0755))
	if err != nil {
		return err
	}
	err = ioutil.WriteFile(_filePath(dir, name), data, info.Mode())
	if err != nil {
		return err
	}
	err = os.Chtimes(_filePath(dir, name), info.ModTime(), info.ModTime())
	if err != nil {
		return err
	}
	return nil
}

// RestoreAssets restores an asset under the given directory recursively
func RestoreAssets(dir, name string) error {
	children, err := AssetDir(name)
	// File
	if err != nil {
		return RestoreAsset(dir, name)
	}
	// Dir
	for _, child := range children {
		err = RestoreAssets(dir, filepath.Join(name, child))
		if err != nil {
			return err
		}
	}
	return nil
}

func _filePath(dir, name string) string {
	cannonicalName := strings.Replace(name, "\\", "/", -1)
	return filepath.Join(append([]string{dir}, strings.Split(cannonicalName, "/")...)...)
}
