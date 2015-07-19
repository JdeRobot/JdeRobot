/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Rubén González Barriada <ruben.gbarriada@gmail.com>
 *            Alejandro Hernández Cordero <ahcorde@gmail.com> 
 *            Óscar Javier García Baudet <oscar.robotica@linaresdigital.com>
 *
 */

#include "viewer.h"  

#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>
#include <opencv2/opencv.hpp>

#include <visionlib/colorspaces/colorspacesmm.h>

namespace opencvdemo {

const std::string kGladePath = std::string("./opencvdemo.glade");

int opflow_first = 1;

/**
 * Constructor.
 * Load glade GUI from xml, initialize references to widgets (buttons, combo
 * boxes, images) and event handlers.
 */
Viewer::Viewer()
    : gtk_main_(0, 0) {
  /* Load glade GUI from XML file */
  std::cout << "Loading glade" << std::endl;
  ref_xml_ = Gnome::Glade::Xml::create(kGladePath);

  /* Load GUI components on private */
  ref_xml_->get_widget("imageI", gtk_image_in_);
  ref_xml_->get_widget("imageO", gtk_image_out_);
  ref_xml_->get_widget("mainwindow", main_window_);
  ref_xml_->get_widget("scale_sobel", scale_sobel_);
  ref_xml_->get_widget("scale_canny", scale_canny_);
  ref_xml_->get_widget("hough_combobox", combobox_hough_);
  ref_xml_->get_widget("conv_combobox", combobox_conv_);
  ref_xml_->get_widget("label_long", label_hough_long_);
  ref_xml_->get_widget("label_gap", label_hough_gap_);
  ref_xml_->get_widget("hough_threshold", scale_hough_threshold_);
  ref_xml_->get_widget("hough_long", scale_hough_long_);
  ref_xml_->get_widget("hough_gap", scale_hough_gap_);
  ref_xml_->get_widget("Hmax", scale_h_max_);
  ref_xml_->get_widget("Hmin", scale_h_min_);
  ref_xml_->get_widget("Smax", scale_s_max_);
  ref_xml_->get_widget("Smin", scale_s_min_);
  ref_xml_->get_widget("Vmax", scale_v_max_);
  ref_xml_->get_widget("Vmin", scale_v_min_);
  ref_xml_->get_widget("button_harris", button_harris_);
  ref_xml_->get_widget("button_hough", button_hough_);
  ref_xml_->get_widget("button_laplace", button_laplace_);
  ref_xml_->get_widget("button_sobel", button_sobel_);
  ref_xml_->get_widget("button_canny", button_canny_);
  ref_xml_->get_widget("button_default", button_default_);
  ref_xml_->get_widget("button_gray", button_gray_);
  ref_xml_->get_widget("button_flow", button_flow_);
  ref_xml_->get_widget("button_color", button_color_);
  ref_xml_->get_widget("button_conv", button_conv_);
  ref_xml_->get_widget("button_pyramid", button_pyramid_);
  ref_xml_->get_widget("button_houghcircles", button_houghcircles_);
  ref_xml_->get_widget("eventbox", eventbox_);

  /* Define callbacks on toggle GUI checkbox  */
  button_canny_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_sobel_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_laplace_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_hough_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_harris_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_default_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_gray_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_flow_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_color_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_conv_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_pyramid_->signal_clicked().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  button_houghcircles_->signal_toggled().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));
  combobox_hough_->signal_changed().connect(
      sigc::mem_fun(this, &Viewer::ButtonClicked));

  eventbox_->signal_button_press_event().connect(
      sigc::mem_fun(this, &Viewer::OnClickedEventBox));

  /* Set default element to first one */
  combobox_hough_->set_active(0);
  combobox_conv_->set_active(0);

  /* Initialize all private checkbox controls simulating an update */
  ButtonClicked();

  pthread_mutex_init(&mutex_, NULL);
  pthread_mutex_lock(&mutex_);
}

Viewer::~Viewer() {
}

/**
 * Get GUI visibility.
 * @return  true if it is visible, false if not.
 */
bool Viewer::isVisible() {
  return main_window_->is_visible();
}

