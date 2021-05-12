#ifndef __R61509_H__
#define __R61509_H__

void r61509_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety);
void r61509_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void r61509_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void r61509_lcdDisplayOff(TFT_t * dev);
void r61509_lcdDisplayOn(TFT_t * dev);
void r61509_lcdInversionOff(TFT_t * dev);
void r61509_lcdInversionOn(TFT_t * dev);

#endif

