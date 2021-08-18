#include <mcu.h>
#include <ir.h>
#include <packet.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>

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
        assert(htole16(*reg_p) == (reg.address.high | (reg.address.low << 8)));

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

    // Test jctools - libjoycon compatibility
    {
        uint8_t buf[OUTPUT_REPORT_LEGNTH];
        memset(buf, 0, OUTPUT_REPORT_LEGNTH);
        struct brcm_hdr *hdr = (struct brcm_hdr *)buf;
        struct brcm_cmd_01 *pkt = (struct brcm_cmd_01 *)(hdr + 1);
        struct PacketMCUConf *conf_packet = (struct PacketMCUConf *)buf;
        hdr->cmd = 0x01;
        pkt->subcmd = 0x21;
        assert(conf_packet->header.command == 0x01);
        assert(conf_packet->subcommand == 0x21);

        pkt->subcmd_21_23_04.mcu_cmd    = 0x23; // Write register cmd
        pkt->subcmd_21_23_04.mcu_subcmd = 0x04; // Write register to IR mode subcmd
        pkt->subcmd_21_23_04.no_of_reg  = 0x09; // Number of registers to write. Max 9. 
        assert(conf_packet->conf.command.command == 0x23);
        assert(conf_packet->conf.command.subcommand == 0x04);
        assert(conf_packet->conf.register_conf.registers.number == 0x09);

        pkt->subcmd_21_23_04.reg1_addr  = 0x2e00; // R: 0x002e - Set Resolution based on sensor binning and skipping
        pkt->subcmd_21_23_04.reg1_val   = 0;
        assert(conf_packet->conf.register_conf.registers.regs[0].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[0].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[0].address.low) == IR_RESOLUTION_REG);

        pkt->subcmd_21_23_04.reg2_addr  = 0x3001; // R: 0x0130 - Set Exposure time LSByte - (31200 * us /1000) & 0xFF - Max: 600us, Max encoded: 0x4920.
        pkt->subcmd_21_23_04.reg2_val   = 0x4920 & 0xFF;
        assert(conf_packet->conf.register_conf.registers.regs[1].value == 0x20);
        assert(((conf_packet->conf.register_conf.registers.regs[1].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[1].address.low) == IR_EXPOSURE_TIME_LSB_REG);

        pkt->subcmd_21_23_04.reg3_addr  = 0x3101; // R: 0x0131 - Set Exposure time MSByte - ((31200 * us /1000) & 0xFF00) >> 8
        pkt->subcmd_21_23_04.reg3_val   = (0x4920 & 0xFF00) >> 8;
        assert(conf_packet->conf.register_conf.registers.regs[2].value == 0x49);
        assert(((conf_packet->conf.register_conf.registers.regs[2].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[2].address.low) == IR_EXPOSURE_TIME_MSB_REG);

        pkt->subcmd_21_23_04.reg4_addr  = 0x3201; // R: 0x0132 - Enable Max exposure Time - 0: Manual exposure, 1: Max exposure
        pkt->subcmd_21_23_04.reg4_val   = 0x00;
        assert(conf_packet->conf.register_conf.registers.regs[3].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[3].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[3].address.low) == IR_EXPOSURE_TIME_MAX_REG);
        
        pkt->subcmd_21_23_04.reg5_addr  = 0x1000; // R: 0x0010 - Set IR Leds groups state - Only 3 LSB usable
        pkt->subcmd_21_23_04.reg5_val   = 0b000000;
        assert(conf_packet->conf.register_conf.registers.regs[4].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[4].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[4].address.low) == IR_LED_GROUP_STATE_REG);

        pkt->subcmd_21_23_04.reg6_addr  = 0x2e01; // R: 0x012e - Set digital gain LSB 4 bits of the value - 0-0xff
        pkt->subcmd_21_23_04.reg6_val   = 0xFF;
        assert(conf_packet->conf.register_conf.registers.regs[5].value == 0xFF);
        assert(((conf_packet->conf.register_conf.registers.regs[5].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[5].address.low) == IR_DIGITAL_GAIN_LSB_REG);

        pkt->subcmd_21_23_04.reg7_addr  = 0x2f01; // R: 0x012f - Set digital gain MSB 4 bits of the value - 0-0x7
        pkt->subcmd_21_23_04.reg7_val   = 0x07;
        assert(conf_packet->conf.register_conf.registers.regs[6].value == 0x07);
        assert(((conf_packet->conf.register_conf.registers.regs[6].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[6].address.low) == IR_DIGITAL_GAIN_MSB_REG);

        pkt->subcmd_21_23_04.reg8_addr  = 0x0e00; // R: 0x00e0 - External light filter - LS o bit0: Off/On, bit1: 0x/1x, bit2: ??, bit4,5: ??.
        pkt->subcmd_21_23_04.reg8_val   = 0;
        assert(conf_packet->conf.register_conf.registers.regs[7].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[7].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[7].address.low) == IR_EX_LIGHT_FILTER_REG);

        pkt->subcmd_21_23_04.reg9_addr  = 0x4301; // R: 0x0143 - ExLF/White pixel stats threshold - 200: Default
        pkt->subcmd_21_23_04.reg9_val   = 0xc8;
        assert(conf_packet->conf.register_conf.registers.regs[8].value == 0xc8);
        assert(((conf_packet->conf.register_conf.registers.regs[8].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[8].address.low) == IR_WHITE_PIXEL_THRES_REG);


        memset(buf, 0, OUTPUT_REPORT_LEGNTH);
        hdr->cmd = 0x01;
        pkt->subcmd = 0x21;
        assert(conf_packet->header.command == 0x01);
        assert(conf_packet->subcommand == 0x21);

        pkt->subcmd_21_23_04.mcu_cmd    = 0x23; // Write register cmd
        pkt->subcmd_21_23_04.mcu_subcmd = 0x04; // Write register to IR mode subcmd
        pkt->subcmd_21_23_04.no_of_reg  = 0x08; // Number of registers to write. Max 9.   
        assert(conf_packet->conf.command.command == 0x23);
        assert(conf_packet->conf.command.subcommand == 0x04);
        assert(conf_packet->conf.register_conf.registers.number == 0x08);

        pkt->subcmd_21_23_04.reg1_addr  = 0x1100; // R: 0x0011 - Leds 1/2 Intensity - Max 0x0F.
        pkt->subcmd_21_23_04.reg1_val   = (0x0F) & 0xFF;
        assert(conf_packet->conf.register_conf.registers.regs[0].value == 0x0F);
        assert(((conf_packet->conf.register_conf.registers.regs[0].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[0].address.low) == IR_LED_INTENSITY_12_REG);

        pkt->subcmd_21_23_04.reg2_addr  = 0x1200; // R: 0x0012 - Leds 3/4 Intensity - Max 0x10.
        pkt->subcmd_21_23_04.reg2_val   = 0x10 & 0xFF;
        assert(conf_packet->conf.register_conf.registers.regs[1].value == 0x10);
        assert(((conf_packet->conf.register_conf.registers.regs[1].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[1].address.low) == IR_LED_INTENSITY_34_REG);

        pkt->subcmd_21_23_04.reg3_addr  = 0x2d00; // R: 0x002d - Flip image - 0: Normal, 1: Vertically, 2: Horizontally, 3: Both 
        pkt->subcmd_21_23_04.reg3_val   = 0;
        assert(conf_packet->conf.register_conf.registers.regs[2].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[2].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[2].address.low) == IR_FLIP_IMAGE_REG);

        pkt->subcmd_21_23_04.reg4_addr  = 0x6701; // R: 0x0167 - Enable De-noise smoothing algorithms - 0: Disable, 1: Enable.
        pkt->subcmd_21_23_04.reg4_val   = 0;
        assert(conf_packet->conf.register_conf.registers.regs[3].value == 0x0);
        assert(((conf_packet->conf.register_conf.registers.regs[3].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[3].address.low) == IR_DENOISE_SMOOTH_ALGO_REG);
        
        pkt->subcmd_21_23_04.reg5_addr  = 0x6801; // R: 0x0168 - Edge smoothing threshold - Max 0xFF, Default 0x23
        pkt->subcmd_21_23_04.reg5_val   = 0x23;
        assert(conf_packet->conf.register_conf.registers.regs[4].value == 0x23);
        assert(((conf_packet->conf.register_conf.registers.regs[4].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[4].address.low) == IR_DENOISE_SMOOTH_THRES_REG);

        pkt->subcmd_21_23_04.reg6_addr  = 0x6901; // R: 0x0169 - Color Interpolation threshold - Max 0xFF, Default 0x44
        pkt->subcmd_21_23_04.reg6_val   = 0x44;
        assert(conf_packet->conf.register_conf.registers.regs[5].value == 0x44);
        assert(((conf_packet->conf.register_conf.registers.regs[5].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[5].address.low) == IR_COLOR_INTERPOL_THRES_REG);

        pkt->subcmd_21_23_04.reg7_addr  = 0x0400; // R: 0x0004 - LSB Buffer Update Time - Default 0x32
        pkt->subcmd_21_23_04.reg7_val   = 0x32; // All the other resolutions the default is enough. Otherwise a lower value can break hand analysis.
        assert(conf_packet->conf.register_conf.registers.regs[6].value == 0x32);
        assert(((conf_packet->conf.register_conf.registers.regs[6].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[6].address.low) == IR_UPDATE_TIME_LSB_REG);

        pkt->subcmd_21_23_04.reg8_addr  = 0x0700; // R: 0x0007 - Finalize config - Without this, the register changes do not have any effect.
        pkt->subcmd_21_23_04.reg8_val   = 0x01;
        assert(conf_packet->conf.register_conf.registers.regs[7].value == 0x01);
        assert(((conf_packet->conf.register_conf.registers.regs[7].address.high << 8)
            | conf_packet->conf.register_conf.registers.regs[7].address.low) == IR_APPLY_REG);
    }

    {
        uint8_t buf[OUTPUT_REPORT_LEGNTH];
        // Test status
        memset(buf, 0, sizeof(buf));
        auto hdr = (struct brcm_hdr *)buf;
        auto pkt = (struct brcm_cmd_01 *)(hdr + 1);
        hdr->cmd = 0x01;

        pkt->subcmd = 0x21;
        pkt->subcmd_21_23_01.mcu_cmd     = 0x23;
        pkt->subcmd_21_23_01.mcu_subcmd  = 0x01; // Set IR mode cmd
        pkt->subcmd_21_23_01.mcu_ir_mode = 0x07; // IR mode - 2: No mode/Disable?, 3: Moment, 4: Dpd (Wii-style pointing), 6: Clustering,
                                                 // 7: Image transfer, 8-10: Hand analysis (Silhouette, Image, Silhouette/Image), 0,1/5/10+: Unknown
        pkt->subcmd_21_23_01.no_of_frags = ir_max_frag_no; // Set number of packets to output per buffer
        pkt->subcmd_21_23_01.mcu_major_v = 0x0500; // Set required IR MCU FW v5.18. Major 0x0005.
        pkt->subcmd_21_23_01.mcu_minor_v = 0x1800; // Set required IR MCU FW v5.18. Minor 0x0018.

        buf[48] = mcu_crc8_calc(buf + 12, 36);
    }
    return 0;
}
