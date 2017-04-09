#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "ili9325.h"

#define LCD_RST 7
#define LCD_CS  8
#define LCD_RS  9
#define LCD_WR 10
#define LCD_RD 11

#define SR595_DATA  4
#define SR595_CLOCK 5
#define SR595_LATCH  6
#define ORDER LSBFIRST

#define XMAX    240
#define YMAX    320

void lcdWrite8(uint16_t data) {
  int  bit;
  digitalWrite (SR595_LATCH, LOW) ;
  if (ORDER == LSBFIRST) {
    for (bit = 8 - 1 ; bit >= 0 ; --bit) {
      digitalWrite (SR595_DATA, data & (1 << bit)) ;
      digitalWrite (SR595_CLOCK, HIGH) ;
      digitalWrite (SR595_CLOCK, LOW) ;
    }
  } else {
    for (bit = 0 ; bit < 8 ; bit++) {
      digitalWrite (SR595_DATA, data & (1 << bit)) ;
      digitalWrite (SR595_CLOCK, HIGH) ;
      digitalWrite (SR595_CLOCK, LOW) ;
    }
  }
  digitalWrite (SR595_LATCH, HIGH) ;
}

void lcdWriteData(uint16_t data) {
//  lcdSetWriteDir();
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_WR, HIGH);
  
  lcdWrite8(data >> 8);
  
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
  digitalWrite(LCD_WR, HIGH);
  
  lcdWrite8(data);
  
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
  digitalWrite(LCD_WR, HIGH);
  
  digitalWrite(LCD_CS, HIGH);  
}

void lcdWriteCommand(uint16_t command) {
//  lcdSetWriteDir(); 
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_WR, HIGH);  
  lcdWrite8(command >> 8);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
  digitalWrite(LCD_WR, HIGH);
  lcdWrite8(command);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, HIGH);    
}


void lcdWriteRegister(uint16_t addr, uint16_t data) {
  lcdWriteCommand(addr);
  lcdWriteData(data);
}


void lcdInit(void) {
  pinMode(LCD_CS, OUTPUT);
  digitalWrite(LCD_CS, HIGH);
  pinMode(LCD_RS, OUTPUT);
  digitalWrite(LCD_RS, HIGH);
  pinMode(LCD_WR, OUTPUT);
  digitalWrite(LCD_WR, HIGH);
  pinMode(LCD_RD, OUTPUT);
  digitalWrite(LCD_RD, HIGH);
  pinMode(LCD_RST, OUTPUT);
  digitalWrite(LCD_RST, HIGH);  

  pinMode(SR595_DATA,OUTPUT);
  pinMode(SR595_CLOCK,OUTPUT);
  pinMode(SR595_LATCH,OUTPUT);

}

void lcdReset(void) {
  digitalWrite(LCD_RST, LOW);
  delay(2); 
  digitalWrite(LCD_RST, HIGH);
  lcdWriteData(0);
  lcdWriteData(0);
  lcdWriteData(0);
  lcdWriteData(0);
}


