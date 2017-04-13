#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <wiringPi.h>
#include "ili9325.h"

#define XMAX    240
#define YMAX    320

#define XMAX2   239
#define YMAX2   319

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];

#define _DEBUG_ 0

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

  lcdInit();
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
  sleep(1);
if(_DEBUG_)inputKey();

  //coner angle
  lcdFillScreen(WHITE);
  color = RED;
  lcdDrawAngle(0,0,20,20,color);
  lcdDrawFillArrow(30,30,20,20,10,color);
  xpos = 30;
  ypos = 30;
  strcpy(utf,"0,0");
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = GRAY;
  lcdDrawAngle(XMAX-20,0,XMAX,20,color);
  lcdDrawFillArrow(XMAX-30,30,XMAX-20,20,10,color);
  xpos = 130;
  ypos = 30;
  strcpy(utf,"240,0");
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = BLUE;
  lcdDrawAngle(0,YMAX-20,20,YMAX,color);
  lcdDrawFillArrow(30,YMAX-30,20,YMAX-20,10,color);
  xpos = 30;
  ypos = 250;
  strcpy(utf,"0,320");
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  color = PURPLE;
  lcdDrawAngle(XMAX-20,YMAX-20,XMAX,YMAX,color);
  lcdDrawFillArrow(XMAX-30,YMAX-30,XMAX-20,YMAX-20,10,color);
  xpos = 130;
  ypos = 250;
  strcpy(utf,"240,320");
  lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  sleep(1);
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
  sleep(1);
if(_DEBUG_)inputKey();

  //drawRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRect(i,i,XMAX-i,YMAX-i,color);
  }
  sleep(1);
if(_DEBUG_)inputKey();

  //drawCircle
  lcdFillScreen(WHITE);
  color=GRAY;
  xpos=XMAX/2;
  ypos=YMAX/2;
  for(i=10;i<220;i=i+10) {
    lcdDrawCircle(xpos,ypos,i,color);
  }
  sleep(1);
if(_DEBUG_)inputKey();

  //drawRoundRect
  lcdFillScreen(WHITE);
  color=BLUE;
  for(i=10;i<120;i=i+10) {
    lcdDrawRoundRect(i,i,XMAX-i,YMAX-i,10,color);
  }
  sleep(1);
if(_DEBUG_)inputKey();

  //drawString
  lcdFillScreen(WHITE);
  xpos = 0;
  ypos = YMAX2-(32*1);
  color = BLACK;
  strcpy(utf,"ILI9325");
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
  color = GRAY;
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);

  xpos = (32*2);
  ypos = 0;
  lcdSetFontFill(CYAN);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontFill();

  xpos = (32*3);
  ypos = 0;
  lcdSetFontUnderLine(GRAY);
  ypos = lcdDrawUTF8String(fxG32, xpos, ypos, utf, color);
  lcdUnsetFontUnderLine();
  sleep(1);
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

  sleep(1);
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
    lcdDrawAngle(xpos,ypos,xpos+size,ypos+size,color);
  }
}
