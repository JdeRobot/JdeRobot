/*
 * Name: ImageHandler.h
 * @Author: Eduardo Perdices (eperdices@gsyc.es)
 *  
 * Description: Class that handles the image.
 * 
 * Copyright (C) 2009-2010 Universidad Rey Juan Carlos
 * All Rights Reserved. 
 */

#ifndef _IMAGEHANDLER_H_
#define _IMAGEHANDLER_H_

#include "Singleton.h"
#include "ImageInput.h"
#include "vision/colorFilter/SimpleColorFilter.h"

using namespace std;

class ImageHandler: public Singleton<ImageHandler>
{
public:
	ImageHandler();
	~ImageHandler();

	void setImage(int camera, char * data, SimpleColorFilter * colorFilter);
	void getImageRGB(char * img, bool filter, int color);
	void getImageRGBInverted(char * img, bool filter, int color);
	void getImageHSV(char * img);

private:
	static const int MAX_WRONG_PIXELS = 2;
	static const int MAX_NOT_BALL_PIXELS = 4;

	int camera;
	SimpleColorFilter * colorFilter;

	char image[ImageInput::IMG_WIDTH][ImageInput::IMG_HEIGHT][ImageInput::IMG_CHANNELS_YUV];
	int colors[ImageInput::IMG_WIDTH][ImageInput::IMG_HEIGHT];
	bool filtered[ImageInput::IMG_WIDTH][ImageInput::IMG_HEIGHT];	
};

#endif //_IMAGEHANDLER_H_
