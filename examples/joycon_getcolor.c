#include <mcu.h>
#include <rumble.h>
#include <packet.h>

#include <stdio.h>

#include "hidapi_connection.h"

int main()
{
    uint8_t buf[OUTPUT_REPORT_LEGNTH];     // Buffer for an output packet
    uint8_t timer = 0;      // Timer for the output packets, incr by 1 for each sending

    // Connect and get color
    check_connection_ok = 1;
    joycon con;
    if (!device_connection()) {
        fprintf(stderr, "No connected device\n");
        return 1;
    }
    con.handle = handle;
    fprintf(stderr, "Connected device\n");

    uint8_t buf_read[INPUT_REPORT_STANDARD_LEGNTH];
    joycon_packet_read_spi_body_color(buf, timer & 0x0F);
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
    struct SubcommandBodySPIColorData *data = (struct SubcommandBodySPIColorData *)reply->data;
    fprintf(stderr, "ADDR:\t0x%08X LEN:\t0x%02X\n", data->data.offset, data->data.size);
    fprintf(stderr, "R: 0x%02X G: 0x%02X B: 0x%02X\n", data->color.r, data->color.g, data->color.b);

    timer++;
    joycon_packet_read_spi_btn_color(buf, timer & 0x0F);
    hid_write(handle, buf, sizeof(buf));

    hid_read_timeout(handle, buf_read, sizeof(buf_read), 500);

    fprintf(stderr, "ID:\t0x%02X\n", header->id);
    fprintf(stderr, "Timer:\t0x%02X\n", header->timer);
    fprintf(stderr, "Batt:\t0x%02X\n", header->battery_level);
    fprintf(stderr, "Conn:\t0x%02X\n", header->conn_info);

    fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
    fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
    data = (struct SubcommandBodySPIColorData *)reply->data;
    fprintf(stderr, "ADDR:\t0x%08X LEN:\t0x%02X\n", data->data.offset, data->data.size);
    fprintf(stderr, "R: 0x%02X G: 0x%02X B: 0x%02X\n", data->color.r, data->color.g, data->color.b);

    release_joycon(&con);

    return 0;
}
