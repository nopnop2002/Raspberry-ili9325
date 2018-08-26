#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include "ili93xx.h"

typedef struct {
  char name[32];
  char font[32];
  unsigned char utf[32];
  uint16_t x1;
  uint16_t y1;
  uint16_t x2;
  uint16_t y2;
  uint16_t r;
  uint16_t t;
  uint16_t color;
  uint16_t dir;
} cmd_t;

FontxFile fxG32[2];
FontxFile fxM32[2];
FontxFile fxG24[2];
FontxFile fxM24[2];
FontxFile fxG16[2];
FontxFile fxM16[2];

#define _DEBUG_ 0

int cmdParse(char * buf, cmd_t * hoge) {
  int pos=0;
  int index=0;
  int i;
  char wk[10][64];
  for(i=0;i<strlen(buf);i++) {
    if (buf[i] == ',') {
      //printf("wk=%s\n",wk[index]);
      pos = 0;
      index++;
    } else {
      if (pos < 63) {
        wk[index][pos++] = buf[i];
        wk[index][pos] = 0;
      }
    }
  }
  if (pos != 0) index++;
 
#if 0
  printf("index=%d\n",index);
  for(i=0;i<index;i++) {
    printf("wk[%d]=%s\n",i,wk[i]);
  }
#endif

  if (strcmp(wk[0],"DrawPixel") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->color = strtol(wk[3],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawRect") == 0 ||
             strcmp(wk[0],"DrawFillRect") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->x2 = strtol(wk[3],NULL,0);
    hoge->y2 = strtol(wk[4],NULL,0);
    hoge->color = strtol(wk[5],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawRoundRect") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->x2 = strtol(wk[3],NULL,0);
    hoge->y2 = strtol(wk[4],NULL,0);
    hoge->r = strtol(wk[5],NULL,0);
    hoge->color = strtol(wk[6],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"FillScrren") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->color = strtol(wk[1],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawLine") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->x2 = strtol(wk[3],NULL,0);
    hoge->y2 = strtol(wk[4],NULL,0);
    hoge->color = strtol(wk[5],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawCircle") == 0 || 
             strcmp(wk[0],"DrawFillCircle") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->r = strtol(wk[3],NULL,0);
    hoge->color = strtol(wk[4],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawArrow") == 0 ||
             strcmp(wk[0],"DrawFillArrow") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->x1 = strtol(wk[1],NULL,0);
    hoge->y1 = strtol(wk[2],NULL,0);
    hoge->x2 = strtol(wk[3],NULL,0);
    hoge->y2 = strtol(wk[4],NULL,0);
    hoge->t = strtol(wk[5],NULL,0);
    hoge->color = strtol(wk[6],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"DrawUTF8String") == 0) {
    strcpy(hoge->name,wk[0]);
    strcpy(hoge->font,wk[1]);
    hoge->x1 = strtol(wk[2],NULL,0);
    hoge->y1 = strtol(wk[3],NULL,0);
    strcpy((char *)hoge->utf,wk[4]);
    hoge->color = strtol(wk[5],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"SetFontDirection") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->dir = strtol(wk[1],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"SetFontFill") == 0 ||
             strcmp(wk[0],"SetFontUnderLine") == 0) {
    strcpy(hoge->name,wk[0]);
    hoge->color = strtol(wk[1],NULL,0);
    return 1;

  } else if (strcmp(wk[0],"UnsetFontFill") == 0 ||
             strcmp(wk[0],"UnsetFontUnderLine") == 0) {
    strcpy(hoge->name,wk[0]);
    return 1;
  } else {
    return 0;
  }
}


void cmdDump(cmd_t hoge) {
  printf("name=%s\n",hoge.name);
  printf("font=%s ",hoge.font);
  printf("x1=%d ",hoge.x1);
  printf("y1=%d ",hoge.y1);
  printf("x2=%d ",hoge.x2);
  printf("y2=%d ",hoge.y2);
  printf("r=%d ",hoge.r);
  printf("t=%d ",hoge.t);
  printf("utf=%s ",hoge.utf);
  printf("dir=%d ",hoge.dir);
  printf("color=%x\n",hoge.color);
}



int main(int argc, char **argv){
  int i;
  int XMAX,YMAX;
  char model[20];

if(_DEBUG_)printf("argc=%d\n",argc);
  if (argc != 2) {
    printf("%s draw_file_name\n",argv[0]);
    return 1;
  }

  if(wiringPiSetup() == -1) {
    printf("Setup Fail\n");
    return 1;
  }

  char base[128];
  strcpy(base, argv[0]);
  for(i=strlen(base);i>0;i--) {
    if (base[i-1] == '/') {
      base[i] = 0;
      break;
    }
  }
//printf("base=%s\n",base);

  // You can change font file
  // 32Dot Gothic
  char G32[2][128];
  strcpy(G32[0],base);
  strcpy(G32[1],base);
  strcat(G32[0],"fontx/ILGH32XB.FNT");
  strcat(G32[1],"fontx/ILGZ32XB.FNT");
//printf("%s\n",G32[0]);
//printf("%s\n",G32[1]);
  Fontx_init(fxG32,G32[0],G32[1]);

  // 32Dot Mincho
  char M32[2][128];
  strcpy(M32[0],base);
  strcpy(M32[1],base);
  strcat(M32[0],"fontx/ILMH32XF.FNT");
  strcat(M32[1],"fontx/ILMZ32XF.FNT");
//printf("%s\n",M32[0]);
//printf("%s\n",M32[1]);
  Fontx_init(fxM32,M32[0],M32[1]);

  // 24Dot Gothic
  char G24[2][128];
  strcpy(G24[0],base);
  strcpy(G24[1],base);
  strcat(G24[0],"fontx/ILGH24XB.FNT");
  strcat(G24[1],"fontx/ILGZ24XB.FNT");
//printf("%s\n",G24[0]);
//printf("%s\n",G24[1]);
  Fontx_init(fxG24,G24[0],G24[1]);

  // 24Dot Mincho
  char M24[2][128];
  strcpy(M24[0],base);
  strcpy(M24[1],base);
  strcat(M24[0],"fontx/ILMH24XF.FNT");
  strcat(M24[1],"fontx/ILMZ24XF.FNT");
//printf("%s\n",M24[0]);
//printf("%s\n",M24[1]);
  Fontx_init(fxM24,M24[0],M24[1]);

  // 16Dot Gothic
  char G16[2][128];
  strcpy(G16[0],base);
  strcpy(G16[1],base);
  strcat(G16[0],"fontx/ILGH16XB.FNT");
  strcat(G16[1],"fontx/ILGZ16XB.FNT");
  Fontx_init(fxG16,G16[0],G16[1]);

  // 16Dot Mincho
  char M16[2][128];
  strcpy(M16[0],base);
  strcpy(M16[1],base);
  strcat(M16[0],"fontx/ILMH16XB.FNT");
  strcat(M16[1],"fontx/ILMZ16XB.FNT");
  Fontx_init(fxM16,M16[0],M16[1]);


#if 0
  Fontx_dump(fxG32);
  Fontx_dump(fxG24);
  Fontx_dump(fxM32);
  Fontx_dump(fxM24);
  Fontx_dump(fxM16);
  Fontx_dump(fxM16);
#endif

  // set full path of pin.con
  char ppath[128];
  strcpy(ppath,base);
  strcat(ppath,"pin.conf");
//printf("ppath=%s\n",ppath);

  // set full path of draw file
  char dpath[128];
  FILE *fp;
  strcpy(dpath,base);
  strcat(dpath,argv[1]);
//printf("dpath=%s\n",dpath);
  if ((fp = fopen(dpath, "r")) == NULL) {
    printf("draw file [%s] not found\n",dpath);
    return 1;
  }

#ifdef ILI9325
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9325");
  printf("mode:%s\n",model);
  lcdInit(0x9325,XMAX,YMAX,ppath);
#endif

#ifdef ILI9327
  XMAX = 240;
  YMAX = 400;
  strcpy(model,"ILI9327");
  printf("mode:%s\n",model);
  lcdInit(0x9327,XMAX,YMAX,ppath);
#endif

#ifdef SPFD5408
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"SPFD5408");
  printf("mode:%s\n",model);
  lcdInit(0x5408,XMAX,YMAX,ppath);
#endif

#ifdef R61505U
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"R61505U");
  printf("mode:%s\n",model);
  lcdInit(0x1505,XMAX,YMAX,ppath);
#endif

#ifdef ILI9341
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9341");
#ifndef P16BIT
  printf("mode:%s\n",model);
#else
  printf("mode:%s(16Bit Parallel)\n",model);
#endif
  lcdInit(0x9341,XMAX,YMAX,ppath);
#endif

#ifdef ILI9342
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"ILI9342");
  printf("mode:%s\n",model);
  lcdInit(0x9342,XMAX,YMAX,ppath);
#endif

#ifdef ILI9481
  XMAX = 320;
  YMAX = 480;
  strcpy(model,"ILI9481");
  printf("mode:%s\n",model);
  lcdInit(0x9481,XMAX,YMAX,ppath);
#endif

#ifdef S6D1121
  XMAX = 240;
  YMAX = 320;
  strcpy(model,"S6D1121");
  printf("mode:%s\n",model);
  lcdInit(0x1121,XMAX,YMAX,ppath);
#endif

  lcdReset();
  lcdSetup();

  char buf[64];
  cmd_t cmd;
  int blen;

  while ( fgets(buf, sizeof(buf), fp) != NULL ) {
    blen = strlen(buf);
    buf[blen-1] = 0;
if(_DEBUG_)printf("buf=[%s]\n",buf);
    if (strlen(buf) == 0) continue;
    if (cmdParse(buf,&cmd) == 0) continue;
if(_DEBUG_)cmdDump(cmd);
    if (strcmp(cmd.name,"DrawPixel") == 0) {
      lcdDrawPixel(cmd.x1, cmd.y1, cmd.color);
    } else if (strcmp(cmd.name,"DrawFillRect") == 0) {
      lcdDrawFillRect(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
    } else if (strcmp(cmd.name,"FillScrren") == 0) {
      lcdFillScreen(cmd.color);
    } else if (strcmp(cmd.name,"DrawLine") == 0) {
      lcdDrawLine(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
    } else if (strcmp(cmd.name,"DrawRect") == 0) {
      lcdDrawRect(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.color);
    } else if (strcmp(cmd.name,"DrawCircle") == 0) {
      lcdDrawCircle(cmd.x1, cmd.y1, cmd.r, cmd.color);
    } else if (strcmp(cmd.name,"DrawFillCircle") == 0) {
      lcdDrawFillCircle(cmd.x1, cmd.y1, cmd.r, cmd.color);
    } else if (strcmp(cmd.name,"DrawRoundRect") == 0) {
      lcdDrawRoundRect(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.r, cmd.color);
    } else if (strcmp(cmd.name,"DrawArrow") == 0) {
      lcdDrawArrow(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.t, cmd.color);
    } else if (strcmp(cmd.name,"DrawFillArrow") == 0) {
      lcdDrawFillArrow(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.t, cmd.color);
    } else if (strcmp(cmd.name,"DrawUTF8String") == 0) {
      if (strcmp(cmd.font,"G32") == 0)
      lcdDrawUTF8String(fxG32, cmd.x1, cmd.y1, cmd.utf, cmd.color);
      if (strcmp(cmd.font,"G24") == 0)
      lcdDrawUTF8String(fxG24, cmd.x1, cmd.y1, cmd.utf, cmd.color);
      if (strcmp(cmd.font,"G16") == 0)
      lcdDrawUTF8String(fxG16, cmd.x1, cmd.y1, cmd.utf, cmd.color);

      if (strcmp(cmd.font,"M32") == 0)
      lcdDrawUTF8String(fxM32, cmd.x1, cmd.y1, cmd.utf, cmd.color);
      if (strcmp(cmd.font,"M24") == 0)
      lcdDrawUTF8String(fxM24, cmd.x1, cmd.y1, cmd.utf, cmd.color);
      if (strcmp(cmd.font,"M16") == 0)
      lcdDrawUTF8String(fxM16, cmd.x1, cmd.y1, cmd.utf, cmd.color);
    } else if (strcmp(cmd.name,"SetFontDirection") == 0) {
      lcdSetFontDirection(cmd.dir);
    } else if (strcmp(cmd.name,"SetFontFill") == 0) {
      lcdSetFontFill(cmd.color);
    } else if (strcmp(cmd.name,"UnsetFontFill") == 0) {
      lcdUnsetFontFill();
    } else if (strcmp(cmd.name,"SetFontUnderLine") == 0) {
      lcdSetFontUnderLine(cmd.color);
    } else if (strcmp(cmd.name,"UnsetFontUnderLine") == 0) {
      lcdUnsetFontUnderLine();
    }

  }
  fclose(fp);
  return 0;
}
