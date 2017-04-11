# ili9325_rpi
ILI9325 TFT Shield Demo Code


This program is example code for RaspberryPi/OrangePi.   
This program can show a chart to ILI9325 TFT Shield.   
Primarily, this TFT Shiled is the one for Arduino.   

----

Wirering(Without 74x595)   

|TFT||Rpi/Opi|
|:-:|:-:|:-:|
|LCD_RST|--|Pin#7|
|LCD_CS|--|Pin#3|
|LCD_RS|--|Pin#5|
|LCD_WR|--|Pin#24|
|LCD_RD|--|Pin#26|
|LCD_D0|--|Pin#29|
|LCD_D1|--|Pin#31|
|LCD_D2|--|Pin#33|
|LCD_D3|--|Pin#35|
|LCD_D4|--|Pin#37|
|LCD_D5|--|Pin#32|
|LCD_D6|--|Pin#36|
|LCD_D7|--|Pin#38|
|5V|--|5V|
|GND|--|GND|

build   
cc -o demo demo.c fontx.c ili9325.c -lwiringPi -lm   

----

Wirering(With 74x595)   

|TFT||Rpi/Opi||74x595|
|:-:|:-:|:-:|:-:|:-:|
|LCD_RST|--|Pin#7|||
|LCD_CS|--|Pin#3|||
|LCD_RS|--|Pin#5|||
|LCD_WR|--|Pin#24|||
|LCD_RD|--|Pin#26|||
|||Pin#16|--|DATA|
|||Pin#18|--|CLOCK|
|||Pin#22|--|LATCH|
|LCD_D0|--||--|QA|
|LCD_D1|--||--|QB
|LCD_D2|--||--|QC|
|LCD_D3|--||--|QD|
|LCD_D4|--||--|QE|
|LCD_D5|--||--|QF|
|LCD_D6|--||--|QG|
|LCD_D7|--||--|QH|
|5V|--|5V|||
|GND|--|GND|||

build   
cc -o demo-sr595 demo.c fontx.c ili9325.c -lwiringPi -lm -DSR595   

---

![2 4tft-1](https://cloud.githubusercontent.com/assets/6020549/24932714/bff42314-1f4d-11e7-8c13-28a6c4652992.JPG)

![2 4tft-2](https://cloud.githubusercontent.com/assets/6020549/24932732/d45d7080-1f4d-11e7-95d8-a34c0cb901b3.JPG)

![ili9325-para1](https://cloud.githubusercontent.com/assets/6020549/24833926/38353b08-1d12-11e7-8d0d-5dae59c3499b.JPG)

![ili9325-para2](https://cloud.githubusercontent.com/assets/6020549/24932530/df3273a8-1f4c-11e7-96b2-bb1281adbae1.JPG)

![ili9325-para3](https://cloud.githubusercontent.com/assets/6020549/24833940/c58dc0ec-1d12-11e7-85b6-7f57a80b5e66.JPG)

![ili9325-para4](https://cloud.githubusercontent.com/assets/6020549/24833950/f48f0e6e-1d12-11e7-95a5-28ef894ea311.JPG)

![ili9325-para7](https://cloud.githubusercontent.com/assets/6020549/24932578/0c0319fa-1f4d-11e7-8da8-901a94e89e28.JPG)
