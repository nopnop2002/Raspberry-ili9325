/*
 ILI9325 :  8Bit Instruction
 ILI9341 :  8Bit Instruction
 ILI9342 :  8Bit Instruction
 ILI9481 :  8Bit Instruction
 S6D1121 : 16Bit Instruction
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "ili93xx.h"

#define LCD_RST 7
#define LCD_CS  8
#define LCD_RS  9
#define LCD_WR 11
#define LCD_RD 31

#ifndef SR595
#define LCD_D0 21
#define LCD_D1 22
#define LCD_D2 23
#define LCD_D3 24
#define LCD_D4 25
#define LCD_D5 26
#define LCD_D6 27
#define LCD_D7 28
#endif

#ifdef SR595
#define SR595_DATA  4
#define SR595_CLOCK 5
#define SR595_LATCH  6
#define ORDER LSBFIRST
#endif

//#define _width    240
//#define _height    320
#define _DEBUG_ 0

uint16_t _FONT_DIRECTION_;
uint16_t _FONT_FILL_;
uint16_t _FONT_FILL_COLOR_;
uint16_t _FONT_UNDER_LINE_;
uint16_t _FONT_UNDER_LINE_COLOR_;

uint16_t _model;
uint16_t _width;
uint16_t _height;

#ifndef SR595
void lcdWriteByte(uint8_t data) {
  digitalWrite(LCD_D0, data & 1);
  digitalWrite(LCD_D1, (data & 2) >> 1);
  digitalWrite(LCD_D2, (data & 4) >> 2);
  digitalWrite(LCD_D3, (data & 8) >> 3);
  digitalWrite(LCD_D4, (data & 16) >> 4); 
  digitalWrite(LCD_D5, (data & 32) >> 5);
  digitalWrite(LCD_D6, (data & 64) >> 6);
  digitalWrite(LCD_D7, (data & 128) >> 7);  
}
#endif

#ifdef SR595
void lcdWriteByte(uint8_t data) {
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
#endif

void lcdWriteDataWord(uint16_t data) {
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_RD, HIGH);

  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(data >> 8);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(data);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, HIGH);  
}

void lcdWriteDataByte(uint8_t data) {
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_RD, HIGH);
#if 0
  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(data >> 8);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
#endif
  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(data);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, HIGH);  
}

void lcdWriteCommandWord(uint16_t command) {
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_RD, HIGH);

  digitalWrite(LCD_WR, HIGH);  
  lcdWriteByte(command >> 8);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(command);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, HIGH);    
}

void lcdWriteCommandByte(uint8_t command) {
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_RD, HIGH);
#if 0
  digitalWrite(LCD_WR, HIGH);  
  lcdWriteByte(command >> 8);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);
#endif
  digitalWrite(LCD_WR, HIGH);
  lcdWriteByte(command);
  digitalWrite(LCD_WR, LOW);
  delayMicroseconds(10);

  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, HIGH);    
}


void lcdWriteRegisterWord(uint16_t addr, uint16_t data) {
  lcdWriteCommandWord(addr);
  lcdWriteDataWord(data);
}

void lcdWriteRegisterByte(uint8_t addr, uint16_t data) {
  lcdWriteCommandByte(addr);
  lcdWriteDataWord(data);
}


void lcdInit(uint16_t model, uint16_t width, uint16_t height) {
  _model = model;
  _width = width;
  _height = height;
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

#ifndef SR595
  pinMode(LCD_D0, OUTPUT);
  pinMode(LCD_D1, OUTPUT);
  pinMode(LCD_D2, OUTPUT);
  pinMode(LCD_D3, OUTPUT);  
  pinMode(LCD_D4, OUTPUT);  
  pinMode(LCD_D5, OUTPUT);
  pinMode(LCD_D6, OUTPUT);
  pinMode(LCD_D7, OUTPUT);  
#endif

#ifdef SR595
  pinMode(SR595_DATA,OUTPUT);
  pinMode(SR595_CLOCK,OUTPUT);
  pinMode(SR595_LATCH,OUTPUT);
#endif

  _FONT_DIRECTION_ = DIRECTION0;
  _FONT_FILL_ = false;
  _FONT_UNDER_LINE_ = false;
}

void lcdReset(void) {
  digitalWrite(LCD_RST, LOW);
  delay(100); 
  digitalWrite(LCD_RST, HIGH);
  delay(100);
}


void lcdSetup(void) {
  if (_model == 0x9325) {
   lcdWriteRegisterByte(0xe5,0x8000);
   lcdWriteRegisterByte(0x00,0x0001);
   lcdWriteRegisterByte(0x01,0x0100);
//  lcdWriteRegisterByte(0x01,0x0000);
   lcdWriteRegisterByte(0x02,0x0700);
   lcdWriteRegisterByte(0x03,0x1030);
   lcdWriteRegisterByte(0x04,0x0000);
   lcdWriteRegisterByte(0x08,0x0202);
   lcdWriteRegisterByte(0x09,0x0000);
   lcdWriteRegisterByte(0x0a,0x0000);
   lcdWriteRegisterByte(0x0c,0x0000);
   lcdWriteRegisterByte(0x0d,0x0000);
   lcdWriteRegisterByte(0x0f,0x0000);
   lcdWriteRegisterByte(0x10,0x0000);
   lcdWriteRegisterByte(0x11,0x0000);
   lcdWriteRegisterByte(0x12,0x0000);
   lcdWriteRegisterByte(0x13,0x0000);
   lcdWriteRegisterByte(0x10,0x17b0);
   lcdWriteRegisterByte(0x11,0x0037);
   lcdWriteRegisterByte(0x12,0x0138);
   lcdWriteRegisterByte(0x13,0x1700);
   lcdWriteRegisterByte(0x29,0x000d);
   lcdWriteRegisterByte(0x20,0x0000);
   lcdWriteRegisterByte(0x21,0x0000);
   lcdWriteRegisterByte(0x30,0x0001);
   lcdWriteRegisterByte(0x31,0x0606);
   lcdWriteRegisterByte(0x32,0x0304);
   lcdWriteRegisterByte(0x33,0x0202);
   lcdWriteRegisterByte(0x34,0x0202);
   lcdWriteRegisterByte(0x35,0x0103);
   lcdWriteRegisterByte(0x36,0x011d);
   lcdWriteRegisterByte(0x37,0x0404);
   lcdWriteRegisterByte(0x38,0x0404);
   lcdWriteRegisterByte(0x39,0x0404);
   lcdWriteRegisterByte(0x3c,0x0700);
   lcdWriteRegisterByte(0x3d,0x0a1f);
   lcdWriteRegisterByte(0x50,0x0000);
   lcdWriteRegisterByte(0x51,0x00ef);
   lcdWriteRegisterByte(0x52,0x0000);
   lcdWriteRegisterByte(0x53,0x013f);
   lcdWriteRegisterByte(0x60,0x2700);
   lcdWriteRegisterByte(0x61,0x0001);
   lcdWriteRegisterByte(0x6a,0x0000);
   lcdWriteRegisterByte(0x90,0x0010);
   lcdWriteRegisterByte(0x92,0x0000);
   lcdWriteRegisterByte(0x93,0x0003);
   lcdWriteRegisterByte(0x95,0x0101);
   lcdWriteRegisterByte(0x97,0x0000);
   lcdWriteRegisterByte(0x98,0x0000);
   lcdWriteRegisterByte(0x07,0x0021);
   lcdWriteRegisterByte(0x07,0x0031);
   lcdWriteRegisterByte(0x07,0x0173);
  } else if (_model == 0x9341) {
   lcdWriteCommandByte(0xC0);    //Power control 
   lcdWriteDataByte(0x23);

   lcdWriteCommandByte(0xC1);    //Power control 
   lcdWriteDataByte(0x10);

   lcdWriteCommandByte(0xC5);    //VCM control 
   lcdWriteDataByte(0x3e);
   lcdWriteDataByte(0x28); 

   lcdWriteCommandByte(0xC7);    //VCM control2 
   lcdWriteDataByte(0x86);

   lcdWriteCommandByte(0x36);    // Memory Access Control 
   lcdWriteDataByte(0x98);

   lcdWriteCommandByte(0x3A);    
   lcdWriteDataByte(0x55); 
 
   lcdWriteCommandByte(0xB1);    
   lcdWriteDataByte(0x00);  
   lcdWriteDataByte(0x18); 

   lcdWriteCommandByte(0xB6);    // Display Function Control 
   lcdWriteDataByte(0x0A); 
//   lcdWriteDataByte(0x82);
   lcdWriteDataByte(0xA2);
   lcdWriteDataByte(0x27);  
   lcdWriteDataByte(0x04);  

   lcdWriteCommandByte(0x26);    //Gamma curve selected 
   lcdWriteDataByte(0x01); 

   lcdWriteCommandByte(0xE0);    //Set Gamma 
   lcdWriteDataByte(0x0F); 
   lcdWriteDataByte(0x31); 
   lcdWriteDataByte(0x2B); 
   lcdWriteDataByte(0x0C); 
   lcdWriteDataByte(0x0E); 
   lcdWriteDataByte(0x08); 
   lcdWriteDataByte(0x4E); 
   lcdWriteDataByte(0xF1); 
   lcdWriteDataByte(0x37); 
   lcdWriteDataByte(0x07); 
   lcdWriteDataByte(0x10); 
   lcdWriteDataByte(0x03); 
   lcdWriteDataByte(0x0E); 
   lcdWriteDataByte(0x09); 
   lcdWriteDataByte(0x00); 
 
   lcdWriteCommandByte(0XE1);    //Set Gamma 
   lcdWriteDataByte(0x00); 
   lcdWriteDataByte(0x0E); 
   lcdWriteDataByte(0x14); 
   lcdWriteDataByte(0x03); 
   lcdWriteDataByte(0x11); 
   lcdWriteDataByte(0x07); 
   lcdWriteDataByte(0x31); 
   lcdWriteDataByte(0xC1); 
   lcdWriteDataByte(0x48); 
   lcdWriteDataByte(0x08); 
   lcdWriteDataByte(0x0F); 
   lcdWriteDataByte(0x0C); 
   lcdWriteDataByte(0x31); 
   lcdWriteDataByte(0x36); 
   lcdWriteDataByte(0x0F); 

   lcdWriteCommandByte(0x11);    //Exit Sleep 
   delay(120); 
  
   lcdWriteCommandByte(0x29);    //Display on 
  } else if (_model == 0x9342) {
   lcdWriteCommandByte(0xC0);    //Power control
   lcdWriteDataByte(0x26);
   lcdWriteDataByte(0x09);

   lcdWriteCommandByte(0xC1);    //Power control
   lcdWriteDataByte(0x10);

   lcdWriteCommandByte(0xC5);    //VCM control
   lcdWriteDataByte(0x3e);
   lcdWriteDataByte(0x28);

   lcdWriteCommandByte(0xC7);    //VCM control2
   lcdWriteDataByte(0x86);

   lcdWriteCommandByte(0x36);
   lcdWriteDataByte(0xE8);

   lcdWriteCommandByte(0x3A);
   lcdWriteDataByte(0x55);

   lcdWriteCommandByte(0xB1);
   lcdWriteDataByte(0x00);
   lcdWriteDataByte(0x18);


   lcdWriteCommandByte(0xB6);    // Display Function Control
   lcdWriteDataByte(0x0A);
//   lcdWriteDataByte(0x82);
   lcdWriteDataByte(0xA2);
   lcdWriteDataByte(0x27);
   lcdWriteDataByte(0x04);

   lcdWriteCommandByte(0x21);    //Display Inversion ON(I don't know Why need)

   lcdWriteCommandByte(0x26);    //Gamma curve selected
   lcdWriteDataByte(0x01);

   lcdWriteCommandByte(0xE0);    //Set Gamma
   lcdWriteDataByte(0x0F);
   lcdWriteDataByte(0x31);
   lcdWriteDataByte(0x2B);
   lcdWriteDataByte(0x0C);
   lcdWriteDataByte(0x0E);
   lcdWriteDataByte(0x08);
   lcdWriteDataByte(0x4E);
   lcdWriteDataByte(0xF1);
   lcdWriteDataByte(0x37);
   lcdWriteDataByte(0x07);
   lcdWriteDataByte(0x10);
   lcdWriteDataByte(0x03);
   lcdWriteDataByte(0x0E);
   lcdWriteDataByte(0x09);
   lcdWriteDataByte(0x00);

   lcdWriteCommandByte(0XE1);    //Set Gamma
   lcdWriteDataByte(0x00);
   lcdWriteDataByte(0x0E);
   lcdWriteDataByte(0x14);
   lcdWriteDataByte(0x03);
   lcdWriteDataByte(0x11);
   lcdWriteDataByte(0x07);
   lcdWriteDataByte(0x31);
   lcdWriteDataByte(0xC1);
   lcdWriteDataByte(0x48);
   lcdWriteDataByte(0x08);
   lcdWriteDataByte(0x0F);
   lcdWriteDataByte(0x0C);
   lcdWriteDataByte(0x31);
   lcdWriteDataByte(0x36);
   lcdWriteDataByte(0x0F);

   lcdWriteCommandByte(0x11);    //Exit Sleep
   delay(120);

   lcdWriteCommandByte(0x29);    //Display on
  } else if (_model == 0x9481) {
   lcdWriteCommandByte(0x11);    //Exit Sleep 
   delay(50); 

   lcdWriteCommandByte(0xD0);    //Power Setting
   lcdWriteDataByte(0x07);
   lcdWriteDataByte(0x42);
   lcdWriteDataByte(0x18);

   lcdWriteCommandByte(0xD1);    //VCOM Control
   lcdWriteDataByte(0x00);
   lcdWriteDataByte(0x07);
   lcdWriteDataByte(0x18);

   lcdWriteCommandByte(0xD2);    //Power Setting for Normal Mode
   lcdWriteDataByte(0x01);
   lcdWriteDataByte(0x02);

   lcdWriteCommandByte(0xC0);    //Panel Driving Setting
   lcdWriteDataByte(0x10);
   lcdWriteDataByte(0x3B);
   lcdWriteDataByte(0x00);
   lcdWriteDataByte(0x02);
   lcdWriteDataByte(0x11);

   lcdWriteCommandByte(0xC5);    //Frame Rate and Inversion Control
   lcdWriteDataByte(0x03);

   lcdWriteCommandByte(0x36);    // Set Address Mode   
   lcdWriteDataByte(0x08); 

   lcdWriteCommandByte(0x3A);    // Set Pixel Format   
   lcdWriteDataByte(0x55); 

   lcdWriteCommandByte(0xC8);    //Set Gamma 
   lcdWriteDataByte(0x00); 
   lcdWriteDataByte(0x32); 
   lcdWriteDataByte(0x36); 
   lcdWriteDataByte(0x45); 
   lcdWriteDataByte(0x06); 
   lcdWriteDataByte(0x16); 
   lcdWriteDataByte(0x37); 
   lcdWriteDataByte(0x75); 
   lcdWriteDataByte(0x77); 
   lcdWriteDataByte(0x54); 
   lcdWriteDataByte(0x0C); 
   lcdWriteDataByte(0x00); 
 
   lcdWriteCommandByte(0x29);    //Display on 

  } else if (_model == 0x1121) {
   lcdWriteRegisterWord(0x0011,0x2004);
   lcdWriteRegisterWord(0x0013,0xCC00);
   lcdWriteRegisterWord(0x0015,0x2600);
   lcdWriteRegisterWord(0x0014,0x252A);
   lcdWriteRegisterWord(0x0012,0x0033);
   lcdWriteRegisterWord(0x0013,0xCC04);
   lcdWriteRegisterWord(0x0013,0xCC06);
   lcdWriteRegisterWord(0x0013,0xCC4F);
   lcdWriteRegisterWord(0x0013,0x674F);
   lcdWriteRegisterWord(0x0011,0x2003);
   lcdWriteRegisterWord(0x0030,0x2609);
   lcdWriteRegisterWord(0x0031,0x242C);
   lcdWriteRegisterWord(0x0032,0x1F23);
   lcdWriteRegisterWord(0x0033,0x2425);
   lcdWriteRegisterWord(0x0034,0x2226);
   lcdWriteRegisterWord(0x0035,0x2523);
   lcdWriteRegisterWord(0x0036,0x1C1A);
   lcdWriteRegisterWord(0x0037,0x131D);
   lcdWriteRegisterWord(0x0038,0x0B11);
   lcdWriteRegisterWord(0x0039,0x1210);
   lcdWriteRegisterWord(0x003A,0x1315);
   lcdWriteRegisterWord(0x003B,0x3619);
   lcdWriteRegisterWord(0x003C,0x0D00);
   lcdWriteRegisterWord(0x003D,0x000D);
   lcdWriteRegisterWord(0x0016,0x0007);
//   lcdWriteRegisterWord(0x0001,0x0127);
   lcdWriteRegisterWord(0x0001,0x0027);
   lcdWriteRegisterWord(0x0002,0x0013);
   lcdWriteRegisterWord(0x0003,0x0003);
   lcdWriteRegisterWord(0x0008,0x0303);
   lcdWriteRegisterWord(0x000A,0x000B);
   lcdWriteRegisterWord(0x000B,0x0003);
   lcdWriteRegisterWord(0x000C,0x0000);
   lcdWriteRegisterWord(0x0041,0x0000);
   lcdWriteRegisterWord(0x0050,0x0000);
   lcdWriteRegisterWord(0x0060,0x0005);
   lcdWriteRegisterWord(0x0070,0x000B);
   lcdWriteRegisterWord(0x0071,0x0000);
   lcdWriteRegisterWord(0x0078,0x0000);
   lcdWriteRegisterWord(0x007A,0x0000);
   lcdWriteRegisterWord(0x0079,0x0007);
//   lcdWriteRegisterWord(0x0007,0x0051);
   lcdWriteRegisterWord(0x0007,0x0053);
   lcdWriteRegisterWord(0x0079,0x0000);
  }
}


// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if (x < 0 || x >= _width) return;
  if (y < 0 || y >= _height) return;

  if (_model == 0x9325) {
   lcdWriteRegisterByte(0x20, x); // RAM Address Set 1
   lcdWriteRegisterByte(0x21, y); // RAM Address Set 2
   lcdWriteRegisterByte(0x22, color); // Write Data to GRAM
  } else if (_model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x2A); // set column(x) address
   lcdWriteDataWord(x);
   lcdWriteDataWord(x);
   lcdWriteCommandByte(0x2B); // set Page(y) address
   lcdWriteDataWord(y);
   lcdWriteDataWord(y);
   lcdWriteCommandByte(0x2C); // Memory Write
   lcdWriteDataWord(color);
  } else if (_model == 0x1121) {
   lcdWriteRegisterWord(0x0020,x); // RAM Address Set 1
   lcdWriteRegisterWord(0x0021,y); // RAM Address Set 2
   lcdWriteRegisterWord(0x0022,color); // Write Data to GRAM
  }
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  int i,j;
  if (x1 >= _width) return;
  if (x1 < 0) x1=0;
  if (x2 < 0) return;
  if (x2 >= _width) x2=_width-1;
  if (y1 >= _height) return;
  if (y1 < 0) y1=0;
  if (y2 < 0) return;
  if (y2 >= _height) y2=_height-1;

  if (_model == 0x9325) {
   for(j=y1;j<=y2;j++){
    lcdWriteRegisterByte(0x20, x1); // RAM Address Set 1
    lcdWriteRegisterByte(0x21, j); // RAM Address Set 2
    lcdWriteCommandByte(0x22); // Write Data to GRAM
    for(i=x1;i<=x2;i++){
      lcdWriteDataWord(color); // Write Data to GRAM
    }
   }
  } else if (_model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x2A); // set column(x) address
   lcdWriteDataWord(x1);
   lcdWriteDataWord(x2);
   lcdWriteCommandByte(0x2B); // set Page(y) address
   lcdWriteDataWord(y1);
   lcdWriteDataWord(y2);
   lcdWriteCommandByte(0x2C); // Memory Write
   for(i=x1;i<=x2;i++){
    for(j=y1;j<=y2;j++){
      lcdWriteDataWord(color);
    }
   }
  } else if (_model == 0x1121) {
   for(j=y1;j<=y2;j++){
     lcdWriteRegisterWord(0x020,x1);
     lcdWriteRegisterWord(0x021,j);
     for(i=x1;i<=x2;i++){
       lcdWriteRegisterWord(0x022,color);
     }
   }
  }

}

// Display Off
void lcdDisplayOff(void) {
  if (_model == 0x9325) {
   lcdWriteRegisterByte(0x07, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
  } else if (_model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x28); 
  } else if (_model == 0x1121) {
   lcdWriteRegisterWord(0x0007, 0x0010); // Set GON=0 D1=0, D0=0
  }
}

// Display On
void lcdDisplayOn(void) {
  if (_model == 0x9325) {
   lcdWriteRegisterByte(0x07, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
  } else if (_model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x29); 
  } else if (_model == 0x1121) {
   lcdWriteRegisterWord(0x0007, 0x0053); // Set GON=1 D1=1, D0=1
  }
}

// Fill screen
// color:color
void lcdFillScreen(uint16_t color) {
  lcdDrawFillRect(0, 0, _width-1, _height-1, color);
}

// Draw line
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color 
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  int i;
  int dx,dy;
  int sx,sy;
  int E;

  /* distance between two points */
  dx = ( x2 > x1 ) ? x2 - x1 : x1 - x2;
  dy = ( y2 > y1 ) ? y2 - y1 : y1 - y2;

  /* direction of two point */
  sx = ( x2 > x1 ) ? 1 : -1;
  sy = ( y2 > y1 ) ? 1 : -1;

  /* inclination < 1 */
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
  /* inclination >= 1 */
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

