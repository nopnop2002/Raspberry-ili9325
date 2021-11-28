#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <wiringPi.h>

#include "tft_lib.h"

#include "tjpgd2/tjpgd.h"
#include "tjpgd2/decode_jpeg.h"

#include "pngle/pngle.h"
#include "pngle/decode_png.h"

#ifdef ILI9225
#include "driver/ili9225.h"
#define DRIVER_NAME "ILI9225"
#define SCREEN_WIDTH	176
#define SCREEN_HEIGHT 220
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9225_lcdInit(a, b, c, d, e)

#elif ILI9320
#include "driver/ili9320.h"
#define DRIVER_NAME "ILI9320"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9320_lcdInit(a, b, c, d, e)

#elif ILI9325
#include "driver/ili9325.h"
#define DRIVER_NAME "ILI9325"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9325_lcdInit(a, b, c, d, e)

#elif ILI9327
#include "driver/ili9327.h"
#define DRIVER_NAME "ILI9327"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 400
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9327_lcdInit(a, b, c, d, e)

#elif ILI9340
#include "driver/ili9341.h"
#define DRIVER_NAME "ILI9340"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9341_lcdInit(a, b, c, d, e)

#elif ILI9341
#include "driver/ili9341.h"
#ifndef P16BIT
#define DRIVER_NAME "ILI9341"
#else
#define DRIVER_NAME "ILI9341(16Bit)"
#endif
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9341_lcdInit(a, b, c, d, e)

#elif ILI9342
#include "driver/ili9342.h"
#define DRIVER_NAME "ILI9342"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9342_lcdInit(a, b, c, d, e)

#elif ILI9481
#include "driver/ili9481.h"
#define DRIVER_NAME "ILI9481"
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT 480
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9481_lcdInit(a, b, c, d, e)

#elif ILI9486
#include "driver/ili9486.h"
#define DRIVER_NAME "ILI9486"
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT 480
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9486_lcdInit(a, b, c, d, e)

#elif ILI9488
#include "driver/ili9488.h"
#define DRIVER_NAME "ILI9488"
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT 480
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9488_lcdInit(a, b, c, d, e)

#elif SPFD5408
#include "driver/ili9320.h"
#define DRIVER_NAME "SPFD5408"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9320_lcdInit(a, b, c, d, e)

#elif R61505U
#include "driver/ili9320.h"
#define DRIVER_NAME "R61505U"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9320_lcdInit(a, b, c, d, e)

#elif R61509V
#include "driver/r61509.h"
#define DRIVER_NAME "R61509V"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 400
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) r61509_lcdInit(a, b, c, d, e)

#elif S6D1121
#include "driver/s6d1121.h"
#define DRIVER_NAME "S6D1121"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) s6d1121_lcdInit(a, b, c, d, e)

#elif ST7775
#include "driver/ili9225.h"
#define DRIVER_NAME "ST7775"
#define SCREEN_WIDTH	176
#define SCREEN_HEIGHT 220
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9225_lcdInit(a, b, c, d, e)

#elif ST7781
#include "driver/st7781.h"
#define DRIVER_NAME "ST7781"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) st7781_lcdInit(a, b, c, d, e)

#elif ST7783
#include "driver/st7781.h"
#define DRIVER_NAME "ST7783"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) st7781_lcdInit(a, b, c, d, e)

#elif ST7793
#include "driver/r61509.h"
#define DRIVER_NAME "ST7793"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 400
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) r61509_lcdInit(a, b, c, d, e)

#elif ST7796
#include "driver/ili9486.h"
#define DRIVER_NAME "ST7796"
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT 480
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9486_lcdInit(a, b, c, d, e)

#endif

#define _DEBUG_ 0

//When you'd like to wait by a keyboard entries, enable this line.
//#define WAIT inputKey()

//When you'd like to wait in the waiting time, enable this line.
#define WAIT sleep(5)

void inputKey() {
	char ch;
	printf("Hit any key");
	scanf("%c",&ch);
}

time_t elapsedTime(struct timeval startTime, struct timeval endTime) {
	time_t diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
	suseconds_t diffsub = endTime.tv_usec - startTime.tv_usec;
//printf("diffsec=%ld diffsub=%ld\n",diffsec, diffsub);
	if(diffsub < 0) {
		diffsec--;
		diffsub = (endTime.tv_usec+1000000) - startTime.tv_usec;
	}
	uint16_t diffmsec = diffsub / 1000;
	time_t diff = (diffsec * 1000) + diffmsec;
	return diff;
}

