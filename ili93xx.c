/*
 ILI9325 : 16Bit Instruction
 ILI9327 :  8Bit Instruction
 ILI9341 :  8Bit Instruction
 ILI9342 :  8Bit Instruction
 ILI9481 :  8Bit Instruction
 SPFD5408: 16Bit Instruction
 S6D1121 : 16Bit Instruction
 R61505U : 16Bit Instruction
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "ili93xx.h"

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

TFTPin pins;

#ifndef SR595

void lcdWriteByte(uint8_t data) {
  digitalWrite(pins.d0, data & 1);
  digitalWrite(pins.d1, (data & 2) >> 1);
  digitalWrite(pins.d2, (data & 4) >> 2);
  digitalWrite(pins.d3, (data & 8) >> 3);
  digitalWrite(pins.d4, (data & 16) >> 4); 
  digitalWrite(pins.d5, (data & 32) >> 5);
  digitalWrite(pins.d6, (data & 64) >> 6);
  digitalWrite(pins.d7, (data & 128) >> 7);  
}

void lcdWriteWord(uint16_t data) {
  digitalWrite(pins.d0, data & 1);
  digitalWrite(pins.d1, (data & 2) >> 1);
  digitalWrite(pins.d2, (data & 4) >> 2);
  digitalWrite(pins.d3, (data & 8) >> 3);
  digitalWrite(pins.d4, (data & 16) >> 4);
  digitalWrite(pins.d5, (data & 32) >> 5);
  digitalWrite(pins.d6, (data & 64) >> 6);
  digitalWrite(pins.d7, (data & 128) >> 7);
  digitalWrite(pins.d8, (data & 256) >> 8);
  digitalWrite(pins.d9, (data & 512) >> 9);
  digitalWrite(pins.d10, (data & 1024) >> 10);
  digitalWrite(pins.d11, (data & 2048) >> 11);
  digitalWrite(pins.d12, (data & 4096) >> 12);
  digitalWrite(pins.d13, (data & 8192) >> 13);
  digitalWrite(pins.d14, (data & 16384) >> 14);
  digitalWrite(pins.d15, (data & 32768) >> 15);
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
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, HIGH);
  digitalWrite(pins.rd, HIGH);

  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(data >> 8);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(data);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);  
}

// For 16bit parallel interface
// It's effective only at color data.
// Many thanks for myg3nx
void lcdWriteDataWord_16bit(uint16_t data) {
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, HIGH);
  digitalWrite(pins.rd, HIGH);
  digitalWrite(pins.wr, HIGH);
  lcdWriteWord(data);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);
  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);
}

void lcdWriteDataByte(uint8_t data) {
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, HIGH);
  digitalWrite(pins.rd, HIGH);
#if 0
  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(data >> 8);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);
#endif
  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(data);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);  
}

void lcdWriteCommandWord(uint16_t command) {
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, LOW);
  digitalWrite(pins.rd, HIGH);

  digitalWrite(pins.wr, HIGH);  
  lcdWriteByte(command >> 8);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(command);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);    
}

#if 0
void lcdWriteCommandWord(uint16_t command) {
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, LOW);
  digitalWrite(pins.rd, HIGH);

  digitalWrite(pins.wr, HIGH);  
  lcdWriteWord(command);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);    
}
#endif

void lcdWriteCommandByte(uint8_t command) {
  digitalWrite(pins.cs, LOW);
  digitalWrite(pins.rs, LOW);
  digitalWrite(pins.rd, HIGH);
#if 0
  digitalWrite(pins.wr, HIGH);  
  lcdWriteByte(command >> 8);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);
#endif
  digitalWrite(pins.wr, HIGH);
  lcdWriteByte(command);
  digitalWrite(pins.wr, LOW);
  delayMicroseconds(10);

  digitalWrite(pins.wr, HIGH);
  digitalWrite(pins.cs, HIGH);    
}


void lcdWriteRegisterWord(uint16_t addr, uint16_t data) {
  lcdWriteCommandWord(addr);
  lcdWriteDataWord(data);
}

void lcdWriteRegisterByte(uint8_t addr, uint16_t data) {
  lcdWriteCommandByte(addr);
  lcdWriteDataWord(data);
}


#define TFTLCD_DELAY 0xFFFF
#define TFTLCD_DELAY8 0x7F

void init_table(const void *table, int16_t size)
{
    int i;
    uint8_t *p = (uint8_t *) table;
    while (size > 0) {
        uint8_t cmd = *p++;
        uint8_t len = *p++;
        if (cmd == TFTLCD_DELAY8) {
            delay(len);
            len = 0;
        } else {
            lcdWriteCommandByte( cmd );
            for (i = 0; i < len; i++) {
              uint8_t data = *p++;
              lcdWriteDataByte( data );
            }
        }
        size -= len + 2;
    }
}
void lcdInit(uint16_t model, uint16_t width, uint16_t height, char* ppath) {
  _model = model;
  _width = width;
  _height = height;

#if 0
  pins.rst = 7;
  pins.cs  = 8;
  pins.rs  = 9;
  pins.wr  = 11;
  pins.rd  = 31;
  pins.d0  = 21;
  pins.d1  = 22;
  pins.d2  = 23;
  pins.d3  = 24;
  pins.d4  = 25;
  pins.d5  = 26;
  pins.d6  = 27;
  pins.d7  = 28;
#endif

if(_DEBUG_)  printf("ppath=%s\n",ppath);

  ReadPinConfig(&pins, ppath);
if(_DEBUG_)printf("rst=%d cs=%d rs=%d wr=%d rd=%d\n",
  pins.rst,pins.cs,pins.rs,pins.wr,pins.rd);
if(_DEBUG_)printf("d0=%d d1=%d d2=%d d4=%d d4=%d d5=%d d6=%d d7=%d\n",
  pins.d0,pins.d1,pins.d2,pins.d3,pins.d4,pins.d5,pins.d6,pins.d7);
#ifdef P16BIT
if(_DEBUG_)printf("d8=%d d9=%d d10=%d d11=%d d12=%d d13=%d d14=%d d15=%d\n",
  pins.d8,pins.d9,pins.d10,pins.d11,pins.d12,pins.d13,pins.d14,pins.d15);
#endif

  pinMode(pins.cs, OUTPUT);
  digitalWrite(pins.cs, HIGH);
  pinMode(pins.rs, OUTPUT);
  digitalWrite(pins.rs, HIGH);
  pinMode(pins.wr, OUTPUT);
  digitalWrite(pins.wr, HIGH);
  pinMode(pins.rd, OUTPUT);
  digitalWrite(pins.rd, HIGH);
  pinMode(pins.rst, OUTPUT);
  digitalWrite(pins.rst, HIGH);  

#ifndef SR595
  pinMode(pins.d0, OUTPUT);
  pinMode(pins.d1, OUTPUT);
  pinMode(pins.d2, OUTPUT);
  pinMode(pins.d3, OUTPUT);  
  pinMode(pins.d4, OUTPUT);  
  pinMode(pins.d5, OUTPUT);
  pinMode(pins.d6, OUTPUT);
  pinMode(pins.d7, OUTPUT);  

  pinMode(pins.d8, OUTPUT);  
  pinMode(pins.d9, OUTPUT);  
  pinMode(pins.d10, OUTPUT);  
  pinMode(pins.d11, OUTPUT);  
  pinMode(pins.d12, OUTPUT);  
  pinMode(pins.d13, OUTPUT);  
  pinMode(pins.d14, OUTPUT);  
  pinMode(pins.d15, OUTPUT);  

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
  digitalWrite(pins.rst, LOW);
  delay(100); 
  digitalWrite(pins.rst, HIGH);
  delay(100);
}


void lcdSetup(void) {
  static const uint8_t reset_off[] = {
    0x01, 0,            //Soft Reset
    TFTLCD_DELAY8, 150,
    0x28, 0,            //Display Off
    0x3A, 1, 0x55,      //Pixel read=565, write=565.
  };

  static const uint8_t wake_on[] = {
    0x11, 0,            //Sleep Out
    TFTLCD_DELAY8, 150,
    0x29, 0,            //Display On
  };

//  if (_model == 0x9325) {
  if (_model == 0x9325 || _model == 0x5408 || _model == 0x1505) {
   lcdWriteRegisterWord(0x00e5,0x8000);
   lcdWriteRegisterWord(0x0000,0x0001);
   if (_model == 0x9325)
     lcdWriteRegisterWord(0x0001,0x0100);
   if (_model == 0x5408)
     lcdWriteRegisterWord(0x0001,0x0000);
   if (_model == 0x1505)
     lcdWriteRegisterWord(0x0001,0x0000);
   lcdWriteRegisterWord(0x0002,0x0700);
   lcdWriteRegisterWord(0x0003,0x1030);
   lcdWriteRegisterWord(0x0004,0x0000);
   lcdWriteRegisterWord(0x0008,0x0202);
   lcdWriteRegisterWord(0x0009,0x0000);
   lcdWriteRegisterWord(0x000a,0x0000);
   lcdWriteRegisterWord(0x000c,0x0000);
   lcdWriteRegisterWord(0x000d,0x0000);
   lcdWriteRegisterWord(0x000f,0x0000);
   lcdWriteRegisterWord(0x0010,0x0000);
   lcdWriteRegisterWord(0x0011,0x0000);
   lcdWriteRegisterWord(0x0012,0x0000);
   lcdWriteRegisterWord(0x0013,0x0000);
   lcdWriteRegisterWord(0x0010,0x17b0);
   lcdWriteRegisterWord(0x0011,0x0037);
   lcdWriteRegisterWord(0x0012,0x0138);
   lcdWriteRegisterWord(0x0013,0x1700);
   lcdWriteRegisterWord(0x0029,0x000d);
   lcdWriteRegisterWord(0x0020,0x0000);
   lcdWriteRegisterWord(0x0021,0x0000);
   lcdWriteRegisterWord(0x0030,0x0001);
   lcdWriteRegisterWord(0x0031,0x0606);
   lcdWriteRegisterWord(0x0032,0x0304);
   lcdWriteRegisterWord(0x0033,0x0202);
   lcdWriteRegisterWord(0x0034,0x0202);
   lcdWriteRegisterWord(0x0035,0x0103);
   lcdWriteRegisterWord(0x0036,0x011d);
   lcdWriteRegisterWord(0x0037,0x0404);
   lcdWriteRegisterWord(0x0038,0x0404);
   lcdWriteRegisterWord(0x0039,0x0404);
   lcdWriteRegisterWord(0x003c,0x0700);
   lcdWriteRegisterWord(0x003d,0x0a1f);
   lcdWriteRegisterWord(0x0050,0x0000);
   lcdWriteRegisterWord(0x0051,0x00ef);
   lcdWriteRegisterWord(0x0052,0x0000);
   lcdWriteRegisterWord(0x0053,0x013f);
   lcdWriteRegisterWord(0x0060,0x2700);
   lcdWriteRegisterWord(0x0061,0x0001);
   lcdWriteRegisterWord(0x006a,0x0000);
   lcdWriteRegisterWord(0x0090,0x0010);
   lcdWriteRegisterWord(0x0092,0x0000);
   lcdWriteRegisterWord(0x0093,0x0003);
   lcdWriteRegisterWord(0x0095,0x0101);
   lcdWriteRegisterWord(0x0097,0x0000);
   lcdWriteRegisterWord(0x0098,0x0000);
   lcdWriteRegisterWord(0x0007,0x0021);
   lcdWriteRegisterWord(0x0007,0x0031);
   lcdWriteRegisterWord(0x0007,0x0173);

  } else if (_model == 0x9327) {
   static const uint8_t regValues[] = {
      0x36, 1, 0x08,
      0x3A, 1, 0x05,
      0xC0, 6, 0x00, 0x35, 0x00, 0x00, 0x01, 0x02,
      0xC1, 4, 0x10, 0x10, 0x02, 0x02,
      0xC5, 1, 0x04,
      0xC8,15, 0x04, 0x67, 0x35, 0x04, 0x08, 0x06, 0x24, 0x01, 0x37, 0x40, 0x03, 0x10, 0x08, 0x80, 0x00,
      0xD0, 3, 0x07, 0x04, 0x00,
      0xD1, 3, 0x00, 0x0C, 0x0F,
      0xD2, 2, 0x01, 0x44,
    };
    init_table(reset_off, sizeof(reset_off));
    init_table(regValues, sizeof(regValues));
    init_table(wake_on, sizeof(wake_on));
  } else if (_model == 0x9341) {
   static const uint8_t regValues[] = {
      0xC0, 1, 0x23,
      0xC1, 1, 0x10,
      0xC5, 2, 0x3E, 0x28,
      0xC7, 1, 0x86,
      0x36, 1, 0x98,
      0xB1, 2, 0x00, 0x18,
      0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
      0x26, 1, 0x01,
      0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
      0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    };
    init_table(reset_off, sizeof(reset_off));
    init_table(regValues, sizeof(regValues));
    init_table(wake_on, sizeof(wake_on));

  } else if (_model == 0x9342) {
    static const uint8_t regValues[] = {
      0xC0, 2, 0x26, 0x09,
      0xC1, 1, 0x10,
      0xC5, 2, 0x3E, 0x28,
      0xC7, 1, 0x86,
      0x36, 1, 0xE8,
      0xB1, 2, 0x00, 0x18,
      0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
      0x21, 0,         // Display Inversion ON
      0x26, 1, 0x01,
      0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
      0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    };
    init_table(reset_off, sizeof(reset_off));
    init_table(regValues, sizeof(regValues));
    init_table(wake_on, sizeof(wake_on));

  } else if (_model == 0x9481) {
   static const uint8_t regValues[] = {
      0xB0, 1, 0x00,              // unlocks E0, F0
      0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
      0xB4, 1, 0x00,              // Frame mode [00]
      0xD0, 3, 0x07, 0x42, 0x18,
      0xD1, 3, 0x00, 0x07, 0x18,
      0xD2, 2, 0x01, 0x02,
      0xD3, 2, 0x01, 0x02,        // Set Power for Partial Mode [01 22]
      0xD4, 2, 0x01, 0x02,        // Set Power for Idle Mode [01 22]
      0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
      0xC1, 3, 0x10, 0x10, 0x88,  // Display Timing Normal [10 10 88]
      0xC5, 1, 0x03,      //Frame Rate [03]
      0xC6, 1, 0x02,      //Interface Control [02]
      0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
      0xCC, 1, 0x00,      //Panel Control [00]
      0x36, 1, 0x08,
    };
    init_table(reset_off, sizeof(reset_off));
    init_table(regValues, sizeof(regValues));
    init_table(wake_on, sizeof(wake_on));

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

  if (_model == 0x9325 || _model == 0x5408 || _model == 0x1505) {
   lcdWriteRegisterWord(0x0020, x); // RAM Address Set 1
   lcdWriteRegisterWord(0x0021, y); // RAM Address Set 2
   lcdWriteRegisterWord(0x0022, color); // Write Data to GRAM

  } else if (_model == 0x9327 || _model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x2A); // set column(x) address
   lcdWriteDataWord(x);
   lcdWriteDataWord(x);
   lcdWriteCommandByte(0x2B); // set Page(y) address
   lcdWriteDataWord(y);
   lcdWriteDataWord(y);
   lcdWriteCommandByte(0x2C); // Memory Write
#ifndef P16BIT
   lcdWriteDataWord(color);
#else
   lcdWriteDataWord_16bit(color);
#endif

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

  if (_model == 0x9325 || _model == 0x5408 || _model == 0x1505) {
   for(j=y1;j<=y2;j++){
    lcdWriteRegisterWord(0x0020, x1); // RAM Address Set 1
    lcdWriteRegisterWord(0x0021, j); // RAM Address Set 2
    lcdWriteCommandWord(0x0022); // Write Data to GRAM
    for(i=x1;i<=x2;i++){
      lcdWriteDataWord(color); // Write Data to GRAM
    }
   }

  } else if (_model == 0x9327 || _model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x2A); // set column(x) address
   lcdWriteDataWord(x1);
   lcdWriteDataWord(x2);
   lcdWriteCommandByte(0x2B); // set Page(y) address
   lcdWriteDataWord(y1);
   lcdWriteDataWord(y2);
   lcdWriteCommandByte(0x2C); // Memory Write
   for(i=x1;i<=x2;i++){
    for(j=y1;j<=y2;j++){
#ifndef P16BIT
      lcdWriteDataWord(color);
#else
      lcdWriteDataWord_16bit(color);
#endif
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
  if (_model == 0x9325 || _model == 0x5408 || _model == 0x1505) {
   lcdWriteRegisterWord(0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
  } else if (_model == 0x9327 || _model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
   lcdWriteCommandByte(0x28); 
  } else if (_model == 0x1121) {
   lcdWriteRegisterWord(0x0007, 0x0010); // Set GON=0 D1=0, D0=0
  }
}

// Display On
void lcdDisplayOn(void) {
  if (_model == 0x9325 || _model == 0x5408 || _model == 0x1505) {
   lcdWriteRegisterWord(0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
  } else if (_model == 0x9327 || _model == 0x9341 || _model == 0x9342 || _model == 0x9481) {
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
  if (!rc) return -1;

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
        if (h == (ph-2) && _FONT_UNDER_LINE_)
          lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);
        if (h == (ph-1) && _FONT_UNDER_LINE_)
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
int lcdDrawUTF8String(FontxFile *fx, uint16_t x,uint16_t y,unsigned char *utfs,uint16_t color) {

  int i;
  int spos;
  uint16_t sjis[64];
  spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 64);
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
  return 0;
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


int ReadPinConfig(TFTPin *pin, char *path) {
  FILE *fp;
  char buff[128];

//  printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
//    printf("buf=%s\n",buff);
//    printf("buff[0]=%x\n",buff[0]);
    if (buff[0] == '#') continue;
    if (buff[0] == 0x0a) continue;
    if (strncmp(buff,"RST=",4) == 0) {
      sscanf(buff, "RST=%d", &(pin->rst));
    } else if (strncmp(buff,"RS=",3) == 0) {
      sscanf(buff, "RS=%d", &(pin->rs));
    }  else if (strncmp(buff,"CS=",3) == 0) {
      sscanf(buff, "CS=%d", &(pin->cs));
    } else if (strncmp(buff,"WR=",3) == 0) {
      sscanf(buff, "WR=%d", &(pin->wr));
    } else if (strncmp(buff,"RD=",3) == 0) {
      sscanf(buff, "RD=%d", &(pin->rd));
    } else if (strncmp(buff,"D0=",3) == 0) {
      sscanf(buff, "D0=%d", &(pin->d0));
    } else if (strncmp(buff,"D1=",3) == 0) {
      sscanf(buff, "D1=%d", &(pin->d1));
    } else if (strncmp(buff,"D2=",3) == 0) {
      sscanf(buff, "D2=%d", &(pin->d2));
    } else if (strncmp(buff,"D3=",3) == 0) {
      sscanf(buff, "D3=%d", &(pin->d3));
    } else if (strncmp(buff,"D4=",3) == 0) {
      sscanf(buff, "D4=%d", &(pin->d4));
    } else if (strncmp(buff,"D5=",3) == 0) {
      sscanf(buff, "D5=%d", &(pin->d5));
    } else if (strncmp(buff,"D6=",3) == 0) {
      sscanf(buff, "D6=%d", &(pin->d6));
    } else if (strncmp(buff,"D7=",3) == 0) {
      sscanf(buff, "D7=%d", &(pin->d7));
    } else if (strncmp(buff,"D8=",3) == 0) {
      sscanf(buff, "D8=%d", &(pin->d8));
    } else if (strncmp(buff,"D9=",3) == 0) {
      sscanf(buff, "D9=%d", &(pin->d9));
    } else if (strncmp(buff,"D10=",4) == 0) {
      sscanf(buff, "D10=%d", &(pin->d10));
    } else if (strncmp(buff,"D11=",4) == 0) {
      sscanf(buff, "D11=%d", &(pin->d11));
    } else if (strncmp(buff,"D12=",4) == 0) {
      sscanf(buff, "D12=%d", &(pin->d12));
    } else if (strncmp(buff,"D13=",4) == 0) {
      sscanf(buff, "D13=%d", &(pin->d13));
    } else if (strncmp(buff,"D14=",4) == 0) {
      sscanf(buff, "D14=%d", &(pin->d14));
    } else if (strncmp(buff,"D15=",4) == 0) {
      sscanf(buff, "D15=%d", &(pin->d15));
    }

  }
  fclose(fp);
  return 1;

}