void lcdRegister9325(void) {
  lcdWriteRegister(0x00e5,0x8000);
  lcdWriteRegister(0x0000,0x0001);
  lcdWriteRegister(0x0001,0x0100);
  lcdWriteRegister(0x0002,0x0700);
  lcdWriteRegister(0x0003,0x1030);
  lcdWriteRegister(0x0004,0x0000);
  lcdWriteRegister(0x0008,0x0202);
  lcdWriteRegister(0x0009,0x0000);
  lcdWriteRegister(0x000a,0x0000);
  lcdWriteRegister(0x000c,0x0000);
  lcdWriteRegister(0x000d,0x0000);
  lcdWriteRegister(0x000f,0x0000);
  lcdWriteRegister(0x0010,0x0000);
  lcdWriteRegister(0x0011,0x0000);
  lcdWriteRegister(0x0012,0x0000);
  lcdWriteRegister(0x0013,0x0000);
  lcdWriteRegister(0x0010,0x17b0);
  lcdWriteRegister(0x0011,0x0037);
  lcdWriteRegister(0x0012,0x0138);
  lcdWriteRegister(0x0013,0x1700);
  lcdWriteRegister(0x0029,0x000d);
  lcdWriteRegister(0x0020,0x0000);
  lcdWriteRegister(0x0021,0x0000);
  lcdWriteRegister(0x0030,0x0001);
  lcdWriteRegister(0x0031,0x0606);
  lcdWriteRegister(0x0032,0x0304);
  lcdWriteRegister(0x0033,0x0202);
  lcdWriteRegister(0x0034,0x0202);
  lcdWriteRegister(0x0035,0x0103);
  lcdWriteRegister(0x0036,0x011d);
  lcdWriteRegister(0x0037,0x0404);
  lcdWriteRegister(0x0038,0x0404);
  lcdWriteRegister(0x0039,0x0404);
  lcdWriteRegister(0x003c,0x0700);
  lcdWriteRegister(0x003d,0x0a1f);
  lcdWriteRegister(0x0050,0x0000);
  lcdWriteRegister(0x0051,0x00ef);
  lcdWriteRegister(0x0052,0x0000);
  lcdWriteRegister(0x0053,0x013f);
  lcdWriteRegister(0x0060,0x2700);
  lcdWriteRegister(0x0061,0x0001);
  lcdWriteRegister(0x006a,0x0000);
  lcdWriteRegister(0x0090,0x0010);
  lcdWriteRegister(0x0092,0x0000);
  lcdWriteRegister(0x0093,0x0003);
  lcdWriteRegister(0x0095,0x0101);
  lcdWriteRegister(0x0097,0x0000);
  lcdWriteRegister(0x0098,0x0000);
  lcdWriteRegister(0x0007,0x0021);
  lcdWriteRegister(0x0007,0x0031);
  lcdWriteRegister(0x0007,0x0173);
}


//pixel drawing
//x:X coordinate
//y:Y coordinate
//color:color
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  lcdWriteRegister(0x0020, x); // RAM Address Set 1
  lcdWriteRegister(0x0021, y); // RAM Address Set 2
  lcdWriteRegister(0x0022, color); // Write Data to GRAM
}

//angle drawing
//x1:Start X coordinate
//y1:Start Y coordinate
//x2:End X coordinate
//y2:End Y coordinate
//color:color
void lcdDrawAngle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  int i,j;
  if (x1 > XMAX) return;
  if (x1 < 0) x1=0;
  if (x2 > XMAX) x2=XMAX;
  if (y1 > YMAX) return;
  if (y1 < 0) y1=0;
  if (y2 > YMAX) y2=YMAX;

#if 0
  for(i=x1;i<x2+1;i++){
    lcdWriteRegister(0x0020, i); // RAM Address Set 1
    lcdWriteRegister(0x0021, y1); // RAM Address Set 2
    lcdWriteCommand(0x0022); // Write Data to GRAM
    for(j=y1;j<y2+1;j++){
      lcdWriteData(color); // Write Data to GRAM
    }
  }
#endif
  for(i=x1;i<x2+1;i++){
    for(j=y1;j<y2+1;j++){
      lcdWriteRegister(0x0020, i); // RAM Address Set 1
      lcdWriteRegister(0x0021, j); // RAM Address Set 2
      lcdWriteRegister(0x0022, color); // Write Data to GRAM
    }
  }
}

//fill screen
//color:color
void lcdFillScreen(uint16_t color) {
  lcdWriteRegister(0x0020, 0); // RAM Address Set 1
  lcdWriteRegister(0x0021, 0); // RAM Address Set 2
  lcdWriteCommand(0x0022); // Write Data to GRAM
  
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_WR, HIGH);
  
  uint32_t i = 320;
  i *=240;

  while( i-- ) {
    lcdWrite8(color >> 8);
    digitalWrite(LCD_WR, LOW);
    delayMicroseconds(10);
    digitalWrite(LCD_WR, HIGH); 
    lcdWrite8(color);
    digitalWrite(LCD_WR, LOW);
    delayMicroseconds(10);
    digitalWrite(LCD_WR, HIGH); 
  }
  
  digitalWrite(LCD_CS, HIGH); 
  
}

