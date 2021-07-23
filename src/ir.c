#include <ir.h>

struct MCUReg joycon_ir_flip_encode(uint8_t value)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_FLIP_IMAGE_REG);

    // 0: Normal, 1: Vertically, 2: Horizontally, 3: Both 
    switch (value)
    {
    case 1:case 2:case 3:
        reg.value = value;
        break;
    default:
        reg.value = 0;
        break;
    }
    return reg;
}

struct MCUReg joycon_ir_resolution_encode(enum IRResolution res)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_RESOLUTION_REG);

    switch (res)
    {
    case FULL:
        reg.value = IR_RESOLUTION_FULL;
        break;
    case HALF:
        reg.value = IR_RESOLUTION_HALF;
        break;
    case QUAT:
        reg.value = IR_RESOLUTION_QUAT;
        break;
    case OCTO:
    default:
        reg.value = IR_RESOLUTION_OCTO;
        break;
    }

    return reg;
}

struct MCUReg joycon_ir_led_intensity_high_encode(uint16_t intensity)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_LED_INTENSITY_12_REG);
    reg.value = (intensity >> 8) & 0xFF;
    return reg;
}

struct MCUReg joycon_ir_led_intensity_low_encode(uint16_t intensity)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_LED_INTENSITY_34_REG);
    reg.value = intensity & 0xFF;
    return reg;
}

struct MCUReg joycon_ir_enable_denoise_smooth_encode(uint8_t enabled)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_DENOISE_SMOOTH_ALGO_REG);

    // Enable De-noise smoothing algorithms - 0: Disable, 1: Enable.
    reg.value = enabled ? 1 : 0;
    return reg;
}

struct MCUReg joycon_ir_denoise_smooth_threshold_encode(uint8_t threshold)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_DENOISE_SMOOTH_THRES_REG);

    // Edge smoothing threshold - Max 0xFF, Default 0x23
    reg.value = threshold;
    return reg;
}

struct MCUReg joycon_ir_color_interpol_threshold_encode(uint8_t threshold)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_COLOR_INTERPOL_THRES_REG);

    // Color Interpolation threshold - Max 0xFF, Default 0x44
    reg.value = threshold;
    return reg;
}

struct MCUReg joycon_ir_buffer_update_time_low_encode(uint8_t time)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_UPDATE_TIME_LSB_REG);

    // Default 0x32
    reg.value = time;
    return reg;
}

struct MCUReg joycon_ir_exposure_time_low_encode(uint16_t time)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_EXPOSURE_TIME_LSB_REG);

    reg.value = time & 0xFF;
    return reg;
}

struct MCUReg joycon_ir_exposure_time_high_encode(uint16_t time)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_EXPOSURE_TIME_MSB_REG);

    reg.value = (time & 0xFF00) >> 8;
    return reg;
}

struct MCUReg joycon_ir_enable_max_exposure_time_encode(uint8_t enabled)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_EXPOSURE_TIME_MAX_REG);

    // Enable Max exposure Time - 0: Manual exposure, 1: Max exposure
    reg.value = enabled ? 1 : 0;
    return reg;
}

struct MCUReg joycon_ir_led_group_state_encode(uint8_t state)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_LED_GROUP_STATE_REG);

    // Set IR Leds groups state - Only 3 LSB usable
    reg.value = state;
    return reg;
}

struct MCUReg joycon_ir_external_light_filter_encode(uint8_t threshold)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_EX_LIGHT_FILTER_REG);

    // External light filter - LS o bit0: Off/On, bit1: 0x/1x, bit2: ??, bit4,5: ??.
    reg.value = threshold;
    return reg;
}

struct MCUReg joycon_ir_white_pixel_threshold_encode(uint8_t threshold)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_WHITE_PIXEL_THRES_REG);

    // ExLF/White pixel stats threshold - 200: Default
    reg.value = threshold;
    return reg;
}

struct MCUReg joycon_ir_digital_gain_low_encode(uint8_t gain)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_DIGITAL_GAIN_LSB_REG);

    // Set digital gain LSB 4 bits of the value - 0-0xff
    reg.value = (gain & 0xF) << 4;
    return reg;
}

struct MCUReg joycon_ir_digital_gain_high_encode(uint8_t gain)
{
    struct MCUReg reg;
    reg.address = joycon_mcu_register_addr_encode(IR_DIGITAL_GAIN_MSB_REG);

    // Set digital gain MSB 4 bits of the value - 0-0x7
    reg.value = (gain & 0xF0) >> 4;
    return reg;
}

struct MCUReg joycon_ir_finalize_config_encode()
{
    struct MCUReg reg = {
        .address = joycon_mcu_register_addr_encode(IR_APPLY_REG),
        .value = 0x01
    };
    return reg;
}