bool Viewer::OnClickedEventBox(GdkEventButton* event) {
  cv::Mat hsvimage(previous_image_.size(), CV_8UC1);
  int posX;
  int posY;
  double r, g, b;
  double h, s, v;
  int indice;

  posX = (int) event->x;
  posY = (int) event->y;

  pthread_mutex_lock(&mutex_);

  indice = posY * previous_image_.step + posX * previous_image_.channels();

  previous_image_.copyTo(hsvimage);
  r = (float) (unsigned int) (unsigned char) hsvimage.data[indice];
  g = (float) (unsigned int) (unsigned char) hsvimage.data[indice + 1];
  b = (float) (unsigned int) (unsigned char) hsvimage.data[indice + 2];
  pthread_mutex_unlock(&mutex_);

  h = GetH(r, g, b);
  s = GetS(r, g, b);
  v = GetV(r, g, b);

  double rmax, rmin, gmax, gmin, bmax, bmin;
  rmax = h * DEGTORAD + 0.2;
  rmin = h * DEGTORAD - 0.2;
  if (rmax > 6.28)
    rmax = 6.28;
  if (rmin < 0.0)
    rmin = 0.0;

  gmax = s + 0.1;
  gmin = s - 0.1;
  if (gmax > 1.0)
    gmax = 1.0;
  if (gmin < 0.0)
    gmin = 0.0;

  bmax = v + 50.0;
  bmin = v - 50.0;
  if (bmax > 255.0)
    bmax = 255.0;
  if (bmin < 0.0)
    bmin = 0.0;

  scale_h_max_->set_value(rmax);
  scale_h_min_->set_value(rmin);
  scale_s_max_->set_value(gmax);
  scale_s_min_->set_value(gmin);
  scale_v_max_->set_value(bmax);
  scale_v_min_->set_value(bmin);

  if (rmin > rmax) {
    rmax = rmin;
    scale_h_max_->set_value(rmax);
  }

  if (gmin > gmax) {
    gmax = gmin;
    scale_s_max_->set_value(gmax);
  }

  if (bmin > bmax) {
    bmax = bmin;
    scale_v_max_->set_value(bmax);
  }
  std::cout << "Mouse click on: X=" << event->x << ", Y=" << event->y
            << std::endl;
  return true;
}

/**
 * Laplace operator.
 * @param image Input/output frame
 * @see <a href="http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html">Laplace Operator</a>
 */
void Viewer::Laplace(cv::Mat image) {
  /* Aperture size used to compute the second-derivative filters.
   * The size must be positive and odd. */
  int aperture = scale_sobel_->get_value();
  if (aperture % 2 == 0) {
    aperture++;
  }
  std::cout << "Laplace aperture: " << aperture << std::endl;

  /* Get a working copy of input image */
  cv::Mat working_copy = image.clone();

  /* Convert working copy to grey scale */
  cv::cvtColor(working_copy, working_copy, CV_RGB2GRAY);
  /* Apply Laplace operator:
   * http://docs.opencv.org/modules/imgproc/doc/filtering.html#laplacian */
  cv::Laplacian(working_copy, working_copy, working_copy.depth(), aperture);
  /* Prescale values, get absolute value and apply alpha 1 and beta 0 */
  cv::convertScaleAbs(working_copy, working_copy);
  /* Convert result image back to RGB8 */
  cv::cvtColor(working_copy, image, CV_GRAY2RGB);
}

int Viewer::CheckHsvValues(double H, double S, double V) {

  if (!((S <= scale_s_max_->get_value()) && (S >= scale_s_min_->get_value())
      && (V <= scale_v_max_->get_value()) && (V >= scale_v_min_->get_value())))
    return 0;

  H = H * PI / 180.0;

  if (scale_h_min_->get_value() < scale_h_max_->get_value()) {
    if ((H <= scale_h_max_->get_value()) && (H >= scale_h_min_->get_value()))
      return 1;
  } else {
    if (((H >= 0.0) && (H <= scale_h_max_->get_value()))
        || ((H <= 2 * PI) && (H >= scale_h_min_->get_value())))
      return 1;
  }

  return 0;
}

