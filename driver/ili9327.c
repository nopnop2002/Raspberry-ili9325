#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "ili9327.h"

void ili9327_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
	lcdInit(dev, 0x9327, width, height, offsetx, offsety);

	static const uint8_t reset_off[] = {
		0x01, 0,			//Soft Reset
		TFTLCD_DELAY8, 150,
		0x28, 0,			//Display Off
		0x3A, 1, 0x55,		//Pixel read=565, write=565.
	};

	static const uint8_t wake_on[] = {
		0x11, 0,			//Sleep Out
		TFTLCD_DELAY8, 150,
		0x29, 0,			//Display On
	};

	static const uint8_t regValues[] = {
		0x36, 1, 0x08,
		0x3A, 1, 0x05,
		0xC0, 6, 0x00, 0x35, 0x00, 0x00, 0x01, 0x02,
		0xC1, 4, 0x10, 0x10, 0x02, 0x02,
		0xC5, 1, 0x04,
		0xC8,15, 0x04, 0x67, 0x35, 0x04, 0x08, 0x06, 0x24, 0x01, 0x37, 0x40, 0x03, 0x10, 0x08, 0x80, 0x00,
		0xD0, 3, 0x07, 0x04, 0x00,
		0xD1, 3, 0x00, 0x0C, 0x0F,
		0xD2, 2, 0x01, 0x44,
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

	// ili9327 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
}
