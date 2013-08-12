/*
 * Name: SimpleColorFilter.cpp
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *
 * Description: Specific implementation of the generic class 'ColorFiltfer'. The algorith implemented is a
 * simple HSV threshold filter.
 *
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved.
 */

#include "SimpleColorFilter.h"

/**
 * Class constructor that initializes the specific color filter with a set of HSV thresholds and
 * with the parameters of the images to filter.
 *
 * @param hmin Sets the lower threshold of the H filter's component.
 * @param hmax Sets the upper threshold of the H filter's component.
 * @param smin Sets the lower threshold of the S filter's component.
 * @param smax Sets the upper threshold of the S filter's component.
 * @param vmin Sets the lower threshold of the V filter's component.
 * @param vmax Sets the upper threshold of the V filter's component.
 * @param width Sets the width of the image to filter.
 * @param height Sets the height of the image to filter.
 * @param channels Sets the number of the channels of the image to filter.
 */
SimpleColorFilter::SimpleColorFilter() : ImageInput()
{
}


/**
 * Class destructor.
 **/
SimpleColorFilter::~SimpleColorFilter()
{
}

/**
 * Obtain the color in RGB of the pixel selected
 * @param image Source.
 **/
void
SimpleColorFilter::getColorRGB(const int cam, char *image, int col, int row, char &r, char &g, char &b, bool filter, int color)
{
	unsigned char rt=0, gt=0, bt=0, intensity;	
	int pixcolor;
	int pixel, posSrc;

	pixel = row * width + col;
	posSrc = pixel * 2;

	/*Get real pixel color*/
	if (pixel % 2 == 0)
		ColorModelConversions::yuv2rgb(image[posSrc], image[posSrc + 1], image[posSrc + 3], rt, gt, bt);
	else
		ColorModelConversions::yuv2rgb(image[posSrc], image[posSrc - 1], image[posSrc + 1], rt, gt, bt);

	r = (char) bt;
	g = (char) gt;
	b = (char) rt;
}

/**
 * Obtain the color in RGB of the pixel selected
 * @param image Source.
 **/
void
SimpleColorFilter::getColorHSV(const int cam, char *image, int col, int row, char &h, char &s, char &v)
{
	unsigned char rt=0, gt=0, bt=0;	
	float ht=0, st=0, vt=0;	
	int pixel, posSrc;

	pixel = row * width + col;
	posSrc = pixel * 2;

	/*Get real pixel color*/
	if (pixel % 2 == 0)
		ColorModelConversions::yuv2rgb(image[posSrc], image[posSrc + 1], image[posSrc + 3], rt, gt, bt);
	else
		ColorModelConversions::yuv2rgb(image[posSrc], image[posSrc - 1], image[posSrc + 1], rt, gt, bt);

	/*Convert to HSV*/
	ColorModelConversions::rgb2hsv(rt, gt, bt, ht, st, vt);

	ht = 255.0 * ht / 360.0;
	st = st * 100.0;
	vt = vt;

	h = (char) (unsigned char) ht;
	s = (char) (unsigned char) st;
	v = (char) (unsigned char) vt;
}