//line drawing
//x1:Start X coordinate
//y1:Start Y coordinate
//x2:End X coordinate
//y2:End Y coordinate
//color:color 
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  int i;
  int dx,dy;
  int sx,sy;
  int E;

  /* 二点間の距離 */
  dx = ( x2 > x1 ) ? x2 - x1 : x1 - x2;
  dy = ( y2 > y1 ) ? y2 - y1 : y1 - y2;

  /* 二点の方向 */
  sx = ( x2 > x1 ) ? 1 : -1;
  sy = ( y2 > y1 ) ? 1 : -1;

  /* 傾きが1より小さい場合 */
  if ( dx > dy ) {
    E = -dx;
    for ( i = 0 ; i <= dx ; i++ ) {
      lcdDrawPixel( x1, y1, color );
      x1 += sx;
      E += 2 * dy;
      if ( E >= 0 ) {
        y1 += sy;
        E -= 2 * dx;
      }
    }
  /* 傾きが1以上の場合 */
  } else {
    E = -dy;
    for ( i = 0 ; i <= dy ; i++ ) {
      lcdDrawPixel( x1, y1, color );
      y1 += sy;
      E += 2 * dx;
      if ( E >= 0 ) {
        x1 += sx;
        E -= 2 * dy;
      }
    }
  }
}

// Rectangular drawing
//x1:Start X coordinate
//y1:Start Y coordinate
//x2:End   X coordinate
//y2:End   Y coordinate
//color:color
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  lcdDrawLine(x1,y1,x2,y1,color);
  lcdDrawLine(x2,y1,x2,y2,color);
  lcdDrawLine(x2,y2,x1,y2,color);
  lcdDrawLine(x1,y2,x1,y1,color);
}

//Circular drawing
//x0:Central X coordinate
//y0:Central Y coordinate
//r:radius
//color:color
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  
  x=0;
  y=-r;
  err=2-2*r;
  do{
    lcdDrawPixel(x0-x,y0+y,color); 
    lcdDrawPixel(x0-y,y0-x,color); 
    lcdDrawPixel(x0+x,y0-y,color); 
    lcdDrawPixel(x0+y,y0+x,color); 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
  
}

//Circular drawing of filling
//x0:Central X coordinate
//y0:Central Y coordinate
//r:radius
//color:color
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  int ChangeX;
  
  x=0;
  y=-r;
  err=2-2*r;
  ChangeX=1;
  do{
    if(ChangeX) {
      lcdDrawLine(x0-x,y0-y,x0-x,y0+y,color);
      lcdDrawLine(x0+x,y0-y,x0+x,y0+y,color);
    } // if
    ChangeX=(old_err=err)<=x;
    if (ChangeX)            err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;
  } while(y<=0);
    
} 

//A corner is round rectangular drawing
//x1:Start X coordinate
//y1:Start Y coordinate
//x2:End   X coordinate
//y2:End   Y coordinate
//r:radius
//color:color
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  unsigned char temp;

  if(x1>x2) {
    temp=x1; x1=x2; x2=temp;
  } // if
  
  if(y1>y2) {
    temp=y1; y1=y2; y2=temp;
  } // if
  
  
  x=0;
  y=-r;
  err=2-2*r;

  do{
    if(x) {
      lcdDrawPixel(x1+r-x,y1+r+y,color); 
      lcdDrawPixel(x2-r+x,y1+r+y,color); 
      lcdDrawPixel(x1+r-x,y2-r-y,color); 
      lcdDrawPixel(x2-r+x,y2-r-y,color);
    } // if 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
   
  lcdDrawLine(x1+r,y1  ,x2-r,y1  ,color);
  lcdDrawLine(x1+r,y2  ,x2-r,y2  ,color);
  lcdDrawLine(x1  ,y1+r,x1  ,y2-r,color);
  lcdDrawLine(x2  ,y1+r,x2  ,y2-r,color);  
} 

// RGB565 conversion
// RGB565 is R(5)+G(6)+B(5)=16bit color format.
// Bit image "RRRRRGGGGGGBBBBB"
//
unsigned int rgb565_conv(uint16_t r,uint16_t g,uint16_t b)
{
   unsigned int RR,GG,BB;
   RR = (r * 31 / 255) << 11;
   GG = (g * 63 / 255) << 5;
   BB = (b * 31 / 255);
   return(RR | GG | BB);
}
