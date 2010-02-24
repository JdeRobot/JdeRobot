#include <colorspaces/colorspacesmm.h>
#include <iostream>
#include <tr1/memory>

using namespace colorspaces;

int main(int argc, char **argv){
  std::cerr << *ImageRGB888::FORMAT_RGB888 << "\n";
  std::cerr << *ImageYUY2::FORMAT_YUY2 << "\n";
  std::cerr << *ImageGRAY8::FORMAT_GRAY8 << "\n";

  char img1_data[] = { 0,128,255, 128,128,128 };

  ImagePtr img1(ImageRGB888::FORMAT_RGB888->createInstance(2,1,img1_data));
  ImagePtr img2(new ImageYUY2(2,1));
  ImagePtr img3(new ImageGRAY8(*img1));//grayscale from RGB

  std::cerr << *img1 << "\n";
  std::cerr << *img2 << "\n";
  std::cerr << *img3 << "\n";

  std::cerr << "Converting img1 to img2\n";
  img1->convert(*img2);
  std::cerr << *img2 << "\n";

  std::cerr << "Creating a 640x480 image\n";
  ImageRGB888Ptr img4(new ImageRGB888(640,480));
  std::cerr << *img4 << "\n";

  std::cerr << "Converting to YUY2\n";
  ImagePtr img5(new ImageYUY2(*img4));
  std::cerr << *img5 << "\n";
}
