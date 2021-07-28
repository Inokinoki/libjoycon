#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

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
