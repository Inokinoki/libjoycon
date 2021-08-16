#ifndef MCU_H
#define MCU_H

#include "types.h"

#define MCU_SUSPEND 0x00
#define MCU_RESUME  0x01
#define MCU_UPDATE  0x02

#pragma pack(push, 1)
struct MCUCommand {
    uint8_t command;
    uint8_t subcommand;
};

struct MCUIRVersion {
    uint8_t low;
    uint8_t high;
};

struct MCUIRMode {
    uint8_t mode;
    uint8_t number;
    struct MCUIRVersion major_version;
    struct MCUIRVersion minor_version;
};

struct MCURegAddr {
    uint8_t high;
    uint8_t low;
};

struct MCUReg {
    struct MCURegAddr address;  // LE
    uint8_t value;
};

struct MCUWriteRegister {
    uint8_t number;
    struct MCUReg regs[9];
};

#define MCU_CONF_LEN 36
#define MCU_CRC_LEN 1
struct MCUConfiguration {
    struct MCUCommand command;
    union {
        struct {
            uint8_t req_index;
            uint8_t ack_index;
        } comm_conf;
        struct {
            uint8_t mode;
            uint8_t padding[MCU_CONF_LEN - 1 - sizeof(struct MCUCommand)];
        } mode_conf;
        struct {
            struct MCUIRMode ir_mode;
        } ir_conf;
        struct {
            struct MCUWriteRegister registers;
        } register_conf;
    };
};
#pragma pack(pop)

enum MCUMode {
    MCUMode_STANDBY = 0x01,
    MCUMode_NFC     = 0x04,
    MCUMode_IR      = 0x05
};

uint8_t mcu_crc8(uint8_t *buf, uint8_t size);

struct MCURegAddr joycon_mcu_register_addr_encode(uint16_t address);
struct MCUReg joycon_mcu_register_encode(uint16_t address, uint8_t value);
uint16_t joycon_mcu_register_addr_decode(struct MCURegAddr address);

#endif
