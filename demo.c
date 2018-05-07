#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include "ili93xx.h"

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];

#define _DEBUG_ 1

void inputKey() {
  char ch;
  printf("Hit any key");
  scanf("%c",&ch);
}


int main()
{
  int i,j;
  uint16_t Y1,Y2;
  uint16_t X1,X2;
  uint16_t xpos,ypos;
  uint16_t color;
  uint16_t size;
  uint8_t utf[10];

  if(wiringPiSetup() == -1) {
    printf("Setup Fail\n");
    return 1;
  }

  // You can change font file
  // 32Dot Gothic
  Fontx_init(fxG32,"./fontx/ILGH32XB.FNT","./fontx/ILGZ32XB.FNT");
  // 32Dot Mincho
  Fontx_init(fxM32,"./fontx/ILMH32XF.FNT","./fontx/ILMZ32XF.FNT");
  // 24Dot Gothic
  Fontx_init(fxG24,"./fontx/ILGH24XB.FNT","./fontx/ILGZ24XB.FNT");
  // 24Dot Mincho
  Fontx_init(fxM24,"./fontx/ILMH24XF.FNT","./fontx/ILMZ24XF.FNT");

  int XMAX,YMAX;
  int XMAX2,YMAX2;
  char model[10];

#ifdef ILI9325
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9325");
  printf("mode:%s\n",model);
  lcdInit(0x9325,XMAX,YMAX);
#endif

#ifdef SPFD5408
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"SPFD5408");
  printf("mode:%s\n",model);
  lcdInit(0x5408,XMAX,YMAX);
#endif

#ifdef R61505U
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"R61505U");
  printf("mode:%s\n",model);
  lcdInit(0x1505,XMAX,YMAX);
#endif

#ifdef ILI9341
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9341");
  printf("mode:%s\n",model);
  lcdInit(0x9341,XMAX,YMAX);
#endif

#ifdef ILI9342
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9342");
  printf("mode:%s\n",model);
  lcdInit(0x9342,XMAX,YMAX);
#endif

#ifdef ILI9481
  XMAX = 320;
  YMAX = 480;
  strcpy(model,"ILI9481");
  printf("mode:%s\n",model);
  lcdInit(0x9481,XMAX,YMAX);
#endif

#ifdef S6D1121
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"S6D1121");
  printf("mode:%s\n",model);
  lcdInit(0x1121,XMAX,YMAX);
#endif

  XMAX2 = XMAX - 1;
  YMAX2 = YMAX - 1;

  lcdReset();
  lcdSetup();

  //color bar
  X1=XMAX/3;
  X2=(XMAX/3)*2;
  Y1=YMAX/3;
  Y2=(YMAX/3)*2;
  for(i=0;i<XMAX;i++){
    for(j=0;j<YMAX;j++){
    if(j<Y1){
      lcdDrawPixel(i,j,RED);
    } else if(j<Y2) {
      lcdDrawPixel(i,j,GREEN);
    } else {
      lcdDrawPixel(i,j,BLUE);
    }
   }
  }
if(_DEBUG_)inputKey();
  
  for(i=0;i<2;i++) {
    lcdDisplayOff();
    sleep(1);
    lcdDisplayOn();
    sleep(1);
  }
if(_DEBUG_)inputKey();

  //coner angle
  lcdFillScreen(WHITE);
  color = RED;
  lcdDrawFillRect(0,0,20,20,color);
  lcdDrawFillArrow(30,30,20,20,10,color);
  xpos = 30;
  ypos = 30;
  strcpy(utf,"0,0");
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = GREEN;
  lcdDrawFillRect(XMAX-20,0,XMAX,20,color);
  lcdDrawFillArrow(XMAX-30,30,XMAX-20,20,10,color);
//  xpos = 130;
  xpos = XMAX-110;
  ypos = 30;
//  strcpy(utf,"239,0");
  sprintf(utf,"%d,0",XMAX2);
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = BLUE;
  lcdDrawFillRect(0,YMAX-20,20,YMAX,color);
  lcdDrawFillArrow(30,YMAX-30,20,YMAX-20,10,color);
  xpos = 30;
//  ypos = 250;
  ypos = YMAX-70;
