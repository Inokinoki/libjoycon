#ifndef IMU_H
#define IMU_H

#include "types.h"

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
#pragma pack(pop)

// Add decoder
#define SENSITIVITY_ACCELEROMETER_2  0.000061f
#define SENSITIVITY_ACCELEROMETER_4  0.000122f
#define SENSITIVITY_ACCELEROMETER_8  0.000244f
#define SENSITIVITY_ACCELEROMETER_16 0.000732f
#define SENSITIVITY_GYROSCOPE_245    0.00875f
#define SENSITIVITY_GYROSCOPE_500    0.0175f
#define SENSITIVITY_GYROSCOPE_2000   0.07f

enum IMUSensitivity {
    Sensitivity0 = 0x00,
    Sensitivity1,
    Sensitivity2,
    Sensitivity3
};

float joycon_gyro_decode(int16_t data, enum IMUSensitivity sensitivity);
float joycon_accel_decode(int16_t data, enum IMUSensitivity sensitivity);

float joycon_gyro_raw_decode(struct IMUPackedLEUnit unit, enum IMUSensitivity sensitivity);
float joycon_accel_raw_decode(struct IMUPackedLEUnit unit, enum IMUSensitivity sensitivity);

#endif