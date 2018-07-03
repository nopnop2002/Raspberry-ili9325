#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Convert RBG to RGB565.
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


void usage (char * prog) {
  printf("%s HTML_Color_Code\n",prog);
  printf("%s 0xff0000\n",prog);
}


int main(int argc, char **argv)
{
  if(argc != 2) {
    usage(argv[0]);
    return 1;
  }

  char html[10];
  strcpy(html,argv[1]);
  if (strncmp(html,"0x",2) == 0) {
    strcpy(html,&html[2]);
  }
  //printf("html=%s\n",html);
  if (strlen(html) != 6) {
    usage(argv[0]);
    return 1;
  }
  uint16_t r,g,b;
  char rc[3],gc[3],bc[3];
  memset(rc,0,sizeof(rc));
  memset(gc,0,sizeof(gc));
  memset(bc,0,sizeof(bc));
  rc[0]=html[0];
  rc[1]=html[1];
  gc[0]=html[2];
  gc[1]=html[3];
  bc[0]=html[4];
  bc[1]=html[5];

  r = strtol(rc,NULL,16);
  g = strtol(gc,NULL,16);
  b = strtol(bc,NULL,16);
  //printf("r=%x g=%x b=%x\n",r,g,b);
  uint16_t color;
  color = rgb565_conv(r, g, b);
  printf("0x%04x\n",color);
}

