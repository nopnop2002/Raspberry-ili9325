#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../tft_lib.h"
#include "st7781.h"

void st7781_lcdInit(TFT_t *dev, int width, int height, int offsetx, int offsety) {
#ifdef ST7781
	lcdInit(dev, 0x7781, width, height, offsetx, offsety);
#elif defined ST7783
	lcdInit(dev, 0x7783, width, height, offsetx, offsety);
#endif
	printf("st7781_lcdInit _model=%x\n", dev->_model);

	static const uint16_t regValues[] = {
		0x00FF, 0x0001,		//can we do 0xFF
		0x00F3, 0x0008,
		0x0000, 0x0001,
		0x0001, 0x0000,		// Driver Output Control Register (R01h)
		0x0002, 0x0700,		// LCD Driving Waveform Control (R02h)
		0x0003, 0x1030,		// Entry Mode (R03h)
		0x0008, 0x0302,
		0x0009, 0x0000,
		0x0010, 0x0000,		// Power Control 1 (R10h)
		0x0011, 0x0007,		// Power Control 2 (R11h)
		0x0012, 0x0000,		// Power Control 3 (R12h)
		0x0013, 0x0000,		// Power Control 4 (R13h)
		TFTLCD_DELAY16,50,
		0x0010, 0x14B0,		// Power Control 1 SAP=1, BT=4, APE=1, AP=3
		TFTLCD_DELAY16,10,
		0x0011, 0x0007,		// Power Control 2 VC=7
		TFTLCD_DELAY16,10,
		0x0012, 0x008E,		// Power Control 3 VCIRE=1, VRH=14
		0x0013, 0x0C00,		// Power Control 4 VDV=12
		0x0029, 0x0015,		// NVM read data 2 VCM=21
		TFTLCD_DELAY16,10,
		0x0030, 0x0000,		// Gamma Control 1
		0x0031, 0x0107,		// Gamma Control 2
		0x0032, 0x0000,		// Gamma Control 3
		0x0035, 0x0203,		// Gamma Control 6
		0x0036, 0x0402,		// Gamma Control 7
		0x0037, 0x0000,		// Gamma Control 8
		0x0038, 0x0207,		// Gamma Control 9
		0x0039, 0x0000,		// Gamma Control 10
		0x003C, 0x0203,		// Gamma Control 13
		0x003D, 0x0403,		// Gamma Control 14
		0x0060, 0xA700,		// Driver Output Control (R60h) .kbv was 0xa700
		0x0061, 0x0001,		// Driver Output Control (R61h)
		0x0090, 0X0029,		// Panel Interface Control 1 (R90h)
		0x0007, 0x0133,		// Display Control (R07h)
	};
	lcdInitTable16(dev, regValues, sizeof(regValues));

#if 0
	lcdWriteRegisterWord(dev, 0x00FF, 0x0001);	   //can we do 0xFF
	lcdWriteRegisterWord(dev, 0x00F3, 0x0008);
	lcdWriteRegisterWord(dev, 0x0000, 0x0001);
	lcdWriteRegisterWord(dev, 0x0001, 0x0000);	   // Driver Output Control Register (R01h)
	lcdWriteRegisterWord(dev, 0x0002, 0x0700);	   // LCD Driving Waveform Control (R02h)
	lcdWriteRegisterWord(dev, 0x0003, 0x1030);	   // Entry Mode (R03h)
	lcdWriteRegisterWord(dev, 0x0008, 0x0302);
	lcdWriteRegisterWord(dev, 0x0009, 0x0000);
	lcdWriteRegisterWord(dev, 0x0010, 0x0000);	   // Power Control 1 (R10h)
	lcdWriteRegisterWord(dev, 0x0011, 0x0007);	   // Power Control 2 (R11h)
	lcdWriteRegisterWord(dev, 0x0012, 0x0000);	   // Power Control 3 (R12h)
	lcdWriteRegisterWord(dev, 0x0013, 0x0000);	   // Power Control 4 (R13h)
	TFTLCD_DELAY16(50);
	lcdWriteRegisterWord(dev, 0x0010, 0x14B0);	   // Power Control 1 SAP=1, BT=4, APE=1, AP=3
	TFTLCD_DELAY16(10);
	lcdWriteRegisterWord(dev, 0x0011, 0x0007);	   // Power Control 2 VC=7
	TFTLCD_DELAY16(10);
	lcdWriteRegisterWord(dev, 0x0012, 0x008E);	   // Power Control 3 VCIRE=1, VRH=14
	lcdWriteRegisterWord(dev, 0x0013, 0x0C00);	   // Power Control 4 VDV=12
	lcdWriteRegisterWord(dev, 0x0029, 0x0015);	   // NVM read data 2 VCM=21
	TFTLCD_DELAY16(10);
	lcdWriteRegisterWord(dev, 0x0030, 0x0000);	   // Gamma Control 1
	lcdWriteRegisterWord(dev, 0x0031, 0x0107);	   // Gamma Control 2
	lcdWriteRegisterWord(dev, 0x0032, 0x0000);	   // Gamma Control 3
	lcdWriteRegisterWord(dev, 0x0035, 0x0203);	   // Gamma Control 6
	lcdWriteRegisterWord(dev, 0x0036, 0x0402);	   // Gamma Control 7
	lcdWriteRegisterWord(dev, 0x0037, 0x0000);	   // Gamma Control 8
	lcdWriteRegisterWord(dev, 0x0038, 0x0207);	   // Gamma Control 9
	lcdWriteRegisterWord(dev, 0x0039, 0x0000);	   // Gamma Control 10
	lcdWriteRegisterWord(dev, 0x003C, 0x0203);	   // Gamma Control 13
	lcdWriteRegisterWord(dev, 0x003D, 0x0403);	   // Gamma Control 14
	lcdWriteRegisterWord(dev, 0x0060, 0xA700);	   // Driver Output Control (R60h) .kbv was 0xa700
	lcdWriteRegisterWord(dev, 0x0061, 0x0001);	   // Driver Output Control (R61h)
	lcdWriteRegisterWord(dev, 0x0090, 0X0029);	   // Panel Interface Control 1 (R90h)
	lcdWriteRegisterWord(dev, 0x0007, 0x0133);	   // Display Control (R07h)
#endif

	// st7781 custom function
	DrawPixel = ili9325_lcdDrawPixel;
	DrawFillRect = ili9325_lcdDrawFillRect;
	DisplayOff = ili9325_lcdDisplayOff;
	DisplayOn = ili9325_lcdDisplayOn;
	InversionOff = ili9325_lcdInversionOff;
	InversionOn = ili9325_lcdInversionOn;
}

