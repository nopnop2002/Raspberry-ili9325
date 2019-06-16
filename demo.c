#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>

#include "ili93xx.h"


#define _DEBUG_ 0

//When you'd like to wait by a keyboard entries, enable this line.
//#define WAIT inputKey()

//When you'd like to wait in the waiting time, enable this line.
#define WAIT sleep(10)

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

time_t ColorBarTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t y1 = height/3;
    uint16_t y2 = (height/3)*2;
    lcdDrawFillRect(0, 0, width-1, y1-1, RED);
    lcdDrawFillRect(0, y1-1, width-1, y2-1, GREEN);
    lcdDrawFillRect(0, y2-1, width-1, height-1, BLUE);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
} 

time_t ArrowTest(FontxFile *fx, char * model, int width, int height) {
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
    uint16_t color;

    lcdFillScreen(BLACK);
    xpos = ((width - fontHeight) / 2) - 1;
    ypos = height - ((height - (strlen(model) * (fontWidth/2))) / 2) - 1;
if(_DEBUG_)printf("xpos=%d ypos=%d\n",xpos,ypos);
    lcdSetFontDirection(DIRECTION90);
    color = WHITE;
    lcdDrawUTF8String(fx, xpos, ypos, (unsigned char *)model, color);

    lcdSetFontDirection(0);
    color = RED;
    unsigned char utf8[10];
    lcdDrawFillArrow(10, 10, 0, 0, 5, color);
    strcpy((char *)utf8, "0,0");
    lcdDrawUTF8String(fx, 0, 20, utf8, color);

    color = GREEN;
    lcdDrawFillArrow(width-11, 10, width-1, 0, 5, color);
    sprintf((char *)utf8, "%d,0",width-1);
    stlen = strlen((char *)utf8);
    xpos = (width-1) - (fontWidth*stlen);
    lcdDrawUTF8String(fx, xpos, 20, utf8, color);

    color = GRAY;
    lcdDrawFillArrow(10, height-11, 0, height-1, 5, color);
    sprintf((char *)utf8, "0,%d",height-1);
    ypos = (height-11) - (fontHeight) - 5;
    lcdDrawUTF8String(fx, 0, ypos, utf8, color);

    color = CYAN;
    lcdDrawFillArrow(width-11, height-11, width-1, height-1, 5, color);
    sprintf((char *)utf8, "%d,%d",width-1, height-1);
    stlen = strlen((char *)utf8);
    xpos = (width-1) - (fontWidth*stlen);
    lcdDrawUTF8String(fx, xpos, ypos, utf8, color);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t LineTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(BLACK);
    color = RED;
    uint16_t xpos;
    uint16_t ypos;
    for(ypos=0;ypos<height;ypos=ypos+10) {
        lcdDrawLine(0, ypos, width, ypos, color);
    }

    for(xpos=0;xpos<width;xpos=xpos+10) {
        lcdDrawLine(xpos, 0, xpos, height, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t CircleTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(BLACK);
    color = GRAY;
    uint16_t xpos = width/2;
    uint16_t ypos = height/2;
    int i;
    for(i=5;i<height;i=i+5) {
        lcdDrawCircle(xpos, ypos, i, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t RoundRectTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    uint16_t limit = width;
    if (width > height) limit = height;
    lcdFillScreen(BLACK);
    color = BLUE;
    int i;
    for(i=5;i<limit;i=i+5) {
        if (i > (limit-i-1) ) break;
        lcdDrawRoundRect(i, i, (width-i-1), (height-i-1), 10, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t DirectionTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    unsigned char utf8[20];

    color = RED;
    strcpy((char *)utf8, "Direction=0");
    lcdSetFontDirection(DIRECTION0);
    lcdDrawUTF8String(fx, 0, height-fontHeight-1, utf8, color);

    color = BLUE;
    strcpy((char *)utf8, "Direction=180");
    lcdSetFontDirection(DIRECTION180);
    lcdDrawUTF8String(fx, width-1, fontHeight-1, utf8, color);

    color = CYAN;
    strcpy((char *)utf8, "Direction=90");
    lcdSetFontDirection(DIRECTION90);
    lcdDrawUTF8String(fx, width-fontHeight-1, height-1, utf8, color);

    color = GREEN;
    strcpy((char *)utf8, "Direction=270");
    lcdSetFontDirection(DIRECTION270);
    lcdDrawUTF8String(fx, fontHeight-1, 0, utf8, color);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t HorizontalTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    unsigned char utf8[20];

    color = RED;
    strcpy((char *)utf8, "Direction=0");
    lcdSetFontDirection(DIRECTION0);
    uint16_t ypos = height - fontHeight - 1;
    lcdDrawUTF8String(fx, 0, ypos, utf8, color);

    lcdSetFontUnderLine(RED);
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, utf8, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(GREEN);
    ypos = ypos - fontHeight;
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, utf8, color);

    lcdSetFontUnderLine(RED);
    ypos = ypos - fontHeight;
    lcdDrawUTF8String(fx, 0, ypos, utf8, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    color = BLUE;
    strcpy((char *)utf8, "Direction=2");
    lcdSetFontDirection(DIRECTION180);
    ypos = fontHeight - 1;
    lcdDrawUTF8String(fx, width-1, ypos, utf8, color);

    lcdSetFontUnderLine(BLUE);
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, utf8, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(YELLOW);
    ypos = ypos + fontHeight;
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, utf8, color);

    lcdSetFontUnderLine(BLUE);
    ypos = ypos + fontHeight;
    lcdDrawUTF8String(fx, width-1, ypos, utf8, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t VerticalTest(FontxFile *fx, int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    // get font width & height
    uint8_t buffer[FontxGlyphBufSize];
    uint8_t fontWidth;
    uint8_t fontHeight;
    GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
if(_DEBUG_)printf("fontWidth=%d fontHeight=%d\n",fontWidth,fontHeight);

    uint16_t color;
    lcdFillScreen(BLACK);
    unsigned char utf8[20];

    color = RED;
    strcpy((char *)utf8, "Direction=1");
    lcdSetFontDirection(DIRECTION90);
    uint16_t xpos = width - fontHeight - 1;
    lcdDrawUTF8String(fx, xpos, height-1, utf8, color);

    lcdSetFontUnderLine(RED);
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, utf8, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(GREEN);
    xpos = xpos - fontHeight;
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, utf8, color);

    lcdSetFontUnderLine(RED);
    xpos = xpos - fontHeight;
    lcdDrawUTF8String(fx, xpos, height-1, utf8, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    color = BLUE;
    strcpy((char *)utf8, "Direction=3");
    lcdSetFontDirection(DIRECTION270);
    xpos = fontHeight -1;
    lcdDrawUTF8String(fx, xpos, 0, utf8, color);

    lcdSetFontUnderLine(BLUE);
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, utf8, color);
    lcdUnsetFontUnderLine();

    lcdSetFontFill(YELLOW);
    xpos = xpos + fontHeight;
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, utf8, color);

    lcdSetFontUnderLine(BLUE);
    xpos = xpos + fontHeight;
    lcdDrawUTF8String(fx, xpos, 0, utf8, color);
    lcdUnsetFontFill();
    lcdUnsetFontUnderLine();

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t FillRectTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(CYAN);

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
        lcdDrawFillRect(xpos, ypos, xpos+size, ypos+size, color);
    }

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
}

time_t ColorTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t color;
    lcdFillScreen(WHITE);
    color = RED;
    uint16_t delta = height/16;
    uint16_t ypos = 0;
    int i;
    for(i=0;i<16;i++) {
        lcdDrawFillRect(0, ypos, width-1, ypos+delta, color);
        color = color >> 1;
        ypos = ypos + delta;
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

  int screenWidth,screenHeight;
  char model[20];

#ifdef ILI9325
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"ILI9325");
  lcdInit(0x9325,screenWidth,screenHeight,ppath);
#endif

#ifdef ILI9327
  screenWidth = 240;
  screenHeight = 400;
  strcpy(model,"ILI9327");
  lcdInit(0x9327,screenWidth,screenHeight,ppath);
#endif

#ifdef SPFD5408
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"SPFD5408");
  lcdInit(0x5408,screenWidth,screenHeight,ppath);
#endif

#ifdef R61505U
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"R61505U");
  lcdInit(0x1505,screenWidth,screenHeight,ppath);
#endif

#ifdef ILI9341
  screenWidth = 240;
  screenHeight = 320;
#ifndef P16BIT
  strcpy(model,"ILI9341");
#else
  strcpy(model,"ILI9341(16Bit)");
#endif
  lcdInit(0x9341,screenWidth,screenHeight,ppath);
#endif

#ifdef ILI9342
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"ILI9342");
  lcdInit(0x9342,screenWidth,screenHeight,ppath);
#endif

#ifdef ILI9481
  screenWidth = 320;
  screenHeight = 480;
  strcpy(model,"ILI9481");
  lcdInit(0x9481,screenWidth,screenHeight,ppath);
#endif

#ifdef S6D1121
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"S6D1121");
  lcdInit(0x1121,screenWidth,screenHeight,ppath);
