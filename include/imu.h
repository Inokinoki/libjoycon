#ifndef IMU_H
#define IMU_H

#include "output_report.h"

enum IMUTemperatureUnit {
    IMU_TEMP_C,
    IMU_TEMP_F,
    IMU_TEMP_K
};

float joycon_temperature_decode(unsigned char *temp, enum IMUTemperatureUnit unit);

#endif