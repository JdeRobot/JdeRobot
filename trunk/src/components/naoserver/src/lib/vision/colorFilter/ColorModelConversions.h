/*
 * Name: ColorModelConversions.h
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *  
 * Description: A class that defines static conversions between color models for single pixels.
 * 
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved. 
 */

#ifndef _ColorModelConversions_h_
#define _ColorModelConversions_h_

/**
 * A class that defines static conversions between color models for single pixels.
 */
class ColorModelConversions
{
public:

	/** Converts an YUV pixel into an RGB pixel.
	 *  @param Y The Y channel of the source pixel.
	 *  @param U The U channel of the source pixel.
	 *  @param V The V channel of the source pixel.
	 *  @param R The R channel of the target pixel.
	 *  @param G The G channel of the target pixel.
	 *  @param B The B channel of the target pixel.
	 */
	static void yuv2rgb(unsigned char Y, unsigned char U, unsigned char V, 
			unsigned char &R, unsigned char &G, unsigned char &B)
	{
		int r = Y + ((1436 * (V - 128)) >> 10),
				g = Y - ((354 * (U - 128) + 732 * (V - 128)) >> 10),
				b = Y + ((1814 * (U - 128)) >> 10);

		if(r < 0) r = 0; else if(r > 255) r = 255;
		if(g < 0) g = 0; else if(g > 255) g = 255;
		if(b < 0) b = 0; else if(b > 255) b = 255;

		//Crop RGB
		R = (unsigned char) r;
		G = (unsigned char) g;
		B = (unsigned char) b;
	};

	/** Converts an RGB pixel into an YUV pixel.
	 *  @param r The R channel of the source pixel.
	 *  @param g The G channel of the source pixel.
	 *  @param b The B channel of the source pixel.
	 *  @param Y The Y channel of the target pixel.
	 *  @param U The U channel of the target pixel.
	 *  @param V The V channel of the target pixel.
	 */
	static void rgb2yuv(unsigned char r, unsigned char g, unsigned char b, unsigned char& Y, unsigned char& U, unsigned char& V)
	{
		Y = r * .299000 + g * .587000 + b * .114000;
		U = r * -.168736 + g * -.331264 + b * .500000 + 128;
		V = r * .500000 + g * -.418688 + b * -.081312 + 128;
	}

	/** Converts an RGB pixel into an HSV pixel.
	 *  @param r The R channel of the source pixel.
	 *  @param g The G channel of the source pixel.
	 *  @param b The B channel of the source pixel.
	 *  @param H The H channel of the target pixel.
	 *  @param S The S channel of the target pixel.
	 *  @param V The V channel of the target pixel.
	 */
	static void rgb2hsv(unsigned char r, unsigned char g, unsigned char b, float& H, float& S, float& V)
	{
		double min, max;

		// Calculamos el minimo
		if ((r <= g) && (r <= b))
			min = r;
		else if ((g <= r) && (g <= b))
			min = g;
		else
			min = b;

		// Calculamos el máximo
		if ((r >= g) && (r >= b))
			max = r;
		else if ((g >= r) && (g >= b))
			max = g;
		else
			max = b;

		//printf("min=%.1f - max=%.1f - r=%.1f - g=%.1f - b=%.1f\n",min,max,r,g,b);
		// Calculamos valor de H
		if (max==min) {
			H=.0; // En estos casos, H no tiene sentido
		} else if (max==r && g>=b) {
			H=60*((g-b)/(max-min));
		} else if (max==r && g<b) {
			H=60*((g-b)/(max-min))+360;
		} else if (max==g) {
			H=60*((b-r)/(max-min))+120;
		} else if (max==b) {
			H=60*((r-g)/(max-min))+240;
		}

		// Calculamos el valor de S
		if (max==0)
			S=0.0;
		else
			S= 1-(min/max);

		// Calculamos el valor si V
		V=max;
	};

};

#endif //__ColorModelConversions_h_
