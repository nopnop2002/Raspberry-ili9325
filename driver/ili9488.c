#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "ili9488.h"

void ili9488_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
	lcdInit(dev, 0x9488, width, height, offsetx, offsety);

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
		0xC0, 2, 0x10, 0x10,		//Power Control 1 [0E 0E]
		0xC1, 1, 0x41,		//Power Control 2 [43]
		0xC5, 4, 0x00, 0x22, 0x80, 0x40,	//VCOM	Control 1 [00 40 00 40]
		//0x36, 1, 0x68,		//Memory Access [00]
		//0x36, 1, 0x98,	  //Memory Access [00]
		0x36, 1, 0x18,	  //Memory Access [00]
		0xB0, 1, 0x00,		//Interface		[00]
		0xB1, 2, 0xB0, 0x11,		//Frame Rate Control [B0 11]
		0xB4, 1, 0x02,		//Inversion Control [02]
		0xB6, 3, 0x02, 0x02, 0x3B,	// Display Function Control [02 02 3B] .kbv NL=480
		0xB7, 1, 0xC6,		//Entry Mode	  [06]
		0x3A, 1, 0x55,		//Interlace Pixel Format [XX]
		0xF7, 4, 0xA9, 0x51, 0x2C, 0x82,	//Adjustment Control 3 [A9 51 2C 82]
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

	// ili9488 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
}

