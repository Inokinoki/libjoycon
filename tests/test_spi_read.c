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

static int handle_ok;
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

    uint8_t buf[64];
    uint8_t buf_read[64];
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_body_color_encode();
    hid_write(handle, buf, sizeof(buf));

    int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 500);

    struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
    struct InputReportSubcommandReply *reply = (struct InputReportSubcommandReply *)(header + 1);
    fprintf(stderr, "ID:\t0x%02X\n", header->id);
    fprintf(stderr, "Timer:\t0x%02X\n", header->timer);
    fprintf(stderr, "Batt:\t0x%02X\n", header->battery_level);
    fprintf(stderr, "Conn:\t0x%02X\n", header->conn_info);

    fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
    fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
    fprintf(stderr, "ADDR:\t0x%08X LEN:\t0x%02X\n", reply->spi_data.offset, reply->spi_data.size);
    fprintf(stderr, "R: 0x%02X G: 0x%02X B: 0x%02X\n", reply->data[0], reply->data[1], reply->data[2]);

    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_btn_color_encode();
    hid_write(handle, buf, sizeof(buf));

    hid_read_timeout(handle, buf_read, sizeof(buf_read), 500);

    fprintf(stderr, "ID:\t0x%02X\n", header->id);
    fprintf(stderr, "Timer:\t0x%02X\n", header->timer);
    fprintf(stderr, "Batt:\t0x%02X\n", header->battery_level);
    fprintf(stderr, "Conn:\t0x%02X\n", header->conn_info);

    fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
    fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
    fprintf(stderr, "ADDR:\t0x%08X LEN:\t0x%02X\n", reply->spi_data.offset, reply->spi_data.size);
    fprintf(stderr, "R: 0x%02X G: 0x%02X B: 0x%02X\n", reply->data[0], reply->data[1], reply->data[2]);

    return 0;
}
