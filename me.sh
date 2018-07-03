#!/bin/bash
#set -x

function _echo () {
  echo $1 >> $DrawFile
}

DrawFile="./draw.txt"
if [ -f ${DrawFile} ]; then
  rm $DrawFile
fi
_echo "FillScrren,0xffff"
_echo "SetFontDirection,1"

_echo "DrawRoundRect,205,40,230,300,10,0x0000"
_echo "DrawUTF8String,G24,205,280,System Information,0x0000"

#Blue(0000ff)
color=`./rgb2color 0x0000ff`
_echo "DrawFillRect,175,305,185,315,${color}"
#_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,170,300,wlan0 IP Address,${color}"
#_echo "UnsetFontUnderLine"
IpAddr=`ifconfig wlan0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  _echo "DrawUTF8String,G24,150,300,${IpAddr},${color}"
else
  _echo "DrawUTF8String,G24,150,300,<NONE>,${color}"
fi

#Magenta(ff0090)
color=`./rgb2color 0xff0090`
_echo "DrawFillRect,135,305,145,315,${color}"
#_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,130,300,eth0 IP Address,${color}"
#_echo "UnsetFontUnderLine"
IpAddr=`ifconfig eth0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  _echo "DrawUTF8String,G24,110,300,${IpAddr},${color}"
else
  _echo "DrawUTF8String,G24,110,300,<NONE>,${color}"
fi

#Orange(ff5c00)
color=`./rgb2color 0xff5c00`
_echo "DrawFillRect,95,305,105,315,${color}"
#_echo "SetFontUnderLine,${color}"
_echo "DrawUTF8String,G24,90,300,kernel release,${color}"
#_echo "UnsetFontUnderLine"
Release=`uname -r`
_echo "DrawUTF8String,G24,70,300,${Release},${color}"

#Lime Green(00ff00)
color=`./rgb2color 0x00ff00`
_echo "DrawFillRect,55,305,65,315,${color}"
_echo "DrawUTF8String,G24,50,300,CPU Temp.,${color}"
temp=`vcgencmd measure_temp | cut -d= -f2`
_echo "DrawUTF8String,G24,30,300,${temp},${color}"

./draw $DrawFile > /dev/null
