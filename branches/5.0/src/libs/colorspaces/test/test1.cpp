#include <colorspaces/colorspacesmm.h>
#include <iostream>
#include <tr1/memory>

using namespace colorspaces;

int main(int argc, char **argv){
  std::cerr << *ImageRGB888::FORMAT_RGB888 << "\n";
  std::cerr << *ImageYUY2::FORMAT_YUY2 << "\n";
  std::cerr << *ImageGRAY8::FORMAT_GRAY8 << "\n";

  char img1_data[] = { 0,128,255, 128,128,128 };

  Image img1 = Image(2,1,ImageRGB888::FORMAT_RGB888,img1_data);
  Image img2 = Image(2,1,ImageYUY2::FORMAT_YUY2);
  Image img3 = Image(ImageGRAY8(img1));//grayscale from img1

  std::cerr << img1 << "\n";
  std::cerr << img2 << "\n";
  std::cerr << img3 << "\n";

  std::cerr << "Converting img1 to img2\n";
  img1.convert(img2);
  std::cerr << img2 << "\n";

  std::cerr << "Clone im1 into img2\n";
  img2 = img1.clone();
  std::cerr << img2 << "\n";

  std::cerr << "Creating a 640x480 RGB black image\n";
  Image img4(cv::Mat::zeros(480,640,ImageRGB888::FORMAT_RGB888->cvType),ImageRGB888::FORMAT_RGB888);
  std::cerr << img4 << "\n";

  //RGB888 conversions
  std::cerr << "RGB888 to..........\n";
  std::cerr << "Converting to YUY2\n";
  ImageYUY2 img5 = img4;
  std::cerr << img5 << "\n";

  std::cerr << "Converting to GRAY8\n";
  ImageGRAY8 img6 = img4;
  std::cerr << img6 << "\n";

  std::cerr << "Converting to RGB888\n";
  ImageRGB888 img7 = img4;
  std::cerr << img7 << "\n";

  std::cerr << "Converting to HSV888\n";
  ImageHSV888 img71 = img4;
  std::cerr << img71 << "\n";

  std::cerr << "Converting to HSV888\n";
  ImageYCRCB img72 = img4;
  std::cerr << img72 << "\n";

  //YUY2 conversions
  std::cerr << "YUY2 to..........\n";
  std::cerr << "Converting to GRAY8\n";
  ImageGRAY8 img8 = img5;
  std::cerr << img8 << "\n";

  std::cerr << "Converting to RGB888\n";
  ImageRGB888 img9 = img5;
  std::cerr << img9 << "\n";

  std::cerr << "Converting to YUY2\n";
  ImageYUY2 img10 = img5;
  std::cerr << img10 << "\n";

  std::cerr << "Converting to YCRCB\n";
  ImageYCRCB img101 = img5;
  std::cerr << img101 << "\n";

  //GRAY8 conversions
  std::cerr << "GRAY8 to..........\n";
  std::cerr << "Converting to RGB888\n";
  ImageRGB888 img11 = img6;
  std::cerr << img11 << "\n";

  std::cerr << "Converting to YUY2\n";
  ImageYUY2 img12 = img6;
  std::cerr << img12 << "\n";

  std::cerr << "Converting to GRAY8\n";
  ImageGRAY8 img13 = img6;
  std::cerr << img13 << "\n";

  //HSV888 Conversions
  std::cerr << "HSV888 to..........\n";
  std::cerr << "Converting to RGB888\n";
  ImageRGB888 img121 = img71;
  std::cerr << img121 << "\n";

  //YCRCB Conversions
  std::cerr << "YCRCB to..........\n";
  std::cerr << "Converting to RGB888\n";
  ImageRGB888 img122 = img101;
  std::cerr << img122 << "\n";

  //read/write
  std::cerr << "Read/write RGB888..........\n";
  ImageRGB888 readImage(ImageRGB888::read("testimg.jpg"));
  readImage.write("testimgout.pnm");

  std::cerr << "Read/write GRAY8..........\n";
  ImageGRAY8 readImageG(ImageGRAY8::read("testimg.jpg"));
  readImageG.write("testimgoutG.pnm");
}
