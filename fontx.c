#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <iconv.h>
#include "fontx.h"

#define FontxDebug 0 // for Debug

// フォントファイルパスを構造体に保存
void Fontx_addFont(FontxFile *fx, const char *path)
{
  memset(fx,0,sizeof(FontxFile));
  fx->path = path;
  fx->opened = false;
}

// フォント構造体を初期化
void Fontx_init(FontxFile *fxs,const char *f0,const char *f1)
{
  Fontx_addFont(&fxs[0],f0);
  Fontx_addFont(&fxs[1],f1);
}

// フォントファイルをOPEN
bool Fontx_openFontxFile(FontxFile *fx)
{
  FILE *f;

  if(!fx->opened){
    fx->opened = true;
    f = fopen(fx->path,"r");
    if(!f){
      fx->valid = false;
      printf("FsFontx:%s not found.\n",fx->path);
    } else {
      fx->file = f;
      char buf[18];

      fread(buf, sizeof buf, 1, fx->file);
//      for(i=0;i<sizeof(buf);i++) {
//        printf("buf[%d]=%x\n",i,buf[i]);
//      }
      memcpy(fx->fxname,&buf[6],8);
      fx->w = buf[14];
      fx->h = buf[15];
      fx->is_ank = (buf[16] == 0);
      fx->bc = buf[17];
      fx->fsz = (fx->w + 7)/8 * fx->h;
      if(fx->fsz > FontxGlyphBufSize){
	printf("too big font size.\n");
	fx->valid = false;
      } else {
	fx->valid = true;
      }
    }
  }
  return fx->valid;
}

// フォントファイルをCLOSE
void Fontx_closeFontxFile(FontxFile *fx)
{
  if(fx->opened){
    fclose(fx->file);
    fx->opened = false;
  }
}


