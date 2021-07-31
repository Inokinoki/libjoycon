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

    // Set keep mode
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_KEEP_0);
    hid_write(handle, buf, sizeof(buf));
    Sleep(250);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_KEEP_1);
    hid_write(handle, buf, sizeof(buf));
    Sleep(250);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_KEEP_2);
    hid_write(handle, buf, sizeof(buf));
    Sleep(250);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_KEEP_3);
    hid_write(handle, buf, sizeof(buf));
    Sleep(250);
    timer++;
    joycon_packet_player_light_disable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));
    Sleep(250);

    // Set flash mode
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_FLASH_0);
    hid_write(handle, buf, sizeof(buf));
    Sleep(1000);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_FLASH_1);
    hid_write(handle, buf, sizeof(buf));
    Sleep(1000);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_FLASH_2);
    hid_write(handle, buf, sizeof(buf));
    Sleep(1000);
    timer++;
    joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_FLASH_3);
    hid_write(handle, buf, sizeof(buf));
    Sleep(1000);
    timer++;
    joycon_packet_player_light_disable(buf, timer & 0xF);
    hid_write(handle, buf, sizeof(buf));

    release_joycon(&con);

    return 0;
}