// Draw rectangule
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// color:color
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  lcdDrawLine(x1,y1,x2,y1,color);
  lcdDrawLine(x2,y1,x2,y2,color);
  lcdDrawLine(x2,y2,x1,y2,color);
  lcdDrawLine(x1,y2,x1,y1,color);
}

// Draw round
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
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

// Draw round of filling
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
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

// Draw rectangule with round corner
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// r:radius
// color:color
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

// Draw arrow
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
// Thanks http://k-hiura.cocolog-nifty.com/blog/2010/11/post-2a62.html
void lcdDrawArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//   printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//   printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

//   lcdDrawLine(x0,y0,x1,y1,color);
   lcdDrawLine(x1,y1,L[0],L[1],color);
   lcdDrawLine(x1,y1,R[0],R[1],color);
   lcdDrawLine(L[0],L[1],R[0],R[1],color);
}


// Draw arrow of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End   X coordinate
// y2:End   Y coordinate
// w:Width of the botom
// color:color
void lcdDrawFillArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//   printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//   printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

   lcdDrawLine(x0,y0,x1,y1,color);
   lcdDrawLine(x1,y1,L[0],L[1],color);
   lcdDrawLine(x1,y1,R[0],R[1],color);
   lcdDrawLine(L[0],L[1],R[0],R[1],color);

   int ww;
   for(ww=w-1;ww>0;ww--) {
     L[0]= x1 - Uy*ww - Ux*v;
     L[1]= y1 + Ux*ww - Uy*v;
     R[0]= x1 + Uy*ww - Ux*v;
     R[1]= y1 - Ux*ww - Uy*v;
//     printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
     lcdDrawLine(x1,y1,L[0],L[1],color);
     lcdDrawLine(x1,y1,R[0],R[1],color);
   }
}


