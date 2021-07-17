#include <libjoycon.h>

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