/*
 フォントファイルからフォントパターンを取り出す

 フォントの並び(16X16ドット)
    00000000    01111111
    12345678    90123456
 01 pGlyph[000] pGlyph[001]
 02 pGlyph[002] pGlyph[003]
 03 pGlyph[004] pGlyph[005]
 04 pGlyph[006] pGlyph[007]
 05 pGlyph[008] pGlyph[009]
 06 pGlyph[010] pGlyph[011]
 07 pGlyph[012] pGlyph[013]
 08 pGlyph[014] pGlyph[015]
 09 pGlyph[016] pGlyph[017]
 10 pGlyph[018] pGlyph[019]
 11 pGlyph[020] pGlyph[021]
 12 pGlyph[022] pGlyph[023]
 13 pGlyph[024] pGlyph[025]
 14 pGlyph[026] pGlyph[027]
 15 pGlyph[028] pGlyph[029]
 16 pGlyph[030] pGlyph[031]

 フォントの並び(24X24ドット)
    00000000    01111111    11122222
    12345678    90123456    78901234
 01 pGlyph[000] pGlyph[001] pGlyph[002]
 02 pGlyph[003] pGlyph[004] pGlyph[005]
 03 pGlyph[006] pGlyph[007] pGlyph[008]
 04 pGlyph[009] pGlyph[010] pGlyph[011]
 05 pGlyph[012] pGlyph[013] pGlyph[014]
 06 pGlyph[015] pGlyph[016] pGlyph[017]
 07 pGlyph[018] pGlyph[019] pGlyph[020]
 08 pGlyph[021] pGlyph[022] pGlyph[023]
 09 pGlyph[024] pGlyph[025] pGlyph[026]
 10 pGlyph[027] pGlyph[028] pGlyph[029]
 11 pGlyph[030] pGlyph[031] pGlyph[032]
 12 pGlyph[033] pGlyph[034] pGlyph[035]
 13 pGlyph[036] pGlyph[037] pGlyph[038]
 14 pGlyph[039] pGlyph[040] pGlyph[041]
 15 pGlyph[042] pGlyph[043] pGlyph[044]
 16 pGlyph[045] pGlyph[046] pGlyph[047]
 17 pGlyph[048] pGlyph[049] pGlyph[050]
 18 pGlyph[051] pGlyph[052] pGlyph[053]
 19 pGlyph[054] pGlyph[055] pGlyph[056]
 20 pGlyph[057] pGlyph[058] pGlyph[059]
 21 pGlyph[060] pGlyph[061] pGlyph[062]
 22 pGlyph[063] pGlyph[064] pGlyph[065]
 23 pGlyph[066] pGlyph[067] pGlyph[068]
 24 pGlyph[069] pGlyph[070] pGlyph[071]

 フォントの並び(32X32ドット)
    00000000    01111111    11122222    22222333
    12345678    90123456    78901234    56789012
 01 pGlyph[000] pGlyph[001] pGlyph[002] pGlyph[003]
 02 pGlyph[004] pGlyph[005] pGlyph[006] pGlyph[007]
 03 pGlyph[008] pGlyph[009] pGlyph[010] pGlyph[011]
 04 pGlyph[012] pGlyph[013] pGlyph[014] pGlyph[015]
 05 pGlyph[016] pGlyph[017] pGlyph[018] pGlyph[019]
 06 pGlyph[020] pGlyph[021] pGlyph[022] pGlyph[023]
 07 pGlyph[024] pGlyph[025] pGlyph[026] pGlyph[027]
 08 pGlyph[028] pGlyph[029] pGlyph[030] pGlyph[031]
 09 pGlyph[032] pGlyph[033] pGlyph[034] pGlyph[035]
 10 pGlyph[036] pGlyph[037] pGlyph[038] pGlyph[039]
 11 pGlyph[040] pGlyph[041] pGlyph[042] pGlyph[043]
 12 pGlyph[044] pGlyph[045] pGlyph[046] pGlyph[047]
 13 pGlyph[048] pGlyph[049] pGlyph[050] pGlyph[051]
 14 pGlyph[052] pGlyph[053] pGlyph[054] pGlyph[055]
 15 pGlyph[056] pGlyph[057] pGlyph[058] pGlyph[059]
 16 pGlyph[060] pGlyph[061] pGlyph[062] pGlyph[063]
 17 pGlyph[064] pGlyph[065] pGlyph[066] pGlyph[067]
 18 pGlyph[068] pGlyph[069] pGlyph[070] pGlyph[071]
 19 pGlyph[072] pGlyph[073] pGlyph[074] pGlyph[075]
 20 pGlyph[076] pGlyph[077] pGlyph[078] pGlyph[079]
 21 pGlyph[080] pGlyph[081] pGlyph[082] pGlyph[083]
 22 pGlyph[084] pGlyph[085] pGlyph[086] pGlyph[087]
 23 pGlyph[088] pGlyph[089] pGlyph[090] pGlyph[091]
 24 pGlyph[092] pGlyph[093] pGlyph[094] pGlyph[095]
 25 pGlyph[096] pGlyph[097] pGlyph[098] pGlyph[099]
 26 pGlyph[100] pGlyph[101] pGlyph[102] pGlyph[103]
 27 pGlyph[104] pGlyph[105] pGlyph[106] pGlyph[107]
 28 pGlyph[108] pGlyph[109] pGlyph[110] pGlyph[111]
 29 pGlyph[112] pGlyph[113] pGlyph[114] pGlyph[115]
 30 pGlyph[116] pGlyph[117] pGlyph[118] pGlyph[119]
 31 pGlyph[120] pGlyph[121] pGlyph[122] pGlyph[123]
 32 pGlyph[124] pGlyph[125] pGlyph[127] pGlyph[128]

*/

