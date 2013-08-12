#ifndef _VISIONDEFINITIONS_H_
#define _VISIONDEFINITIONS_H_

typedef struct {
	float hmin; 
	float hmax; 
	float smin; 
	float smax; 
	float vmin; 
	float vmax; 
}HSVClrParams;

typedef struct {
	float x;
	float y;
	float h;
} Point2D;

typedef struct {
	float a;
	float b;
	float c;
} Line2D;


static const int ORANGE_MASK 	= 128;
static const int BLUE_MASK 		= 64;
static const int YELLOW_MASK	= 32;
static const int GREEN_MASK 	= 16;
static const int WHITE_MASK 	= 8;
static const int MAGENTA_MASK	= 4;
static const int CYAN_MASK		= 2;

#endif //_VISIONDEFINITIONS_H_
