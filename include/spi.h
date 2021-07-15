#ifndef SPI_H
#define SPI_H

#include "output_report.h"

/*
Initial PatchRAM 	    x0000 	x1000 	Stores static data like BT address, OTA DS 1 offset, etc
Failsafe 	            x1000 	x1000 	Stores OTA Signature Magic and OTA Dynamic Section 2 offset
Pairing info 	        x2000 	x1000 	Stores last connected device, data for Sticks and Sensor calibration
Pairing info (fa) 	    x3000 	x1000 	Empty. (Dev-units use it?)
Pairing info (fa2) 	    x4000 	x1000 	Empty. (Dev-units use it?)
Shipment 	            x5000 	x1000 	Only first byte is used
Config and calibration 	x6000 	xA000 	Stores Factory configuration and calibration, user calibration
PatchRAM section 	    x10000 	x70000
*/

#define SPI_REGION_CONF         0x6000
#define SPI_REGION_CONF_SIZE    0xA000
#define SPI_REGION_CONF_OFF_SN          (SPI_REGION_CONF + 0x00)
#define SPI_REGION_CONF_OFF_COLOR       (SPI_REGION_CONF + 0x50)
#define SPI_REGION_CONF_OFF_COLOR_BODY  (SPI_REGION_CONF_OFF_COLOR + 0x0)
#define SPI_REGION_CONF_OFF_COLOR_BTN   (SPI_REGION_CONF_OFF_COLOR + 0x3)

struct SubcommandBodySPIData joycon_spi_read_sn_encode();
struct SubcommandBodySPIData joycon_spi_read_body_color_encode();
struct SubcommandBodySPIData joycon_spi_read_btn_color_encode();
struct SubcommandBodySPIData joycon_spi_read_color_encode();

#endif