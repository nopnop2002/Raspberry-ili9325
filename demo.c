#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "ili9325.h"

#define XMAX    240
#define YMAX    320

int main()
{
  int i,j;
  uint16_t Y1,Y2;
  uint16_t X1,X2;
  uint16_t xpos,ypos;
  uint16_t color;
  uint16_t size;

  if(wiringPiSetup() == -1) {
    printf("Setup Fail\n");
    return 1;
  }

  lcdSetWriteDir();
  lcdInit();
  lcdReset();
  delay(500);

  lcdRegister9325();

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

  //coner angle
  lcdFillScreen(WHITE);
  lcdDrawAngle(0,0,19,19,RED);
  lcdDrawAngle(XMAX-20,0,XMAX,19,GREEN);
  lcdDrawAngle(0,YMAX-20,19,YMAX,BLUE);
  lcdDrawAngle(XMAX-20,YMAX-20,XMAX,YMAX,CYAN);
  sleep(1);

  //drawLine
  color=RED;
  lcdFillScreen(WHITE);
  for(ypos=0;ypos<YMAX;ypos=ypos+10) {
    lcdDrawLine(0,ypos,XMAX,ypos,color);
  }

  for(xpos=0;xpos<XMAX;xpos=xpos+10) {
    lcdDrawLine(xpos,0,xpos,YMAX,color);
  }
  sleep(1);

  //drawRect
  color=BLUE;
  lcdFillScreen(WHITE);
  for(i=10;i<120;i=i+10) {
    lcdDrawRect(i,i,XMAX-i,YMAX-i,color);
  }
  sleep(1);

  //drawCircle
  color=GRAY;
  lcdFillScreen(WHITE);
  xpos=XMAX/2;
  ypos=YMAX/2;
  for(i=10;i<220;i=i+10) {
    lcdDrawCircle(xpos,ypos,i,color);
  }

  //drawRoundRect
  color=BLUE;
  lcdFillScreen(WHITE);
  for(i=10;i<120;i=i+10) {
    lcdDrawRoundRect(i,i,XMAX-i,YMAX-i,10,color);
  }

  //drawFillRect
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  srand( (unsigned int)time( NULL ) );
  lcdFillScreen(WHITE);
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
