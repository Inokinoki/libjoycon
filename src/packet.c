#include "../include/packet.h"
// #include <packet.h>


void joycon_packet_mcu_read_ack_encode(uint8_t *buffer, uint8_t timer, uint8_t index)
{
    memset(buffer, 0, 64);
    struct PacketMCUReadComm *pkt = (struct PacketMCUReadComm *)buffer;
    pkt->header.command = MCURead;
    pkt->header.counter = timer;

    pkt->subcommand = 0x03;
    pkt->conf.command.command = 0x00;
    pkt->conf.comm_conf.ack_index = index;
    pkt->crc = mcu_crc8(&pkt->conf, MCU_CONF_LEN);
    pkt->padding = 0xFF;
}

void joycon_packet_mcu_read_req_encode(uint8_t *buffer, uint8_t timer, uint8_t index)
{
    memset(buffer, 0, 64);
    struct PacketMCUReadComm *pkt = (struct PacketMCUReadComm *)buffer;
    pkt->header.command = MCURead;
    pkt->header.counter = timer;

    pkt->subcommand = 0x03;
    pkt->conf.command.command = 0x00;
    pkt->conf.command.subcommand = 0x01;
    pkt->conf.comm_conf.req_index = index;
    pkt->crc = mcu_crc8(&pkt->conf, MCU_CONF_LEN);
    pkt->padding = 0xFF;
}
