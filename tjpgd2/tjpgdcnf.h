/*----------------------------------------------*/
/* TJpgDec System Configurations                */
/*----------------------------------------------*/

/* Size of stream input buffer */
#define	JD_SZBUF		512

/* Output pixel format 0:RGB888 (3-uint8_t/pix), 1:RGB565 (1-uint16_t/pix), 2:Grayscale (1-uint8_t/pix) */
#define JD_FORMAT		0

/* Enable output descaling feature */
#define	JD_USE_SCALE	1

/* Use table conversion for saturation (might be a bit faster on 8-bit MCUs but increases 1 KB of code size) */
#define JD_TBLCLIP		0

