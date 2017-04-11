#define FontxGlyphBufSize	(32*32/8)

typedef struct {
  const char *path;
  char  fxname[9];
  bool  opened;
  bool  valid;
  bool  is_ank;
  uint8_t w;
  uint8_t h;
  uint16_t fsz;
  uint8_t bc;
  FILE *file;
} FontxFile;

bool GetFontx (FontxFile *fxs, uint32_t sjis , uint8_t *pGlyph,
               uint8_t *pw, uint8_t *ph);
void Font2Bitmap(uint8_t *fonts, uint8_t *line, uint8_t w, uint8_t h,
		 uint8_t inverse);
uint16_t UTF2SJIS(uint8_t *utf8);
void UnderlineBitmap(uint8_t *line, uint8_t w, uint8_t h);
void ReversBitmap(uint8_t *line, uint8_t w, uint8_t h);
void ShowFont(uint8_t *fonts, uint8_t pw, uint8_t ph);
void ShowBitmap(uint8_t *bitmap, uint8_t pw, uint8_t ph);
void DumpFX(FontxFile *fxs);
uint16_t UTF2SJIS(uint8_t *utf8);
int String2SJIS(unsigned char *str_in, uint8_t stlen, uint16_t *sjis,
                uint8_t ssize);
uint8_t RotateByte(uint8_t ch);
