#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "xpt2046.h"

#define SPI_CHANNEL 0 // /dev/spidev0.0
//#define SPI_CHANNEL 1 // /dev/spidev0.1
#define GPIO_PEN 6
#define CALIBRATIO 0

int main (int argc, char **argv){
  int x, y;
  int xpos, ypos;
  int pen_irq;

  if (wiringPiSPISetup(SPI_CHANNEL, 500000) < 0) {
    printf("wiringPiSPISetup failed:\n");
    return -1;
  }

  if (wiringPiSetup() < 0) {
    printf("wiringPiSetup failed:\n");
    return -1;
  }
  pinMode(GPIO_PEN,INPUT);

#if(CALIBRATIO)
  int xmin=99999;
  int xmax=0;
  int ymin=99999;
  int ymax=0;
#endif

  for (;;) {
    usleep(10000);	/* do it anyway ; settle time when pen goes up */
    pen_irq = digitalRead(GPIO_PEN);
    if (pen_irq == LOW)	{ /* PenIRQ is LOW : touch! pen is down */
      // Get physically position
      xptGetxy(SPI_CHANNEL, &x, &y);
      // Get scrren position
      xpos = xptScrren(x);
      ypos = yptScrren(y);
      printf("touch !! x=%5d(%3d) y=%5d(%3d)\n", x, xpos, y, ypos);

#if(CALIBRATIO)
      if (xmin > x) xmin=x;
      if (xmax < x) xmax=x;
      if (ymin > y) ymin=y;
      if (ymax < y) ymax=y;
      printf("touch !! xmin=%5d xmax=%5d\n", xmin, xmax);
      printf("touch !! ymin=%5d ymax=%5d\n", ymin, ymax);
#endif

    } else { /* pen is up */
    }
  }

//  bcm2835_close();
}