double Viewer::GetH(double r, double g, double b) {
  double max = 0.0;
  double min = 255.0;

  if (r >= g && r >= b)
    max = r;
  if (g >= r && g >= b)
    max = g;
  if (b >= r && b >= g)
    max = b;

  if (r <= g && r <= b)
    min = r;
  if (g <= r && g <= b)
    min = g;
  if (b <= r && b <= g)
    min = b;

  if (max == min)
    return 0;

  if (max == r) {
    if (g >= b) {
      return (60.0 * (g - b) / (max - min));
    } else {
      return ((60.0 * (g - b) / (max - min)) + 360.0);
    }
  }
  if (max == g) {
    return ((60.0 * (b - r) / (max - min)) + 120.0);
  }
  if (max == b) {
    return ((60.0 * (r - g) / (max - min)) + 240.0);
  }

  return 0;
}
double Viewer::GetS(double r, double g, double b) {
  double max = 0.0;
  double min = 255.0;

  if (r >= g && r >= b)
    max = r;
  if (g >= r && g >= b)
    max = g;
  if (b >= r && b >= g)
    max = b;
  if (max == 0.0)
    return 0.0;
  if (r <= g && r <= b)
    min = r;
  if (g <= r && g <= b)
    min = g;
  if (b <= r && b <= g)
    min = b;

  return (1.0 - (min / max));
}
double Viewer::GetV(double r, double g, double b) {
  if (r >= g && r >= b)
    return r;
  if (g >= r && g >= b)
    return g;
  if (b >= r && b >= g)
    return b;

  return 0;
}

void Viewer::ColorFilter(cv::Mat image) {
  cv::Mat src;
  image.copyTo(src);

  cv::Mat cvResultado(src.size(), CV_8UC1);
  src.copyTo(cvResultado);

  double r, g, b;
  int i;
  double h, s, v;
  cv::Size size = cvResultado.size();

  for (i = 0; i < size.width * size.height; i++) {
    r = (float) (unsigned int) (unsigned char) cvResultado.data[i * 3];
    g = (float) (unsigned int) (unsigned char) cvResultado.data[i * 3 + 1];
    b = (float) (unsigned int) (unsigned char) cvResultado.data[i * 3 + 2];

    h = GetH(r, g, b);
    s = GetS(r, g, b);
    v = GetV(r, g, b);

    if (scale_h_max_->get_value() >= h * DEGTORAD
        && scale_h_min_->get_value() <= h * DEGTORAD
        && scale_s_max_->get_value() >= s && scale_s_min_->get_value() <= s
        && scale_v_max_->get_value() >= v && scale_v_min_->get_value() <= v) {
    } else {
      /* Gray Scale */
      cvResultado.data[i * 3] = 0;  //(unsigned char) (v*100/255);
      cvResultado.data[i * 3 + 1] = 0;  //(unsigned char) (v*100/255);
      cvResultado.data[i * 3 + 2] = 0;  //(unsigned char) (v*100/255);
    }
  }
  cvResultado.copyTo(image);
}

/**
 * Convolution.
 * @param image Input/output frame
 * @see <a href="http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/filter_2d/filter_2d.html">Making your own linear filters!</a>
 */
