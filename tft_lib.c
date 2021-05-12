#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include "tft_lib.h"

#define _DEBUG_ 0

// delay defined in WiringPi.h
void lcdDelay(int ms) {
	delay(ms);
}

void lcdWriteByte(TFT_t *dev, uint8_t data) {
  digitalWrite(dev->pins.d0, data & 1);
  digitalWrite(dev->pins.d1, (data & 2) >> 1);
  digitalWrite(dev->pins.d2, (data & 4) >> 2);
  digitalWrite(dev->pins.d3, (data & 8) >> 3);
  digitalWrite(dev->pins.d4, (data & 16) >> 4); 
  digitalWrite(dev->pins.d5, (data & 32) >> 5);
  digitalWrite(dev->pins.d6, (data & 64) >> 6);
  digitalWrite(dev->pins.d7, (data & 128) >> 7);  

  digitalWrite(dev->pins.wr, LOW);
  //delayMicroseconds(10);
  digitalWrite(dev->pins.wr, HIGH);
}

void lcdWriteWord(TFT_t *dev, uint16_t data) {
  digitalWrite(dev->pins.d0, data & 1);
  digitalWrite(dev->pins.d1, (data & 2) >> 1);
  digitalWrite(dev->pins.d2, (data & 4) >> 2);
  digitalWrite(dev->pins.d3, (data & 8) >> 3);
  digitalWrite(dev->pins.d4, (data & 16) >> 4);
  digitalWrite(dev->pins.d5, (data & 32) >> 5);
  digitalWrite(dev->pins.d6, (data & 64) >> 6);
  digitalWrite(dev->pins.d7, (data & 128) >> 7);
  digitalWrite(dev->pins.d8, (data & 256) >> 8);
  digitalWrite(dev->pins.d9, (data & 512) >> 9);
  digitalWrite(dev->pins.d10, (data & 1024) >> 10);
  digitalWrite(dev->pins.d11, (data & 2048) >> 11);
  digitalWrite(dev->pins.d12, (data & 4096) >> 12);
  digitalWrite(dev->pins.d13, (data & 8192) >> 13);
  digitalWrite(dev->pins.d14, (data & 16384) >> 14);
  digitalWrite(dev->pins.d15, (data & 32768) >> 15);

  digitalWrite(dev->pins.wr, LOW);
  //delayMicroseconds(10);
  digitalWrite(dev->pins.wr, HIGH);
}


void lcdWriteDataWord(TFT_t *dev, uint16_t data) {
  digitalWrite(dev->pins.cs, LOW);
  digitalWrite(dev->pins.rs, HIGH);
  digitalWrite(dev->pins.rd, HIGH);
  digitalWrite(dev->pins.wr, HIGH);
  lcdWriteByte(dev, data >> 8);
  lcdWriteByte(dev, data);
  digitalWrite(dev->pins.cs, HIGH);  
}


// For 16bit parallel interface
// It's effective only at color data.
// Many thanks for myg3nx
void lcdWriteDataWord_16bit(TFT_t *dev, uint16_t data) {
  digitalWrite(dev->pins.cs, LOW);
  digitalWrite(dev->pins.rs, HIGH);
  digitalWrite(dev->pins.rd, HIGH);
  digitalWrite(dev->pins.wr, HIGH);
  lcdWriteWord(dev, data);
  digitalWrite(dev->pins.cs, HIGH);
}

void lcdWriteDataByte(TFT_t *dev, uint8_t data) {
  digitalWrite(dev->pins.cs, LOW);
  digitalWrite(dev->pins.rs, HIGH);
  digitalWrite(dev->pins.rd, HIGH);
  digitalWrite(dev->pins.wr, HIGH);
  lcdWriteByte(dev, data);
  digitalWrite(dev->pins.cs, HIGH);  
}

void lcdWriteCommandWord(TFT_t *dev, uint16_t command) {
  digitalWrite(dev->pins.cs, LOW);
  digitalWrite(dev->pins.rs, LOW);
  digitalWrite(dev->pins.rd, HIGH);
  digitalWrite(dev->pins.wr, HIGH);  
  lcdWriteByte(dev, command >> 8);
  lcdWriteByte(dev, command);
  digitalWrite(dev->pins.cs, HIGH);    
}

void lcdWriteCommandByte(TFT_t *dev, uint8_t command) {
  digitalWrite(dev->pins.cs, LOW);
  digitalWrite(dev->pins.rs, LOW);
  digitalWrite(dev->pins.rd, HIGH);
  digitalWrite(dev->pins.wr, HIGH);
  lcdWriteByte(dev, command);
  digitalWrite(dev->pins.cs, HIGH);    
}


void lcdWriteRegisterWord(TFT_t *dev, uint16_t addr, uint16_t data) {
  lcdWriteCommandWord(dev, addr);
  lcdWriteDataWord(dev, data);
}

void lcdWriteRegisterByte(TFT_t *dev, uint8_t addr, uint16_t data) {
  lcdWriteCommandByte(dev, addr);
  lcdWriteDataWord(dev, data);
}

