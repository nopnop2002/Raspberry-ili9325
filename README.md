# ili9325_rpi
ILI9325 TFT Shield Demo Code for Raspberry


This program is example code for RaspberryPi/OrangePi.   
This program can show a chart to ILI9325 TFT Shield.   
Primarily, this TFT Shiled is the one for Arduino.   

----

Wirering   
LCD_RST Pin#7   
LCD_CS  Pin#3   
LCD_RS  Pin#5   
LCD_WR  Pin#24   
LCD_RD  Pin#26   
LCD_D0  Pin#29   
LCD_D1  Pin#31   
LCD_D2  Pin#33   
LCD_D3  Pin#35   
LCD_D4  Pin#37   
LCD_D5  Pin#32   
LCD_D6  Pin#36   
LCD_D7  Pin#38   

----

build   
cc -o demo demo.c ili9325,c -lwiringPi   

---


![2 4tft-2](https://cloud.githubusercontent.com/assets/6020549/24829861/13e50e52-1cb5-11e7-9e5e-cfd573922199.JPG)


![2 4tft-3](https://cloud.githubusercontent.com/assets/6020549/24829868/4290e0a0-1cb5-11e7-9b52-c97fc201dabb.JPG)
