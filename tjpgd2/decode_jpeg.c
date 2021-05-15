/*
   jpeg decoder.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


/*
The image used for the effect on the LCD in the SPI master example is stored in flash 
as a jpeg file. This file contains the decode_image routine, which uses the tiny JPEG 
decoder library in ROM to decode this JPEG into a format that can be sent to the display.

Keep in mind that the decoder library cannot handle progressive files (will give 
``Image decoder: jd_prepare failed (8)`` as an error) so make sure to save in the correct
format if you want to use a different image file.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode_jpeg.h"
#include "tjpgd.h"

#define	_DEBUG_	0

//Data that is passed from the decoder function to the infunc/outfunc functions.
typedef struct {
	pixel_s **outData;		// Array of IMAGE_H pointers to arrays of 16-bit pixel values
	int screenWidth;		// Width of the screen
	int screenHeight;		// Height of the screen
	FILE* fp;				// File pointer of jpeg file
} JpegDev;


//Input function for jpeg decoder. Just returns bytes from the inData field of the JpegDev structure.
static size_t infunc(JDEC *decoder, uint8_t *buf, size_t len) {
	JpegDev *jd = (JpegDev *) decoder->device;
	if (_DEBUG_) printf("infunc len=%d fp=%p\n", len, jd->fp);
	int rlen;
	if (buf != NULL) { /* Read nd bytes from the input strem */
		rlen = fread(buf, 1, len, jd->fp);
		if (_DEBUG_) printf("rlen=%d\n",rlen);
	} else { /* Skip nd bytes on the input stream */
		if (_DEBUG_) printf("buff is NULL\n");
		fseek(jd->fp, len, SEEK_CUR);
		rlen = len;
	}
	return rlen;
}

//Output function. Re-encodes the RGB888 data from the decoder as big-endian RGB565 and
//stores it in the outData array of the JpegDev structure.
static int outfunc(JDEC *decoder, void *bitmap, JRECT *rect) {
	JpegDev *jd = (JpegDev *) decoder->device;
	uint8_t *in = (uint8_t *) bitmap;
	if (_DEBUG_) printf("rect->top=%d rect->bottom=%d\n", rect->top, rect->bottom);
	if (_DEBUG_) printf("rect->left=%d rect->right=%d\n", rect->left, rect->right);
	if (_DEBUG_) printf("jd->screenWidth=%d jd->screenHeight=%d\n", jd->screenWidth, jd->screenHeight);

	for (int y = rect->top; y <= rect->bottom; y++) {
		for (int x = rect->left; x <= rect->right; x++) {

			if (y < jd->screenHeight && x < jd->screenWidth) {
				jd->outData[y][x].red = in[0];
				jd->outData[y][x].green = in[1];
				jd->outData[y][x].blue = in[2];
			}

			in += 3;
		}
	}
	return 1;
}

// Specifies scaling factor N for output. The output image is descaled to 1 / 2 ^ N (N = 0 to 3).
// When scaling feature is disabled (JD_USE_SCALE == 0), it must be 0.
uint8_t getScale(uint16_t screenWidth, uint16_t screenHeight, uint16_t imageWidth, uint16_t imageHeight) {
	if (screenWidth >= imageWidth && screenHeight >= imageHeight)  return 0;

	if (_DEBUG_) printf("screenWidth=%d imageWidth=%d\n", screenWidth, imageWidth);
	if (_DEBUG_) printf("screenHeight=%d imageHeight=%d\n", screenHeight, imageHeight);
	double scaleWidth = (double)imageWidth / (double)screenWidth;
	double scaleHeight = (double)imageHeight / (double)screenHeight;
	if (_DEBUG_) printf("scaleWidth=%f scaleHeight=%f\n", scaleWidth, scaleHeight);
	double scale = scaleWidth;
	if (scaleWidth < scaleHeight) scale = scaleHeight;
	if (_DEBUG_) printf("scale=%f\n", scale);
	//if (scale < 2.0) return 1;
	//if (scale < 4.0) return 2;
	if (scale <= 2.0) return 1;
	if (scale <= 4.0) return 2;
	return 3;

}

