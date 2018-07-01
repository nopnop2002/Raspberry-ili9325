#include <sys/time.h>

typedef struct {
  uint32_t x1;
  uint32_t x2;
  uint32_t y1;
  uint32_t y2;
  uint32_t id;
} TouchPoint;

#define MAXTP 20

typedef struct {
  uint16_t tpc;
  uint16_t tpx;
  time_t lsec;
  suseconds_t lusec;
  TouchPoint tps[MAXTP];
} TouchInfo;

#define XRATE 133
#define YRATE  94
#define DTMAX 800000 // 800m sec

int xptGetit(int spiCh, int cmd);
void xptGetxy(int spiCh, int *xp, int *yp);
int xptScrren(int xp);
int yptScrren(int yp);
void xptInit(TouchInfo *hoge);
void xptDump(TouchInfo *hoge);
void xptSetPoint(TouchInfo *hoge,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id);
int xptGetPoint(int spiCh, TouchInfo *hoge);
