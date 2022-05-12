#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>

#define __TFT_LIB_C__
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

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(TFT_t *dev, uint16_t x, uint16_t y, uint16_t color) {
	(*DrawPixel)(dev, x, y, color);
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(TFT_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	(*DrawFillRect)(dev, x1, y1, x2, y2, color);
}

// Display Off
void lcdDisplayOff(TFT_t *dev) {
	(*DisplayOff)(dev);
}

// Display On
void lcdDisplayOn(TFT_t *dev) {
	(*DisplayOn)(dev);
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
  if (direction == DIRECTION0) {
	for (int x=0;x<dev->_width-1;x++) {
	  lcdDrawFillRect(dev, x, 0, x+1, dev->_height-1, color);
    }
  } else if (direction == DIRECTION90) {
	for (int y=dev->_height-1;y>0;y--) {
	  lcdDrawFillRect(dev, 0, y-1, dev->_width-1, y, color);
	}
  } else if (direction == DIRECTION180) {
	for (int x=dev->_width-1;x>0;x--) {
	  lcdDrawFillRect(dev, x-1, 0, x, dev->_height-1, color);
    }
  } else if (direction == DIRECTION270) {
	for (int y=0;y<dev->_height-1;y++) {
	  lcdDrawFillRect(dev, 0, y, dev->_width-1, y+1, color);
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

  //printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
	//printf("buf=[%s]\n",buff);
	//printf("buff[0]=%x\n",buff[0]);

	int buffLen = strlen(buff);
	for (int index=0;index<strlen(buff);index++) {
	  //printf("buff[%d]=%x\n", index, buff[index]);
	  if (buff[index] == '#') buff[index] = 0;
	  if (buff[index] == ';') buff[index] = 0;
	  if (buff[index] == 0x0d) buff[index] = 0;
	  if (buff[index] == 0x0a) buff[index] = 0;
 	}
	//printf("buf=%d [%s]\n",strlen(buff), buff);
	if (strlen(buff) == 0) continue;

	//if (buff[0] == '#') continue;
	//if (buff[0] == 0x0a) continue;
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