void Viewer::Conv(cv::Mat image) {
  cv::Mat mask;

  /* We set default positive and negative sums */
  double negative = 0, positive = 0;

  /* Get effect selected in combo box */
  switch (combobox_conv_->get_active_row_number()) {
    case 0: /* Sharpenning with forzed 1:1 scale (color will saturate) */
      negative = 0;
      positive = 1;
    case 1: /* Sharpenning with maximum and minimum range adapted to 0..255 */
      mask = (cv::Mat_<float>(3, 3) <<
      /**/0, -1, 0,
      /**/-1, 5, -1,
      /**/0, -1, 0);
      break;
    case 2: /* Gaussian Blur */
      mask = (cv::Mat_<float>(3, 3) <<
      /**/0, 1, 0,
      /**/1, 1, 1,
      /**/0, 1, 0);
      break;
    case 3: /* Embossing with forzed 1:1 scale (color will saturate) */
      negative = 0;
      positive = 1;
    case 4: /* Embossing with maximum and minimum range adapted to 0..255 */
      mask = (cv::Mat_<float>(3, 3) <<
      /**/-2, -1, 0,
      /**/-1, 1, 1,
      /**/0, 1, 2);
      break;
    case 5: /* Edge Detector with forzed color saturatation */
      negative = -0.25;
      positive = 0.25;
    case 6: /* Edge Detector with maximum and minimum range adapted to 0..255 */
      mask = (cv::Mat_<float>(3, 3) <<
      /**/0, -1, 0,
      /**/-1, 4, -1,
      /**/0, -1, 0);
      break;
  }
  /* If range was not forced, calculate it */
  if ((positive == 0) && (negative == 0)) {
    /* Calculate maximum and minimum values to adjust offset and scale */
    for (int i = 0; i < mask.rows; i++) {
      for (int j = 0; j < mask.cols; j++) {
        if (mask.at<float>(i, j) > 0) {
          positive += mask.at<float>(i, j);
        } else {
          negative -= mask.at<float>(i, j);
        }
      }
    }
  }

  /* Normalize difference between negative and negative (range) to 1 (0..255) */
  float range = positive + negative;
  if (range != 0.0F) {
    mask = mask / range;
  }
  /* Get a working copy of input image */
  cv::Mat working_image = image.clone();

  int ddepth = -1; /* Same pixel format as source */
  cv::Point anchor = cv::Point(-1, -1); /* Center anchor */
  /* Apply selected filter to working image and output it to image cv::Mat
   * http://docs.opencv.org/modules/imgproc/doc/filtering.html#filter2d */
  filter2D(working_image, image, ddepth, mask, anchor, negative);
}

void Viewer::Pyramid(cv::Mat image) {

  cv::Mat src;
  image.copyTo(src);
  int i, w, w2, tmp;
  cv::Size imasize;

  imasize = image.size();

  cv::Mat div2(cv::Size(imasize.width / 2, imasize.height / 2), CV_8UC3);
  cv::Mat div4(cv::Size(imasize.width / 4, imasize.height / 4), CV_8UC3);
  cv::Mat div8(cv::Size(imasize.width / 8, imasize.height / 8), CV_8UC3);
  cv::Mat div16(cv::Size(imasize.width / 16, imasize.height / 16), CV_8UC3);
  cv::Mat dst(imasize, CV_8UC3);

  pyrDown(src, div2);
  pyrDown(div2, div4);
  pyrDown(div4, div8);
  pyrDown(div8, div16);

  dst = cv::Mat::zeros(dst.size(), CV_8UC3);
  w = imasize.width * 3;
  cv::Size div2size = div2.size();
  cv::Size div4size = div4.size();
  cv::Size div8size = div8.size();
  cv::Size div16size = div16.size();
  for (i = 0; i < imasize.height / 2; i++) {
    w2 = div2size.width * div2.channels();
    memcpy((dst.data) + w * i, (div2.data) + w2 * i, w2);
    //if(i<image.height/4) {
    if (i < imasize.height / 4) {
      //tmp = (image.width/2)*3;
      tmp = (imasize.width / 2) * 3;
      w2 = div4size.width * div4.channels();
      //memcpy((dst->imageData)+w*i+tmp, (div4->imageData)+w2*i, w2);
      memcpy((dst.data) + w * i + tmp, (div4.data) + w2 * i, w2);
    }
    if (i < imasize.height / 8) {
      tmp = (imasize.width / 2 + imasize.width / 4) * 3;
      w2 = div8size.width * div8.channels();
      //memcpy((dst->imageData)+w*i+tmp, (div8->imageData)+w2*i, w2);
      memcpy((dst.data) + w * i + tmp, (div8.data) + w2 * i, w2);
    }
    if (i < imasize.height / 16) {
      tmp = (imasize.width / 2 + imasize.width / 4 + imasize.width / 8) * 3;
      w2 = div16size.width * div16.channels();
      //memcpy((dst->imageData)+w*i+tmp, (div16->imageData)+w2*i, w2);
      memcpy((dst.data) + w * i + tmp, (div16.data) + w2 * i, w2);
    }
  }
  dst.copyTo(image);
}

/**
 * Sobel Derivatives.
 * @param image Input/output frame
 * @see <a href="http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html">Sobel Derivatives</a>
 */