void lcdInterface(TFT_t *dev, char* ppath) {
#if 0
	dev->pins.rst = 7;
	dev->pins.cs  = 8;
	dev->pins.rs  = 9;
	dev->pins.wr  = 11;
	dev->pins.rd  = 31;
	dev->pins.d0  = 21;
	dev->pins.d1  = 22;
	dev->pins.d2  = 23;
	dev->pins.d3  = 24;
	dev->pins.d4  = 25;
	dev->pins.d5  = 26;
	dev->pins.d6  = 27;
	dev->pins.d7  = 28;
#endif

	if(_DEBUG_) printf("ppath=%s\n",ppath);

	ReadPinConfig(&(dev->pins), ppath);
	if(_DEBUG_)printf("rst=%d cs=%d rs=%d wr=%d rd=%d\n",
		dev->pins.rst, dev->pins.cs, dev->pins.rs, dev->pins.wr, dev->pins.rd);
	if(_DEBUG_)printf("d0=%d d1=%d d2=%d d4=%d d4=%d d5=%d d6=%d d7=%d\n",
		dev->pins.d0, dev->pins.d1, dev->pins.d2, dev->pins.d3, dev->pins.d4, dev->pins.d5, dev->pins.d6, dev->pins.d7);
#ifdef P16BIT
	if(_DEBUG_)printf("d8=%d d9=%d d10=%d d11=%d d12=%d d13=%d d14=%d d15=%d\n",
		dev->pins.d8, dev->pins.d9, dev->pins.d10, dev->pins.d11, dev->pins.d12, dev->pins.d13, dev->pins.d14, dev->pins.d15);
#endif

	pinMode(dev->pins.cs, OUTPUT);
	digitalWrite(dev->pins.cs, HIGH);
	pinMode(dev->pins.rs, OUTPUT);
	digitalWrite(dev->pins.rs, HIGH);
	pinMode(dev->pins.wr, OUTPUT);
	digitalWrite(dev->pins.wr, HIGH);
	pinMode(dev->pins.rd, OUTPUT);
	digitalWrite(dev->pins.rd, HIGH);
	pinMode(dev->pins.rst, OUTPUT);
	digitalWrite(dev->pins.rst, HIGH);

	pinMode(dev->pins.d0, OUTPUT);
	pinMode(dev->pins.d1, OUTPUT);
	pinMode(dev->pins.d2, OUTPUT);
	pinMode(dev->pins.d3, OUTPUT);
	pinMode(dev->pins.d4, OUTPUT);
	pinMode(dev->pins.d5, OUTPUT);
	pinMode(dev->pins.d6, OUTPUT);
	pinMode(dev->pins.d7, OUTPUT);

	pinMode(dev->pins.d8, OUTPUT);
	pinMode(dev->pins.d9, OUTPUT);
	pinMode(dev->pins.d10, OUTPUT);
	pinMode(dev->pins.d11, OUTPUT);
	pinMode(dev->pins.d12, OUTPUT);
	pinMode(dev->pins.d13, OUTPUT);
	pinMode(dev->pins.d14, OUTPUT);
	pinMode(dev->pins.d15, OUTPUT);
}

void lcdReset(TFT_t *dev) {
  digitalWrite(dev->pins.rst, HIGH);
  delay(100); 
  digitalWrite(dev->pins.rst, LOW);
  delay(100); 
  digitalWrite(dev->pins.rst, HIGH);
  delay(100);
}

void lcdInitTable(TFT_t *dev, const void *table, int16_t size)
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
			lcdWriteCommandByte(dev, cmd);
			for (i = 0; i < len; i++) {
				uint8_t data = *p++;
				lcdWriteDataByte(dev,  data);
			}
		}
		size -= len + 2;
	}
}

void lcdInitTable16(TFT_t *dev, const void *table, int16_t size)
{
	uint16_t *p = (uint16_t *) table;
	while (size > 0) {
		uint16_t cmd = *p++;
		uint16_t dat = *p++;
		if (cmd == TFTLCD_DELAY16)
			delay(dat);
		else {
			lcdWriteRegisterWord(dev, cmd, dat);
		}
		size -= 2 * sizeof(int16_t);
	}
}

void lcdInit(TFT_t *dev, uint16_t model, int width, int height, int offsetx, int offsety) {
  dev->_model = model;
  dev->_width = width;
  dev->_height = height;
  dev->_offsetx = offsetx;
  dev->_offsety = offsety;
  dev->_font_direction = DIRECTION0;
  dev->_font_fill = false;
  dev->_font_underline = false;
}



