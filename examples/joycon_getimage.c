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

    // TODO: Enable MCU

    // TODO: Request MCU mode status
    // Not necessary, but we keep to make sure the MCU is ready.

    // TODO: Set MCU mode

    // TODO: Request MCU mode status
    // Not necessary, but we keep to make sure the MCU is ready.

    // TODO: Set IR mode and number of packets for each data blob. Blob size is packets * 300 bytes.

    // TODO: Request IR mode status
    // Not necessary, but we keep to make sure the MCU is ready.

    // TODO: Write to registers for the selected IR mode

    // TODO: Another function: Stream or Capture images from NIR Camera

    // TODO: Disable MCU

    // Set input report back to x3f
    timer++;
    joycon_packet_input_report_mode(buf, timer & 0xF, ButtonTrigged);
    hid_write(handle, buf, sizeof(buf));

    release_joycon(&con);

    return 0;
}