void Viewer::Sobel(cv::Mat image) {
  /* Size of the extended Sobel kernel: it must be 1, 3, 5 or 7 */
  int aperture = scale_sobel_->get_value();
  if (aperture % 2 == 0) {
    aperture++;
  }
  if (aperture > 7) {
    aperture = 7;
  }
  std::cout << "Sobel aperture: " << aperture << std::endl;

  /* Get a working copy of input image */
  cv::Mat working_copy = image.clone();

  /* Define images for X and Y gradients */
  cv::Mat gradient_x, gradient_y;

  /* Convert working copy to grey scale */
  cv::cvtColor(working_copy, working_copy, CV_RGB2GRAY);
  /* Get gradient X using Sobel derivative:
   * http://docs.opencv.org/modules/imgproc/doc/filtering.html#sobel */
  cv::Sobel(working_copy, gradient_x, working_copy.depth(), 1, 0, aperture);
  /* Prescale every value and get absolute value with alpha 1 and beta 0 */
  cv::convertScaleAbs(gradient_x, gradient_x);
  /* Get gradient Y using Sobel derivative */
  cv::Sobel(working_copy, gradient_y, working_copy.depth(), 0, 1, aperture);
  /* Prescale every value and get absolute value with alpha 1 and beta 0 */
  cv::convertScaleAbs(gradient_y, gradient_y);
  /* Mix both gradients with same weigth (50%-50%) in working copy */
  cv::addWeighted(gradient_y, 0.5, gradient_y, 0.5, 0, working_copy);
  /* Copy back processed image converted to RGB8 */
  cv::cvtColor(working_copy, image, CV_GRAY2RGB);
}

/**
 * Canny Edge Detector.
 * @param image Input/output frame
 * @see <a href="http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html">Canny Edge Detector</a>
 */
void Viewer::Canny(cv::Mat image) {
  /* Aperture size for internal Sobel operator: it must be 3, 5 or 7 */
  int aperture = scale_sobel_->get_value();
  if (aperture % 2 == 0) {
    aperture++;
  }
  if (aperture > 7) {
    aperture = 7;
  } else if (aperture < 3) {
    aperture = 3;
  }
  double threshold = scale_canny_->get_value();
  std::cout << "Threshold1: " << threshold << ", Threshold2: "
            << (threshold * 3) << ", Sobel aperture: " << aperture << std::endl;

  /* Get a working copy of input image */
  cv::Mat working_copy = image.clone();

  /* Convert working copy to grey scale */
  cv::cvtColor(working_copy, working_copy, CV_RGB2GRAY);
  /* Apply Canny Edge Detector:
   * http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=canny#canny */
  cv::Canny(working_copy, working_copy, threshold, threshold * 3, aperture);
  /* Convert result image back to RGB8 */
  cv::cvtColor(working_copy, image, CV_GRAY2RGB);
}

/**
 * Transform an image from BGR to gray scale format by using cvtColor.
 * @param image Input/output frame
 * @see <a href="http://docs.opencv.org/doc/tutorials/introduction/load_save_image/load_save_image.html">BGR to Grayscale</a>
 */
void Viewer::Gray(cv::Mat image) {
  /* Get a working copy of input image */
  cv::Mat working_copy = image.clone();

  /* Convert working copy to grey scale */
  cv::cvtColor(image, working_copy, CV_RGB2GRAY);
  /* Convert result image back to RGB8 (still looking grey scale) */
  cv::cvtColor(working_copy, image, CV_GRAY2RGB);
}

void Viewer::Harris(cv::Mat image) {
  cv::Mat src;
  image.copyTo(src);

  int aperture = scale_sobel_->get_value();
  if (aperture % 2 == 0) {
    aperture += 1;
  }
  cv::Mat gray(image.size(), CV_8UC1);
  cv::Mat dst(image.size(), CV_32FC1);
  cv::Mat gaux(image.size(), CV_8UC1);

  cv::cvtColor(src, gray, CV_RGB2GRAY);
  cv::cornerHarris(gray, dst, 5, aperture, 0.04);
  dst.convertTo(gaux, gaux.type(), 1, 0);
  cv::cvtColor(gaux, src, CV_GRAY2RGB);

  src.copyTo(image);
}

