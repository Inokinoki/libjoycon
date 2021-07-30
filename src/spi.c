#include <spi.h>

struct SubcommandBodySPIData joycon_spi_read_sn_encode()
{
    struct SubcommandBodySPIData spi_data = {
        .offset = SPI_REGION_CONF_OFF_SN,
        .size = 0xF
    };
    return spi_data;
}

struct SubcommandBodySPIData joycon_spi_read_body_color_encode()
{
    struct SubcommandBodySPIData spi_data = {
        .offset = SPI_REGION_CONF_OFF_COLOR_BODY,
        .size = 0x3
    };
    return spi_data;
}

struct SubcommandBodySPIData joycon_spi_read_btn_color_encode()
{
    struct SubcommandBodySPIData spi_data = {
        .offset = SPI_REGION_CONF_OFF_COLOR_BTN,
        .size = 0x3
    };
    return spi_data;
}

struct SubcommandBodySPIData joycon_spi_read_color_encode()
{
    struct SubcommandBodySPIData spi_data = {
        .offset = SPI_REGION_CONF_OFF_COLOR_BODY,
        .size = 0x6
    };
    return spi_data;
}

void joycon_packet_read_spi_sn(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, 64);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_sn_encode();
}

void joycon_packet_read_spi_body_color(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, 64);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_body_color_encode();
}

void joycon_packet_read_spi_btn_color(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, 64);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_btn_color_encode();
}

void joycon_packet_read_spi_color(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, 64);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = SPIFlashRead;
    pkt->spi_data = joycon_spi_read_color_encode();
}
