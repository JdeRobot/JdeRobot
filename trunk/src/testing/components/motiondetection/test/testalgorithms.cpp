#include "motiondetection.h"
#include <colorspaces/colorspacesmm.h>
#include <iostream>

using namespace motiondetection;
using namespace colorspaces;

int main(int argc, char** argv){
  Image img1(cv::Mat::zeros(240,320,ImageRGB8::FORMAT_RGB8->cvType), ImageRGB8::FORMAT_RGB8);
  Image img2(img1.clone());//2 copies of same image

  std::cout << "Instantiating optical flow algorithm\n";
  MotionDetectionAlgorithmPtr a(new OpticalFlowAlgorithm(img1));
  a->calcMotion(img2);

  std::cout << "Instantiating pixel difference algorithm\n";
  a.reset(new PixelDifferenceAlgorithm(img1));
  a->calcMotion(img2);
}
