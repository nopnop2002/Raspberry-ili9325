/*
 XPT2046 Touch Screen Controller Library for Raspberry

 Original is
 https://github.com/xofc/xpt2uinput
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPiSPI.h>
#include "xpt2046.h"

#define MAX_LEN 4

#define	START	0x80
#define XPOS	0x50
#define YPOS	0x10

#define XMIN 2000.0
#define XMAX 30000.0
#define YMIN 3000.0
#define YMAX 30000.0

#define _DEBUG_ 0

int xptGetit(int spiCh, int cmd){
  char buf[MAX_LEN];
  int ret;
  int i;

  memset(buf, 0, sizeof(buf));
  buf[0] = cmd;
//  printf("wbuf=0x%02x\n",wbuf[0]);
  ret=wiringPiSPIDataRW (spiCh,buf,sizeof(buf));
//  for(i=0;i<MAX_LEN;i++) printf("%02x ",buf[i]);
//  printf("\n");
  return((buf[1]<<8)+buf[2]);
}

void xptGetxy(int spiCh, int *xp, int *yp){
  *xp = xptGetit(spiCh, START |  XPOS);
  *yp = xptGetit(spiCh, START |  YPOS);
}

int xptScrren(int xp) {
  float dx, dy;
  int   xpos, ypos;

  dx = 240.0 / (XMAX - XMIN);
  xpos = (xp-XMIN) * dx;
  xpos = 239 - xpos;
  if (xpos < 0) xpos = 0;
  if (xpos > 239) xpos = 239;
  return xpos;
}

int yptScrren(int yp) {
  float dx, dy;
  int   xpos, ypos;

  dy = 320.0 / (YMAX - YMIN);
  ypos = (yp-YMIN) * dy;
  if (ypos < 0) ypos = 0;
  if (ypos > 319) ypos = 319;
  return ypos;
}


void xptInit(TouchInfo *hoge){
  hoge->tpc = 0;
  hoge->tpx = MAXTP;
  hoge->lsec = 0;
  hoge->lusec = 0;
}

void xptDump(TouchInfo *hoge){
  int i;
  if (_DEBUG_ == 0) return;
  printf("tpc=%d\n",hoge->tpc);
  printf("tpx=%d\n",hoge->tpx);
  for(i=0;i<hoge->tpc;i++) {
    printf("x1[%02d]=%d",i,hoge->tps[i].x1);
    printf(" x2[%02d]=%d",i,hoge->tps[i].x2);
    printf(" y1[%02d]=%d",i,hoge->tps[i].y1);
    printf(" y2[%02d]=%d",i,hoge->tps[i].y2);
    printf(" id[%02d]=%d\n",i,hoge->tps[i].id);
  }
}

void xptSetPoint(TouchInfo *hoge,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id){
  if (hoge->tpc == hoge->tpx) return;
  int index = hoge->tpc;
  hoge->tps[index].x1 = x1;
  hoge->tps[index].x2 = x2;
  hoge->tps[index].y1 = y2;
  hoge->tps[index].y2 = y1;
  hoge->tps[index].id = id;
  hoge->tpc++;
}

int xptGetPoint(int spiCh, TouchInfo *hoge) {
  int x, y;
  int xpos, ypos;
  int i;
  struct timeval myTime;
  struct tm *time_st;
  suseconds_t dt;

  // Get physically position
  xptGetxy(spiCh, &x, &y);
if(_DEBUG_)printf("touch !! x=%5d y=%5d\n", x, y);
  gettimeofday(&myTime, NULL);
  time_st = localtime(&myTime.tv_sec);
  if (myTime.tv_sec == hoge->lsec) {
    dt = myTime.tv_usec - hoge->lusec;
if(_DEBUG_)printf("dt=%06d\n",dt);
    if (dt < DTMAX) return -1;
  }

  // Get scrren position
  xpos = xptScrren(x);
  ypos = yptScrren(y);
if(_DEBUG_)printf("touch !! xpos=%5d ypos=%5d\n", xpos, ypos);

  for(i=0;i<hoge->tpc;i++) {
    if(_DEBUG_) {
    printf("x1[%02d]=%d",i,hoge->tps[i].x1);
    printf(" x2[%02d]=%d",i,hoge->tps[i].x2);
    printf(" y1[%02d]=%d",i,hoge->tps[i].y1);
    printf(" y2[%02d]=%d",i,hoge->tps[i].y2);
    printf(" id[%02d]=%d\n",i,hoge->tps[i].id);
    }
    if (xpos > hoge->tps[i].x1 && xpos < hoge->tps[i].x2) {
      if (ypos > hoge->tps[i].y1 && ypos < hoge->tps[i].y2) {
        gettimeofday(&myTime, NULL);
        time_st = localtime(&myTime.tv_sec);
if(_DEBUG_)printf("myTime.tv_sec=%06d tv_usec=%06d\n",myTime.tv_sec,myTime.tv_usec);
        hoge->lsec = myTime.tv_sec;
        hoge->lusec = myTime.tv_usec;
if(_DEBUG_)printf("usec=%06d-%06d\n",hoge->lsec,hoge->lusec);
        return hoge->tps[i].id;
      }
    }
  }
  return -1;
}