bool GetFontx(FontxFile *fxs, uint32_t sjis , uint8_t *pGlyph,
		uint8_t *pw, uint8_t *ph)
{
  
  int i;
//  FontxFile fx;
  long offset;

if(FontxDebug)printf("[GetFontx]sjis=%x %d\n",sjis,sjis);
  for(i=0; i<2; i++){
    if(!Fontx_openFontxFile(&fxs[i])) continue;
//    printf("openFontxFile[%d]\n",i);
    
    if(sjis < 0x100){
      if(fxs[i].is_ank){
if(FontxDebug)printf("[GetFontx]fxs.is_ank fxs.fsz=%d\n",fxs[i].fsz);
	offset = 17 + sjis * fxs[i].fsz;
if(FontxDebug)printf("[GetFontx]offset=%ld\n",offset);
	if(fseek(fxs[i].file, offset, SEEK_SET)) {
  	  printf("Fontx::fseek(18) failed.\n");
	  return false;
        }
	if(fread(pGlyph, 1, fxs[i].fsz, fxs[i].file) != fxs[i].fsz){
	  printf("Fontx::fread failed.\n");
	  return false;
        }
	if(pw) *pw = fxs[i].w;
	if(ph) *ph = fxs[i].h;
	return true;
      }
    }
    else {
      if(!fxs[i].is_ank){
        if(fseek(fxs[i].file, 18, SEEK_SET)) {
  	  printf("Fontx::fseek(18) failed.\n");
	  return false;
        }
        uint16_t buf[2], nc = 0, bc = fxs[i].bc;
    
        while(bc--){ 
	  if(fread((char *)buf, 1, 4, fxs[i].file) != 4){
	    printf("Fontx::fread failed.\n");
	    return false;
	  }
if(FontxDebug)printf("[GetFontx]buf=%x %x\n",buf[0],buf[1]);
	  if(sjis >= buf[0] && sjis <= buf[1]) {
	    nc += sjis - buf[0];
	    uint32_t pos = 18 + fxs[i].bc * 4 + nc * fxs[i].fsz;
	    if(fseek(fxs[i].file, pos, SEEK_SET)) {
	      printf("FsFontx::seek(%u) failed.\n",pos);
	      return false;
	    }
	    if(fread(pGlyph, 1, fxs[i].fsz, fxs[i].file) != fxs[i].fsz){
	      printf("Fontx::fread failed.\n");
	      return false;
            }
	    if(pw) *pw = fxs[i].w;
	    if(ph) *ph = fxs[i].h;
	    return true;
	  }
	  nc += buf[1] - buf[0] + 1;
        }
      }
    }
  }
  return false;
}


