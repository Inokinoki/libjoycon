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

struct MCUReg {
    uint16_t address;           // TODO: BE/LE?
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

uint8_t mcu_crc8(uint8_t *buf, uint8_t size);

#endif
