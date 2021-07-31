#include <imu.h>
#include <output_report.h>

float joycon_temperature_decode(unsigned char *temp_info, enum IMUTemperatureUnit unit)
{
    // Convert reading to Celsius according to datasheet
    uint16_t temp = (temp_info[1] << 8 | temp_info[0]);
    float temperature_c = 25.0f + (*(int16_t *)temp) * 0.0625f;
    if (unit == IMU_TEMP_C) return temperature_c;
    if (unit == IMU_TEMP_F) return temperature_c * 1.8f + 32;
    if (unit == IMU_TEMP_K) return temperature_c + 273.15f;
}

float joycon_accel_decode(int16_t data, enum IMUSensitivity sensitivity)
{
    switch (sensitivity)
    {
    case Sensitivity0:
        return SENSITIVITY_ACCELEROMETER_8 * data;
    case Sensitivity1:
        return SENSITIVITY_ACCELEROMETER_4 * data;
    case Sensitivity2:
        return SENSITIVITY_ACCELEROMETER_2 * data;
    case Sensitivity3:
    default:
        return SENSITIVITY_ACCELEROMETER_16 * data;
    }
}

float joycon_gyro_decode(int16_t data, enum IMUSensitivity sensitivity)
{
    switch (sensitivity)
    {
    case Sensitivity0:
        return SENSITIVITY_GYROSCOPE_245 * data;
    case Sensitivity1:
        return SENSITIVITY_GYROSCOPE_500 * data;
    case Sensitivity3:
    default:
        return SENSITIVITY_GYROSCOPE_2000 * data;
    }
}

float joycon_gyro_raw_decode(struct IMUPackedLEUnit unit, enum IMUSensitivity sensitivity)
{
    return joycon_gyro_decode((unit.high << 8) | unit.low, sensitivity);
}
float joycon_accel_raw_decode(struct IMUPackedLEUnit unit, enum IMUSensitivity sensitivity)
{
    return joycon_accel_decode((unit.high << 8) | unit.low, sensitivity);
}

void joycon_packet_imu_enable(uint8_t *buf, uint8_t timer)
{
    memset(buf, 0, 64);
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = EnableIMU;
    pkt->args.arg1 = IMU_ENABLE;
}

void joycon_packet_imu_disable(uint8_t *buf, uint8_t timer)
{
    memset(buf, 0, 64);
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = EnableIMU;
    pkt->args.arg1 = IMU_DISABLE;
}

