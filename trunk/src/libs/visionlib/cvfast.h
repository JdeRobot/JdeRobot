#ifndef CVFAST_H
#define CVFAST_H

#include <opencv/cvaux.h>
#include <opencv/cxtypes.h>
#include <stdlib.h>

CVAPI(void)  cvCornerFast( const CvArr* image, int threshold, int N,	int nonmax_suppression, int* ret_number_of_corners,	CvPoint** ret_corners, int ** scores);

#endif
