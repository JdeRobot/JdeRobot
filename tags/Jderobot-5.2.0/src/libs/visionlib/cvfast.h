#ifndef CVFAST_H
#define CVFAST_H

#include <opencv/cvaux.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>

CVAPI(void)  cvCornerFast(cv::Mat &src, int threshold, int N,	int nonmax_suppression, int* ret_number_of_corners,	CvPoint** ret_corners, int ** scores);

#endif
