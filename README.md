# 8Bit/16Bit Parallel TFT Library for WiringPi

This is library for RaspberryPi.   
You can use a TFT Shield for UNO R3.   
This library can show a chart to TFT Shield.   

![TFT-Shield](https://user-images.githubusercontent.com/6020549/104253960-10a71380-54b9-11eb-8789-a12c2c769ab4.JPG)

Primarily, these TFT Shiled is the one for Arduino.   
This may works with other boards like OrangePi/NanoPi.   

# Supported TFT controllers:   

## Generic 8bit Sheild
- ILI9320(240x320)      
- ILI9325(240x320)      
- ILI9341(240x320)      
- ILI9342(240x320)      
- ILI9481(320x480)      
- ILI9486(320x480)      
- ILI9488(320x480)      
- SPFD5408(240x320 Same as ILI9320)      
- R61505U(240x320 Same as ILI9320)      
- R61509V(240x400)      
- ST7781(240x320)     
- ST7783(240x320 Same as ST7781)      
- ST7793(240x400 Same as R61509)      
- ST7796(320x480 Same as ILI9486)      

## Generic 16bit Sheild
- ILI9341(240x320)   

## OPEN-SMART Products
- OPEN-SMART ILI9225 TFT-Shield(176x220)      
- OPEN-SMART ILI9327 TFT-Shield(240x400)      
- OPEN-SMART ILI9340 TFT-Shield(240x320)   
- OPEN-SMART S6D1121 16Pin-Parallel(240x320)   
- OPEN-SMART ST7775 16Pin-Parallel(176x220 Same as ILI9225)   
- OPEN-SMART ST7783 16Pin-Parallel(240x320)   
- OPEN-SMART R61509V 16Pin-Parallel(240x400)   
- OPEN-SMART ILI9488 16Pin-Parallel(320x400 Color inversion)

These are all 2.4 inch, 320x240 TFTs.
![TFT-SHIELD-2](https://user-images.githubusercontent.com/6020549/104244320-873a1600-54a5-11eb-93c0-9fad671fdfed.JPG)

3.95 inches is almost twice as large as 2.4 inches.
![TFT-SHIELD-3](https://user-images.githubusercontent.com/6020549/104244328-8903d980-54a5-11eb-8c9a-d26408e04f92.JPG)

These are OPEN-SMART 16Pin-Parallel Products.
![OPEN-SMART-16Pin](https://user-images.githubusercontent.com/6020549/110071604-ad9e7280-7dbf-11eb-8e09-a9ebfacfd795.JPG)

----

# Software requirement   
- WiringPi Library   
 This project uses the wiringPiSetup() function to initialize GPIOs.   
 If you use it on a board other than the RPI board, you may need to change the GPIO number.   
 As far as I know, there are these libraries.   
	- WiringPi for OrangePi   
	- WiringPi for OrangePi Zero   
	- WiringPi for OrangePi3   
	- WiringPi for BananaPi   
	- WiringPi for NanoPi   
	- WiringPi for Pine-64   

----

# Wirering for 8Bit Parallel   

|TFT||RPI|
|:-:|:-:|:-:|
|LCD_RST|--|Pin#7|
|LCD_CS|--|Pin#3|
|LCD_RS|--|Pin#5|
|LCD_WR|--|Pin#26|
|LCD_RD|--|Pin#28|
|LCD_D0|--|Pin#29|
|LCD_D1|--|Pin#31|
|LCD_D2|--|Pin#33|
|LCD_D3|--|Pin#35|
|LCD_D4|--|Pin#37|
|LCD_D5|--|Pin#32|
|LCD_D6|--|Pin#36|
|LCD_D7|--|Pin#38|
|5V|--|5V(*)|
|3.3V|--|3.3V(*)|
|GND|--|GND|

\*When a regulator(It's often AMS1117) is mounted on the back, it's operated 5V.   
\*When a regulator is NOT mounted on the back, it's operated 3.3V.   

__NOTE__   
My R61509V has a regulator on the back.   
Normally, a TFT with a regulator works at 5V, but my R61509V doesn't work unless I supply both 5V and 3.3V.   

__NOTE__   
You can change any pin.   
Pin define is "pin.conf".   
If you use a 26-pin header, you can use "pin.conf.26pin" instead of "pin.conf".   

----

# Build   

- for ILI9225   
`make ILI9225`   

- for ILI9320   
`make ILI9320`   

- for ILI9325   
`make LI9325`   

- for ILI9327   
`make ILI9327`   

- for ILI9341   
`make ILI9341`   

- for ILI9342   
`make ILI9342`   

- for ILI9481   
`make ILI9481`   

- for ILI9486   
`make ILI9486`   

- for ILI9488   
`make ILI9488`   

- for ILI9488 Inversion Color    
`make ILI9488-INVERT`   

- for SPFD5408   
`make SPFD5408`   

- for R61505U   
`make R61505U`  

- for R61509V   
`make R61509V`  

- for S6D1121   
`make S6D1121`  

- for ST7775   
`make ST7775`   

- for ST7781   
`make ST7781`   

- for ST7783   
`make ST7783`   

- for ST7793   
`make ST7793`   

- for ST7796   
`make ST7796`   

----

# How to find the TFT driver name   
![](https://img.shields.io/badge/_IMPORTANT-important)  
The information provided by sellers on Ebay or AliExpress is largely incorrect.   
You waste time if you don't choose the right driver.   
There are many [variations](http://domoticx.com/arduino-shield-2-4-tft-lcd-touch/) of the 2.4 inch shield.   
You can use [this](https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/LCD_ID_readreg) to find out your driver.   
This is for Arduino UNO.   
Do not use this on the RPI as the GPIO on the RPI is not 5V tolerant.   
__Never believe the seller's information.__   

----

# Graphic support   
![ILI9341-10](https://user-images.githubusercontent.com/6020549/59558184-52df4900-9027-11e9-8e80-7c85fdb4213c.JPG)
![ILI9341-12](https://user-images.githubusercontent.com/6020549/59558186-5377df80-9027-11e9-88a8-4c8c4df9a830.JPG)
![ILI9341-13](https://user-images.githubusercontent.com/6020549/59558187-5377df80-9027-11e9-8f72-a7ebbbe57c0b.JPG)
![ILI9341-14](https://user-images.githubusercontent.com/6020549/59558188-5377df80-9027-11e9-99f7-de3c8b1878dd.JPG)
![ILI9341-18](https://user-images.githubusercontent.com/6020549/59558192-54107600-9027-11e9-946c-39443ae83053.JPG)

# Fonts support   
![ILI9341-11](https://user-images.githubusercontent.com/6020549/59558185-52df4900-9027-11e9-84d6-cf6e69051ce8.JPG)
![ILI9341-15](https://user-images.githubusercontent.com/6020549/59558189-54107600-9027-11e9-89eb-39e8f2cbeab2.JPG)
![ILI9341-16](https://user-images.githubusercontent.com/6020549/59558190-54107600-9027-11e9-9d23-d71c3997a3d0.JPG)
![ILI9341-17](https://user-images.githubusercontent.com/6020549/59558191-54107600-9027-11e9-888c-a1485758e943.JPG)
![ILI9341-19](https://user-images.githubusercontent.com/6020549/59558183-52df4900-9027-11e9-9a79-8969d47f902e.JPG)

# Image support   
JPEG file      
![ILI9341-JPEG](https://user-images.githubusercontent.com/6020549/118343557-c2884680-b564-11eb-9009-cd29ff6c0bdb.JPG)
PNG file   
![ILI9341-PNG](https://user-images.githubusercontent.com/6020549/118343563-cddb7200-b564-11eb-983f-8d09fc5c52cd.JPG)

----

# Start Auto Demo   
./demo   

# Start Interpreter mode   
You can change draw1.txt & draw2.txt & me.sh   

`./draw draw1.txt`   
`./draw draw2.txt`   
`bash ./me.sh`   

![me_sh](https://user-images.githubusercontent.com/6020549/118217481-8398b900-b4b0-11eb-91b7-1d7e14ef6ab8.JPG)

## Take animal picture from www.365calendar.net and display.   
To use this, you need python3 and Pillow.   
And reduce the security level of OpenSSL encryption.   
```
$ sudo apt install libtiff5 libopenjp2-7
$ python -m pip install -U Pillow
$ sudo cp /usr/lib/ssl/openssl.cnf /usr/lib/ssl/openssl.cnf.old
$ sudo vi /usr/lib/ssl/openssl.cnf
#CipherString = DEFAULT@SECLEVEL=2
CipherString = DEFAULT@SECLEVEL=1
$ bash ./365calendar.sh
```

![365calendar](https://user-images.githubusercontent.com/6020549/130392539-d335b6ee-c335-4305-8830-887833b2e61d.JPG)

## Take wallpaper from bing and display   
To use this, you need python3 and imagemagick.   
```
$ sudo apt install imagemagick
$ bash ./bing_wallpaper.sh
```

![bing_wallpaper](https://user-images.githubusercontent.com/6020549/130392570-7da882dc-a357-4370-8c24-5b238456066a.JPG)

----

# Wirering for 16Bit Parallel   
|TFT||RPI|
|:-:|:-:|:-:|
|LCD_RST|--|Pin#7|
|LCD_CS|--|Pin#3|
|LCD_RS|--|Pin#5|
|LCD_WR|--|Pin#26|
|LCD_RD|--|Pin#28|
|LCD_D0|--|Pin#29|
|LCD_D1|--|Pin#31|
|LCD_D2|--|Pin#33|
|LCD_D3|--|Pin#35|
|LCD_D4|--|Pin#37|
|LCD_D5|--|Pin#32|
|LCD_D6|--|Pin#36|
|LCD_D7|--|Pin#38|
|LCD_D8|--|Pin#13|
|LCD_D9|--|Pin#15|
|LCD_D10|--|Pin#16|
|LCD_D11|--|Pin#18|
|LCD_D12|--|Pin#8|
|LCD_D13|--|Pin#10|
|LCD_D14|--|Pin#40|
|LCD_D15|--|Pin#27|
|5V|--|5V(*)|
|3.3V|--|3.3V(*)|
|GND|--|GND|

\*When a regulator(It's often AMS1117) is mounted on the back, it's operated 5V.   
\*When a regulator is NOT mounted on the back, it's operated 3.3V.   

__NOTE__   
You can change any pin.   
Pin define is "pin.conf".   

----

# Build   
`cc -o demo demo.c fontx.c tft_lib.c driver/*.c -lwiringPi -lm -lpthread -DILI9341 -DP16BIT`   
`cc -o draw draw.c fontx.c tft_lib.c driver/*.c -lwiringPi -lm -lpthread -DILI9341 -DP16BIT`   

----

# Start Auto Demo   
`./demo`   

![ili9341-16bit-1](https://user-images.githubusercontent.com/6020549/42122957-c62b7b66-7c84-11e8-8619-5c4ab0d624f5.JPG)

![ili9341-16bit-2](https://user-images.githubusercontent.com/6020549/42122960-c92f25e2-7c84-11e8-970b-11f0f08a88cd.JPG)

----

# SPI Touch Sensor   
![ili9341-16bit-2](https://user-images.githubusercontent.com/6020549/42122960-c92f25e2-7c84-11e8-970b-11f0f08a88cd.JPG)
This module has SPI Touch Sensor.   
So you can read touch position.   
Use raspi-config to enable SPI.

Wirering for SPI-Touch sensor   

|TFT||RPI|
|:-:|:-:|:-:|
|PEN|--|Pin#22(\*\*)|
|T_CS|--|Pin#24(\*\*)|
|MOSI|--|Pin#19(\*\*\*)|
|MISO|--|Pin#21(\*\*\*)|
|CLK|--|Pin#23(\*\*\*)|


\*\*It's defined in the source code.   
#define SPI_CHANNEL 0 // /dev/spidev0.0  
//#define SPI_CHANNEL 1 // /dev/spidev0.1  
#define GPIO_PEN 6   

\*\*\*Can't change   

```
cc -o xpt xpt.c xpt2046.c -lwiringPi -lm -lpthread
sudo ./xpt
```   

![touch-1](https://user-images.githubusercontent.com/6020549/42130602-8710a000-7d24-11e8-9b8e-b1819301bc0f.jpg)


And you can operate Pi using touch sensor.  
```
cc -o touch touch.c fontx.c tft_lib.c driver/ili9341.c xpt2046.c -lwiringPi -lm -lpthread -DILI9341 -DP16BIT
sudo ./touch
```   


![touch-2](https://user-images.githubusercontent.com/6020549/42130606-8b7c0ed6-7d24-11e8-97e8-41f7cf18d4bc.JPG)

----

# 4-wire resistive touch screen   
OPEN SMART products have 4-wire resistive touch screen, but Pi cannot use this because Pi cannot input analog.

---

# JPEG Decoder   
This project uses [this](http://elm-chan.org/fsw/tjpgd/00index.html) JPEG library.   
This can reduce the image to 1/2 1/4 1/8.   

---

# PNG Decoder   
I ported the pngle library from [here](https://github.com/kikuchan/pngle).   
This can reduce the image to any size.   

---

