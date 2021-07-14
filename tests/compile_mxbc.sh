#!/bin/bash

gcc test_rumble_mxbc.c ../src/rumble.c -g -lm \
    `pkg-config --cflags hidapi-hidraw hidapi-libusb libudev`\
    `pkg-config --libs hidapi-hidraw hidapi-libusb libudev`\
    -o test_rumble_mxbc.out