/*
 フォントパターンをビットマップイメージに変換する

 fonts(16X16ドット)
    00000000    01111111
    12345678    90123456
 01 pGlyph[000] pGlyph[001]
 02 pGlyph[002] pGlyph[003]
 03 pGlyph[004] pGlyph[005]
 04 pGlyph[006] pGlyph[007]
 05 pGlyph[008] pGlyph[009]
 06 pGlyph[010] pGlyph[011]
 07 pGlyph[012] pGlyph[013]
 08 pGlyph[014] pGlyph[015]
 09 pGlyph[016] pGlyph[017]
 10 pGlyph[018] pGlyph[019]
 11 pGlyph[020] pGlyph[021]
 12 pGlyph[022] pGlyph[023]
 13 pGlyph[024] pGlyph[025]
 14 pGlyph[026] pGlyph[027]
 15 pGlyph[028] pGlyph[029]
 16 pGlyph[030] pGlyph[031]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[014] line[015] line[016-031]
 |                                                         Not Use
 07 line[000] line[001] line[002] .... line[014] line[015] line[016-031]

 08 line[032] line[033] line[034] .... line[046] line[047] line[048-063]
 |                                                         Not Use
 16 line[032] line[033] line[034] .... line[046] line[047] line[048-063]



 fonts(24X24ドット)
    00000000    01111111    11122222
    12345678    90123456    78901234
 01 pGlyph[000] pGlyph[001] pGlyph[002]
 02 pGlyph[003] pGlyph[004] pGlyph[005]
 03 pGlyph[006] pGlyph[007] pGlyph[008]
 04 pGlyph[009] pGlyph[010] pGlyph[011]
 05 pGlyph[012] pGlyph[013] pGlyph[014]
 06 pGlyph[015] pGlyph[016] pGlyph[017]
 07 pGlyph[018] pGlyph[019] pGlyph[020]
 08 pGlyph[021] pGlyph[022] pGlyph[023]
 09 pGlyph[024] pGlyph[025] pGlyph[026]
 10 pGlyph[027] pGlyph[028] pGlyph[029]
 11 pGlyph[030] pGlyph[031] pGlyph[032]
 12 pGlyph[033] pGlyph[034] pGlyph[035]
 13 pGlyph[036] pGlyph[037] pGlyph[038]
 14 pGlyph[039] pGlyph[040] pGlyph[041]
 15 pGlyph[042] pGlyph[043] pGlyph[044]
 16 pGlyph[045] pGlyph[046] pGlyph[047]
 17 pGlyph[048] pGlyph[049] pGlyph[050]
 18 pGlyph[051] pGlyph[052] pGlyph[053]
 19 pGlyph[054] pGlyph[055] pGlyph[056]
 20 pGlyph[057] pGlyph[058] pGlyph[059]
 21 pGlyph[060] pGlyph[061] pGlyph[062]
 22 pGlyph[063] pGlyph[064] pGlyph[065]
 23 pGlyph[066] pGlyph[067] pGlyph[068]
 24 pGlyph[069] pGlyph[070] pGlyph[071]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[022] line[023] line[024-031]
 |                                                         Not Use
 08 line[000] line[001] line[002] .... line[022] line[023] line[024-031]

 09 line[032] line[033] line[034] .... line[054] line[055] line[056-063]
 |                                                         Not Use
 16 line[032] line[033] line[034] .... line[054] line[055] line[056-063]

 17 line[064] line[065] line[066] .... line[086] line[087] line[088-095]
 |                                                         Not Use
 24 line[064] line[065] line[066] .... line[086] line[087] line[088-095]


 fonts(32X32ドット)
    00000000    01111111    11122222    22222333
    12345678    90123456    78901234    56789012
 01 pGlyph[000] pGlyph[001] pGlyph[002] pGlyph[003]
 02 pGlyph[004] pGlyph[005] pGlyph[006] pGlyph[007]
 03 pGlyph[008] pGlyph[009] pGlyph[010] pGlyph[011]
 04 pGlyph[012] pGlyph[013] pGlyph[014] pGlyph[015]
 05 pGlyph[016] pGlyph[017] pGlyph[018] pGlyph[019]
 06 pGlyph[020] pGlyph[021] pGlyph[022] pGlyph[023]
 07 pGlyph[024] pGlyph[025] pGlyph[026] pGlyph[027]
 08 pGlyph[028] pGlyph[029] pGlyph[030] pGlyph[031]
 09 pGlyph[032] pGlyph[033] pGlyph[034] pGlyph[035]
 10 pGlyph[036] pGlyph[037] pGlyph[038] pGlyph[039]
 11 pGlyph[040] pGlyph[041] pGlyph[042] pGlyph[043]
 12 pGlyph[044] pGlyph[045] pGlyph[046] pGlyph[047]
 13 pGlyph[048] pGlyph[049] pGlyph[050] pGlyph[051]
 14 pGlyph[052] pGlyph[053] pGlyph[054] pGlyph[055]
 15 pGlyph[056] pGlyph[057] pGlyph[058] pGlyph[059]
 16 pGlyph[060] pGlyph[061] pGlyph[062] pGlyph[063]
 17 pGlyph[064] pGlyph[065] pGlyph[066] pGlyph[067]
 18 pGlyph[068] pGlyph[069] pGlyph[070] pGlyph[071]
 19 pGlyph[072] pGlyph[073] pGlyph[074] pGlyph[075]
 20 pGlyph[076] pGlyph[077] pGlyph[078] pGlyph[079]
 21 pGlyph[080] pGlyph[081] pGlyph[082] pGlyph[083]
 22 pGlyph[084] pGlyph[085] pGlyph[086] pGlyph[087]
 23 pGlyph[088] pGlyph[089] pGlyph[090] pGlyph[091]
 24 pGlyph[092] pGlyph[093] pGlyph[094] pGlyph[095]
 25 pGlyph[096] pGlyph[097] pGlyph[098] pGlyph[099]
 26 pGlyph[100] pGlyph[101] pGlyph[102] pGlyph[103]
 27 pGlyph[104] pGlyph[105] pGlyph[106] pGlyph[107]
 28 pGlyph[108] pGlyph[109] pGlyph[110] pGlyph[111]
 29 pGlyph[112] pGlyph[113] pGlyph[114] pGlyph[115]
 30 pGlyph[116] pGlyph[117] pGlyph[118] pGlyph[119]
 31 pGlyph[120] pGlyph[121] pGlyph[122] pGlyph[123]
 32 pGlyph[124] pGlyph[125] pGlyph[127] pGlyph[128]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[030] line[031]
 |
 08 line[000] line[001] line[002] .... line[030] line[031]

 09 line[032] line[033] line[034] .... line[062] line[063]
 |
 16 line[032] line[033] line[034] .... line[062] line[063]

 17 line[064] line[065] line[066] .... line[094] line[095]
 |
 24 line[064] line[065] line[066] .... line[094] line[095]

 25 line[096] line[097] line[098] .... line[126] line[127]
 |
 32 line[096] line[097] line[098] .... line[126] line[127]

*/
void Font2Bitmap(uint8_t *fonts, uint8_t *line, uint8_t w, uint8_t h, uint8_t inverse) {
  int x,y;
  for(y=0; y<(h/8); y++){
    for(x=0; x<w; x++){
      line[y*32+x] = 0;
    }
  }

  int mask = 7;
  int fontp;
  fontp = 0;
  for(y=0; y<h; y++){
    for(x=0; x<w; x++){
      uint8_t d = fonts[fontp+x/8];
      uint8_t linep = (y/8)*32+x;
      if (d & (0x80 >> (x % 8))) line[linep] = line[linep] + (1 << mask);
    }
    mask--;
    if (mask < 0) mask = 7;
    fontp += (w + 7)/8;
  }

  if (inverse) {
    for(y=0; y<(h/8); y++){
      for(x=0; x<w; x++){
        line[y*32+x] = RotateByte(line[y*32+x]);
      }
    }
  }
}

