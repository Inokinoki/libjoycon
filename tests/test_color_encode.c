#include "../include/spi.h"
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

    return 0;
}