void Viewer::HoughCircles(cv::Mat image) {
  cv::Mat src;
  image.copyTo(src);

  cv::Mat gray(image.size(), CV_8UC1);
  std::vector<cv::Vec3f> circles;
  cv::cvtColor(src, gray, CV_BGR2GRAY);
  cv::Size graysize = gray.size();
  cv::GaussianBlur(gray, gray, cv::Size(9, 9), 0, 0);  // smooth it, otherwise a lot of false circles may be detected
  cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, graysize.height / 4,
                   200, 100);

  size_t i;

  for (i = 0; i < circles.size(); i++) {
    cv::Point p(cvRound(circles[i][0]), cvRound(circles[i][1]));
    int radius = cvRound(circles[i][2]);
    cv::circle(gray, p, 3, cv::Scalar(255, 255, 0), -1, 8, 0);
    cv::circle(gray, p, radius, cv::Scalar(255, 255, 0), 3, 8, 0);
  }
  cv::cvtColor(gray, src, CV_GRAY2RGB);

  src.copyTo(image);
}

void Viewer::Hough(cv::Mat image) {
  int aperture = scale_sobel_->get_value();
  if (aperture % 2 == 0) {
    aperture++;
  }
  if (aperture < 3)
    aperture = 3;
  else if (aperture > 7)
    aperture = 7;

  cv::Mat src;
  image.copyTo(src);

  int method = combobox_hough_->get_active_row_number();

  cv::Mat color_dst(image.size(), CV_8UC3);
  cv::Mat dst(image.size(), CV_8UC1);
  cv::Mat gray(image.size(), CV_8UC1);
  std::vector<cv::Vec2f> lines;
  size_t i;

  cv::cvtColor(src, gray, CV_RGB2GRAY);
  cv::Canny(gray, dst, scale_canny_->get_value(), scale_canny_->get_value() * 3,
            aperture);
  cv::cvtColor(dst, color_dst, CV_GRAY2BGR);

  if (method == 0) {
    HoughLines(dst, lines, 1, CV_PI / 180, scale_hough_threshold_->get_value(),
               0, 0);

    for (i = 0; i < MIN(lines.size(), 100); i++) {

      float rho = lines[i][0];
      float theta = lines[i][1];
      double a = cos(theta), b = sin(theta);
      double x0 = a * rho, y0 = b * rho;
      cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
      cv::Point pt2(cvRound(cvRound(x0 - 1000 * (-b))),
                    cvRound(y0 - 1000 * (a)));
      line(color_dst, pt1, pt2, cv::Scalar(255, 0, 0), 3, 8);
    }
  } else {
    if (method == 1) {
      std::vector<cv::Vec4i> linesp;

      HoughLinesP(dst, linesp, 1, CV_PI / 180,
                  scale_hough_threshold_->get_value(),
                  scale_hough_long_->get_value(),
                  scale_hough_gap_->get_value());

      for (i = 0; i < linesp.size(); i++) {
        line(color_dst, cv::Point(linesp[i][0], linesp[i][1]),
             cv::Point(linesp[i][2], linesp[i][3]), cv::Scalar(255, 0, 0), 3,
             8);
      }
    }
  }
  color_dst.copyTo(src);
  src.copyTo(image);
}