#endif

#ifdef ST7781
  screenWidth = 240;
  screenHeight = 320;
  strcpy(model,"ST7781");
  lcdInit(0x7781,screenWidth,screenHeight,ppath);
#endif

#ifdef R61509V
  screenWidth = 240;
  screenHeight = 400;
  strcpy(model,"R61509V");
  lcdInit(0xB509,screenWidth,screenHeight,ppath);
#endif

  printf("Your TFT controller is %s.\n",model); 
  printf("TFT resolution is %d x %d.\n",screenWidth, screenHeight); 
  lcdReset();
  lcdSetup();

  ColorBarTest(screenWidth, screenHeight);
  WAIT;

  for(i=0;i<2;i++) {
    lcdDisplayOff();
    sleep(1);
    lcdDisplayOn();
    sleep(1);
  }
  WAIT;

  ArrowTest(fx32G, model, screenWidth, screenHeight);
  WAIT;

  LineTest(screenWidth, screenHeight);
  WAIT;

  CircleTest(screenWidth, screenHeight);
  WAIT;

  RoundRectTest(screenWidth, screenHeight);
  WAIT;

  DirectionTest(fx24G, screenWidth, screenHeight);
  WAIT;

  HorizontalTest(fx24G, screenWidth, screenHeight);
  WAIT;

  VerticalTest(fx24G, screenWidth, screenHeight);
  WAIT;

  FillRectTest(screenWidth, screenHeight);
  WAIT;

  ColorTest(screenWidth, screenHeight);
  WAIT;

  //draw multi font
  unsigned char utf[64];
  uint16_t xpos,ypos;
  uint16_t color;

  lcdFillScreen(CYAN);
  lcdSetFontDirection(DIRECTION90);

  xpos = screenWidth - 32 - 1;
  ypos = screenHeight - 1;
  color = BLACK;
  strcpy((char *)utf,"32Dot Gothic");
  lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);

  xpos = xpos - 32;
  strcpy((char *)utf,"ABCDEFGabcdefg");
  lcdDrawUTF8String(fx32G, xpos, ypos, utf, color);

  xpos = xpos - 32;
  strcpy((char *)utf,"32Dot Mincho");
  lcdDrawUTF8String(fx32M, xpos, ypos, utf, color);

  xpos = xpos - 32;
  strcpy((char *)utf,"ABCDEFGabcdefg");
  lcdDrawUTF8String(fx32M, xpos, ypos, utf, color);

  xpos = xpos - 32;
  strcpy((char *)utf,"24Dot Gothic");
  lcdDrawUTF8String(fx24G, xpos, ypos, utf, color);

  xpos = xpos - 24;
  strcpy((char *)utf,"ABCDEFGabcdefg");
  lcdDrawUTF8String(fx24G, xpos, ypos, utf, color);

  xpos = xpos - 24;
  strcpy((char *)utf,"24Dot Mincho");
  lcdDrawUTF8String(fx24M, xpos, ypos, utf, color);

  xpos = xpos - 24;
  strcpy((char *)utf,"ABCDEFGabcdefg");
  lcdDrawUTF8String(fx24M, xpos, ypos, utf, color);

  return 0;
}
