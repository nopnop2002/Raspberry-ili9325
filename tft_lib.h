#ifndef __TFT_LIB_H__
#define __TFT_LIB_H__


/*
The version of GCC has changed.
raspberrypi 5.10.103-v7+:gcc (Raspbian 8.3.0-6+rpi1) 8.3.0
raspberrypi 5.15.32-v7+:gcc (Raspbian 10.2.1-6+rpi1) 10.2.1 20210110

"Multiple definition of" on GCC 10.2.1 but not GCC 8.3.0

There was a change in behaviour about global variables between GCC 8.3.0 and GCC 10.2.1.
In C you are supposed to only define a global variable in one translation unit,
other translation unit that want to access the variable should declare it as "extern".

GCC 8.3.0 has a collaborative definition, so no error occurs.
But GCC 10.2.1 don't have cooperative definition, so error occurs.
*/

#ifndef __TFT_LIB_C__
    #define DECLARE extern
#else
    #define DECLARE
#endif

#include "fontx.h"

#define RED				0xf800
#define GREEN			0x07e0
#define BLUE			0x001f
#define BLACK			0x0000
#define WHITE			0xffff
#define GRAY			0x8c51
#define YELLOW			0xFFE0
#define CYAN			0x07FF
#define PURPLE			0xF81F

#define DIRECTION0		0
#define DIRECTION90		1
#define DIRECTION180	2
#define DIRECTION270	3

#define TFTLCD_DELAY   0xFFFF
#define TFTLCD_DELAY16 0xFFFF
#define TFTLCD_DELAY8  0x7F

typedef struct {
	int rst;
	int cs;
	int rs;
	int wr;
	int rd;
	int d0;
	int d1;
	int d2;
	int d3;
	int d4;
	int d5;
	int d6;
	int d7;
	int d8;
	int d9;
	int d10;
	int d11;
	int d12;
	int d13;
	int d14;
	int d15;
} GPIO_t;

typedef struct {
	uint16_t _model;
	uint16_t _width;
	uint16_t _height;
	uint16_t _offsetx;
	uint16_t _offsety;
	int16_t _font_direction;
	int16_t _font_fill;
	uint16_t _font_fill_color;
	int16_t _font_underline;
	uint16_t _font_underline_color;
	GPIO_t	pins;
	int16_t _delay;
	bool _debug;
} TFT_t;

// Driver dependent function
DECLARE void (*DrawPixel)(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
DECLARE void (*DrawMultiPixels)(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
DECLARE void (*DrawFillRect)(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
DECLARE void (*DisplayOff)(TFT_t * dev);
DECLARE void (*DisplayOn)(TFT_t * dev);
DECLARE void (*InversionOff)(TFT_t * dev);
DECLARE void (*InversionOn)(TFT_t * dev);

// Global function
void lcdDelay(int ms);
void lcdWriteByte(TFT_t *dev, uint8_t data);
void lcdWriteWord(TFT_t *dev, uint16_t data);
void lcdWriteDataWord(TFT_t *dev, uint16_t data);
void lcdWriteDataWord_16bit(TFT_t *dev, uint16_t data);
void lcdWriteDataByte(TFT_t *dev, uint8_t data);
void lcdWriteCommandWord(TFT_t *dev, uint16_t command);
void lcdWriteCommandByte(TFT_t *dev, uint8_t command);
void lcdWriteRegisterWord(TFT_t *dev, uint16_t addr, uint16_t data);
void lcdWriteRegisterByte(TFT_t *dev, uint8_t addr, uint16_t data);
void lcdInterface(TFT_t *dev, char* ppath);
void lcdInit(TFT_t *dev, uint16_t model, int width, int height, int offsetx, int offsety);
void lcdReset(TFT_t *dev);
void lcdInitTable(TFT_t *dev, const void *table, int16_t size);
void lcdInitTable16(TFT_t *dev, const void *table, int16_t size);
void lcdDrawPixel(TFT_t *dev, uint16_t x, uint16_t y, uint16_t color);
void lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDisplayOff(TFT_t *dev);
void lcdDisplayOn(TFT_t *dev);
void lcdInversionOn(TFT_t *dev);
void lcdInversionOff(TFT_t *dev);
void lcdFillScreen(TFT_t *dev, uint16_t color);
void lcdFadeout(TFT_t *dev, int direction, uint16_t color);
void lcdDrawLine(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawRoundRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
void lcdDrawArrow(TFT_t *dev, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color);
void lcdDrawFillArrow(TFT_t *dev, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color);
uint16_t rgb565_conv(uint16_t r,uint16_t g,uint16_t b);
int lcdDrawSJISChar(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color);
int lcdDrawUTF8Char(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color);
int lcdDrawUTF8String(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,unsigned char *utfs,uint16_t color);
void lcdSetFontDirection(TFT_t *dev, uint16_t dir);
void lcdSetFontFill(TFT_t *dev, uint16_t color);
void lcdUnsetFontFill(TFT_t *dev);
void lcdSetFontUnderLine(TFT_t *dev, uint16_t color);
void lcdUnsetFontUnderLine(TFT_t *dev);
int ReadPinConfig(GPIO_t *pin, char *path);


#endif