// RGB565 conversion
// RGB565 is R(5)+G(6)+B(5)=16bit color format.
// Bit image "RRRRRGGGGGGBBBBB"
uint16_t rgb565_conv(uint16_t r,uint16_t g,uint16_t b)
{
   unsigned int RR,GG,BB;
   RR = (r * 31 / 255) << 11;
   GG = (g * 63 / 255) << 5;
   BB = (b * 31 / 255);
   return(RR | GG | BB);
}


// Draw SJIS character
// x:X coordinate
// y:Y coordinate
// sjis: SJIS code
// color:color
int lcdDrawSJISChar(FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color) {
  uint16_t xx,yy,bit,ofs;
  unsigned char fonts[128]; // font pattern
  unsigned char pw, ph;
  int h,w;
  uint16_t mask;
  bool rc;
  
if(_DEBUG_)printf("_FONT_DIRECTION_=%d\n",_FONT_DIRECTION_);
//  sjis = UTF2SJIS(utf8);
//if(_DEBUG_)printf("sjis=%04x\n",sjis);

  rc = GetFontx(fx, sjis, fonts, &pw, &ph); // SJIS -> Font pattern
if(_DEBUG_)printf("GetFontx rc=%d pw=%d ph=%d\n",rc,pw,ph);
  if (!rc) return;

  uint16_t xd1, yd1;
  uint16_t xd2, yd2;
  uint16_t xss, yss;
  uint16_t xsd, ysd;
  int next;
  if (_FONT_DIRECTION_ == 0) {
    xd1 = +1;
    yd1 = -1;
    xd2 =  0;
    yd2 =  0;
    xss =  x;
    yss =  y + ph - 1;
    xsd =  1;
    ysd =  0;
    next = x + pw;
  } else if (_FONT_DIRECTION_ == 2) {
    xd1 = -1;
    yd1 = +1;
    xd2 =  0;
    yd2 =  0;
    xss =  x;
    yss =  y - ph + 1;
    xsd =  1;
    ysd =  0;
    next = x - pw;
  } else if (_FONT_DIRECTION_ == 1) {
    xd1 =  0;
    yd1 =  0;
    xd2 = -1;
    yd2 = -1;
    xss =  x + ph;
    yss =  y;
    xsd =  0;
    ysd =  1;
    next = y - pw;
  } else if (_FONT_DIRECTION_ == 3) {
    xd1 =  0;
    yd1 =  0;
    xd2 = +1;
    yd2 = +1;
    xss =  x - ph - 1;
    yss =  y;
    xsd =  0;
    ysd =  1;
    next = y + pw;
  }

  int bits;
if(_DEBUG_)printf("xss=%d yss=%d\n",xss,yss);
  ofs = 0;
  yy = yss;
  xx = xss;
  for(h=0;h<ph;h++) {
    if(xsd) xx = xss;
    if(ysd) yy = yss;
//    for(w=0;w<(pw/8);w++) {
    bits = pw;
    for(w=0;w<((pw+4)/8);w++) {
      mask = 0x80;
      for(bit=0;bit<8;bit++) {
        bits--;
        if (bits < 0) continue;
//if(_DEBUG_)printf("xx=%d yy=%d mask=%02x fonts[%d]=%02x\n",xx,yy,mask,ofs,fonts[ofs]);
        if (fonts[ofs] & mask) {
          lcdDrawPixel(xx,yy,color);
        } else {
          if (_FONT_FILL_) lcdDrawPixel(xx,yy,_FONT_FILL_COLOR_);
        }
        if (h == (ph-2) & _FONT_UNDER_LINE_)
          lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);
        if (h == (ph-1) & _FONT_UNDER_LINE_)
          lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);

        xx = xx + xd1;
        yy = yy + yd2;
        mask = mask >> 1;
      }
      ofs++;
    }
    yy = yy + yd1;
    xx = xx + xd2;

  }
  
  if (next < 0) next = 0;
  return next;
}

