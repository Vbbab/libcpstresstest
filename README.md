# libcpstresstest
Simple stress-testing library for cp

## Building
**NOTE:** This library is only meant to be used on Linux distributions! It has been tested on Ubuntu 20.04.

1. Install `libexplain` dependency (used for error logging):
```
sudo apt-get install libexplain-dev
```

2. Build with CMake:
```sh
cd 'PATH_TO_LIBCPSTRESSTEST (the very same folder that contains this README!)'
mkdir build
cd build
cmake ..
make
```

3. If everything went successfully, a static `libcpstresstest.a` should be built that you can then link with your tests!