void Viewer::OpticalFlow(cv::Mat image) {
  cv::Mat src;
  image.copyTo(src);

  if (opflow_first) {
    if (previous_image_.empty())
      return;
  }
  /* Images with feature points */
  cv::Mat img1(image.size(), CV_8UC1);
  cv::Mat img2(image.size(), CV_8UC1);

  cv::TermCriteria criteria = cv::TermCriteria(
      CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .03);

  /*Temp images for algorithms*/
  cv::cvtColor(previous_image_, img1, CV_RGB2GRAY);
  cv::cvtColor(src, img2, CV_RGB2GRAY);

  int i;
  int numpoints = 90;  // 300;
  std::vector<cv::Point2f> points[2]; /* Feature points from img1 */
  std::vector<uchar> foundPoint;
  std::vector<float> errors;
  cv::Size sizeWindow(31, 31), pixWinSize(15, 15);
  //CvTermCriteria termCriteria;

  //termCriteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );

  /*  Shi and Tomasi algorithm, get feature points from img1 */
  //cvGoodFeaturesToTrack(img1, aux1, aux2, points1, &numpoints, .01,.01,NULL);
  goodFeaturesToTrack(img1, points[0], numpoints, .01, .01);

  if (points[0].size() > 0) {

    cornerSubPix(img1, points[0], pixWinSize, cv::Size(-1, -1), criteria);
    /* Pyramidal Lucas Kanade Optical Flow algorithm, search feature points in img2 */
    calcOpticalFlowPyrLK(img1, img2, points[0], points[1], foundPoint, errors,
                         sizeWindow, 5, criteria);

    /*Esta funcion sirve para colorear los puntos encontrados. Reservado para depuración.*/
    /* Draw arrows*/
    for (i = 0; i < numpoints; i++) {
      if (foundPoint[i] == 0)
        continue;

      int line_thickness = 1;
      cv::Scalar line_color(255, 0, 0);

      cv::Point p((int) points[0][i].x, (int) points[0][i].y);
      cv::Point q((int) points[1][i].x, (int) points[1][i].y);

      double angle = atan2((double) p.y - q.y, (double) p.x - q.x);
      double hypotenuse = sqrt(SQUARE(p.y - q.y) + SQUARE(p.x - q.x));

      if (hypotenuse < 10 || hypotenuse > 40)
        continue;

      /*Line*/
      q.x = (int) (p.x - 1 * hypotenuse * cos(angle));
      q.y = (int) (p.y - 1 * hypotenuse * sin(angle));
      line(src, p, q, line_color, line_thickness, CV_AA, 0);

      /*Arrow*/
      p.x = (int) (q.x + 9 * cos(angle + PI / 4));
      p.y = (int) (q.y + 9 * sin(angle + PI / 4));
      line(src, p, q, line_color, line_thickness, CV_AA, 0);
      p.x = (int) (q.x + 9 * cos(angle - PI / 4));
      p.y = (int) (q.y + 9 * sin(angle - PI / 4));
      line(src, p, q, line_color, line_thickness, CV_AA, 0);
    }
  }

  src.copyTo(image);
}

/**
 * Display error image in GUI.
 */
void Viewer::DisplayError() {
  pthread_mutex_unlock(&mutex_);
  gtk_image_in_->set(Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_DIALOG);
  main_window_->resize(1, 1);
  while (gtk_main_.events_pending())
    gtk_main_.iteration();
  pthread_mutex_lock(&mutex_);
}

/**
 * Display input image, process it and display it too in GUI.
 * @param image Input frame to be processed
 */
void Viewer::Display(cv::Mat image) {
  /* Get a copy of input image and apply on it user selection */
  cv::Mat output_image = image.clone();
  ApplySelection(output_image);

  /* Get a copy of this frame, will be previous one next time */
  previous_image_ = image.clone();

  pthread_mutex_unlock(&mutex_);

  /* Both images (input and output) remain the same size */
  cv::Size image_size = image.size();

  /* Create GUI image from RGB8 input image data */
  Glib::RefPtr<Gdk::Pixbuf> gtk_input_image = Gdk::Pixbuf::create_from_data(
      (const guint8*) image.data, Gdk::COLORSPACE_RGB, false, 8,
      image_size.width, image_size.height, image.step);

  /* Create GUI image from RGB8 output image data */
  Glib::RefPtr<Gdk::Pixbuf> gtk_output_image = Gdk::Pixbuf::create_from_data(
      (const guint8*) output_image.data, Gdk::COLORSPACE_RGB, false, 8,
      image_size.width, image_size.height, output_image.step);

  /* Update input image in GUI */
  gtk_image_in_->clear();
  gtk_image_in_->set(gtk_input_image);

  /* Update output image in GUI */
  gtk_image_out_->clear();
  gtk_image_out_->set(gtk_output_image);

  /* Force update GUI and process events */
  main_window_->resize(1, 1);
  while (gtk_main_.events_pending())
    gtk_main_.iteration();

  pthread_mutex_lock(&mutex_);
}

/**
 * Process GUI events on most widgets and update class members.
 */
