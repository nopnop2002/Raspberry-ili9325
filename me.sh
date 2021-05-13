#!/bin/bash
#set -x

#for debug
function __echo () {
  echo $1
  echo $1 >> $DrawFile
}

function _echo () {
  echo $1 >> $DrawFile
}

DrawFile="./draw.txt"
if [ -f ${DrawFile} ]; then
  rm $DrawFile
fi

#Inquiry tft information
screen_width=`./draw width`
screen_height=`./draw height`
driver_name=`./draw driver`
echo -n "Your TFT is "
echo -n ${driver_name}
echo -n " "
echo -n ${screen_width}
echo -n "x"
echo ${screen_height}

#Calculate Y position
xorg=10
ycenter=$((screen_height/2))
yorg=$((ycenter-130))
#echo $ycenter
#echo $yorg
if [ $yorg -lt 0 ]; then
yorg=0
fi

#Black(0x000000)
_echo "FillScrren,0x0000"
_echo "SetFontDirection,3"

#White(0xffffff)
color=`./rgb2color 0xffffff`
y1=$yorg
y2=$((y1+235))
x1=$xorg
x2=$((x1+30))
_echo "DrawRoundRect,${x1},${y1},${x2},${y2},10,${color}"
xs=$((xorg+30))
ys=$((yorg+10))
_echo "DrawUTF8String,G24,${xs},${ys},System Information,${color}"

#Cyan(0x00ffff)
color=`./rgb2color 0x00ffff`
x1=$((xorg+44))
x2=$((x1+10))
y1=$((yorg+10))
y2=$((y1+10))
xs=$((xs+30))
ys=$((yorg+30))
_echo "DrawFillRect,${x1},${y1},${x2},${y2},${color}"
#_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,${xs},${ys},wlan0 IP Address,${color}"
#_echo "UnsetFontUnderLine"
IpAddr=`ifconfig wlan0 | grep "inet " | awk {'print $2'}`
xs=$((xs+20))
_echo "SetFontUnderLine,${color}"
if [ -n "$IpAddr" ]; then
  _echo "DrawUTF8String,G24,${xs},${ys},${IpAddr},${color}"
else
  _echo "DrawUTF8String,G24,${xs},${ys},<NONE>,${color}"
fi
_echo "UnsetFontUnderLine"

#Magenta(0xff0090)
color=`./rgb2color 0xff0090`
x1=$((x1+50))
x2=$((x1+10))
xs=$((xs+30))
_echo "DrawFillRect,${x1},${y1},${x2},${y2},${color}"
_echo "DrawUTF8String,G24,${xs},${ys},eth0 IP Address,${color}"
IpAddr=`ifconfig eth0 | grep "inet " | awk {'print $2'}`
xs=$((xs+20))
_echo "SetFontUnderLine,${color}"
if [ -n "$IpAddr" ]; then
  _echo "DrawUTF8String,G24,${xs},${ys},${IpAddr},${color}"
else
  _echo "DrawUTF8String,G24,${xs},${ys},<NONE>,${color}"
fi
_echo "UnsetFontUnderLine"

#Orange(0xff5c00)
color=`./rgb2color 0xff5c00`
x1=$((x1+50))
x2=$((x1+10))
xs=$((xs+30))
_echo "DrawFillRect,${x1},${y1},${x2},${y2},${color}"
_echo "DrawUTF8String,G24,${xs},${ys},kernel release,${color}"
Release=`uname -r`
xs=$((xs+20))
_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,${xs},${ys},${Release},${color}"
_echo "UnsetFontUnderLine"

#Lime Green(0x00ff00)
color=`./rgb2color 0x00ff00`
x1=$((x1+50))
x2=$((x1+10))
xs=$((xs+30))
_echo "DrawFillRect,${x1},${y1},${x2},${y2},${color}"
_echo "DrawUTF8String,G24,${xs},${ys},CPU Temp.,${color}"
temp=`vcgencmd measure_temp | cut -d= -f2`
xs=$((xs+20))
_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,${xs},${ys},${temp},${color}"
_echo "UnsetFontUnderLine"

if [ $screen_width -gt 240 ]; then
#Blue(0x0000ff)
color=`./rgb2color 0x0000ff`
x1=$((x1+50))
x2=$((x1+10))
xs=$((xs+30))
_echo "DrawFillRect,${x1},${y1},${x2},${y2},${color}"
_echo "DrawUTF8String,G24,${xs},${ys},DISK Usage,${color}"
temp=`df | grep root | awk {'print $5'}`
xs=$((xs+20))
_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,${xs},${ys},${temp},${color}"
_echo "UnsetFontUnderLine"
fi

./draw $DrawFile > /dev/null
