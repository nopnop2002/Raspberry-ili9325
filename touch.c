#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "ili93xx.h"
#include "xpt2046.h"

#define SPI_CHANNEL 0 // /dev/spidev0.0
//#define SPI_CHANNEL 1 // /dev/spidev0.1
#define GPIO_PEN 6
#define _DEBUG_ 0

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];
FontxFile fxG16[2];
FontxFile fxM16[2];

TouchInfo tinfo;

int main(int argc, char **argv){
  int i,j;
  uint16_t xpos,ypos;
  uint16_t color;
  uint8_t utf[64];

  int XMAX,YMAX;
  int XMAX2,YMAX2;

  if (wiringPiSPISetup(SPI_CHANNEL, 500000) < 0) {
    printf("wiringPiSPISetup failed:\n");
    return -1;
  }

  if (wiringPiSetup() < 0) {
    printf("wiringPiSetup failed:\n");
    return -1;
  }
  pinMode(GPIO_PEN,INPUT);


  char base[128];
  strcpy(base, argv[0]);
  for(i=strlen(base);i>0;i--) {
    if (base[i-1] == '/') {
      base[i] = 0;
      break;
    }
  }
//printf("base=%s\n",base);

  // You can change font file
  // 32Dot Gothic
  char G32[2][128];
  strcpy(G32[0],base);
  strcpy(G32[1],base);
  strcat(G32[0],"fontx/ILGH32XB.FNT");
  strcat(G32[1],"fontx/ILGZ32XB.FNT");
//printf("%s\n",G32[0]);
//printf("%s\n",G32[1]);
  Fontx_init(fxG32,G32[0],G32[1]);

  // 32Dot Mincho
  char M32[2][128];
  strcpy(M32[0],base);
  strcpy(M32[1],base);
  strcat(M32[0],"fontx/ILMH32XF.FNT");
  strcat(M32[1],"fontx/ILMZ32XF.FNT");
//printf("%s\n",M32[0]);
//printf("%s\n",M32[1]);
  Fontx_init(fxM32,M32[0],M32[1]);

  // 24Dot Gothic
  char G24[2][128];
  strcpy(G24[0],base);
  strcpy(G24[1],base);
  strcat(G24[0],"fontx/ILGH24XB.FNT");
  strcat(G24[1],"fontx/ILGZ24XB.FNT");
//printf("%s\n",G24[0]);
//printf("%s\n",G24[1]);
  Fontx_init(fxG24,G24[0],G24[1]);

  // 24Dot Mincho
  char M24[2][128];
  strcpy(M24[0],base);
  strcpy(M24[1],base);
  strcat(M24[0],"fontx/ILMH24XF.FNT");
  strcat(M24[1],"fontx/ILMZ24XF.FNT");
//printf("%s\n",M24[0]);
//printf("%s\n",M24[1]);
  Fontx_init(fxM24,M24[0],M24[1]);

#if 0
  // 32Dot Gothic
  Fontx_init(fxG32,"./fontx/ILGH32XB.FNT","./fontx/ILGZ32XB.FNT");
  // 32Dot Mincho
  Fontx_init(fxM32,"./fontx/ILMH32XF.FNT","./fontx/ILMZ32XF.FNT");
  // 24Dot Gothic
  Fontx_init(fxG24,"./fontx/ILGH24XB.FNT","./fontx/ILGZ24XB.FNT");
  // 24Dot Mincho
  Fontx_init(fxM24,"./fontx/ILMH24XF.FNT","./fontx/ILMZ24XF.FNT");
  // 16Dot Gothic
  Fontx_init(fxG16,"./fontx/ILGH16XB.FNT","./fontx/ILGZ16XB.FNT");
  // 16Dot Mincho
  Fontx_init(fxM16,"./fontx/ILMH16XB.FNT","./fontx/ILMZ16XF.FNT");
#endif

  char ppath[128];
  strcpy(ppath,base);
  strcat(ppath,"pin.conf");
//printf("ppath=%s\n",ppath);

  xptInit(&tinfo);

  XMAX = 240;
  YMAX = 320;
  lcdInit(0x9341,XMAX,YMAX,ppath);

  XMAX2 = XMAX - 1;
  YMAX2 = YMAX - 1;

  lcdReset();
  lcdSetup();

  //drawString
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);
  xpos = 180;
  ypos = YMAX2-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i);
    xptDump(&tinfo);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 48;
    utf[1] = 0;
    ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }

  xpos = 120;
  ypos = YMAX2-(32*1);
  for (i=0;i<5;i++) {
    lcdDrawRect(xpos-8, ypos+16, xpos+40, ypos-32, BLACK);
    xptSetPoint(&tinfo, xpos-8 ,ypos+16 ,xpos+40, ypos-32, i+5);
    xptDump(&tinfo);
    color = BLACK;
//    strcpy(utf,"1");
    utf[0] = i + 53;
    utf[1] = 0;
    ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
    ypos = ypos - 32;
  }


  // read xpt2046
  int x, y;
  int pen_irq;
  int id;

  xpos = 40;
  ypos = YMAX2-(32*1);
  color = RED;
  for (;;) {
    usleep(10000);      /* do it anyway ; settle time when pen goes up */
    pen_irq = digitalRead(GPIO_PEN);
    if (pen_irq == LOW) { /* PenIRQ is LOW : touch! pen is down */
      id = xptGetPoint(SPI_CHANNEL, &tinfo);
      if (id != -1) {
if(_DEBUG_)printf("id=%d\n",id);
        lcdInit(0x9341,XMAX,YMAX,ppath);
        lcdSetFontDirection(DIRECTION90);
        utf[0] = id + 48;
        ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
      }
    }
  }

}
