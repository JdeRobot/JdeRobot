/*
 * Name: ImageHandler.h
 * @Author: Eduardo Perdices (eperdices@gsyc.es)
 *  
 * Description: Class that handles the image.
 * 
 * Copyright (C) 2009-2010 Universidad Rey Juan Carlos
 * All Rights Reserved. 
 */

#include "ImageHandler.h"

ImageHandler::ImageHandler()
{
}


ImageHandler::~ImageHandler()
{
}

void
ImageHandler::setImage(int camera, char * data, SimpleColorFilter * colorFilter)
{
	this->camera = camera;
	this->colorFilter = colorFilter;
	memcpy(this->image, data, ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV);
	memset(this->filtered, 0, sizeof(bool)*ImageInput::IMG_WIDTH*ImageInput::IMG_HEIGHT);
}

void
ImageHandler::getImageRGB(char * img, bool filter, int color)
{
	char r, g, b;
	int row, col;
	int pixel;

	for(col = 0;col < ImageInput::IMG_WIDTH; col++) {
		for(row = 0;row < ImageInput::IMG_HEIGHT; row++) {		
			/*Get the RGB value*/
			this->colorFilter->getColorRGB(this->camera, (char *)this->image, col, row, r, g, b, filter, color);
			
			pixel = (row * ImageInput::IMG_WIDTH + col)*3;

			img[pixel] = r;
			img[pixel+1] = g;
			img[pixel+2] = b;
		}		
	}
}

void
ImageHandler::getImageRGBInverted(char * img, bool filter, int color)
{
	char r, g, b;
	int row, col;
	int pixel;

	for(col = 0;col < ImageInput::IMG_WIDTH; col++) {
		for(row = 0;row < ImageInput::IMG_HEIGHT; row++) {		
			/*Get the RGB value*/
			this->colorFilter->getColorRGB(this->camera, (char *)this->image, col, row, r, g, b, filter, color);
			
			pixel = (row * ImageInput::IMG_WIDTH + col)*3;

			img[pixel] = b;
			img[pixel+1] = g;
			img[pixel+2] = r;
		}		
	}
}

void
ImageHandler::getImageHSV(char * img)
{
	char h, s, v;
	int row, col;
	int pixel;

	for(col = 0;col < ImageInput::IMG_WIDTH; col++) {
		for(row = 0;row < ImageInput::IMG_HEIGHT; row++) {		
			/*Get the HSV value*/
			this->colorFilter->getColorHSV(this->camera, (char *)this->image, col, row, h, s, v);
			
			pixel = (row * ImageInput::IMG_WIDTH + col)*3;

			img[pixel] = h;
			img[pixel+1] = s;
			img[pixel+2] = v;
		}		
	}
}