#if 0
void lcdSetup(TFT_t *dev, int width, int height, int offsetx, int offsety) {
  static const uint8_t reset_off[] = {
	0x01, 0,			//Soft Reset
	TFTLCD_DELAY8, 150,
	0x28, 0,			//Display Off
	0x3A, 1, 0x55,		//Pixel read=565, write=565.
  };

  static const uint8_t wake_on[] = {
	0x11, 0,			//Sleep Out
	TFTLCD_DELAY8, 150,
	0x29, 0,			//Display On
  };

  // ILI9325 SPFD5408 R61505U
  if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505) {
   lcdWriteRegisterWord(dev, 0x00e5,0x8000);
   lcdWriteRegisterWord(dev, 0x0000,0x0001);
   if (dev->_model == 0x9325)
	 lcdWriteRegisterWord(dev, 0x0001,0x0100);
   if (dev->_model == 0x5408)
	 lcdWriteRegisterWord(dev, 0x0001,0x0000);
   if (dev->_model == 0x1505)
	 lcdWriteRegisterWord(dev, 0x0001,0x0000);
   lcdWriteRegisterWord(dev, 0x0002,0x0700);
   lcdWriteRegisterWord(dev, 0x0003,0x1030);
   lcdWriteRegisterWord(dev, 0x0004,0x0000);
   lcdWriteRegisterWord(dev, 0x0008,0x0202);
   lcdWriteRegisterWord(dev, 0x0009,0x0000);
   lcdWriteRegisterWord(dev, 0x000a,0x0000);
   lcdWriteRegisterWord(dev, 0x000c,0x0000);
   lcdWriteRegisterWord(dev, 0x000d,0x0000);
   lcdWriteRegisterWord(dev, 0x000f,0x0000);
   lcdWriteRegisterWord(dev, 0x0010,0x0000);
   lcdWriteRegisterWord(dev, 0x0011,0x0000);
   lcdWriteRegisterWord(dev, 0x0012,0x0000);
   lcdWriteRegisterWord(dev, 0x0013,0x0000);
   lcdWriteRegisterWord(dev, 0x0010,0x17b0);
   lcdWriteRegisterWord(dev, 0x0011,0x0037);
   lcdWriteRegisterWord(dev, 0x0012,0x0138);
   lcdWriteRegisterWord(dev, 0x0013,0x1700);
   lcdWriteRegisterWord(dev, 0x0029,0x000d);
   lcdWriteRegisterWord(dev, 0x0020,0x0000);
   lcdWriteRegisterWord(dev, 0x0021,0x0000);
   lcdWriteRegisterWord(dev, 0x0030,0x0001);
   lcdWriteRegisterWord(dev, 0x0031,0x0606);
   lcdWriteRegisterWord(dev, 0x0032,0x0304);
   lcdWriteRegisterWord(dev, 0x0033,0x0202);
   lcdWriteRegisterWord(dev, 0x0034,0x0202);
   lcdWriteRegisterWord(dev, 0x0035,0x0103);
   lcdWriteRegisterWord(dev, 0x0036,0x011d);
   lcdWriteRegisterWord(dev, 0x0037,0x0404);
   lcdWriteRegisterWord(dev, 0x0038,0x0404);
   lcdWriteRegisterWord(dev, 0x0039,0x0404);
   lcdWriteRegisterWord(dev, 0x003c,0x0700);
   lcdWriteRegisterWord(dev, 0x003d,0x0a1f);
   lcdWriteRegisterWord(dev, 0x0050,0x0000);
   lcdWriteRegisterWord(dev, 0x0051,0x00ef);
   lcdWriteRegisterWord(dev, 0x0052,0x0000);
   lcdWriteRegisterWord(dev, 0x0053,0x013f);
   lcdWriteRegisterWord(dev, 0x0060,0x2700);
   lcdWriteRegisterWord(dev, 0x0061,0x0001);
   lcdWriteRegisterWord(dev, 0x006a,0x0000);
   lcdWriteRegisterWord(dev, 0x0090,0x0010);
   lcdWriteRegisterWord(dev, 0x0092,0x0000);
   lcdWriteRegisterWord(dev, 0x0093,0x0003);
   lcdWriteRegisterWord(dev, 0x0095,0x0101);
   lcdWriteRegisterWord(dev, 0x0097,0x0000);
   lcdWriteRegisterWord(dev, 0x0098,0x0000);
   lcdWriteRegisterWord(dev, 0x0007,0x0021);
   lcdWriteRegisterWord(dev, 0x0007,0x0031);
   lcdWriteRegisterWord(dev, 0x0007,0x0173);

  // ILI9327
  } else if (dev->_model == 0x9327) {
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
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

  // ILI9341
  } else if (dev->_model == 0x9341) {
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
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

  // ILI9342
  } else if (dev->_model == 0x9342) {
	static const uint8_t regValues[] = {
	  0xC0, 2, 0x26, 0x09,
	  0xC1, 1, 0x10,
	  0xC5, 2, 0x3E, 0x28,
	  0xC7, 1, 0x86,
	  0x36, 1, 0xE8,
	  0xB1, 2, 0x00, 0x18,
	  0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
	  0x21, 0,		   // Display Inversion ON
	  0x26, 1, 0x01,
	  0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
	  0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

  // ILI9481
  } else if (dev->_model == 0x9481) {
   static const uint8_t regValues[] = {
	  0xB0, 1, 0x00,			  // unlocks E0, F0
	  0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
	  0xB4, 1, 0x00,			  // Frame mode [00]
	  0xD0, 3, 0x07, 0x42, 0x18,
	  0xD1, 3, 0x00, 0x07, 0x18,
	  0xD2, 2, 0x01, 0x02,
	  0xD3, 2, 0x01, 0x02,		  // Set Power for Partial Mode [01 22]
	  0xD4, 2, 0x01, 0x02,		  // Set Power for Idle Mode [01 22]
	  0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
	  0xC1, 3, 0x10, 0x10, 0x88,  // Display Timing Normal [10 10 88]
	  0xC5, 1, 0x03,	  //Frame Rate [03]
	  0xC6, 1, 0x02,	  //Interface Control [02]
	  0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
	  0xCC, 1, 0x00,	  //Panel Control [00]
	  0x36, 1, 0x08,
	};
	lcdInitTable(dev, reset_off, sizeof(reset_off));
	lcdInitTable(dev, regValues, sizeof(regValues));
	lcdInitTable(dev, wake_on, sizeof(wake_on));

  // S6D1121
  } else if (dev->_model == 0x1121) {
   lcdWriteRegisterWord(dev, 0x0011,0x2004);
   lcdWriteRegisterWord(dev, 0x0013,0xCC00);
   lcdWriteRegisterWord(dev, 0x0015,0x2600);
   lcdWriteRegisterWord(dev, 0x0014,0x252A);
   lcdWriteRegisterWord(dev, 0x0012,0x0033);
   lcdWriteRegisterWord(dev, 0x0013,0xCC04);
   lcdWriteRegisterWord(dev, 0x0013,0xCC06);
   lcdWriteRegisterWord(dev, 0x0013,0xCC4F);
   lcdWriteRegisterWord(dev, 0x0013,0x674F);
   lcdWriteRegisterWord(dev, 0x0011,0x2003);
   lcdWriteRegisterWord(dev, 0x0030,0x2609);
   lcdWriteRegisterWord(dev, 0x0031,0x242C);
   lcdWriteRegisterWord(dev, 0x0032,0x1F23);
   lcdWriteRegisterWord(dev, 0x0033,0x2425);
   lcdWriteRegisterWord(dev, 0x0034,0x2226);
   lcdWriteRegisterWord(dev, 0x0035,0x2523);
   lcdWriteRegisterWord(dev, 0x0036,0x1C1A);
   lcdWriteRegisterWord(dev, 0x0037,0x131D);
   lcdWriteRegisterWord(dev, 0x0038,0x0B11);
   lcdWriteRegisterWord(dev, 0x0039,0x1210);
   lcdWriteRegisterWord(dev, 0x003A,0x1315);
   lcdWriteRegisterWord(dev, 0x003B,0x3619);
   lcdWriteRegisterWord(dev, 0x003C,0x0D00);
   lcdWriteRegisterWord(dev, 0x003D,0x000D);
   lcdWriteRegisterWord(dev, 0x0016,0x0007);
//	 lcdWriteRegisterWord(dev, 0x0001,0x0127);
   lcdWriteRegisterWord(dev, 0x0001,0x0027);
   lcdWriteRegisterWord(dev, 0x0002,0x0013);
   lcdWriteRegisterWord(dev, 0x0003,0x0003);
   lcdWriteRegisterWord(dev, 0x0008,0x0303);
   lcdWriteRegisterWord(dev, 0x000A,0x000B);
   lcdWriteRegisterWord(dev, 0x000B,0x0003);
   lcdWriteRegisterWord(dev, 0x000C,0x0000);
   lcdWriteRegisterWord(dev, 0x0041,0x0000);
   lcdWriteRegisterWord(dev, 0x0050,0x0000);
   lcdWriteRegisterWord(dev, 0x0060,0x0005);
   lcdWriteRegisterWord(dev, 0x0070,0x000B);
   lcdWriteRegisterWord(dev, 0x0071,0x0000);
   lcdWriteRegisterWord(dev, 0x0078,0x0000);
   lcdWriteRegisterWord(dev, 0x007A,0x0000);
   lcdWriteRegisterWord(dev, 0x0079,0x0007);
//	 lcdWriteRegisterWord(dev, 0x0007,0x0051);
   lcdWriteRegisterWord(dev, 0x0007,0x0053);
   lcdWriteRegisterWord(dev, 0x0079,0x0000);

  // R61509V
  } else if (dev->_model == 0xB509) {
   lcdWriteRegisterWord(dev, 0x0400, 0x6200);
   lcdWriteRegisterWord(dev, 0x0008, 0x0808);

   lcdWriteRegisterWord(dev, 0x0010, 0x0016);	   //69.5Hz			0016
   lcdWriteRegisterWord(dev, 0x0011, 0x0101);
   lcdWriteRegisterWord(dev, 0x0012, 0x0000);
   lcdWriteRegisterWord(dev, 0x0013, 0x0001);

   lcdWriteRegisterWord(dev, 0x0100, 0x0330);	   //BT,AP
   lcdWriteRegisterWord(dev, 0x0101, 0x0237);	   //DC0,DC1,VC
   lcdWriteRegisterWord(dev, 0x0103, 0x0D00);	   //VDV
   lcdWriteRegisterWord(dev, 0x0280, 0x6100);	   //VCM
   lcdWriteRegisterWord(dev, 0x0102, 0xC1B0);	   //VRH,VCMR,PSON,PON
   delay(50);

//	 lcdWriteRegisterWord(dev, 0x0001, 0x0100);
   lcdWriteRegisterWord(dev, 0x0001, 0x0000);
   lcdWriteRegisterWord(dev, 0x0002, 0x0100);
   lcdWriteRegisterWord(dev, 0x0003, 0x1030);		//1030
   lcdWriteRegisterWord(dev, 0x0009, 0x0001);
   lcdWriteRegisterWord(dev, 0x000C, 0x0000);
   lcdWriteRegisterWord(dev, 0x0090, 0x8000);
   lcdWriteRegisterWord(dev, 0x000F, 0x0000);

   lcdWriteRegisterWord(dev, 0x0210, 0x0000);
   lcdWriteRegisterWord(dev, 0x0211, 0x00EF);
   lcdWriteRegisterWord(dev, 0x0212, 0x0000);
   lcdWriteRegisterWord(dev, 0x0213, 0x018F);		//432=01AF,400=018F
   lcdWriteRegisterWord(dev, 0x0500, 0x0000);
   lcdWriteRegisterWord(dev, 0x0501, 0x0000);
   lcdWriteRegisterWord(dev, 0x0502, 0x005F);
   lcdWriteRegisterWord(dev, 0x0401, 0x0001);
   lcdWriteRegisterWord(dev, 0x0404, 0x0000);
   delay(50);

   lcdWriteRegisterWord(dev, 0x0007, 0x0100);		  //BASEE
   delay(50);

   lcdWriteRegisterWord(dev, 0x0200, 0x0000);
   lcdWriteRegisterWord(dev, 0x0201, 0x0000);

  // ST7781
  } else if (dev->_model == 0x7781) {
   lcdWriteRegisterWord(dev, 0x00FF, 0x0001);	  //can we do 0xFF
   lcdWriteRegisterWord(dev, 0x00F3, 0x0008);
   lcdWriteRegisterWord(dev, 0x0000, 0x0001);
   lcdWriteRegisterWord(dev, 0x0001, 0x0000);	  // Driver Output Control Register (R01h)
   lcdWriteRegisterWord(dev, 0x0002, 0x0700);	  // LCD Driving Waveform Control (R02h)
   lcdWriteRegisterWord(dev, 0x0003, 0x1030);	  // Entry Mode (R03h)
   lcdWriteRegisterWord(dev, 0x0008, 0x0302);
   lcdWriteRegisterWord(dev, 0x0009, 0x0000);
   lcdWriteRegisterWord(dev, 0x0010, 0x0000);	  // Power Control 1 (R10h)
   lcdWriteRegisterWord(dev, 0x0011, 0x0007);	  // Power Control 2 (R11h)
   lcdWriteRegisterWord(dev, 0x0012, 0x0000);	  // Power Control 3 (R12h)
   lcdWriteRegisterWord(dev, 0x0013, 0x0000);	  // Power Control 4 (R13h)
   delay(50);
   lcdWriteRegisterWord(dev, 0x0010, 0x14B0);	  // Power Control 1 SAP=1, BT=4, APE=1, AP=3
   delay(10);
   lcdWriteRegisterWord(dev, 0x0011, 0x0007);	  // Power Control 2 VC=7
   delay(10);
   lcdWriteRegisterWord(dev, 0x0012, 0x008E);	  // Power Control 3 VCIRE=1, VRH=14
   lcdWriteRegisterWord(dev, 0x0013, 0x0C00);	  // Power Control 4 VDV=12
   lcdWriteRegisterWord(dev, 0x0029, 0x0015);	  // NVM read data 2 VCM=21
   delay(10);
   lcdWriteRegisterWord(dev, 0x0030, 0x0000);	  // Gamma Control 1
   lcdWriteRegisterWord(dev, 0x0031, 0x0107);	  // Gamma Control 2
   lcdWriteRegisterWord(dev, 0x0032, 0x0000);	  // Gamma Control 3
   lcdWriteRegisterWord(dev, 0x0035, 0x0203);	  // Gamma Control 6
   lcdWriteRegisterWord(dev, 0x0036, 0x0402);	  // Gamma Control 7
   lcdWriteRegisterWord(dev, 0x0037, 0x0000);	  // Gamma Control 8
   lcdWriteRegisterWord(dev, 0x0038, 0x0207);	  // Gamma Control 9
   lcdWriteRegisterWord(dev, 0x0039, 0x0000);	  // Gamma Control 10
   lcdWriteRegisterWord(dev, 0x003C, 0x0203);	  // Gamma Control 13
   lcdWriteRegisterWord(dev, 0x003D, 0x0403);	  // Gamma Control 14
   lcdWriteRegisterWord(dev, 0x0060, 0xA700);	  // Driver Output Control (R60h) .kbv was 0xa700
   lcdWriteRegisterWord(dev, 0x0061, 0x0001);	  // Driver Output Control (R61h)
   lcdWriteRegisterWord(dev, 0x0090, 0X0029);	  // Panel Interface Control 1 (R90h)
   lcdWriteRegisterWord(dev, 0x0007, 0x0133);	  // Display Control (R07h)

  }
}
#endif


// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(TFT_t *dev, uint16_t x, uint16_t y, uint16_t color) {
	(*DrawPixel)(dev, x, y, color);


#if 0
  //if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505 || dev->_model == 0x7781) {
  if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505
   || dev->_model == 0x7781 || dev->_model == 0x1121) {
   lcdWriteRegisterWord(dev, 0x0020, x); // RAM Address Set 1
   lcdWriteRegisterWord(dev, 0x0021, y); // RAM Address Set 2
   lcdWriteRegisterWord(dev, 0x0022, color); // Write Data to GRAM

  } else if (dev->_model == 0x9327 || dev->_model == 0x9341 || dev->_model == 0x9342 || dev->_model == 0x9481) {
   lcdWriteCommandByte(dev, 0x2A); // set column(x) address
   lcdWriteDataWord(dev, x);
   lcdWriteDataWord(dev, x);
   lcdWriteCommandByte(dev, 0x2B); // set Page(y) address
   lcdWriteDataWord(dev, y);
   lcdWriteDataWord(dev, y);
   lcdWriteCommandByte(dev, 0x2C); // Memory Write
#ifndef P16BIT
   lcdWriteDataWord(dev, color);
#else
   lcdWriteDataWord_16bit(dev, color);
#endif

  } else if (dev->_model == 0xB509) {
	lcdWriteRegisterWord(dev, 0x0200,x); // RAM Address Set 1
	lcdWriteRegisterWord(dev, 0x0201,y); // RAM Address Set 2
	lcdWriteRegisterWord(dev, 0x0202,color); // Write Data to GRAM
  }
