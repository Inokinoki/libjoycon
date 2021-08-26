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

## Usage

Please refer the programs in `examples`.

## Note

Note that this library does not contain Bluetooth/USB HID implementation, you can use one on a specific platform, e.g. hidapi on Linux/Win, to read/send raw packets, and use this library to extract/encode information from/to the packets.

## Have fun

What can it do? Check this twitter:

[IIInoki - Map Joycon to a cube](https://twitter.com/IIInoki/status/1416239484711079938)