//  strcpy(utf,"0,319");
  sprintf(utf,"%d,0",YMAX2);
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = PURPLE;
  lcdDrawFillRect(XMAX-20,YMAX-20,XMAX,YMAX,color);
  lcdDrawFillArrow(XMAX-30,YMAX-30,XMAX-20,YMAX-20,10,color);
//  xpos = 130;
  xpos = XMAX-110;
//  ypos = 250;
  ypos = YMAX-70;
//  strcpy(utf,"239,319");
  sprintf(utf,"%d,%d",XMAX2,YMAX2);
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
if(_DEBUG_)inputKey();

  //drawLine
  lcdFillScreen(WHITE);
  color=RED;
  for(ypos=0;ypos<YMAX;ypos=ypos+10) {
    lcdDrawLine(0,ypos,XMAX,ypos,color);
  }

  for(xpos=0;xpos<XMAX;xpos=xpos+10) {
    lcdDrawLine(xpos,0,xpos,YMAX,color);
  }
if(_DEBUG_)inputKey();

  //drawRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRect(i,i,XMAX-i,YMAX-i,color);
  }
if(_DEBUG_)inputKey();

  //drawCircle
  lcdFillScreen(WHITE);
  color=BLUE;
  xpos=XMAX/2;
  ypos=YMAX/2;
  for(i=10;i<220;i=i+10) {
    lcdDrawCircle(xpos,ypos,i,color);
  }
if(_DEBUG_)inputKey();

  //drawRoundRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRoundRect(i,i,XMAX-i,YMAX-i,10,color);
  }
if(_DEBUG_)inputKey();

  //drawString
  lcdFillScreen(WHITE);
  xpos = 0;
  ypos = YMAX2-(32*1);
  color = BLACK;
//  strcpy(utf,"ILI93XX");
  strcpy(utf,model);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = 0;
  ypos = YMAX2-(32*2);
  lcdSetFontFill(CYAN);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = 0;
  ypos = YMAX2-(32*3);
  lcdSetFontUnderLine(BLACK);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION180);
  xpos = XMAX2;
  ypos = 32*1;
  color = BLUE;
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2;
  ypos = 32*2;
  lcdSetFontFill(YELLOW);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2;
  ypos = 32*3;
  lcdSetFontUnderLine(BLUE);
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION90);
  xpos = XMAX2-(32*1);
  ypos = YMAX2;
  color = RED;
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*2);
  ypos = YMAX2;
  lcdSetFontFill(GREEN);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = XMAX2-(32*3);
  ypos = YMAX2;
  lcdSetFontUnderLine(RED);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();

  lcdSetFontDirection(DIRECTION270);
  xpos = (32*1);
  ypos = 0;
  color = GREEN;
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = (32*2);
  ypos = 0;
  lcdSetFontFill(YELLOW);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = (32*3);
  ypos = 0;
  lcdSetFontUnderLine(GRAY);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();
if(_DEBUG_)inputKey();

  //draw malti font
  lcdFillScreen(WHITE);
  lcdSetFontDirection(DIRECTION90);

  xpos = XMAX2-(32*1);
  ypos = YMAX2;
  color = BLACK;
  strcpy(utf,"32Dot Gothic");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*2);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*3);
  ypos = YMAX2;
  strcpy(utf,"32Dot Mincho");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*4);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM32, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*0);
  ypos = YMAX2;
  strcpy(utf,"24Dot Gothic");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*1);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxG24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*2);
  ypos = YMAX2;
  strcpy(utf,"24Dot Mincho");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);

  xpos = XMAX2-(32*5)-(24*3);
  strcpy(utf,"ABCDEFGabcdefg");
  xpos = lcdDrawUTF8String(fxM24, xpos, ypos, utf, color);
if(_DEBUG_)inputKey();

  //drawFillRect
  lcdFillScreen(WHITE);
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  srand( (unsigned int)time( NULL ) );
  for(i=1;i<100;i++) {
    red=rand()%255;
    green=rand()%255;
    blue=rand()%255;
    color=rgb565_conv(red,green,blue);
    xpos=rand()%XMAX;
    ypos=rand()%YMAX;
    size=rand()%30;
    lcdDrawFillRect(xpos,ypos,xpos+size,ypos+size,color);
  }
}