#endif
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	(*DrawFillRect)(dev, x1, y1, x2, y2, color);

#if 0
  int i,j;
  if (x1 >= dev->_width) return;
  if (x2 >= dev->_width) x2 = dev->_width-1;
  if (y1 >= dev->_height) return;
  if (y2 >= dev->_height) y2 = dev->_height-1;

  //if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505 || dev->_model == 0x7781) {
  if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505
   || dev->_model == 0x7781 || dev->_model == 0x1121) {
   for(j=y1;j<=y2;j++){
	lcdWriteRegisterWord(dev, 0x0020, x1); // RAM Address Set 1
	lcdWriteRegisterWord(dev, 0x0021, j); // RAM Address Set 2
	lcdWriteCommandWord(dev, 0x0022); // Write Data to GRAM
	for(i=x1;i<=x2;i++){
	  lcdWriteDataWord(dev, color); // Write Data to GRAM
	}
   }

  } else if (dev->_model == 0x9327 || dev->_model == 0x9341 || dev->_model == 0x9342 || dev->_model == 0x9481) {
   lcdWriteCommandByte(dev, 0x2A); // set column(x) address
   lcdWriteDataWord(dev, x1);
   lcdWriteDataWord(dev, x2);
   lcdWriteCommandByte(dev, 0x2B); // set Page(y) address
   lcdWriteDataWord(dev, y1);
   lcdWriteDataWord(dev, y2);
   lcdWriteCommandByte(dev, 0x2C); // Memory Write

   for(i=x1;i<=x2;i++){
	for(j=y1;j<=y2;j++){
#ifndef P16BIT
	  lcdWriteDataWord(dev, color);
#else
	  lcdWriteDataWord_16bit(dev, color);
#endif
	}
   }

  } else if (dev->_model == 0xB509) {
	for(i=x1;i<=x2;i++){
	  for(j=y1;j<=y2;j++){
		lcdWriteRegisterWord(dev, 0x0200,i);
		lcdWriteRegisterWord(dev, 0x0201,j);
		lcdWriteRegisterWord(dev, 0x0202,color);
	  }
	}
  }
