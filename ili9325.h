#define RED             0xf800
#define GREEN           0x07e0
#define BLUE            0x001f
#define BLACK           0x0000
#define WHITE           0xffff
#define GRAY            0x8c51
#define YELLOW          0xFFE0
#define CYAN            0x07FF
#define PURPLE          0xF81F

void lcdWrite8(uint16_t data);
void lcdWriteData(uint16_t data);
void lcdWriteCommand(uint16_t command);
void lcdInit(void);
void lcdReset(void);
void lcdWriteRegister(uint16_t addr, uint16_t data);
void lcdRegister9325(void);
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void lcdDrawAngle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdFillScreen(uint16_t color);
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
unsigned int rgb565_conv(uint16_t r,uint16_t g,uint16_t b);
