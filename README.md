# ili9325_rpi
8Bit/16Bit Parallel TFT Library for WiringPi

This is library for RaspberryPi/OrangePi.   
You can use a TFT Shield for UNO R3.   
This library can show a chart to TFT Shield.   

Supported TFT controllers:   
8bit Parallel ILI9325   
8bit Parallel ILI9327   
8bit Parallel ILI9341   
8bit Parallel ILI9342   
8bit Parallel ILI9481   
8bit Parallel SPFD5408   
8bit Parallel S6D1121   
8bit Parallel R61505U   
16bit Parallel ILI9341   

Primarily, these TFT Shiled is the one for Arduino.   

----

# Wirering for 8Bit Parallel   

|TFT||Rpi/Opi|
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

You can change any pin.   
Pin define is "pin.conf".   

----

# Build   

for ILI9325   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9325   

for ILI9327   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9327   

for ILI9341   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9341   

for ILI9342   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9342   

for ILI9481   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9481   

for SPFD5408   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DSPFD5408   

for S6D1121   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DS6D1121   

for R61505U   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DR61505U   

---

![2 4tft-1](https://cloud.githubusercontent.com/assets/6020549/24932714/bff42314-1f4d-11e7-8c13-28a6c4652992.JPG)

![2 4tft-2](https://cloud.githubusercontent.com/assets/6020549/24932732/d45d7080-1f4d-11e7-95d8-a34c0cb901b3.JPG)

![ili9325-para1](https://cloud.githubusercontent.com/assets/6020549/24833926/38353b08-1d12-11e7-8d0d-5dae59c3499b.JPG)

![ili9325-para2](https://cloud.githubusercontent.com/assets/6020549/24995032/3ee3292a-2068-11e7-9179-bf5a74ba2a38.JPG)

![ili9325-para3](https://cloud.githubusercontent.com/assets/6020549/24833940/c58dc0ec-1d12-11e7-85b6-7f57a80b5e66.JPG)

![ili9325-para4](https://cloud.githubusercontent.com/assets/6020549/24833950/f48f0e6e-1d12-11e7-95a5-28ef894ea311.JPG)

![ili9325-para7](https://cloud.githubusercontent.com/assets/6020549/24932578/0c0319fa-1f4d-11e7-8da8-901a94e89e28.JPG)

![ili9325-para8](https://cloud.githubusercontent.com/assets/6020549/24995079/6043fb62-2068-11e7-974b-29e23126e38f.JPG)

----

# Wirering for 16Bit Parallel   
|TFT||Rpi/Opi|
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

----

# Build   

for ILI9341   
cc -o demo demo.c fontx.c ili93xx.c -lwiringPi -lm -lpthread -DILI9341 -DP16BIT   

----

![ili9341-16bit-1](https://user-images.githubusercontent.com/6020549/42122957-c62b7b66-7c84-11e8-8619-5c4ab0d624f5.JPG)

![ili9341-16bit-2](https://user-images.githubusercontent.com/6020549/42122960-c92f25e2-7c84-11e8-970b-11f0f08a88cd.JPG)
