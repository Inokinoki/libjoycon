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

    // TODO: Impl

    release_joycon(&con);

    return 0;
}
