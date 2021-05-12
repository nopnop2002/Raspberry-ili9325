#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "ili9481.h"

void ili9481_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
	lcdInit(dev, 0x9481, width, height, offsetx, offsety);

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
		0xB0, 1, 0x00,				// unlocks E0, F0
		0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
		0xB4, 1, 0x00,				// Frame mode [00]
		0xD0, 3, 0x07, 0x42, 0x18,
		0xD1, 3, 0x00, 0x07, 0x18,
		0xD2, 2, 0x01, 0x02,
		0xD3, 2, 0x01, 0x02,		// Set Power for Partial Mode [01 22]
		0xD4, 2, 0x01, 0x02,		// Set Power for Idle Mode [01 22]
		0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
		//0xC0, 5, 0x14, 0x3B, 0x00, 0x02, 0x11,
		0xC1, 3, 0x10, 0x10, 0x88,	// Display Timing Normal [10 10 88]
		0xC5, 1, 0x03,		//Frame Rate [03]
		0xC6, 1, 0x02,		//Interface Control [02]
		0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
		0xCC, 1, 0x00,		//Panel Control [00]
		0x36, 1, 0x18, //0x08,
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

	// ili9481 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
}