#endif

}

// Display Off
void lcdDisplayOff(TFT_t *dev) {
	(*DisplayOff)(dev);

#if 0
  if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505 || dev->_model == 0x7781) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
  } else if (dev->_model == 0x9327 || dev->_model == 0x9341 || dev->_model == 0x9342 || dev->_model == 0x9481) {
	lcdWriteCommandByte(dev, 0x28); 
  } else if (dev->_model == 0x1121) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0010);	// Set GON=0 D1=0, D0=0
  } else if (dev->_model == 0xB509) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0000);	// Display Control 1
  }
#endif
}

// Display On
void lcdDisplayOn(TFT_t *dev) {
	(*DisplayOn)(dev);

#if 0
  if (dev->_model == 0x9325 || dev->_model == 0x5408 || dev->_model == 0x1505 || dev->_model == 0x7781) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
  } else if (dev->_model == 0x9327 || dev->_model == 0x9341 || dev->_model == 0x9342 || dev->_model == 0x9481) {
	lcdWriteCommandByte(dev, 0x29); 
  } else if (dev->_model == 0x1121) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0053);	// Set GON=1 D1=1, D0=1
  } else if (dev->_model == 0xB509) {
	lcdWriteRegisterWord(dev, 0x0007, 0x0100);	// Display Control 1
  }
#endif
}

// Inversion Off
void lcdInversionOff(TFT_t *dev) {
	(*InversionOff)(dev);
}

// Inversion On
void lcdInversionOn(TFT_t *dev) {
	(*InversionOn)(dev);
}

// Fill screen
// color:color
void lcdFillScreen(TFT_t *dev, uint16_t color) {
  lcdDrawFillRect(dev, 0, 0, dev->_width-1, dev->_height-1, color);
}

// Fadeout screen
// color:color
void lcdFadeout(TFT_t *dev, int direction, uint16_t color) {
  if (direction == 0) {
	for (int x=0;x<dev->_width-1;x++) {
	  lcdDrawFillRect(dev, x, 0, x+1, dev->_height-1, color);
	}
  } else {
	for (int x=dev->_width-1;x>0;x--) {
	  lcdDrawFillRect(dev, x-1, 0, x, dev->_height-1, color);
	}
  }
}

