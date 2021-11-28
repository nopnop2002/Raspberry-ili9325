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

#include "driver/ili9341.h"
#define DRIVER_NAME "ILI9341"
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT 320
#define OFFSET_X 0
#define OFFSET_Y 0
#define INIT_FUNCTION(a, b, c, d, e) ili9341_lcdInit(a, b, c, d, e)

#define _DEBUG_ 0

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
		printf("width=%d height=%d\n", width, height);
		printf("imageWidth=%d imageHeight=%d\n", imageWidth, imageHeight);

		uint16_t jpegWidth = width;
		uint16_t offsetX = 0;
		if (width > imageWidth) {
			jpegWidth = imageWidth;
			offsetX = (width - imageWidth) / 2;
		}
		printf("jpegWidth=%d offsetX=%d\n", jpegWidth, offsetX);

		uint16_t jpegHeight = height;
		uint16_t offsetY = 0;
		if (height > imageHeight) {
			jpegHeight = imageHeight;
			offsetY = (height - imageHeight) / 2;
		}
		printf("jpegHeight=%d offsetY=%d\n", jpegHeight, offsetY);
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


	char file[64];
	//strcpy(file, "./images/RaspberryPi_logo.jpeg");
	//strcpy(file, "./images/Ubuntu_log.jpeg");
	strcpy(file, "./images/dailyImage.jpeg");
	JPEGTest(&dev, file, SCREEN_WIDTH, SCREEN_HEIGHT);
	sleep(5);

	//lcdFadeout(&dev, DIRECTION90, BLACK);
}