void Viewer::ButtonClicked() {
  /* If default button is pressed, reset other filters and itself */
  if (button_default_->get_active()) {
    def_box_ = 0;
    button_default_->set_active(false);
    button_sobel_->set_active(false);
    button_laplace_->set_active(false);
    button_gray_->set_active(false);
    button_conv_->set_active(false);
    button_pyramid_->set_active(false);
    button_color_->set_active(false);
  } else {
    def_box_ = 0;
  }
  /* If some filter is pressed then default button is unset */
  if (button_gray_->get_active()) {
    gray_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    gray_box_ = 0;
  }
  if (button_sobel_->get_active()) {
    sobel_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    sobel_box_ = 0;
  }
  if (button_laplace_->get_active()) {
    laplace_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    laplace_box_ = 0;
  }
  if (button_pyramid_->get_active()) {
    pyramid_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    pyramid_box_ = 0;
  }
  if (button_color_->get_active()) {
    color_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    color_box_ = 0;
  }
  if (button_conv_->get_active()) {
    conv_box_ = 1;
    button_default_->set_active(false);
    def_box_ = 0;
  } else {
    conv_box_ = 0;
  }
  /* Feature detection doesn't reset default button */
  if (button_harris_->get_active()) {
    harris_box_ = 1;
  } else {
    harris_box_ = 0;
  }
  if (button_canny_->get_active()) {
    canny_box_ = 1;
  } else {
    canny_box_ = 0;
  }
  if (button_hough_->get_active()) {
    hough_box_ = 1;
  } else {
    hough_box_ = 0;
  }
  if (button_houghcircles_->get_active()) {
    houghcircles_box_ = 1;
  } else {
    houghcircles_box_ = 0;
  }
  /* Movement detection doesn't reset default button */
  if (button_flow_->get_active()) {
    flow_box_ = 1;
  } else {
    flow_box_ = 0;
  }
  /* If Hough transform and standard algorithm are selected: show more options */
  if (button_hough_->get_active()
      && combobox_hough_->get_active_row_number() == 0) {
    scale_hough_long_->show();
    scale_hough_gap_->show();
    label_hough_long_->show();
    label_hough_gap_->show();
  } else {
    scale_hough_long_->hide();
    scale_hough_gap_->hide();
    label_hough_long_->hide();
    label_hough_gap_->hide();
  }
}

void Viewer::ApplySelection(cv::Mat image) {

  bool INFO = true;

  if (gray_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "GRAY\n";
    }
    Gray(image);
  }

  if (sobel_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "SOBEL : aperture = " << scale_sobel_->get_value() << "\n";
    }
    Sobel(image);
  }

  if (laplace_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "LAPLACE\n";
    }
    Laplace(image);
  }

  /* Convolution filter before color filter to enhance it */
  if (conv_box_) {
    if (INFO) {
      std::cout << "**************\n";
    }
    Conv(image);
  }

  if (color_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "COLOR FILTER : Hmax =" << scale_h_max_->get_value()
                << "; Hmin =" << scale_h_min_->get_value() << "; Smaxn ="
                << scale_s_max_->get_value() << "; Smin ="
                << scale_s_min_->get_value() << "; Vmax ="
                << scale_v_max_->get_value() << "; Vmin ="
                << scale_v_min_->get_value() << "\n";
    }
    ColorFilter(image);
  }

  if (harris_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "HARRIS CORNER\n";
    }
    Harris(image);
  }

  if (canny_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "CANNY FILTER : threshold = " << scale_canny_->get_value()
                << "\n";
    }
    Canny(image);
  }

  if (hough_box_) {
    if (INFO) {
      std::cout << "**************\n";
      if (combobox_hough_->get_active_row_number() == 0)
        std::cout << "HOUGH STANDARD : threshold = "
                  << scale_hough_threshold_->get_value() << "\n";
      else
        std::cout << "HOUGH PROBABILISTIC : threshold = "
                  << scale_sobel_->get_value() << "; length = "
                  << scale_hough_long_->get_value() << "; gap = "
                  << scale_hough_gap_->get_value() << "\n";
    }
    Hough(image);
  }

  if (houghcircles_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "HOUGH CIRCLES\n";
    }
    HoughCircles(image);
  }

  if (flow_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "OPTICAL FLOW\n";
    }
    OpticalFlow(image);
  }

  /* Last filter: pyramid */
  if (pyramid_box_) {
    if (INFO) {
      std::cout << "**************\n";
      std::cout << "PYRAMID\n";
    }
    Pyramid(image);
  }
}

}  // namespace
