/*
 * Name: ImageInput.cpp
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *  
 * Description: Class that stores the parameters of an image and implements some utilities.
 * 
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved. 
 */

#include "ImageInput.h"

/**
 * Class constructor that sets the parameters of the image. 
 *
 * @param width Sets the width of the image.
 * @param height Sets the height of the image.
 * @param channels Sets the number of the channels of the image.
 */
ImageInput::ImageInput()
{
	this->width = ImageInput::IMG_WIDTH;
	this->height = ImageInput::IMG_HEIGHT;
	this->channels = ImageInput::IMG_CHANNELS;
}


/**
 * Class destructor.
 **/
ImageInput::~ImageInput()
{
}


/**
 * getImageParams. Method that allows to read the current image parameteres.
 * @param width Pointer to the width of the next image to filter.
 * @param height Pointer to the height of the next image to filter.
 * @param channels Pointer to the number of channels of the next image to filter.
 **/
void 
ImageInput::getImageParams(unsigned short& width, unsigned short& height, unsigned char& channels)
{
	width = this->width;
	height = this->height;
	channels = this->channels;
}


/**
 * setImageParams. Method that updates the image parameteres.
 * @param width The new width of the next image to filter.
 * @param height The new height of the next image to filter.
 * @param channels The new number of channels of the next image to filter.
 **/
void
ImageInput::setImageParams(unsigned short width, unsigned short height, unsigned char channels)
{
	this->width = width;
	this->height = height;
	this->channels = channels;
}


/**
 * saveImage. Method that saves to disk an image.
 * @param data Sets the image.
 * @param file Sets the name of the file.
 **/
void
ImageInput::saveImage(const char* data, const char *file)
{
	IplImage* img = cvCreateImage( cvSize( width, height ), 8, channels );
	img->imageData = ( char* ) data;
	cvSaveImage( file, img );
	cvReleaseImage(&img);
}
