#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "ili9341.h"

void ili9341_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
	lcdInit(dev, 0x9341, width, height, offsetx, offsety);

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
		0xC0, 1, 0x23,
		0xC1, 1, 0x10,
		0xC5, 2, 0x3E, 0x28,
		0xC7, 1, 0x86,
		0x36, 1, 0x98,
		0xB1, 2, 0x00, 0x18,
		0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
		0x26, 1, 0x01,
		0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
		0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

	// ili9341 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
}

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void ili9341_lcdDrawPixel(TFT_t *dev, uint16_t x, uint16_t y, uint16_t color) {
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

	lcdWriteCommandByte(dev, 0x2A); // set column(x) address
	lcdWriteDataWord(dev, _x);
	lcdWriteDataWord(dev, _x);
	lcdWriteCommandByte(dev, 0x2B); // set Page(y) address
	lcdWriteDataWord(dev, _y);
	lcdWriteDataWord(dev, _y);
	lcdWriteCommandByte(dev, 0x2C); // Memory Write
#ifndef P16BIT
	lcdWriteDataWord(dev, color);
#else
	lcdWriteDataWord_16bit(dev, color);
#endif
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void ili9341_lcdDrawFillRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2 = dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2 = dev->_height-1;

	uint16_t _x1 = x1 + dev->_offsetx;
	uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;

	lcdWriteCommandByte(dev, 0x2A); // set column(x) address
	lcdWriteDataWord(dev, _x1);
	lcdWriteDataWord(dev, _x2);
	lcdWriteCommandByte(dev, 0x2B); // set Page(y) address
	lcdWriteDataWord(dev, _y1);
	lcdWriteDataWord(dev, _y2);
	lcdWriteCommandByte(dev, 0x2C); // Memory Write

	for(int i=_x1;i<=_x2;i++){
		for(int j=_y1;j<=_y2;j++){
#ifndef P16BIT
			lcdWriteDataWord(dev, color);
#else
			lcdWriteDataWord_16bit(dev, color);
#endif
		}
	}
}

// Display OFF
void ili9341_lcdDisplayOff(TFT_t *dev) {
	lcdWriteCommandByte(dev, 0x28);
}

// Display ON
void ili9341_lcdDisplayOn(TFT_t *dev) {
	lcdWriteCommandByte(dev, 0x29);
}

// Display Inversion OFF
void ili9341_lcdInversionOff(TFT_t * dev) {
	lcdWriteCommandByte(dev, 0x20);
}

// Display Inversion ON
void ili9341_lcdInversionOn(TFT_t * dev) {
	lcdWriteCommandByte(dev, 0x21);
}
