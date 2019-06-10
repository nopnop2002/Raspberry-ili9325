#!/bin/bash
#set -x

function _echo () {
  echo $1 >> $DrawFile
}

DrawFile="./draw.txt"
if [ -f ${DrawFile} ]; then
  rm $DrawFile
fi

#Black(0x000000)
_echo "FillScrren,0x0000"
_echo "SetFontDirection,1"

#White(0xffffff)
color=`./rgb2color 0xffffff`
_echo "DrawRoundRect,205,40,230,300,10,${color}"
_echo "DrawUTF8String,G24,205,280,System Information,${color}"

#Blue(0x0000ff)
color=`./rgb2color 0x0000ff`
_echo "DrawFillRect,175,305,185,315,${color}"
#_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,170,300,wlan0 IP Address,${color}"
#_echo "UnsetFontUnderLine"
IpAddr=`ifconfig wlan0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  _echo "SetFontUnderLine,${color}"
  _echo "DrawUTF8String,G24,150,300,${IpAddr},${color}"
  _echo "UnsetFontUnderLine"
else
  _echo "DrawUTF8String,G24,150,300,<NONE>,${color}"
fi

#Magenta(0xff0090)
color=`./rgb2color 0xff0090`
_echo "DrawFillRect,130,305,140,315,${color}"
_echo "DrawUTF8String,G24,125,300,eth0 IP Address,${color}"
IpAddr=`ifconfig eth0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  _echo "SetFontUnderLine,${color}"
  _echo "DrawUTF8String,G24,105,300,${IpAddr},${color}"
  _echo "UnsetFontUnderLine"
else
  _echo "DrawUTF8String,G24,105,300,<NONE>,${color}"
fi

#Orange(0xff5c00)
color=`./rgb2color 0xff5c00`
_echo "DrawFillRect,85,305,95,315,${color}"
_echo "DrawUTF8String,G24,80,300,kernel release,${color}"
Release=`uname -r`
_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,60,300,${Release},${color}"
_echo "UnsetFontUnderLine"

#Lime Green(0x00ff00)
color=`./rgb2color 0x00ff00`
_echo "DrawFillRect,40,305,50,315,${color}"
_echo "DrawUTF8String,G24,35,300,CPU Temp.,${color}"
temp=`vcgencmd measure_temp | cut -d= -f2`
_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,15,300,${temp},${color}"
_echo "UnsetFontUnderLine"

./draw $DrawFile > /dev/null
