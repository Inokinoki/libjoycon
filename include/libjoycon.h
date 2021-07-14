#ifndef LIBJOYCON_H
#define LIBJOYCON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "input_report.h"
#include "output_report.h"
#include "features.h"

#pragma pack(push, 1)
struct Header {
    uint8_t command;
    uint8_t counter;
    struct RumbleEncodedData rumble_l;
    struct RumbleEncodedData rumble_r;
};  // 10 bytes

#ifdef __cplusplus
}
#endif

#endif // LIBJOYCON_H