// アンダーラインを追加
void UnderlineBitmap(uint8_t *line, uint8_t w, uint8_t h) {
  int x,y;
  uint8_t wk;
  for(y=0; y<(h/8); y++){
    for(x=0; x<w; x++){
      wk = line[y*32+x];
      if ( (y+1) == (h/8)) line[y*32+x] = wk + 0x80;
    }
  }
}

// ビットマップを反転
void ReversBitmap(uint8_t *line, uint8_t w, uint8_t h) {
  int x,y;
  uint8_t wk;
  for(y=0; y<(h/8); y++){
    for(x=0; x<w; x++){
      wk = line[y*32+x];
      line[y*32+x] = ~wk;
    }
  }
}

// フォントパターンの表示
void ShowFont(uint8_t *fonts, uint8_t pw, uint8_t ph) {
  int x,y,fpos;
  fpos=0;
  for (y=0;y<ph;y++) {
    printf("%02d",y);
    for (x=0;x<pw;x++) {
      if (fonts[fpos+x/8] & (0x80 >> (x % 8))) {
       printf("*");
      } else {
       printf(".");
      }
    }
    printf("\n");
    fpos=fpos+(pw+7)/8;
  }
}

// Bitmapの表示
void ShowBitmap(uint8_t *bitmap, uint8_t pw, uint8_t ph) {
  int x,y,fpos;
  for (y=0;y<(ph+7)/8;y++) {
    for (x=0;x<pw;x++) {
printf("%02x ",bitmap[x+y*32]);
    }
    printf("\n");
  }

  fpos=0;
  for (y=0;y<ph;y++) {
    printf("%02d",y);
    for (x=0;x<pw;x++) {
//printf("b=%x m=%x\n",bitmap[x+(y/8)*32],0x80 >> fpos);
      if (bitmap[x+(y/8)*32] & (0x80 >> fpos)) {
       printf("*");
      } else {
       printf(".");
      }
    }
    printf("\n");
    fpos++;
    if (fpos > 7) fpos = 0;
  }
}


