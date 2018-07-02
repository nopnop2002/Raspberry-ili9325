#!/bin/bash
#set -x
DrawFile="./draw.txt"
if [ -f ${DrawFile} ]; then
  rm $DrawFile
fi
echo "FillScrren,0xffff" >> $DrawFile
echo "SetFontDirection,1" >> $DrawFile

#echo "SetFontUnderLine,0x0000" >> $DrawFile
echo "DrawUTF8String,G24,200,300,wlan0 IP Address,0x0000" >> $DrawFile
echo "UnsetFontUnderLine" >> $DrawFile
IpAddr=`ifconfig wlan0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  echo -n "DrawUTF8String,G24,180,300," >> $DrawFile
  echo -n ${IpAddr} >> $DrawFile
  echo ",0x0000" >> $DrawFile
else
  echo "DrawUTF8String,G24,180,300,<NONE>,0x0000" >> $DrawFile
fi

#echo "SetFontUnderLine,0x0000" >> $DrawFile
echo "DrawUTF8String,G24,140,300,eth0 IP Address,0x0000" >> $DrawFile
echo "UnsetFontUnderLine" >> $DrawFile
IpAddr=`ifconfig eth0 |grep "inet " |awk {'print $2'}`
if [ -n "$IpAddr" ]; then
  echo -n "DrawUTF8String,G24,120,300," >> $DrawFile
  echo -n ${IpAddr} >> $DrawFile
  echo ",0x0000" >> $DrawFile
else
  echo "DrawUTF8String,G24,120,300,<NONE>,0x0000" >> $DrawFile
fi

#echo "SetFontUnderLine,0x0000" >> $DrawFile
echo "DrawUTF8String,G24,80,300,kernel release,0x0000" >> $DrawFile
echo "UnsetFontUnderLine" >> $DrawFile
Release=`uname -r`
echo -n "DrawUTF8String,G24,60,300," >> $DrawFile
echo -n ${Release} >> $DrawFile
echo ",0x0000" >> $DrawFile

./draw $DrawFile > /dev/null
