#include <mcu.h>
#include <rumble.h>
#include <packet.h>

#include <stdio.h>

#include "hidapi_connection.h"

#define NOTE_LENGTH 15
#define BPM 120
#define BASE_NOTE 4

float mxbc_freq[NOTE_LENGTH] = {
    261.63, 293.66, 329.63, 392, 392, 440, 392, 329.63, 261.63, 293.66, 329.63, 329.63, 293.66, 261.63, 293.66
};
int mxbc_duration[NOTE_LENGTH] = {
    1, 1, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2, 2, 2, 4
};

int main()
{
    uint8_t buf[OUTPUT_REPORT_LEGNTH];     // Buffer for an output packet
    uint8_t timer = 0;      // Timer for the output packets, incr by 1 for each sending
    int res;

    // Connect and do nothing
    check_connection_ok = 1;
    joycon con;
    if (!device_connection()) {
        fprintf(stderr, "No connected device\n");
        return 1;
    }
    con.handle = handle;
    fprintf(stderr, "Connected device\n");

    // Enable Vibration
    joycon_packet_rumble_enable_only(buf, timer & 0xF);
    res = hid_write(handle, buf, sizeof(buf));

    // New vibration like switch
    for (int i = 0; i < NOTE_LENGTH; i++)
    {
        timer++;
        joycon_packet_rumble_only(buf, timer & 0xF, mxbc_freq[i], mxbc_duration[i]);
        res = hid_write(handle, buf, sizeof(buf));
        Sleep(1000 * 60 / BPM / BASE_NOTE * mxbc_duration[i]);
    }

    // Disable vibration
    timer++;
    joycon_packet_rumble_disable(buf, timer & 0xF, 0, 0);
    res = hid_write(handle, buf, sizeof(buf));

    release_joycon(&con);

    return 0;
}