//Size of the work space for the jpeg decoder.
#define WORKSZ 3100

//Decode the embedded image into pixel lines that can be used with the rest of the logic.
uint16_t decode_image(pixel_s ***pixels, char * file, uint16_t width, uint16_t height, uint16_t * imageWidth, uint16_t * imageHeight) {
	char *work = NULL;
	int r;
	JDEC decoder;
	JpegDev jd;
	*pixels = NULL;
	uint16_t ret = ESP_OK;


	//Alocate pixel memory. Each line is an array of IMAGE_W 16-bit pixels; the `*pixels` array itself contains pointers to these lines.
	*pixels = calloc(height, sizeof(pixel_s *));
	if (*pixels == NULL) {
		printf("Error allocating memory for lines\n");
		ret = ESP_ERR_NO_MEM;
		goto err;
	}
	for (int i = 0; i < height; i++) {
		(*pixels)[i] = malloc(width * sizeof(pixel_s));
		if ((*pixels)[i] == NULL) {
			printf("Error allocating memory for line %d\n", i);
			ret = ESP_ERR_NO_MEM;
			goto err;
		}
	}

	//Allocate the work space for the jpeg decoder.
	work = calloc(WORKSZ, 1);
	if (work == NULL) {
		printf("Cannot allocate workspace\n");
		ret = ESP_ERR_NO_MEM;
		goto err;
	}

	
	//Populate fields of the JpegDev struct.
	jd.outData = *pixels;
	jd.screenWidth = width;
	jd.screenHeight = height;
	jd.fp = fopen(file, "rb");
	if (jd.fp == NULL) {
		printf("Image file not found [%s]\n", file);
		ret = ESP_ERR_NOT_FOUND;
		goto err;
	}
	if (_DEBUG_) printf("jd.fp=%p\n", jd.fp);

	//Prepare and decode the jpeg.
	r = jd_prepare(&decoder, infunc, work, WORKSZ, (void *) &jd);
	if (r != JDR_OK) {
		printf("Image decoder: jd_prepare failed (%d)\n", r);
		ret = ESP_ERR_NOT_SUPPORTED;
		goto err;
	}
	if (_DEBUG_) printf("decoder.width=%d decoder.height=%d\n", decoder.width, decoder.height);

	//Calculate Scaling factor
	uint8_t scale = getScale(width, height, decoder.width, decoder.height);
	if (_DEBUG_) printf("scale=%d\n", scale);

	//Calculate image size
	double factor = 1.0;
	if (scale == 1) factor = 0.5;
	if (scale == 2) factor = 0.25;
	if (scale == 3) factor = 0.125;
	if (_DEBUG_) printf("factor=%f\n",factor);
	*imageWidth = (double)decoder.width * factor;
	*imageHeight = (double)decoder.height * factor;
	if (_DEBUG_) printf("imageWidth=%d imageHeight=%d\n", *imageWidth, *imageHeight);


	r = jd_decomp(&decoder, outfunc, scale);
	if (r != JDR_OK) {
		printf("Image decoder: jd_decode failed (%d)\n", r);
		ret = ESP_ERR_NOT_SUPPORTED;
		goto err;
	}

	//All done! Free the work area (as we don't need it anymore) and return victoriously.
	free(work);
	fclose(jd.fp);
	return ret;
	err:
	//Something went wrong! Exit cleanly, de-allocating everything we allocated.
	if (*pixels != NULL) {
		for (int i = 0; i < height; i++) {
			free((*pixels)[i]);
		}
		free(*pixels);
	}
	free(work);
	return ret;
}


uint16_t release_image(pixel_s ***pixels, uint16_t width, uint16_t height) {
	if (*pixels != NULL) {
		for (int i = 0; i < height; i++) {
			free((*pixels)[i]);
		}
		free(*pixels);
	}
	return ESP_OK;
}

