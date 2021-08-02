#include <spi.h>
#include <packet.h>
#include <assert.h>

int main()
{
    struct SubcommandBodySPIData test_data1 = joycon_spi_read_sn_encode();
    assert(test_data1.offset == SPI_REGION_CONF_OFF_SN);
    assert(test_data1.size == 0xF);

    struct SubcommandBodySPIData test_data2 = joycon_spi_read_color_encode();
    assert(test_data2.offset == SPI_REGION_CONF_OFF_COLOR);
    assert(test_data2.size == 0x6);

    struct SubcommandBodySPIData test_data3 = joycon_spi_read_body_color_encode();
    assert(test_data3.offset == SPI_REGION_CONF_OFF_COLOR_BODY);
    assert(test_data3.size == 0x3);

    struct SubcommandBodySPIData test_data4 = joycon_spi_read_btn_color_encode();
    assert(test_data4.offset == SPI_REGION_CONF_OFF_COLOR_BTN);
    assert(test_data4.size == 0x3);

    // Test packet encoding
    uint8_t buf[OUTPUT_REPORT_LEGNTH];
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);

    joycon_packet_read_spi_sn(buf, 0);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SPIFlashRead);
    assert(pkt->spi_data.offset == joycon_spi_read_sn_encode().offset);
    assert(pkt->spi_data.size == joycon_spi_read_sn_encode().size);

    joycon_packet_read_spi_body_color(buf, 0);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SPIFlashRead);
    assert(pkt->spi_data.offset == joycon_spi_read_body_color_encode().offset);
    assert(pkt->spi_data.size == joycon_spi_read_body_color_encode().size);

    joycon_packet_read_spi_btn_color(buf, 0);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SPIFlashRead);
    assert(pkt->spi_data.offset == joycon_spi_read_btn_color_encode().offset);
    assert(pkt->spi_data.size == joycon_spi_read_btn_color_encode().size);

    joycon_packet_read_spi_color(buf, 0);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SPIFlashRead);
    assert(pkt->spi_data.offset == joycon_spi_read_color_encode().offset);
    assert(pkt->spi_data.size == joycon_spi_read_color_encode().size);

    return 0;
}
