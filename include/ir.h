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

enum IRResolution {
    FULL,   // 320 x 240
    HALF,   // 160 x 120
    QUAT,   // 80 x 60
    OCTO    // 40 x 30
};

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

#include "mcu.h"

// TODO: Add explanations
struct IRImageConfig {
    uint8_t ir_flip;
    enum IRResolution ir_res_reg;
    uint16_t ir_leds_intensity;

    uint8_t ir_denoise_smooth_enabled;
    uint8_t ir_denoise_threshold;
    uint8_t ir_color_interpol_threshold;

    uint16_t ir_buffer_update_time;

    uint16_t ir_exposure;

    uint8_t ir_leds;

    uint8_t ir_ex_light_filter;

    uint8_t ir_digital_gain;
};

struct MCUReg joycon_ir_flip_encode(uint8_t value);

struct MCUReg joycon_ir_resolution_encode(enum IRResolution res);

struct MCUReg joycon_ir_led_intensity_high_encode(uint16_t intensity);
struct MCUReg joycon_ir_led_intensity_low_encode(uint16_t intensity);

struct MCUReg joycon_ir_enable_denoise_smooth_encode(uint8_t enabled);
struct MCUReg joycon_ir_denoise_smooth_threshold_encode(uint8_t threshold);
struct MCUReg joycon_ir_color_interpol_threshold_encode(uint8_t threshold);

struct MCUReg joycon_ir_buffer_update_time_low_encode(uint8_t time);

struct MCUReg joycon_ir_exposure_time_low_encode(uint16_t time);
struct MCUReg joycon_ir_exposure_time_high_encode(uint16_t time);

struct MCUReg joycon_ir_enable_max_exposure_time_encode(uint8_t enabled);

struct MCUReg joycon_ir_led_group_state_encode(uint8_t state);

struct MCUReg joycon_ir_external_light_filter_encode(uint8_t threshold);

struct MCUReg joycon_ir_white_pixel_threshold_encode(uint8_t threshold);

struct MCUReg joycon_ir_digital_gain_low_encode(uint8_t gain);
struct MCUReg joycon_ir_digital_gain_high_encode(uint8_t gain);

struct MCUReg joycon_ir_finalize_config_encode();

#endif