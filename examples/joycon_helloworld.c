#include <mcu.h>
#include <rumble.h>
#include <packet.h>

#include <stdio.h>

#include "hidapi_connection.h"

int main()
{
    uint8_t buffer[64];     // Buffer for an output packet
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
    release_joycon(&con);

    return 0;
}
