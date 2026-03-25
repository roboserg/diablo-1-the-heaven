I modified ZFS from http://http://users.telenet.be/tfautre/softdev/zfs to provide dynamic and static libraries.

This is only 'ready-to-use' version of zfs. It doesn't containt zfs sources.

To build example one should fix ZLIB_PATH in example/zlib.props and example/zlibstatic.props. It sould point to you zlib.dll, zlib.h and zlibstatic.lib files.

spaun2002