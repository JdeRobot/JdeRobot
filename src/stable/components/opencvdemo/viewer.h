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

#ifndef JDEROBOT_COMPONENTS_OPENCVDEMO_VIEWER_H_
#define JDEROBOT_COMPONENTS_OPENCVDEMO_VIEWER_H_

#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>
#include <opencv2/opencv.hpp>

/* Trigonometric utilities */
#define PI 3.14159265359
#define DEGTORAD (PI / 180.0)
#define SQUARE(a) (a)*(a)

namespace opencvdemo {

/**
 *  Gtk+ GUI class
 */
class Viewer {
 public:
  Viewer();
  ~Viewer();

  // function that actually displays the images
  void DisplayError();
  void Display(cv::Mat image);
  bool isVisible();

 private:

  Glib::RefPtr<Gnome::Glade::Xml> ref_xml_;

  Gtk::Image* gtk_image_in_; /* GUI input image */
  Gtk::Image* gtk_image_out_; /* GUI output image */
  Gtk::Window* main_window_;
  Gtk::Main gtk_main_;

  //IplImage* imagenO;
  cv::Mat previous_image_;
  pthread_mutex_t mutex_;

  /* Horizontal and vertical slidebars */
  Gtk::HScale* scale_sobel_;
  Gtk::HScale* scale_canny_;
  Gtk::HScale* scale_hough_threshold_;
  Gtk::HScale* scale_hough_long_;
  Gtk::HScale* scale_hough_gap_;
  Gtk::VScale* scale_h_max_;
  Gtk::VScale* scale_h_min_;
  Gtk::VScale* scale_v_max_;
  Gtk::VScale* scale_v_min_;
  Gtk::VScale* scale_s_max_;
  Gtk::VScale* scale_s_min_;

  /* Event box that receives mouse click events on GUI input image */
  Gtk::EventBox* eventbox_;

  /* Check buttons that implement the filters */
  Gtk::CheckButton* button_canny_;
  Gtk::CheckButton* button_sobel_;
  Gtk::CheckButton* button_laplace_;
  Gtk::CheckButton* button_hough_;
  Gtk::CheckButton* button_harris_;
  Gtk::CheckButton* button_default_;
  Gtk::CheckButton* button_gray_;
  Gtk::CheckButton* button_flow_;
  Gtk::CheckButton* button_conv_;
  Gtk::CheckButton* button_pyramid_;
  Gtk::CheckButton* button_color_;
  Gtk::CheckButton* button_houghcircles_;

  /* Combo boxes that make method selection */
  Gtk::ComboBox* combobox_hough_;
  Gtk::ComboBox* combobox_conv_;

  /* Text labels */
  Gtk::Label* label_hough_long_;
  Gtk::Label* label_hough_gap_;

  /* Filters and feature detectors */
  void ApplySelection(cv::Mat image);
  void Canny(cv::Mat image);
  void Sobel(cv::Mat image);
  void Laplace(cv::Mat image);
  void Hough(cv::Mat image);
  void HoughCircles(cv::Mat image);
  void Harris(cv::Mat image);
  void Gray(cv::Mat image);
  void OpticalFlow(cv::Mat image);
  void ColorFilter(cv::Mat image);
  void Conv(cv::Mat image);
  void Pyramid(cv::Mat image);
  int CheckHsvValues(double H, double S, double V);

  /* RGB to HSV converters */
  double GetH(double r, double g, double b);
  double GetS(double r, double g, double b);
  double GetV(double r, double g, double b);

  /* Event listeners: button or combo changed or input image clicked */
  void ButtonClicked();
  bool OnClickedEventBox(GdkEventButton* event);

  /* Checkbox control variables */
  int canny_box_;
  int sobel_box_;
  int laplace_box_;
  int harris_box_;
  int hough_box_;
  int hough_combo_;
  int houghcircles_box_;
  int def_box_;
  int gray_box_;
  int flow_box_;
  int color_box_;
  int conv_box_;
  int pyramid_box_;

};

}  //namespace

#endif  // JDEROBOT_COMPONENTS_OPENCVDEMO_VIEWER_H_
