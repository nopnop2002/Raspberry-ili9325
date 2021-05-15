DRIVER_LIB = driver.a
DRIVER_DIR = driver
DRIVER_SRCS = $(wildcard $(DRIVER_DIR)/*.c)
DRIVER_OBJS = $(subst .c,.o,$(DRIVER_SRCS))

JPEG_LIB = jpeg.a
JPEG_DIR = tjpgd2
JPEG_SRCS = $(wildcard $(JPEG_DIR)/*.c)
JPEG_OBJS = $(subst .c,.o,$(JPEG_SRCS))

PNG_LIB = png.a
PNG_DIR = pngle
PNG_SRCS = $(wildcard $(PNG_DIR)/*.c)
PNG_OBJS = $(subst .c,.o,$(PNG_SRCS))

ARFLAG = crv

SRCS = demo.c draw.c rgb2color.c fontx.c tft_lib.c $(DRIVER_LIB) $(JPEG_LIB) $(PNG_LIB)
LIBS = $(DRIVER_LIB) $(JPEG_LIB) $(PNG_LIB)

$(DRIVER_LIB) : $(DRIVER_OBJS) $(DRIVER_SRCS)
	$(AR) $(ARFLAG) $(DRIVER_LIB) $(DRIVER_OBJS)
	@rm -f $(DRIVER_OBJS)
	@echo "$(DRIVER_LIB) make success"

$(JPEG_LIB) : $(JPEG_OBJS) $(JPEG_SRCS)
	$(AR) $(ARFLAG) $(JPEG_LIB) $(JPEG_OBJS)
	@rm -f $(JPEG_OBJS)
	@echo "$(JPEG_LIB) make success"

$(PNG_LIB) : $(PNG_OBJS) $(PNG_SRCS)
	$(AR) $(ARFLAG) $(PNG_LIB) $(PNG_OBJS)
	@rm -f $(PNG_OBJS)
	@echo "$(PNG_LIB) make success"


ILI9225 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9320 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9325 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9327 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9341 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c
	cc -o jpegtest jpegtest.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o pngtest pngtest.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@

ILI9342 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9481 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9486 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9488 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ILI9488-INVERT : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -DILI9488 -DINVERT
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -DILI9488 -DINVERT
	cc -o rgb2color rgb2color.c

SPFD5408 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

R61505U : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

R61509V : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

S6D1121 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ST7775 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ST7781 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ST7783 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ST7793 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c

ST7796 : $(SRCS)
	cc -o demo demo.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o draw draw.c fontx.c tft_lib.c $(LIBS) -lwiringPi -lm -lpthread -D$@
	cc -o rgb2color rgb2color.c
