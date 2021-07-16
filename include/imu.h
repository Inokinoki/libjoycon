#ifndef IMU_H
#define IMU_H

#include "output_report.h"

enum IMUTemperatureUnit {
    IMU_TEMP_C,
    IMU_TEMP_F,
    IMU_TEMP_K
};

float joycon_temperature_decode(unsigned char *temp, enum IMUTemperatureUnit unit);

#pragma pack(push, 1)
struct IMUPackedLEUnit {
    uint8_t low;
    uint8_t high;
};

struct IMUPackedData {
    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } accel_x;
    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } accel_y;
    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } accel_z;

    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } gyro_1;
    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } gyro_2;
    union {
        struct IMUPackedLEUnit le_bytes;
        uint8_t value[2];
    } gyro_3;
};

// TODO: Add decoder

#endif