// Draw UTF8 character
// x:X coordinate
// y:Y coordinate
// utf8: UTF8 code
// color:color
int lcdDrawUTF8Char(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color) {
  uint16_t sjis[1];
  
  sjis[0] = UTF2SJIS(utf8);
if(_DEBUG_)printf("sjis=%04x\n",sjis[0]);
  return lcdDrawSJISChar(fx,x,y,sjis[0],color);
}

// Draw UTF8 string
// x:X coordinate
// y:Y coordinate
// utfs: UTF8 string
// color:color
int lcdDrawUTF8String(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utfs,uint16_t color) {

  int i;
  int spos;
  uint16_t sjis[64];
  spos = String2SJIS(utfs, strlen(utfs), sjis, 64);
if(_DEBUG_)printf("spos=%d\n",spos);
  for(i=0;i<spos;i++) {
if(_DEBUG_)printf("sjis[%d]=%x y=%d\n",i,sjis[i],y);
    if (_FONT_DIRECTION_ == 0)
      x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 1)
      y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 2)
      x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
    if (_FONT_DIRECTION_ == 3)
      y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
  }
  if (_FONT_DIRECTION_ == 0) return x;
  if (_FONT_DIRECTION_ == 2) return x;
  if (_FONT_DIRECTION_ == 1) return y;
  if (_FONT_DIRECTION_ == 3) return y;
}

// Set font direction
// dir:Direction
void lcdSetFontDirection(uint16_t dir) {
  _FONT_DIRECTION_ = dir;
}

// Set font filling
// color:fill color
void lcdSetFontFill(uint16_t color) {
  _FONT_FILL_ = true;
  _FONT_FILL_COLOR_ = color;
}

// UnSet font filling
void lcdUnsetFontFill(void) {
  _FONT_FILL_ = false;
}

// Set font underline
// color:frame color
void lcdSetFontUnderLine(uint16_t color) {
  _FONT_UNDER_LINE_ = true;
  _FONT_UNDER_LINE_COLOR_ = color;
}

// UnSet font filling
void lcdUnsetFontUnderLine(void) {
  _FONT_UNDER_LINE_ = false;
}


