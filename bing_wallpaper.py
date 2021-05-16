#! /usr/bin/python3
# Author: Maximilian Muth <mail@maxi-muth.de>
# https://github.com/mammuth/bing-wallpaper
# Version: 1.0
# License: GPL-2.0
# Description: Downloads the Bing picture of the Day and sets it as wallpaper (Linux / Windows).

import datetime
from urllib.request import urlopen, urlretrieve
from xml.dom import minidom
import os
import sys

import shutil
import inspect

# How to Install Pillow
# sudo apt install libtiff5
# python -m pip install -U Pillow
from PIL import Image, ImageFilter


def rotateImage(filename):
	function_name = inspect.currentframe().f_code.co_name
	tmpfile = os.path.join(os.getcwd(), "__temp.jpeg")
	#print("{} tmpfile={}".format(function_name,tmpfile))
	shutil.copyfile(filename, tmpfile)
	im = Image.open(tmpfile)
	im_rotate = im.rotate(90, expand=True)
	im_rotate.save(filename)
	os.remove(tmpfile)


"""
def join_path(*args):
	# Takes an list of values or multiple values and returns an valid path.
	if isinstance(args[0], list):
		path_list = args[0]
	else:
		path_list = args
	val = [str(v).strip(' ') for v in path_list]
	return os.path.normpath('/'.join(val))
"""


def download_wallpaper(idx, pic_path):
	# Getting the XML File
	try:
		usock = urlopen(''.join(['http://www.bing.com/HPImageArchive.aspx?format=xml&idx=',
								 str(idx), '&n=1&mkt=ru-RU']))	# ru-RU, because they always have 1920x1200 resolution
	except Exception as e:
		print('Error while downloading #', idx, e)
		return
	try:
		xmldoc = minidom.parse(usock)
	# This is raised when there is trouble finding the image url.
	except Exception as e:
		print('Error while processing XML index #', idx, e)
		return
	# Parsing the XML File
	for element in xmldoc.getElementsByTagName('url'):
		url = 'http://www.bing.com' + element.firstChild.nodeValue
		#print("url={}".format(url))
		# Get Current Date as fileName for the downloaded Picture
		now = datetime.datetime.now()
		date = now - datetime.timedelta(days=int(idx))
		#print('Downloading: ', date.strftime('%d-%m-%Y'), 'index #', idx)

		# Download and Save the Picture
		# Get a higher resolution by replacing the file name
		urlretrieve(url.replace('_1366x768', '_1920x1200'), pic_path)
		print("Downloading from {}".format(url))
		rotateImage(pic_path)

if __name__ == "__main__":
	args = sys.argv
	argc = len(args)
	#print("args[1]={}".format(args[1]))
	#download_wallpaper(0, "./images/bing_wall_paper.jpeg")
	download_wallpaper(0, args[1])
