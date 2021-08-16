#ifndef PACKET_H
#define PACKET_H

#include "output_report.h"
#include "input_report.h"

struct PacketMCUReadComm
{
    struct Header header;
    uint8_t subcommand;
    struct MCUConfiguration conf;
    uint8_t crc;
    uint8_t padding;
};

void joycon_packet_mcu_read_ack_encode(uint8_t *buffer, uint8_t timer, uint8_t index);
void joycon_packet_mcu_read_req_encode(uint8_t *buffer, uint8_t timer, uint8_t index);

struct PacketMCUConf
{
    struct Header header;
    uint8_t subcommand;
    struct MCUConfiguration conf;
    uint8_t padding;
    uint8_t crc;
};

void joycon_packet_mcu_read_status_encode(uint8_t *buf, uint8_t timer);

void joycon_packet_mcu_enable(uint8_t *buf, uint8_t timer);
void joycon_packet_mcu_disable(uint8_t *buf, uint8_t timer);

void joycon_packet_mcu_conf_mode(uint8_t *buffer, uint8_t timer, uint8_t mode);
void joycon_packet_mcu_conf_ir_mode(uint8_t *buffer, uint8_t timer, uint8_t mode, uint8_t number);
void joycon_packet_mcu_conf_registers(uint8_t *buffer, uint8_t timer, uint8_t number,
    uint16_t *addrs, uint8_t *vals);
// TODO: Add a version with va_list for joycon_packet_mcu_conf_registers

// Encode rumble into packet
void joycon_packet_rumble_only(uint8_t *buffer, uint8_t timer, float freq, float amp);
void joycon_packet_append_rumble(uint8_t *buffer, float freq, float amp);

// Enable or disable rumble
void joycon_packet_rumble_enable(uint8_t *buffer, uint8_t timer, float freq, float amp);
void joycon_packet_rumble_disable(uint8_t *buffer, uint8_t timer, float freq, float amp);
void joycon_packet_rumble_enable_only(uint8_t *buffer, uint8_t timer);
void joycon_packet_rumble_disable_only(uint8_t *buffer, uint8_t timer);

// Encode SPI reading data
// TODO: void joycon_packet_read_spi(uint8_t *buffer, uint8_t timer, uint32_t offset, uint8_t size);
void joycon_packet_read_spi_sn(uint8_t *buffer, uint8_t timer);
void joycon_packet_read_spi_body_color(uint8_t *buffer, uint8_t timer);
void joycon_packet_read_spi_btn_color(uint8_t *buffer, uint8_t timer);
void joycon_packet_read_spi_color(uint8_t *buffer, uint8_t timer);

void joycon_packet_player_light_enable(uint8_t *buf, uint8_t timer, uint8_t light);
void joycon_packet_player_light_disable(uint8_t *buf, uint8_t timer);

void joycon_packet_imu_enable(uint8_t *buf, uint8_t timer);
void joycon_packet_imu_disable(uint8_t *buf, uint8_t timer);

void joycon_packet_input_report_mode(uint8_t *buf, uint8_t timer, uint8_t mode);

#endif