time_t AddressTest(TFT_t * dev, int width, int height, uint16_t color) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	lcdFillScreen(dev, color);
	lcdDrawFillRect(dev, 0, 0, 19, 19, RED);
	lcdDrawFillRect(dev, 20, 20, 39, 39, GREEN);
	lcdDrawFillRect(dev, 40, 40, 59, 59, BLUE);
	lcdDrawFillRect(dev, 60, 60, 79, 79, ~color);

#ifdef INVERT
	sleep(2);
	lcdInversionOff(dev);
	sleep(2);
	lcdInversionOn(dev);
#else
	sleep(2);
	lcdInversionOn(dev);
	sleep(2);
	lcdInversionOff(dev);
#endif

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t ColorBarTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t y1 = height/3;
	uint16_t y2 = (height/3)*2;
	lcdDrawFillRect(dev, 0, 0, width-1, y1-1, RED);
	lcdDrawFillRect(dev, 0, y1-1, width-1, y2-1, GREEN);
	lcdDrawFillRect(dev, 0, y2-1, width-1, height-1, BLUE);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
} 

time_t ArrowTest(TFT_t *dev, FontxFile *fx, char * model, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

	uint16_t xpos;
	uint16_t ypos;
	int stlen;
	unsigned char ascii[10];
	uint16_t color;

	lcdFillScreen(dev, BLACK);
	xpos = ((width - fontHeight) / 2) - 1;
	ypos = height - ((height - (strlen(model) * fontWidth)) / 2) - 1;
	lcdSetFontDirection(dev, DIRECTION90);
if(_DEBUG_)printf("xpos=%d ypos=%d\n",xpos,ypos);
	color = WHITE;
	lcdDrawUTF8String(dev, fx, xpos, ypos, (unsigned char *)model, color);

	lcdSetFontDirection(dev, 0);
	color = RED;
	lcdDrawFillArrow(dev, 10, 10, 0, 0, 5, color);
	strcpy((char *)ascii, "0,0");
	ypos = 15;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);

	color = GREEN;
	lcdDrawFillArrow(dev, width-11, 10, width-1, 0, 5, color);
	sprintf((char *)ascii, "%d,0",width-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawUTF8String(dev, fx, xpos, ypos, ascii, color);

	color = GRAY;
	lcdDrawFillArrow(dev, 10, height-11, 0, height-1, 5, color);
	sprintf((char *)ascii, "0,%d",height-1);
	ypos = (height-11) - (fontHeight) - 10;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);

	color = CYAN;
	lcdDrawFillArrow(dev, width-11, height-11, width-1, height-1, 5, color);
	sprintf((char *)ascii, "%d,%d",width-1, height-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawUTF8String(dev, fx, xpos, ypos, ascii, color);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t LineTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color = RED;
	uint16_t xpos;
	uint16_t ypos;
	for(ypos=0;ypos<height;ypos=ypos+10) {
		lcdDrawLine(dev, 0, ypos, width, ypos, color);
	}

	for(xpos=0;xpos<width;xpos=xpos+10) {
		lcdDrawLine(dev, xpos, 0, xpos, height, color);
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t CircleTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color = GRAY;
	uint16_t xpos = width/2;
	uint16_t ypos = height/2;
	int i;
	for(i=5;i<height;i=i+5) {
		lcdDrawCircle(dev, xpos, ypos, i, color);
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t RoundRectTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t color;
	uint16_t limit = width;
	if (width > height) limit = height;
	lcdFillScreen(dev, BLACK);
	color = BLUE;
	int i;
	for(i=5;i<limit;i=i+5) {
		if (i > (limit-i-1) ) break;
		lcdDrawRoundRect(dev, i, i, (width-i-1), (height-i-1), 10, color);
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t DirectionTest(TFT_t *dev, FontxFile *fx, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	unsigned char ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, DIRECTION0);
	lcdDrawUTF8String(dev, fx, 0, height-fontHeight-1, ascii, color);

	color = BLUE;
	strcpy((char *)ascii, "Direction=180");
	lcdSetFontDirection(dev, DIRECTION180);
	lcdDrawUTF8String(dev, fx, width-1, fontHeight-1, ascii, color);

	color = CYAN;
	strcpy((char *)ascii, "Direction=90");
	lcdSetFontDirection(dev, DIRECTION90);
	lcdDrawUTF8String(dev, fx, width-fontHeight-1, height-1, ascii, color);

	color = GREEN;
	strcpy((char *)ascii, "Direction=270");
	lcdSetFontDirection(dev, DIRECTION270);
	lcdDrawUTF8String(dev, fx, fontHeight-1, 0, ascii, color);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t HorizontalTest(TFT_t *dev, FontxFile *fx, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	unsigned char ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, DIRECTION0);
	uint16_t ypos = height - fontHeight - 1;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);

	lcdSetFontUnderLine(dev, RED);
	ypos = ypos - fontHeight;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	ypos = ypos - fontHeight;
	ypos = ypos - fontHeight;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);

	lcdSetFontUnderLine(dev, RED);
	ypos = ypos - fontHeight;
	lcdDrawUTF8String(dev, fx, 0, ypos, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=2");
	lcdSetFontDirection(dev, DIRECTION180);
	ypos = fontHeight - 1;
	lcdDrawUTF8String(dev, fx, width-1, ypos, ascii, color);

	lcdSetFontUnderLine(dev, BLUE);
	ypos = ypos + fontHeight;
	lcdDrawUTF8String(dev, fx, width-1, ypos, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	ypos = ypos + fontHeight;
	ypos = ypos + fontHeight;
	lcdDrawUTF8String(dev, fx, width-1, ypos, ascii, color);

	lcdSetFontUnderLine(dev, BLUE);
	ypos = ypos + fontHeight;
	lcdDrawUTF8String(dev, fx, width-1, ypos, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t VerticalTest(TFT_t *dev, FontxFile *fx, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	unsigned char ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=1");
	lcdSetFontDirection(dev, DIRECTION90);
	uint16_t xpos = width - fontHeight - 1;
	lcdDrawUTF8String(dev, fx, xpos, height-1, ascii, color);

	lcdSetFontUnderLine(dev, RED);
	xpos = xpos - fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, height-1, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	xpos = xpos - fontHeight;
	xpos = xpos - fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, height-1, ascii, color);

	lcdSetFontUnderLine(dev, RED);
	xpos = xpos - fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, height-1, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=3");
	lcdSetFontDirection(dev, DIRECTION270);
	xpos = fontHeight -1;
	lcdDrawUTF8String(dev, fx, xpos, 0, ascii, color);

	lcdSetFontUnderLine(dev, BLUE);
	xpos = xpos + fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, 0, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	xpos = xpos + fontHeight;
	xpos = xpos + fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, 0, ascii, color);

	lcdSetFontUnderLine(dev, BLUE);
	xpos = xpos + fontHeight;
	lcdDrawUTF8String(dev, fx, xpos, 0, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t FillRectTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	int i;
	for(i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565_conv(red, green, blue);
		uint16_t xpos=rand()%width;
		uint16_t ypos=rand()%height;
		uint16_t size=rand()%(width/5);
		lcdDrawFillRect(dev, xpos, ypos, xpos+size, ypos+size, color);
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

time_t ColorTest(TFT_t *dev, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	uint16_t color;
	lcdFillScreen(dev, WHITE);
	color = RED;
	uint16_t delta = height/16;
	uint16_t ypos = 0;
	int i;
	for(i=0;i<16;i++) {
		lcdDrawFillRect(dev, 0, ypos, width-1, ypos+delta, color);
		color = color >> 1;
		ypos = ypos + delta;
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
} 

time_t JPEGTest(TFT_t * dev, char * file, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	lcdFillScreen(dev, BLACK);

	int _width = width;
	if (width > 240) _width = 240;
	int _height = height;
	if (height > 320) _height = 320;

	pixel_s **pixels;
	uint16_t imageWidth;
	uint16_t imageHeight;
	uint16_t err = decode_image(&pixels, file, width, height, &imageWidth, &imageHeight);
	if (err == ESP_OK) {
		//printf("imageWidth=%d imageHeight=%d\n", imageWidth, imageHeight);

		uint16_t jpegWidth = width;
		uint16_t offsetX = 0;
		if (width > imageWidth) {
			jpegWidth = imageWidth;
			offsetX = (width - imageWidth) / 2;
		}
		//printf("jpegWidth=%d offsetX=%d\n", jpegWidth, offsetX);

		uint16_t jpegHeight = height;
		uint16_t offsetY = 0;
		if (height > imageHeight) {
			jpegHeight = imageHeight;
			offsetY = (height - imageHeight) / 2;
		}
		//printf("jpegHeight=%d offsetY=%d\n", jpegHeight, offsetY);
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * jpegWidth);

		int ypos = (height-1) - offsetY;
		for(int y = 0; y < jpegHeight; y++){
			for(int x = 0;x < jpegWidth; x++){
				pixel_s pixel = pixels[y][x];
				uint16_t color = rgb565_conv(pixel.red, pixel.green, pixel.blue);
				//lcdDrawPixel(dev, x+offsetX, y+offsetY, color);
				lcdDrawPixel(dev, x+offsetX, ypos, color);
			}
			ypos--;
		}

		free(colors);
		release_image(&pixels, _width, _height);
		//printf("Finish\n");
	} else {
		printf("decode_image err=%d imageWidth=%d imageHeight=%d\n", err, imageWidth, imageHeight);
	}

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;

}

time_t PNGTest(TFT_t * dev, char * file, int width, int height) {
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);

	lcdFillScreen(dev, BLACK);

	int _width = width;
	if (width > 240) _width = 240;
	int _height = height;
	if (height > 320) _height = 320;

	// open PNG file
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		printf("File not found [%s]\n", file);
		return 0;
	}

	char buf[1024];
	size_t remain = 0;
	int len;

	pngle_t *pngle = pngle_new(_width, _height);

	pngle_set_init_callback(pngle, png_init);
	pngle_set_draw_callback(pngle, png_draw);
	pngle_set_done_callback(pngle, png_finish);

	double display_gamma = 2.2;
	pngle_set_display_gamma(pngle, display_gamma);


	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			printf("Buffer exceeded\n");
			return 0;
		}

		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			printf("ERROR; %s\n", pngle_error(pngle));
			return 0;
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	fclose(fp);

	uint16_t pngWidth = width;
	uint16_t offsetX = 0;
	if (width > pngle->imageWidth) {
		pngWidth = pngle->imageWidth;
		offsetX = (width - pngle->imageWidth) / 2;
	}
	//printf("pngWidth=%d offsetX=%d\n", pngWidth, offsetX);

	uint16_t pngHeight = height;
	uint16_t offsetY = 0;
	if (height > pngle->imageHeight) {
		pngHeight = pngle->imageHeight;
		offsetY = (height - pngle->imageHeight) / 2;
	}
	//printf("pngHeight=%d offsetY=%d\n", pngHeight, offsetY);
	uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * pngWidth);

	int ypos = (height-1) - offsetY;
	for(int y = 0; y < pngHeight; y++){
		for(int x = 0;x < pngWidth; x++){
			pixel_png pixel = pngle->pixels[y][x];
			uint16_t color = rgb565_conv(pixel.red, pixel.green, pixel.blue);
			//lcdDrawPixel(dev, x+offsetX, y+offsetY, color);
			lcdDrawPixel(dev, x+offsetX, ypos, color);
		}
		ypos--;
	}


	free(colors);
	pngle_destroy(pngle, _width, _height);

	gettimeofday(&endTime, NULL);
	time_t diff = elapsedTime(startTime, endTime);
	printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	return diff;
}

int main(int argc, char **argv){

	if(wiringPiSetup() == -1) {
		printf("wiringPiSetup Fail\n");
		return 1;
	}

	int i;
	char base[128];
	strcpy(base, argv[0]);
	for(i=strlen(base);i>0;i--) {
		if (base[i-1] == '/') {
			base[i] = 0;
			break;
		}
	}
if(_DEBUG_)printf("base=%s\n",base);

	// You can change font file
	// 32Dot Gothic
	char G32[2][128];
	strcpy(G32[0],base);
	strcpy(G32[1],base);
	strcat(G32[0],"fontx/ILGH32XB.FNT");
	strcat(G32[1],"fontx/ILGZ32XB.FNT");
	FontxFile fx32G[2];
	Fontx_init(fx32G,G32[0],G32[1]);

	// 32Dot Mincho
	char M32[2][128];
	strcpy(M32[0],base);
	strcpy(M32[1],base);
	strcat(M32[0],"fontx/ILMH32XF.FNT");
	strcat(M32[1],"fontx/ILMZ32XF.FNT");
	FontxFile fx32M[2];
	Fontx_init(fx32M,M32[0],M32[1]);

	// 24Dot Gothic
	char G24[2][128];
	strcpy(G24[0],base);
	strcpy(G24[1],base);
	strcat(G24[0],"fontx/ILGH24XB.FNT");
	strcat(G24[1],"fontx/ILGZ24XB.FNT");
	FontxFile fx24G[2];
	Fontx_init(fx24G,G24[0],G24[1]);

	// 24Dot Mincho
	char M24[2][128];
	strcpy(M24[0],base);
	strcpy(M24[1],base);
	strcat(M24[0],"fontx/ILMH24XF.FNT");
	strcat(M24[1],"fontx/ILMZ24XF.FNT");
	FontxFile fx24M[2];
	Fontx_init(fx24M,M24[0],M24[1]);

	// 16Dot Gothic
	char G16[2][128];
	strcpy(G16[0],base);
	strcpy(G16[1],base);
	strcat(G16[0],"fontx/ILGH16XB.FNT");
	strcat(G16[1],"fontx/ILGZ16XB.FNT");
	FontxFile fx16G[2];
	Fontx_init(fx16G,G16[0],G16[1]);

	// 16Dot Mincho
	char M16[2][128];
	strcpy(M16[0],base);
	strcpy(M16[1],base);
	strcat(M16[0],"fontx/ILMH16XB.FNT");
	strcat(M16[1],"fontx/ILMZ16XB.FNT");
	FontxFile fx16M[2];
	Fontx_init(fx16M,M16[0],M16[1]);

#if 0
	Fontx_dump(fx32G);
	Fontx_dump(fx24G);
	Fontx_dump(fx32M);
	Fontx_dump(fx24M);
#endif

	char ppath[128];
	strcpy(ppath,base);
	strcat(ppath,"pin.conf");
if(_DEBUG_)printf("ppath=%s\n",ppath);
	struct stat buffer;
	if (stat(ppath, &buffer) != 0) {
		printf("pin.conf [%s] not found\n",ppath);
		return 1;
	}

	TFT_t dev;
	lcdInterface(&dev, ppath);
	lcdReset(&dev);
	INIT_FUNCTION(&dev, SCREEN_WIDTH, SCREEN_HEIGHT, OFFSET_X, OFFSET_Y);

#ifdef INVERT
	lcdInversionOn(&dev);
#endif

	printf("Your TFT controller is %s.\n",DRIVER_NAME); 
	printf("TFT resolution is %d x %d.\n",SCREEN_WIDTH, SCREEN_HEIGHT); 

#if 0
while (1) {
	ArrowTest(&dev, fx32G, DRIVER_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	char file[64];
	strcpy(file, "./images/RaspberryPi_logo.jpeg");
	JPEGTest(&dev, file, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	strcpy(file, "./images/Ubuntu_log.png");
	PNGTest(&dev, file, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;
}
#endif

	AddressTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
	WAIT;

	ColorBarTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	ArrowTest(&dev, fx32G, DRIVER_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	LineTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	CircleTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	RoundRectTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	DirectionTest(&dev, fx24G, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	HorizontalTest(&dev, fx24G, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	VerticalTest(&dev, fx24G, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	FillRectTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	ColorTest(&dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	char file[64];
	strcpy(file, "./images/RaspberryPi_logo.jpeg");
	JPEGTest(&dev, file, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	strcpy(file, "./images/Ubuntu_log.png");
	PNGTest(&dev, file, SCREEN_WIDTH, SCREEN_HEIGHT);
	WAIT;

	//draw multi font
	unsigned char utf[64];
	uint16_t xpos,ypos;
	uint16_t color;

	lcdFillScreen(&dev, CYAN);
	lcdSetFontDirection(&dev, DIRECTION90);

	xpos = SCREEN_WIDTH - 32 - 1;
	ypos = SCREEN_HEIGHT - 1;
	color = BLACK;
	strcpy((char *)utf,"32Dot Gothic");
	lcdDrawUTF8String(&dev, fx32G, xpos, ypos, utf, color);

	xpos = xpos - 32;
	strcpy((char *)utf,"ABCDEFGabcdefg");
	lcdDrawUTF8String(&dev, fx32G, xpos, ypos, utf, color);

	xpos = xpos - 32;
	strcpy((char *)utf,"32Dot Mincho");
	lcdDrawUTF8String(&dev, fx32M, xpos, ypos, utf, color);

	xpos = xpos - 32;
	strcpy((char *)utf,"ABCDEFGabcdefg");
	lcdDrawUTF8String(&dev, fx32M, xpos, ypos, utf, color);

	xpos = xpos - 32;
	strcpy((char *)utf,"24Dot Gothic");
	lcdDrawUTF8String(&dev, fx24G, xpos, ypos, utf, color);

	xpos = xpos - 24;
	strcpy((char *)utf,"ABCDEFGabcdefg");
	lcdDrawUTF8String(&dev, fx24G, xpos, ypos, utf, color);

	xpos = xpos - 24;
	strcpy((char *)utf,"24Dot Mincho");
	lcdDrawUTF8String(&dev, fx24M, xpos, ypos, utf, color);

	xpos = xpos - 24;
	strcpy((char *)utf,"ABCDEFGabcdefg");
	lcdDrawUTF8String(&dev, fx24M, xpos, ypos, utf, color);
	WAIT;

	lcdFadeout(&dev, DIRECTION180, BLACK);
	return 0;
}