// Draw line
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color 
void lcdDrawLine(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
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
	  lcdDrawPixel(dev, x1, y1, color);
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
	  lcdDrawPixel(dev, x1, y1, color);
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
// x2:End	X coordinate
// y2:End	Y coordinate
// color:color
void lcdDrawRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  lcdDrawLine(dev,x1,y1,x2,y1,color);
  lcdDrawLine(dev,x2,y1,x2,y2,color);
  lcdDrawLine(dev,x2,y2,x1,y2,color);
  lcdDrawLine(dev,x1,y2,x1,y1,color);
}

// Draw round
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  
  x=0;
  y=-r;
  err=2-2*r;
  do{
	lcdDrawPixel(dev, x0-x, y0+y, color); 
	lcdDrawPixel(dev, x0-y, y0-x, color); 
	lcdDrawPixel(dev, x0+x, y0-y, color); 
	lcdDrawPixel(dev, x0+y, y0+x, color); 
	if ((old_err=err)<=x)	err+=++x*2+1;
	if (old_err>y || err>x) err+=++y*2+1;	 
  } while(y<0);
  
}

// Draw round of filling
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawFillCircle(TFT_t *dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
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
	  lcdDrawLine(dev,x0-x,y0-y,x0-x,y0+y,color);
	  lcdDrawLine(dev,x0+x,y0-y,x0+x,y0+y,color);
	} // if
	ChangeX=(old_err=err)<=x;
	if (ChangeX)			err+=++x*2+1;
	if (old_err>y || err>x) err+=++y*2+1;
  } while(y<=0);
	
} 

// Draw rectangule with round corner
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End	X coordinate
// y2:End	Y coordinate
// r:radius
// color:color
void lcdDrawRoundRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  unsigned char temp;

  if(x1>x2) {
	temp=x1; x1=x2; x2=temp;
  }
  
  if(y1>y2) {
	temp=y1; y1=y2; y2=temp;
  }
  if(x2-x1 < r) return; // Add 20190517
  if(y2-y1 < r) return; // Add 20190517
  
  
  x=0;
  y=-r;
  err=2-2*r;

  do{
	if(x) {
	  lcdDrawPixel(dev, x1+r-x, y1+r+y, color); 
	  lcdDrawPixel(dev, x2-r+x, y1+r+y, color); 
	  lcdDrawPixel(dev, x1+r-x, y2-r-y, color); 
	  lcdDrawPixel(dev, x2-r+x, y2-r-y, color);
	} // if 
	if ((old_err=err)<=x)	err+=++x*2+1;
	if (old_err>y || err>x) err+=++y*2+1;	 
  } while(y<0);
   
  lcdDrawLine(dev, x1+r,y1 ,x2-r,y1 ,color);
  lcdDrawLine(dev, x1+r,y2 ,x2-r,y2 ,color);
  lcdDrawLine(dev, x1 ,y1+r,x1 ,y2-r, color);
  lcdDrawLine(dev, x2 ,y1+r,x2 ,y2-r, color);  
} 

// Draw arrow
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End	X coordinate
// y2:End	Y coordinate
// w:Width of the botom
// color:color
// Thanks http://k-hiura.cocolog-nifty.com/blog/2010/11/post-2a62.html
void lcdDrawArrow(TFT_t *dev, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//	 printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//	 printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

//	 lcdDrawLine(dev,x0,y0,x1,y1,color);
   lcdDrawLine(dev,x1,y1,L[0],L[1],color);
   lcdDrawLine(dev,x1,y1,R[0],R[1],color);
   lcdDrawLine(dev,L[0],L[1],R[0],R[1],color);
}


