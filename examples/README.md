# Examples of libjoycon

These examples are using [HIDAPI](https://github.com/libusb/hidapi) to communicate with Joycon. You will need to pair and connect your Joycon with Bluetooth:

1. Open Bluetooth
2. Press and hold the button between `SL` and `SR` to make Joycon discoverable
3. Actively pair and connect

## Hello World

This example connects to the Joycon and do nothing.

## Get color

This example gets the colors of your Joycon, including the button color and the grip color.

## Player Light

This example controls the four lights on the left/right side of Joycon.

## IMU Mapping

This example receives IMU data from Joycon, and maps the x-axis of the gyroscope to a 3D cube.

## Play MXBC

This example sends the rumble data to Joycon, in order to play a piece of music.
