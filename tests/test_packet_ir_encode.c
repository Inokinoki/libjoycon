#include <mcu.h>
#include <ir.h>
#include <packet.h>

#include <stdio.h>
#include <assert.h>

#include <endian.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

#pragma pack(push, 1)
// Structure from jctools
struct brcm_hdr {
    u8 cmd;
    u8 timer;
    u8 rumble_l[4];
    u8 rumble_r[4];
};

struct brcm_cmd_01 {
    u8 subcmd;
    union {
        struct {
            u32 offset;
            u8 size;
        } spi_data;

        struct {
            u8 arg1;
            u8 arg2;
        } subcmd_arg;

        struct {
            u8  mcu_cmd;
            u8  mcu_subcmd;
            u8  mcu_mode;
        } subcmd_21_21;

        struct {
            u8  mcu_cmd;
            u8  mcu_subcmd;
            u8  no_of_reg;
            // Note: bad portability
            u16 reg1_addr;
            u8  reg1_val;
            u16 reg2_addr;
            u8  reg2_val;
            u16 reg3_addr;
            u8  reg3_val;
            u16 reg4_addr;
            u8  reg4_val;
            u16 reg5_addr;
            u8  reg5_val;
            u16 reg6_addr;
            u8  reg6_val;
            u16 reg7_addr;
            u8  reg7_val;
            u16 reg8_addr;
            u8  reg8_val;
            u16 reg9_addr;
            u8  reg9_val;
        } subcmd_21_23_04;

        struct {
            u8  mcu_cmd;
            u8  mcu_subcmd;
            u8  mcu_ir_mode;
            u8  no_of_frags;
            u16 mcu_major_v;
            u16 mcu_minor_v;
        } subcmd_21_23_01;
    };
};
#pragma pack(pop)

int main()
{
    // Test regs: the packet should have a little-endian address
    {
        struct MCUReg reg;
        uint16_t *reg_p = (uint16_t *)&reg.address;
        reg = joycon_ir_finalize_config_encode();
        assert(le16toh(*reg_p) == (reg.address.high << 8 | (reg.address.low)));
        assert(IR_APPLY_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_buffer_update_time_low_encode(0);
        assert(IR_UPDATE_TIME_LSB_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_color_interpol_threshold_encode(0);
        assert(IR_COLOR_INTERPOL_THRES_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_denoise_smooth_threshold_encode(0);
        assert(IR_DENOISE_SMOOTH_THRES_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_digital_gain_high_encode(0);
        assert(IR_DIGITAL_GAIN_MSB_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_digital_gain_low_encode(0);
        assert(IR_DIGITAL_GAIN_LSB_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_enable_denoise_smooth_encode(0);
        assert(IR_DENOISE_SMOOTH_ALGO_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_enable_max_exposure_time_encode(0);
        assert(IR_EXPOSURE_TIME_MAX_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_exposure_time_high_encode(0);
        assert(IR_EXPOSURE_TIME_MSB_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_exposure_time_low_encode(0);
        assert(IR_EXPOSURE_TIME_LSB_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_external_light_filter_encode(0);
        assert(IR_EX_LIGHT_FILTER_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_flip_encode(3); // Both
        assert(IR_FLIP_IMAGE_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_led_group_state_encode(0);
        assert(IR_LED_GROUP_STATE_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_led_intensity_high_encode(0);
        assert(IR_LED_INTENSITY_12_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_led_intensity_low_encode(0);
        assert(IR_LED_INTENSITY_34_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_resolution_encode(FULL);
        assert(IR_RESOLUTION_REG == (reg.address.high << 8 | (reg.address.low)));

        reg = joycon_ir_white_pixel_threshold_encode(0);
        assert(IR_WHITE_PIXEL_THRES_REG == (reg.address.high << 8 | (reg.address.low)));
    }

    // TODO: Test values
    return 0;
}
