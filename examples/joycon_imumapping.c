#include <stdio.h>
#include <stdio.h>
#include <hidapi.h>
#include <string.h>

#include <libjoycon.h>

#include "hidapi_connection.h"
#include "joycon_imumapping.h"

extern float angles[3];

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

void read_imu_data(int t)
{
    // Update angle
    uint8_t buf_read[64];
    int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 200);
    struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
    switch (header->id)
    {
    case SIRFullSpeed:
        fprintf(stderr, "Received SIR:\t0x%02X, LEN: %d\n", header->id, res);
        struct IMUPackedDataReply *reply = (struct IMUPackedDataReply *)(header + 1);

        angles[0] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_1.le_bytes.high << 8) | reply->data[0].gyro_1.le_bytes.low, Sensitivity3);
        angles[0] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_1.le_bytes.high << 8) | reply->data[1].gyro_1.le_bytes.low, Sensitivity3);
        angles[0] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_1.le_bytes.high << 8) | reply->data[2].gyro_1.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_2.le_bytes.high << 8) | reply->data[0].gyro_2.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_2.le_bytes.high << 8) | reply->data[1].gyro_2.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_2.le_bytes.high << 8) | reply->data[2].gyro_2.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_3.le_bytes.high << 8) | reply->data[0].gyro_3.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_3.le_bytes.high << 8) | reply->data[1].gyro_3.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_3.le_bytes.high << 8) | reply->data[2].gyro_3.le_bytes.low, Sensitivity3);

        postProcess();
        break;
    default:
        fprintf(stderr, "Received Other:\t0x%02X\n", header->id);
        break;
    }

    resetTimer();
}

int main(int argc, char **argv)
{
    uint8_t buf[64];        // Buffer for an output packet
    uint8_t timer = 0;      // Timer for the output packets, incr by 1 for each sending

    // Connect and do nothing
    check_connection_ok = 1;
    joycon con;
    if (!device_connection()) {
        fprintf(stderr, "No connected device\n");
        return 1;
    }
    con.handle = handle;
    fprintf(stderr, "Connected device\n");

    // Enable IMU
    joycon_packet_imu_enable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    /*
    TODO: Use SIR Full Speed
    memset(buf, 0, sizeof(buf));
    hdr->command = Subcommand;
    hdr->counter = timer & 0xF;
    timer++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = SIRFullSpeed;
    hid_write(handle, buf, sizeof(buf));
    */

    // Use mainloop
    mainloop();

    // Disable IMU
    timer++;
    joycon_packet_imu_disable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    /*
    TODO: Recover basic input
    memset(buf, 0, sizeof(buf));
    hdr->counter = timer & 0xF;
    timer++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = ButtonTrigged;
    hid_write(handle, buf, sizeof(buf));
    */

    release_joycon(&con);
    return 0; /* ANSI C requires main to return int. */
}
