#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define OUTPUT_REPORT_LEGNTH 64
#define INPUT_REPORT_STANDARD_LEGNTH 64
#define INPUT_REPORT_MCUIR_LENGTH 0x170

#pragma pack(push, 1)
struct SubcommandBodySPIData {
    uint32_t offset;
    uint8_t size;
};

struct RumbleEncodedData {
    uint8_t data[4];
};
#pragma pack(pop)

#endif
