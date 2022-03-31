# libjoycon [![CMake](https://github.com/Inokinoki/libjoycon/actions/workflows/cmake.yml/badge.svg)](https://github.com/Inokinoki/libjoycon/actions/workflows/cmake.yml)

A Joycon packet encoding and decoding lib

## Build

The library itself only links to the libm (math lib):

```
mkdir build && cd build
cmake ..
make
```

If you want to build examples, the HIDAPI lib is a must. The OpenGL and GLUT/Vulkan is necessary to build IMU Mapping. Configure the project with `BUILD_EXAMPLE` as follows:

```
cmake -DBUILD_EXAMPLE=ON ..
```

### Python bindings

If `swig` is installed, bindings for python will also be built.

To use the Python examples, install `hidapi`(also and `taichi` to run the taichi example) in pip. In Linux, remember to build the `hidapi` manually in order to connect to Joycon through Linux hidraw API:

```
git clone --recursive https://github.com/trezor/cython-hidapi.git
python setup.py build --without-libusb
```

## Usage

Please refer the programs in `examples`.

## Note

Note that this library does not contain Bluetooth/USB HID implementation, you can use one on a specific platform, e.g. hidapi on Linux/Win, to read/send raw packets, and use this library to extract/encode information from/to the packets.

## Having fun

What can it do? Check these tweets:

[IIInoki - Map Joycon to a cube](https://twitter.com/IIInoki/status/1416239484711079938)

[IIInoki - Midi to Joycon rumble](https://mobile.twitter.com/IIInoki/status/1502913550607699969)

## Credits

- [Joycon reverse engineering](https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering) by dekuNukem
- Joy-Con/Joycon is a trademark of Nintendo
