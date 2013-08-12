/*
 * Name: ImageInput.h
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *  
 * Description: Class that stores the parameters of an image and implements some utilities.
 * 
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved. 
 */

#ifndef _IMAGEINPUT_H_
#define _IMAGEINPUT_H_

#include "Singleton.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>

using namespace std;

class ImageInput
{
public:
	ImageInput();
	~ImageInput();

	void getImageParams(unsigned short& width, unsigned short& height, unsigned char& channels);
	void setImageParams(unsigned short width, unsigned short height, unsigned char channels);
	void saveImage(const char* data, const char *file);

	/*Camera selected*/
	static const int UPPER_CAMERA		=	0;
	static const int LOWER_CAMERA		=	1;

	/*Image parameters*/
	static const int IMG_WIDTH			=	320;
	static const int IMG_HEIGHT			=	240;
	static const int IMG_CHANNELS		=	3;
	static const int IMG_CHANNELS_YUV	=	2;
	static const int BYTES_PER_CHANNEL	=	8;
	static const int IMG_STEP			=	8;
	static const int IMG_STEPS_V		=	IMG_WIDTH/IMG_STEP;
	static const int IMG_STEPS_H		=	IMG_HEIGHT/IMG_STEP;
	static const int IMG_MARGIN_STEP	=	IMG_STEP/2;
	static const int IMG_MINISTEP	=	2;

	/*Color filters*/
	static const int CUNKNOWN			=	0;
	static const int CORANGE			=	1;
	static const int CGREEN				=	2;
	static const int CBLUE				=	3;
	static const int CYELLOW			=	4;
	static const int CWHITE				=	5;
	static const int CCYAN				=	6;
	static const int CMAGENTA			=	7;

protected:
	unsigned short width, height;
	unsigned char channels;
};

#endif //_IMAGEINPUT_H_
