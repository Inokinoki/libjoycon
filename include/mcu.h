#ifndef MCU_H
#define MCU_H

#include <stdint.h>

#define MCU_SUSPEND 0x00
#define MCU_RESUME  0x01
#define MCU_UPDATE  0x02

#pragma pack(push, 1)
struct MCUCommand {
    uint8_t command;
    uint8_t subcommand;
};

struct MCUIRMode {
    uint8_t mode;
    uint8_t number;
    uint16_t major_version;     // TODO: BE/LE?
    uint16_t minor_version;     // TODO: BE/LE?
};

struct MCURegAddr {
    uint8_t low;
    uint8_t high;
};

struct MCUReg {
    struct MCURegAddr address;  // LE
    uint8_t value;
};

struct MCUWriteRegister {
    uint8_t number;
    struct MCUReg regs[9];
};

struct MCUConfiguration {
    struct MCUCommand command;
    union {
        uint8_t mode;
        struct MCUIRMode ir_mode;
        struct MCUWriteRegister registers;
    };
};
#pragma pack(pop)

// TODO: Add explanations
struct IRImageConfig {
    uint8_t ir_res_reg;
    uint16_t ir_exposure;
    uint8_t ir_leds;
    uint16_t ir_leds_intensity;
    uint8_t ir_digital_gain;
    uint8_t ir_ex_light_filter;
    uint32_t ir_custom_register;
    uint16_t ir_buffer_update_time;
    uint8_t ir_hand_analysis_mode;
    uint8_t ir_hand_analysis_threshold;
    uint32_t ir_denoise;
    uint8_t ir_flip;
};

uint8_t mcu_crc8(uint8_t *buf, uint8_t size);

struct MCURegAddr joycon_mcu_register_addr_encode(uint16_t address);
struct MCUReg joycon_mcu_register_encode(uint16_t address, uint8_t value);
uint16_t joycon_mcu_register_addr_decode(struct MCURegAddr address);

#endif
