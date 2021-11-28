#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
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

typedef struct {
	char name[32];
	char font[32];
	char file[64];
	unsigned char utf[32];
	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
	uint16_t r;
	uint16_t t;
	uint16_t color;
	uint16_t dir;
} cmd_t;

#define _DEBUG_ 0

int cmdParse(char * buf, cmd_t * hoge) {
	int pos=0;
	int index=0;
	int i;
	char wk[10][64];
	for(i=0;i<strlen(buf);i++) {
		if (buf[i] == ',') {
			//printf("wk=%s\n",wk[index]);
			pos = 0;
			index++;
		} else {
			if (pos < 63) {
				wk[index][pos++] = buf[i];
				wk[index][pos] = 0;
			}
		}
	} // end for
	if (pos != 0) index++;
 
#if 0
	printf("index=%d\n",index);
	for(i=0;i<index;i++) {
		printf("wk[%d]=%s\n",i,wk[i]);
	}
#endif

	if (strcmp(wk[0],"DrawPixel") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->color = strtol(wk[3],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawRect") == 0 ||
		strcmp(wk[0],"DrawFillRect") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->x2 = strtol(wk[3],NULL,0);
		hoge->y2 = strtol(wk[4],NULL,0);
		hoge->color = strtol(wk[5],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawRoundRect") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->x2 = strtol(wk[3],NULL,0);
		hoge->y2 = strtol(wk[4],NULL,0);
		hoge->r = strtol(wk[5],NULL,0);
		hoge->color = strtol(wk[6],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"FillScrren") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->color = strtol(wk[1],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawLine") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->x2 = strtol(wk[3],NULL,0);
		hoge->y2 = strtol(wk[4],NULL,0);
		hoge->color = strtol(wk[5],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawCircle") == 0 || 
		strcmp(wk[0],"DrawFillCircle") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->r = strtol(wk[3],NULL,0);
		hoge->color = strtol(wk[4],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawArrow") == 0 ||
		strcmp(wk[0],"DrawFillArrow") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->x1 = strtol(wk[1],NULL,0);
		hoge->y1 = strtol(wk[2],NULL,0);
		hoge->x2 = strtol(wk[3],NULL,0);
		hoge->y2 = strtol(wk[4],NULL,0);
		hoge->t = strtol(wk[5],NULL,0);
		hoge->color = strtol(wk[6],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"DrawUTF8String") == 0) {
		strcpy(hoge->name,wk[0]);
		strcpy(hoge->font,wk[1]);
		hoge->x1 = strtol(wk[2],NULL,0);
		hoge->y1 = strtol(wk[3],NULL,0);
		strcpy((char *)hoge->utf,wk[4]);
		hoge->color = strtol(wk[5],NULL,0);
		return 1;
	
	} else if (strcmp(wk[0],"SetFontDirection") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->dir = strtol(wk[1],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"SetFontFill") == 0 ||
		strcmp(wk[0],"SetFontUnderLine") == 0) {
		strcpy(hoge->name,wk[0]);
		hoge->color = strtol(wk[1],NULL,0);
		return 1;

	} else if (strcmp(wk[0],"UnsetFontFill") == 0 ||
		strcmp(wk[0],"UnsetFontUnderLine") == 0) {
		strcpy(hoge->name,wk[0]);
		return 1;

	} else if (strcmp(wk[0],"JPEG") == 0) {
		strcpy(hoge->name,wk[0]);
		strcpy(hoge->file,wk[1]);

	} else if (strcmp(wk[0],"PNG") == 0) {
		strcpy(hoge->name,wk[0]);
		strcpy(hoge->file,wk[1]);

	} else {
		return 0;
	}
}


void cmdDump(cmd_t hoge) {
	printf("name=%s\n",hoge.name);
	printf("font=[%s] ",hoge.font);
	printf("file=[%s] ",hoge.file);
	printf("x1=%d ",hoge.x1);
	printf("y1=%d ",hoge.y1);
	printf("x2=%d ",hoge.x2);
	printf("y2=%d ",hoge.y2);
	printf("r=%d ",hoge.r);
	printf("t=%d ",hoge.t);
	printf("utf=%s ",hoge.utf);
	printf("dir=%d ",hoge.dir);
	printf("color=%x\n",hoge.color);
}

int JPEGTest(TFT_t * dev, char * file, int width, int height) {
	//struct timeval startTime, endTime;
	//gettimeofday(&startTime, NULL);

	//lcdFillScreen(dev, BLACK);

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

	//gettimeofday(&endTime, NULL);
	//time_t diff = elapsedTime(startTime, endTime);
	//printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	//return diff;
	return 0;
}


int PNGTest(TFT_t * dev, char * file, int width, int height) {
	//struct timeval startTime, endTime;
	//gettimeofday(&startTime, NULL);

	//lcdFillScreen(dev, BLACK);

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
	printf("pngWidth=%d offsetX=%d", pngWidth, offsetX);

	uint16_t pngHeight = height;
	uint16_t offsetY = 0;
	if (height > pngle->imageHeight) {
		pngHeight = pngle->imageHeight;
		offsetY = (height - pngle->imageHeight) / 2;
	}
	printf("pngHeight=%d offsetY=%d\n", pngHeight, offsetY);
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

	//gettimeofday(&endTime, NULL);
	//time_t diff = elapsedTime(startTime, endTime);
	//printf("%s elapsed time[ms]=%ld\n",__func__, diff);
	//return diff;
	return 0;
}

/*
 Width inquiry : draw width
 Height inquiry : draw height
 Driver inquiry : draw driver
 Execute draw : draw file filename
*/
int main(int argc, char **argv){
	int i;

if(_DEBUG_)printf("argc=%d\n",argc);
	if (argc < 2) {
		printf("%s draw_file_name\n",argv[0]);
		return 1;
	}

	if (argc == 2) {
		if (strcmp(argv[1],"width") == 0) {
			printf("%d\n", SCREEN_WIDTH);
			return 0;
		} else if (strcmp(argv[1],"height") == 0) {
			printf("%d\n", SCREEN_HEIGHT);
			return 0;
		} else if (strcmp(argv[1],"driver") == 0) {
			printf("%s\n", DRIVER_NAME);
			return 0;
		}
	}

	if(wiringPiSetup() == -1) {
		printf("Setup Fail\n");
		return 1;
	}

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
	FontxFile fxG32[2];
	Fontx_init(fxG32,G32[0],G32[1]);

	// 32Dot Mincho
	char M32[2][128];
	strcpy(M32[0],base);
	strcpy(M32[1],base);
	strcat(M32[0],"fontx/ILMH32XF.FNT");
	strcat(M32[1],"fontx/ILMZ32XF.FNT");
	FontxFile fxM32[2];
	Fontx_init(fxM32,M32[0],M32[1]);

	// 24Dot Gothic
	char G24[2][128];
	strcpy(G24[0],base);
	strcpy(G24[1],base);
	strcat(G24[0],"fontx/ILGH24XB.FNT");
	strcat(G24[1],"fontx/ILGZ24XB.FNT");
	FontxFile fxG24[2];
	Fontx_init(fxG24,G24[0],G24[1]);

	// 24Dot Mincho
	char M24[2][128];
	strcpy(M24[0],base);
	strcpy(M24[1],base);
	strcat(M24[0],"fontx/ILMH24XF.FNT");
	strcat(M24[1],"fontx/ILMZ24XF.FNT");
	FontxFile fxM24[2];
	Fontx_init(fxM24,M24[0],M24[1]);

	// 16Dot Gothic
	char G16[2][128];
	strcpy(G16[0],base);
	strcpy(G16[1],base);
	strcat(G16[0],"fontx/ILGH16XB.FNT");
	strcat(G16[1],"fontx/ILGZ16XB.FNT");
	FontxFile fxG16[2];
	Fontx_init(fxG16,G16[0],G16[1]);

	// 16Dot Mincho
	char M16[2][128];
	strcpy(M16[0],base);
	strcpy(M16[1],base);
	strcat(M16[0],"fontx/ILMH16XB.FNT");
	strcat(M16[1],"fontx/ILMZ16XB.FNT");
	FontxFile fxM16[2];
	Fontx_init(fxM16,M16[0],M16[1]);


#if 0
	Fontx_dump(fxG32);
	Fontx_dump(fxG24);
	Fontx_dump(fxM32);
	Fontx_dump(fxM24);
	Fontx_dump(fxM16);
	Fontx_dump(fxM16);
#endif

	// set full path of pin.conf
	char ppath[128];
	strcpy(ppath,base);
	strcat(ppath,"pin.conf");
if(_DEBUG_)printf("ppath=%s\n",ppath);
	struct stat buffer;
	if (stat(ppath, &buffer) != 0) {
		printf("pin.conf [%s] not found\n",ppath);
		return 1;
	}

	// set full path of draw file
	char dpath[128];
	FILE *fp;
	strcpy(dpath,base);
	strcat(dpath,argv[1]);
if(_DEBUG_)printf("dpath=%s\n",dpath);
	if ((fp = fopen(dpath, "r")) == NULL) {
		printf("draw file [%s] not found\n",dpath);
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

	char buf[64];
	cmd_t cmd;
	int blen;

	while ( fgets(buf, sizeof(buf), fp) != NULL ) {
		blen = strlen(buf);
		buf[blen-1] = 0;
if(_DEBUG_)printf("buf=[%s]\n",buf);
		if (strlen(buf) == 0) continue;
		if (cmdParse(buf,&cmd) == 0) continue;
if(_DEBUG_)cmdDump(cmd);
		if (strcmp(cmd.name,"DrawPixel") == 0) {
			lcdDrawPixel(&dev, cmd.x1, cmd.y1, cmd.color);
		} else if (strcmp(cmd.name,"DrawFillRect") == 0) {
			lcdDrawFillRect(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
		} else if (strcmp(cmd.name,"FillScrren") == 0) {
			lcdFillScreen(&dev, cmd.color);
		} else if (strcmp(cmd.name,"DrawLine") == 0) {
			lcdDrawLine(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
		} else if (strcmp(cmd.name,"DrawRect") == 0) {
			lcdDrawRect(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
		} else if (strcmp(cmd.name,"DrawCircle") == 0) {
			lcdDrawCircle(&dev, cmd.x1, cmd.y1, cmd.r, cmd.color);
		} else if (strcmp(cmd.name,"DrawFillCircle") == 0) {
			lcdDrawFillCircle(&dev, cmd.x1, cmd.y1, cmd.r, cmd.color);
		} else if (strcmp(cmd.name,"DrawRoundRect") == 0) {
			lcdDrawRoundRect(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.r, cmd.color);
		} else if (strcmp(cmd.name,"DrawArrow") == 0) {
			lcdDrawArrow(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.t, cmd.color);
		} else if (strcmp(cmd.name,"DrawFillArrow") == 0) {
			lcdDrawFillArrow(&dev, cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.t, cmd.color);
		} else if (strcmp(cmd.name,"DrawUTF8String") == 0) {
			if (strcmp(cmd.font,"G32") == 0)
				lcdDrawUTF8String(&dev, fxG32, cmd.x1, cmd.y1, cmd.utf, cmd.color);
			if (strcmp(cmd.font,"G24") == 0)
				lcdDrawUTF8String(&dev, fxG24, cmd.x1, cmd.y1, cmd.utf, cmd.color);
			if (strcmp(cmd.font,"G16") == 0)
				lcdDrawUTF8String(&dev, fxG16, cmd.x1, cmd.y1, cmd.utf, cmd.color);

			if (strcmp(cmd.font,"M32") == 0)
				lcdDrawUTF8String(&dev, fxM32, cmd.x1, cmd.y1, cmd.utf, cmd.color);
			if (strcmp(cmd.font,"M24") == 0)
				lcdDrawUTF8String(&dev, fxM24, cmd.x1, cmd.y1, cmd.utf, cmd.color);
			if (strcmp(cmd.font,"M16") == 0)
				lcdDrawUTF8String(&dev, fxM16, cmd.x1, cmd.y1, cmd.utf, cmd.color);
		} else if (strcmp(cmd.name,"SetFontDirection") == 0) {
			lcdSetFontDirection(&dev, cmd.dir);
		} else if (strcmp(cmd.name,"SetFontFill") == 0) {
			lcdSetFontFill(&dev, cmd.color);
		} else if (strcmp(cmd.name,"UnsetFontFill") == 0) {
			lcdUnsetFontFill(&dev);
		} else if (strcmp(cmd.name,"SetFontUnderLine") == 0) {
			lcdSetFontUnderLine(&dev, cmd.color);
		} else if (strcmp(cmd.name,"UnsetFontUnderLine") == 0) {
			lcdUnsetFontUnderLine(&dev);
		} else if (strcmp(cmd.name,"JPEG") == 0) {
			JPEGTest(&dev, cmd.file, SCREEN_WIDTH, SCREEN_HEIGHT);
		} else if (strcmp(cmd.name,"PNG") == 0) {
			PNGTest(&dev, cmd.file, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

	} // end while
	fclose(fp);
	return 0;
}
