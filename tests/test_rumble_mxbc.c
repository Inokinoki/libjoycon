#include <stdio.h>
#include <hidapi.h>
#include <string.h>
#include <stdint.h>
#include "../include/libjoycon.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#include <unistd.h>
#define Sleep(x) usleep(x * 1000)

typedef enum {
    NO_ERR,
    ERR_NUL,
    ERR_CON
} joycon_status;

static int enable_traffic_dump = 0;
static int handle_ok;
static int enable_button_test;
static int enable_IRVideoPhoto;
static int enable_IRAutoExposure;
static int enable_NFCScanning;
static int cancel_spi_dump;
static int check_connection_ok;

static uint8_t timming_byte;

hid_device *handle;
hid_device *handle_l;

typedef enum {
    JOYCON_L,
    JOYCON_R,
    PROCON,
    OTHER
} joycon_type;

typedef struct
{
    hid_device *handle;
    joycon_type type;
} joycon;

joycon_status init_joycon(joycon *con, joycon_type type);

joycon_status release_joycon(joycon *con);

joycon_status get_joycon_info(joycon *con, u8 *buffer);

joycon_status get_joycon_spi_data(joycon *con, u32 offset, const u16 read_len, u8 *buffer);

static float mxbc_freq[15] = {
    261.63, 293.66, 329.63, 392, 392, 440, 392, 329.63, 261.63, 293.66, 329.63, 329.63, 293.66, 261.63, 293.66
};
static int mxbc_duration[15] = {
    1, 1, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2, 2, 2, 4
};

int send_rumble() {
    int res;
    u8 buf[64];
    
    // Enable Vibration
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = EnableVibration;
    pkt->args.arg1 = VIBRATION_ENABLE;
    res = hid_write(handle, buf, sizeof(buf));

    // New vibration like switch
    for (int i = 0; i < 15; i++)
    {
        memset(buf, 0, sizeof(buf));
        hdr->command = Subcommand;
        hdr->counter = timming_byte & 0xF;
        timming_byte++;
        hdr->rumble_l = joycon_rumble_encode(mxbc_freq[i], 0.5);
        memcpy(hdr->rumble_r.data, hdr->rumble_l.data, sizeof(hdr->rumble_l));
        res = hid_write(handle, buf, sizeof(buf));
        Sleep(250 * mxbc_duration[i]);
    }

    // Disable vibration
    memset(buf, 0, sizeof(buf));
    hdr = (struct Header *)buf;
    pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    hdr->rumble_l = joycon_rumble_encode(0, 0);
    memcpy(hdr->rumble_r.data, hdr->rumble_l.data, sizeof(hdr->rumble_l));
    pkt->subcommand = EnableVibration;
    pkt->args.arg1 = VIBRATION_DISABLE;
    res = hid_write(handle, buf, sizeof(buf));

    return 0;
}

int device_connection(){
    joycon con;
    if (check_connection_ok) {
        handle_ok = 0;
        // Try Joy-Con (L)
        if (init_joycon(&con, JOYCON_L) == NO_ERR) {
            handle_ok = 1;
            handle = con.handle;
            return handle_ok;
        }
        // Joy-Con (R)
        if (init_joycon(&con, JOYCON_R) == NO_ERR) {
            handle_ok = 2;
            handle = con.handle;
            return handle_ok;
        }
        // Pro Controller
        if (init_joycon(&con, PROCON) == NO_ERR) {
            handle_ok = 3;
            handle = con.handle;
            return handle_ok;
        }
        // Nothing found
        else {
            return 0;
        }
    }
    return handle_ok;
}

joycon_status init_joycon(joycon *con, joycon_type type)
{
    if (!con) {
        return ERR_NUL;
    }

    // Init controller
    switch (type)
    {
    case JOYCON_L:
        con->handle = hid_open(0x57e, 0x2006, NULL);
        break;
    case JOYCON_R:
        con->handle = hid_open(0x57e, 0x2007, NULL);
        break;
    case PROCON:
        con->handle = hid_open(0x57e, 0x2009, NULL);
        break;
    default:
        break;
    }

    // Set controller type
    if (con->handle) {
        con->type = type;
        return NO_ERR;
    }

    return ERR_CON;
}

joycon_status release_joycon(joycon *con)
{
    if (con && con->handle)
    {
        hid_close(con->handle);
        con->handle = NULL;
    }
    return NO_ERR;
}

int main()
{
    check_connection_ok = 1;
    if (!device_connection()) {
        fprintf(stderr, "No connected device\n");
        return 1;
    }
    fprintf(stderr, "Connected device\n");
    send_rumble();
    return 0;
}
