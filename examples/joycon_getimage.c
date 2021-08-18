#include <mcu.h>
#include <ir.h>
#include <packet.h>

#include <stdio.h>

#include "hidapi_connection.h"

int device_right_connection(){
    joycon con;
    if (check_connection_ok) {
        handle_ok = 0;
        // Joy-Con (R)
        if (init_joycon(&con, JOYCON_R) == NO_ERR) {
            handle_ok = 2;
            handle = con.handle;
            return handle_ok;
        }
        return 0;
    }
    return handle_ok;
}

int main()
{
    uint8_t buf[OUTPUT_REPORT_LEGNTH];     // Buffer for an output packet
    uint8_t timer = 0;      // Timer for the output packets, incr by 1 for each sending
    int retries = 0;

    // Connect and get color
    check_connection_ok = 1;
    joycon con;
    if (!device_right_connection()) {
        fprintf(stderr, "No connected device, must be a right Joycon\n");
        return 1;
    }
    con.handle = handle;
    fprintf(stderr, "Connected device\n");

    uint8_t buf_read[INPUT_REPORT_MCUIR_LENGTH];
    // Set input report to x31
    joycon_packet_input_report_mode(buf, timer & 0xF, MCUIR);
    hid_write(handle, buf, sizeof(buf));

    // Enable MCU
    timer++;
    joycon_packet_mcu_enable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    // Request MCU mode status
    // Not necessary, but we keep to make sure the MCU is ready.
    timer++;
    joycon_packet_mcu_read_status_encode(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));
    retries = 0;
    while (1) {
        int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 200);
        if (buf[0] == 0x31) {
            //if (buf[49] == 0x01 && buf[56] == 0x06) // MCU state is Initializing
            // *(u16*)buf[52]LE x04 in lower than 3.89fw, x05 in 3.89
            // *(u16*)buf[54]LE x12 in lower than 3.89fw, x18 in 3.89
            // buf[56]: mcu mode state
            if (buf[49] == 0x01 && buf[56] == MCUMode_STANDBY) // MCU state is Standby
                break;
        }
        retries++;
        if (retries > 8 || res == 0)
            goto giveup;
    }

    // TODO: debug Set MCU mode
    timer++;
    joycon_packet_mcu_conf_mode(buf, timer & 0xF, MCUMode_IR);
    hid_write(handle, buf, sizeof(buf));

    // Request MCU mode status
    // Not necessary, but we keep to make sure the MCU is ready.
    timer++;
    joycon_packet_mcu_read_status_encode(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));
    retries = 0;
    while (1) {
        int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 200);
        if (buf[0] == 0x31) {
            //if (buf[49] == 0x01 && buf[56] == 0x06) // MCU state is Initializing
            // *(u16*)buf[52]LE x04 in lower than 3.89fw, x05 in 3.89
            // *(u16*)buf[54]LE x12 in lower than 3.89fw, x18 in 3.89
            // buf[56]: mcu mode state
            if (buf[49] == 0x01 && buf[56] == MCUMode_IR) // MCU state is Standby
                break;
        }
        retries++;
        if (retries > 8 || res == 0)
            goto giveup;
    }

    // Set IR mode and number of packets for each data blob. Blob size is packets * 300 bytes.
    timer++;
    joycon_packet_mcu_conf_ir_mode(buf, timer & 0xF, 0x07, IR_RESOLUTION_FULL_NUM_FRAG);
    hid_write(handle, buf, sizeof(buf));

    // TODO: Request IR mode status
    // Not necessary, but we keep to make sure the MCU is ready.
    timer++;
    joycon_packet_mcu_read_status_encode(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    // Write to registers for the selected IR mode
    uint16_t addrs1[9], addrs2[8];
    uint8_t vals1[9], vals2[8];
    addrs1[0] = IR_RESOLUTION_REG;          vals1[0] = 0;
    addrs1[1] = IR_EXPOSURE_TIME_LSB_REG;   vals1[1] = 0x4920 & 0xFF;
    addrs1[2] = IR_EXPOSURE_TIME_MSB_REG;   vals1[2] = (0x4920 & 0xFF00) >> 8;
    addrs1[3] = IR_EXPOSURE_TIME_MAX_REG;   vals1[3] = 0x00;
    addrs1[4] = IR_LED_GROUP_STATE_REG;     vals1[4] = 0;
    addrs1[5] = IR_DIGITAL_GAIN_LSB_REG;    vals1[5] = 0xFF;
    addrs1[6] = IR_DIGITAL_GAIN_MSB_REG;    vals1[6] = 0x07;
    addrs1[7] = IR_EX_LIGHT_FILTER_REG;     vals1[7] = 0;
    addrs1[8] = IR_WHITE_PIXEL_THRES_REG;   vals1[8] = 0xc8;
    timer++;
    joycon_packet_mcu_conf_registers(buf, timer & 0xF, 9, addrs1, vals1);
    hid_write(handle, buf, sizeof(buf));
    addrs2[0] = IR_LED_INTENSITY_12_REG;    vals2[0] = 0x0F;
    addrs2[1] = IR_LED_INTENSITY_34_REG;    vals2[1] = 0x10;
    addrs2[2] = IR_FLIP_IMAGE_REG;          vals2[2] = 0;
    addrs2[3] = IR_DENOISE_SMOOTH_ALGO_REG; vals2[3] = 0x00;
    addrs2[4] = IR_DENOISE_SMOOTH_THRES_REG;vals2[4] = 0x23;
    addrs2[5] = IR_COLOR_INTERPOL_THRES_REG;vals2[5] = 0x44;
    addrs2[6] = IR_UPDATE_TIME_LSB_REG;     vals2[6] = 0x32;
    addrs2[7] = IR_APPLY_REG;               vals2[7] = 1;
    timer++;
    joycon_packet_mcu_conf_registers(buf, timer & 0xF, 8, addrs2, vals2);
    hid_write(handle, buf, sizeof(buf));

    // TODO: Another function: Stream or Capture images from NIR Camera

giveup:
    // Disable MCU
    timer++;
    joycon_packet_mcu_disable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    // Set input report back to x3f
    timer++;
    joycon_packet_input_report_mode(buf, timer & 0xF, ButtonTrigged);
    hid_write(handle, buf, sizeof(buf));

    release_joycon(&con);

    return 0;
}