// フォント構造体の表示
void DumpFX(FontxFile *fxs)
{
  int i;
  for(i=0;i<2;i++) {
    printf("fxs[%d]->path=%s\n",i,fxs[i].path);
    printf("fxs[%d]->opened=%d\n",i,fxs[i].opened);
    printf("fxs[%d]->fxname=%s\n",i,fxs[i].fxname);
    printf("fxs[%d]->valid=%d\n",i,fxs[i].valid);
    printf("fxs[%d]->is_ank=%d\n",i,fxs[i].is_ank);
    printf("fxs[%d]->w=%d\n",i,fxs[i].w);
    printf("fxs[%d]->h=%d\n",i,fxs[i].h);
    printf("fxs[%d]->fsz=%d\n",i,fxs[i].fsz);
    printf("fxs[%d]->bc=%d\n",i,fxs[i].bc);
  }
}

// UTF code(3Byte) を SJIS Code(2 Byte) に変換
uint16_t UTF2SJIS(uint8_t *utf8) {
  unsigned char strJIS[3] = {0};
  unsigned char *pi1  = utf8;
  unsigned char **pi2 = &pi1;
  unsigned char *po1  = strJIS;
  unsigned char **po2 = &po1;
  size_t        ilen  = 3;
  size_t        olen  = 2;
  iconv_t cd;
  uint16_t sjis;

  if((cd = iconv_open("sjis","utf-8")) == (iconv_t)-1){
if(FontxDebug)printf("iconv open fail \n");
    return 0;
  }else {
if(FontxDebug)printf("iconv open ok \n");
  }

  iconv(cd,(char**)pi2,&ilen,(char**)po2,&olen);
  iconv_close(cd);

if(FontxDebug)printf("[UTF2SJIS]strJIS=%x-%x\n",strJIS[0],strJIS[1]);
  if (strJIS[0] & 0x80) {
    sjis = strJIS[0] << 8;
    sjis = sjis + strJIS[1];
  } else {
    sjis = strJIS[0];
  }
if(FontxDebug)printf("[UTF2SJIS]sjis=%x\n",sjis);
  return sjis;
}


// UTFを含む文字列をSJISに変換
int String2SJIS(unsigned char *str_in, uint8_t stlen, uint16_t *sjis,
                uint8_t ssize) {
  int i;
  uint8_t sp;
  uint8_t c1 = 0;
  uint8_t c2 = 0;
  uint8_t utf8[3];
  uint16_t sjis2;
  int spos = 0;

  for(i=0;i<stlen;i++) {
    sp = str_in[i];
if(FontxDebug)printf("[String2SJIS]sp[%d]=%x\n",i,sp);
    if ((sp & 0xf0) == 0xe0) { // 上位4ビットが1110なら、3バイト文字の1バイト目
      c1 = sp;
    } else if ((sp & 0xc0) == 0x80) { // 上位2ビットが10なら、他バイト文字の2バイト目以降
      if (c2 == 0) {
        c2 = sp;
      } else {
        if (c1 == 0xef && c2 == 0xbd) {
if(FontxDebug)printf("[String2SJIS]hankaku kana %x-%x-%x\n",c1,c2,sp);
          sjis2 = sp;
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        } else if (c1 == 0xef && c2 == 0xbe) {
if(FontxDebug)printf("[String2SJIS]hankaku kana %x-%x-%x\n",c1,c2,sp);
          sjis2 = 0xc0 + (sp - 0x80);
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        } else {
if(FontxDebug)printf("[String2SJIS]UTF8 %x-%x-%x\n",c1,c2,sp);
          utf8[0] = c1;
          utf8[1] = c2;
          utf8[2] = sp;
          sjis2 = UTF2SJIS(utf8);
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        }
        c1 = c2 = 0;
      }
    } else if ((sp & 0x80) == 0) { // 1バイト文字の場合
if(FontxDebug)printf("[String2SJIS]ANK %x\n",sp);
        if (spos < ssize) sjis[spos++] = sp;
    }
  }
  return spos;
}

// 8ビットデータを反転
uint8_t RotateByte(uint8_t ch1) {
  uint8_t ch2;
  int j;
  for (j=0;j<8;j++) {
    ch2 = (ch2 << 1) + (ch1 & 0x01);
    ch1 = ch1 >> 1;
  }
  return ch2;
}
