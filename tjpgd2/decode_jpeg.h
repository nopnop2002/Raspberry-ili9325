#pragma once
#include <stdint.h>

#define 	ESP_OK   0
#define 	ESP_FAIL   -1
#define 	ESP_ERR_NO_MEM   0x101
#define 	ESP_ERR_INVALID_ARG   0x102
#define 	ESP_ERR_INVALID_STATE   0x103
#define 	ESP_ERR_INVALID_SIZE   0x104
#define 	ESP_ERR_NOT_FOUND   0x105
#define 	ESP_ERR_NOT_SUPPORTED   0x106
#define 	ESP_ERR_TIMEOUT   0x107
#define 	ESP_ERR_INVALID_RESPONSE   0x108
#define 	ESP_ERR_INVALID_CRC   0x109
#define 	ESP_ERR_INVALID_VERSION   0x10A
#define 	ESP_ERR_INVALID_MAC   0x10B
#define 	ESP_ERR_WIFI_BASE   0x3000
#define 	ESP_ERR_MESH_BASE   0x4000

typedef struct __attribute__((__packed__)) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}pixel_s;


/**
 * @brief Decode the jpeg ``image.jpg`` embedded into the program file into pixel data.
 *
 * @param pixels A pointer to a pointer for an array of rows, which themselves are an array of pixels.
 *        Effectively, you can get the pixel data by doing ``decode_image(&myPixels); pixelval=myPixels[ypos][xpos];``
 * @return - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on succesful decode
 */

uint16_t decode_image(pixel_s ***pixels, char * file, uint16_t width, uint16_t height, uint16_t * imageWidth, uint16_t * imageHeight);

/**
 * @brief Release image memory.
 *
 */

uint16_t release_image(pixel_s ***pixels, uint16_t width, uint16_t height);

