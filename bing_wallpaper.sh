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

# download image from https://blog.atwork.at/post/Use-the-Daily-Bing-picture-in-Teams-calls
imageFile="images/dailyImage.jpeg"
python bing_wallpaper.py ${imageFile}

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

#Black(0x000000)
_echo "FillScrren,0x0000"
_echo "JPEG,${imageFile}"

./draw $DrawFile > /dev/null