// Draw arrow of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End	X coordinate
// y2:End	Y coordinate
// w:Width of the botom
// color:color
void lcdDrawFillArrow(TFT_t *dev, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

   double Vx= x1 - x0;
   double Vy= y1 - y0;
   double v = sqrt(Vx*Vx+Vy*Vy);
//	 printf("v=%f\n",v);
   double Ux= Vx/v;
   double Uy= Vy/v;

   uint16_t L[2],R[2];
   L[0]= x1 - Uy*w - Ux*v;
   L[1]= y1 + Ux*w - Uy*v;
   R[0]= x1 + Uy*w - Ux*v;
   R[1]= y1 - Ux*w - Uy*v;
//	 printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

   lcdDrawLine(dev,x0,y0,x1,y1,color);
   lcdDrawLine(dev,x1,y1,L[0],L[1],color);
   lcdDrawLine(dev,x1,y1,R[0],R[1],color);
   lcdDrawLine(dev,L[0],L[1],R[0],R[1],color);

   int ww;
   for(ww=w-1;ww>0;ww--) {
	 L[0]= x1 - Uy*ww - Ux*v;
	 L[1]= y1 + Ux*ww - Uy*v;
	 R[0]= x1 + Uy*ww - Ux*v;
	 R[1]= y1 - Ux*ww - Uy*v;
//	   printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
	 lcdDrawLine(dev,x1,y1,L[0],L[1],color);
	 lcdDrawLine(dev,x1,y1,R[0],R[1],color);
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
int lcdDrawSJISChar(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color) {
  uint16_t xx,yy,bit,ofs;
  unsigned char fonts[128]; // font pattern
  unsigned char pw, ph;
  int h,w;
  uint16_t mask;
  bool rc;
  
if(_DEBUG_)printf("_font_direction=%d\n",dev->_font_direction);
//	sjis = UTF2SJIS(utf8);
//if(_DEBUG_)printf("sjis=%04x\n",sjis);

  rc = GetFontx(fx, sjis, fonts, &pw, &ph); // SJIS -> Font pattern
if(_DEBUG_)printf("GetFontx rc=%d pw=%d ph=%d\n",rc,pw,ph);
  if (!rc) return -1;

  int xd1, yd1;
  int xd2, yd2;
  int xss, yss;
  int xsd, ysd;
  int next;
  if (dev->_font_direction == 0) {
	xd1 = +1;
	yd1 = -1;
	xd2 =  0;
	yd2 =  0;
	xss =  x;
	yss =  y + ph - 1;
	xsd =  1;
	ysd =  0;
	next = x + pw;
  } else if (dev->_font_direction == 2) {
	xd1 = -1;
	yd1 = +1;
	xd2 =  0;
	yd2 =  0;
	xss =  x;
	yss =  y - ph + 1;
	xsd =  1;
	ysd =  0;
	next = x - pw;
  } else if (dev->_font_direction == 1) {
	xd1 =  0;
	yd1 =  0;
	xd2 = -1;
	yd2 = -1;
	xss =  x + ph;
	yss =  y;
	xsd =  0;
	ysd =  1;
	next = y - pw;
  } else if (dev->_font_direction == 3) {
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
//	  for(w=0;w<(pw/8);w++) {
	bits = pw;
	for(w=0;w<((pw+4)/8);w++) {
	  mask = 0x80;
	  for(bit=0;bit<8;bit++) {
		bits--;
		if (bits < 0) continue;
//if(_DEBUG_)printf("xx=%d yy=%d mask=%02x fonts[%d]=%02x\n",xx,yy,mask,ofs,fonts[ofs]);
		if (fonts[ofs] & mask) {
		  lcdDrawPixel(dev, xx, yy, color);
		} else {
		  if (dev->_font_fill) lcdDrawPixel(dev, xx, yy, dev->_font_fill_color);
		}
		if (h == (ph-2) && dev->_font_underline)
		  lcdDrawPixel(dev, xx, yy, dev->_font_underline_color);
		if (h == (ph-1) && dev->_font_underline)
		  lcdDrawPixel(dev, xx, yy, dev->_font_underline_color);

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
int lcdDrawUTF8Char(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color) {
  uint16_t sjis[1];
  
  sjis[0] = UTF2SJIS(utf8);
if(_DEBUG_)printf("sjis=%04x\n",sjis[0]);
  return lcdDrawSJISChar(dev,fx,x,y,sjis[0],color);
}

// Draw UTF8 string
// x:X coordinate
// y:Y coordinate
// utfs: UTF8 string
// color:color
int lcdDrawUTF8String(TFT_t *dev, FontxFile *fx, uint16_t x,uint16_t y,unsigned char *utfs,uint16_t color) {

  int i;
  int spos;
  uint16_t sjis[64];
  spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 64);
if(_DEBUG_)printf("spos=%d\n",spos);
  for(i=0;i<spos;i++) {
if(_DEBUG_)printf("sjis[%d]=0x%x x=%d y=%d\n",i,sjis[i],x,y);
	if (dev->_font_direction == 0)
	  x=lcdDrawSJISChar(dev,fx,x,y,sjis[i],color);
	if (dev->_font_direction == 1)
	  y=lcdDrawSJISChar(dev,fx,x,y,sjis[i],color);
	if (dev->_font_direction == 2)
	  x=lcdDrawSJISChar(dev,fx,x,y,sjis[i],color);
	if (dev->_font_direction == 3)
	  y=lcdDrawSJISChar(dev,fx,x,y,sjis[i],color);
  }
  if (dev->_font_direction == 0) return x;
  if (dev->_font_direction == 2) return x;
  if (dev->_font_direction == 1) return y;
  if (dev->_font_direction == 3) return y;
  return 0;
}

// Set font direction
// dir:Direction
void lcdSetFontDirection(TFT_t *dev, uint16_t dir) {
  dev->_font_direction = dir;
}

// Set font filling
// color:fill color
void lcdSetFontFill(TFT_t *dev, uint16_t color) {
  dev->_font_fill = true;
  dev->_font_fill_color = color;
}

// UnSet font filling
void lcdUnsetFontFill(TFT_t *dev) {
  dev->_font_fill = false;
}

// Set font underline
// color:frame color
void lcdSetFontUnderLine(TFT_t *dev, uint16_t color) {
  dev->_font_underline = true;
  dev->_font_underline_color = color;
}

// UnSet font underline
void lcdUnsetFontUnderLine(TFT_t *dev) {
  dev->_font_underline = false;
}


int ReadPinConfig(GPIO_t *pin, char *path) {
  FILE *fp;
  char buff[128];

//	printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
//	  printf("buf=%s\n",buff);
//	  printf("buff[0]=%x\n",buff[0]);
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
