#include <mcu.h>
#include <ir.h>
#include <packet.h>
#include <string.h>

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

void get_raw_ir_image(uint8_t *buf_image, uint16_t width, uint16_t height, uint8_t *timer, uint8_t ir_max_frag_no)
{
    uint8_t buf[OUTPUT_REPORT_LEGNTH];
    uint8_t buf_reply[INPUT_REPORT_MCUIR_LENGTH];

    struct InputReportHeader *input_header = (struct InputReportHeader *)buf_reply;
    struct InputReportIMUMCU *input_mcu_data = (struct InputReportIMUMCU *)(input_header + 1);

    int initialization = 2;
    int previous_frag_no = 0;
    int got_frag_no = 0;
    int missed_packet_no = 0;
    int missed_packet = 0;

    // Send first ACK
    *timer++;
    joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, 0);
    hid_write(handle, buf, OUTPUT_REPORT_LEGNTH);

    // IR Read/ACK loop for fragmented data packets. 
    // It also avoids requesting missed data fragments, we just skip it to not complicate things.
    while (initialization)
    {
        // Read a fragment
        memset(buf_reply, 0, INPUT_REPORT_MCUIR_LENGTH);
        hid_read_timeout(handle, buf_reply, INPUT_REPORT_MCUIR_LENGTH, 200);
        if (input_header->id == MCUIR)
        {
            if (input_mcu_data->nfc_ir_data[0] == 0x03)
            {
                // Add normal routine
                got_frag_no = input_mcu_data->nfc_ir_data[3];
                if (got_frag_no == (previous_frag_no + 1) % (ir_max_frag_no + 1))
                {
                    // Get the frag
                    previous_frag_no = got_frag_no;

                    if (got_frag_no == ir_max_frag_no)
                    {
                        // The last frame
                        // TODO: Get noise level, white pixel percentage, average_intensity_percentage
                        if (initialization) initialization--;
                    }
                }
                // Repeat/Missed fragment
                else if (got_frag_no  || previous_frag_no)
                {
                    if (got_frag_no == previous_frag_no)
                    {
                        // ACK for fragment
                        *timer++;
                        joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, got_frag_no);
                        hid_write(handle, buf, sizeof(buf));

                        missed_packet = 0;
                    }
                    else if (missed_packet_no != got_frag_no && !missed_packet)
                    {
                        // Check if missed fragment and res is 30x40. Don't request it
                        if (ir_max_frag_no != 0x03)
                        {
                            // Request for missed packet. You send what the next fragment number will be, instead of the actual missed packet.
                            *timer++;
                            joycon_packet_mcu_read_req_encode(buf, *timer & 0x0F, previous_frag_no + 1);
                            hid_write(handle, buf, OUTPUT_REPORT_LEGNTH);

                            memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                            previous_frag_no = got_frag_no;
                            missed_packet_no = got_frag_no - 1;
                            missed_packet = 1;
                        }
                        else
                        {
                            // ACK for fragment
                            *timer++;
                            joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, got_frag_no);
                            hid_write(handle, buf, sizeof(buf));
                            memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                            previous_frag_no = got_frag_no;
                        }
                        
                    }
                    // Got the requested missed fragments.
                    else if (missed_packet_no == got_frag_no)
                    {
                        // ACK for fragment
                        *timer++;
                        joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, got_frag_no);
                        hid_write(handle, buf, sizeof(buf));

                        memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                        previous_frag_no = got_frag_no;
                        missed_packet = 0;
                    }
                    // Repeat of fragment that is not max fragment.
                    else
                    {
                        // ACK for fragment
                        *timer++;
                        joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, got_frag_no);
                        hid_write(handle, buf, sizeof(buf));
                    }
                    fprintf(stderr, "Loading image: %d\%\n", got_frag_no * 100 / (ir_max_frag_no + 1));
                }
                // Streaming start
                else {
                    // ACK for fragment
                    *timer++;
                    joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, got_frag_no);
                    hid_write(handle, buf, OUTPUT_REPORT_LEGNTH);
                    memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                    previous_frag_no = 0;
                }
            }
            else
            {
                // Empty IR report. Send Ack again. Otherwise, it fallbacks to high latency mode (30ms per data fragment)
                switch (input_mcu_data->nfc_ir_data[0])
                {
                case 0xFF:
                    // Send ACK again
                    *timer++;
                    joycon_packet_mcu_read_ack_encode(buf, *timer & 0x0F, previous_frag_no);
                    hid_write(handle, buf, OUTPUT_REPORT_LEGNTH);
                    break;
                case 0x00:
                    // Request missed frag
                    *timer++;
                    joycon_packet_mcu_read_req_encode(buf, *timer & 0x0F, previous_frag_no + 1);
                    hid_write(handle, buf, OUTPUT_REPORT_LEGNTH);
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            fprintf(stderr, "Input header [%X]\n", input_header->id);
        }
    }

    fprintf(stderr, "Read IR Image finished\n");
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
        if (buf_read[0] == MCUIR) {
            //if (buf[49] == 0x01 && buf[56] == 0x06) // MCU state is Initializing
            // *(u16*)buf[52]LE x04 in lower than 3.89fw, x05 in 3.89
            // *(u16*)buf[54]LE x12 in lower than 3.89fw, x18 in 3.89
            // buf[56]: mcu mode state
            if (buf_read[49] == 0x01 && buf_read[56] == MCUMode_STANDBY) // MCU state is Standby
                break;
        }
        retries++;
        if (retries > 8 || res == 0)
        {
            fprintf(stderr, "Enable MCU failed after %d retries\n", retries);
            goto giveup;
        }
    }

    // Set MCU mode
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
        if (buf_read[0] == 0x31) {
            //if (buf[49] == 0x01 && buf[56] == 0x06) // MCU state is Initializing
            // *(u16*)buf[52]LE x04 in lower than 3.89fw, x05 in 3.89
            // *(u16*)buf[54]LE x12 in lower than 3.89fw, x18 in 3.89
            // buf[56]: mcu mode state
            if (buf_read[49] == 0x01 && buf_read[56] == MCUMode_IR) // MCU state is Standby
                break;
        }
        retries++;
        if (retries > 8 || res == 0)
        {
            fprintf(stderr, "Set MCU mode failed after %d retries\n", retries);
            goto giveup;
        }
    }

    // Set IR mode and number of packets for each data blob. Blob size is packets * 300 bytes.
    timer++;
    joycon_packet_mcu_conf_ir_mode(buf, timer & 0xF, 0x07, IR_RESOLUTION_FULL_NUM_FRAG);
    hid_write(handle, buf, sizeof(buf));

    // Request IR mode status
    // Not necessary, but we keep to make sure the MCU is ready.
    timer++;
    joycon_packet_mcu_read_status_encode(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));
    retries = 0;
    while (1) {
        int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 200);
        if (buf_read[0] == 0x31) {
            if (buf_read[49] == 0x13 && buf_read[50] == 0x07) // MCU IR mode is capture
                break;
        }
        retries++;
        if (retries > 8 || res == 0)
        {
            fprintf(stderr, "Set IR Mode failed\n");
            goto giveup;
        }
    }

    // Write to registers for the selected IR mode
    uint16_t addrs1[9], addrs2[8];
    uint8_t vals1[9], vals2[8];
    addrs1[0] = IR_RESOLUTION_REG;          vals1[0] = IR_RESOLUTION_FULL;
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

    // TODO: Debug Another function: Stream or Capture images from NIR Camera
    uint16_t width = 320, height = 240;
    uint8_t image_buffer[19 * 4096];
    get_raw_ir_image(image_buffer, width, height, &timer, IR_RESOLUTION_FULL_NUM_FRAG);

giveup:
    // Conf standby
    fprintf(stderr, "Standby MCU\n");
    timer++;
    joycon_packet_mcu_conf_mode(buf, timer & 0xF, MCUMode_STANDBY);
    hid_write(handle, buf, sizeof(buf));

    // Disable MCU
    fprintf(stderr, "Disable MCU\n");
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
