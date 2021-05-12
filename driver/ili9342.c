#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "ili9342.h"

void ili9342_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
	lcdInit(dev, 0x9342, width, height, offsetx, offsety);

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
      0xC0, 2, 0x26, 0x09,
      0xC1, 1, 0x10,
      0xC5, 2, 0x3E, 0x28,
      0xC7, 1, 0x86,
      0x36, 1, 0xE8,
      0xB1, 2, 0x00, 0x18,
      0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
      0x21, 0,         // Display Inversion ON
      0x26, 1, 0x01,
      0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
      0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    };
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

	// ili9342 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
}
