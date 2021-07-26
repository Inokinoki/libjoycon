#ifndef PACKET_H
#define PACKET_H

#include "output_report.h"
#include "input_report.h"

struct PacketMCUReadComm
{
    struct Header header;
    uint8_t subcommand;
    struct MCUConfiguration conf;
    uint8_t padding;
};

void joycon_packet_mcu_read_ack_encode(uint8_t *buffer, uint8_t timer, uint8_t index);
void joycon_packet_mcu_read_req_encode(uint8_t *buffer, uint8_t timer, uint8_t index);

#endif
