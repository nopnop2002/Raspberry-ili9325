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

# download image from https://www.365calendar.net/
# List is here https://www.365calendar.net/calendar/list.html#dogBlock
# download from https://www.365calendar.net/index?action=user_calendar_detail&calendar_id=${id}&target=${YYMMDD}
# https://www.365calendar.net/index?action=user_calendar_detail&calendar_id=1184&target=20210101
# id = 1166 : 2021 welshcorgi
# id = 1167 : 2021 shetlandsheepdog
# id = 1168 : 2021 pekingese
# id = 1170 : 2021 miniatureschnauzer
# id = 1172 : 2021 dachshund
# id = 1173 : 2021 cavalierkingcharles
# id = 1175 : 2021 maltese
# id = 1178 : 2021 pomeranian
# id = 1185 : 2021 nihonken
# id = 1191 : 2021 papillon
imageFile="images/dailyImage.jpeg"
#python 365calendar.py ${imageFile} 1131

second=`date "+%S"`
second=$((second%10))
if [ $second = 0 ]; then
	python 365calendar.py ${imageFile} 1166 2021
elif [ $second = 1 ]; then
	python 365calendar.py ${imageFile} 1167 2021
elif [ $second = 2 ]; then
	python 365calendar.py ${imageFile} 1168 2021
elif [ $second = 3 ]; then
	python 365calendar.py ${imageFile} 1170 2021
elif [ $second = 4 ]; then
	python 365calendar.py ${imageFile} 1172 2021
elif [ $second = 5 ]; then
	python 365calendar.py ${imageFile} 1173 2021
elif [ $second = 6 ]; then
	python 365calendar.py ${imageFile} 1175 2021
elif [ $second = 7 ]; then
	python 365calendar.py ${imageFile} 1178 2021
elif [ $second = 8 ]; then
	python 365calendar.py ${imageFile} 1185 2021
elif [ $second = 9 ]; then
	python 365calendar.py ${imageFile} 1191 2021
fi

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
