#ifndef IR_H
#define IR_H

// 320 x 240
#define IR_RESOLUTION_FULL              0b00000000
#define IR_RESOLUTION_FULL_NUM_FRAG     0xFF

// 160 x 120
#define IR_RESOLUTION_HALF              0b01010000
#define IR_RESOLUTION_HALF_NUM_FRAG     0x3F

// 80 x 60
#define IR_RESOLUTION_QUAT              0b01100100
#define IR_RESOLUTION_QUAT_NUM_FRAG     0x0F

// 40 x 30
#define IR_RESOLUTION_OCTO              0b01101001;
#define IR_RESOLUTION_OCTO_NUM_FRAG     0x03;

/* IR related MCU commands */
#define IR_UPDATE_TIME_LSB_REG      0x0004      // LSB Buffer Update Time - Default 0x32

#define IR_APPLY_REG                0x0007

#define IR_EX_LIGHT_FILTER_REG      0x000e      // External light filter - LS o bit0: Off/On, bit1: 0x/1x, bit2: ??, bit4,5: ??.

#define IR_LED_GROUP_STATE_REG      0x0010      // Only 3 LSB usable

#define IR_LED_INTENSITY_12_REG     0x0011      // Leds 1/2 Intensity - Max 0x0F.
#define IR_LED_INTENSITY_34_REG     0x0012      // Leds 1/2 Intensity - Max 0x0F.

#define IR_FLIP_IMAGE_REG           0x002d      // Flip image - 0: Normal, 1: Vertically, 2: Horizontally, 3: Both 

#define IR_RESOLUTION_REG           0x002e

#define IR_DIGITAL_GAIN_LSB_REG     0x012e      // Set digital gain LSB 4 bits of the value - 0-0xff
#define IR_DIGITAL_GAIN_MSB_REG     0x012f      // Set digital gain MSB 4 bits of the value - 0-0x7

#define IR_EXPOSURE_TIME_LSB_REG    0x0130      // Set Exposure time LSByte - (31200 * us /1000) & 0xFF - Max: 600us, Max encoded: 0x4920.
#define IR_EXPOSURE_TIME_MSB_REG    0x0131      // Set Exposure time MSByte - ((31200 * us /1000) & 0xFF00) >> 8
#define IR_EXPOSURE_TIME_MAX_REG    0x0132      // Enable Max exposure Time - 0: Manual exposure, 1: Max exposure

#define IR_WHITE_PIXEL_THRES_REG    0x1043      // ExLF/White pixel stats threshold - Default: 200

#define IR_DENOISE_SMOOTH_ALGO_REG  0x1067      // Enable De-noise smoothing algorithms - 0: Disable, 1: Enable.
#define IR_DENOISE_SMOOTH_THRES_REG 0x1068      // Edge smoothing threshold - Max 0xFF, Default 0x23
#define IR_COLOR_INTERPOL_THRES_REG 0x1069      // Color Interpolation threshold - Max 0xFF, Default 0x44

#endif