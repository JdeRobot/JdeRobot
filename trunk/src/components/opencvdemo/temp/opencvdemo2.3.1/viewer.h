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
 *	      Alejandro Hernández Cordero <ahcorde@gmail.com> 
 *            Daniel Gómez Gómez <danigom@terra.es>
 *
 */

#ifndef OPENCVDEMO_VIEWER_H
#define OPENCVDEMO_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>
#include <colorspaces/colorspacesmm.h>

#define DEGTORAD     (3.14159264 / 180.0)

namespace opencvdemo{

  class Viewer
  {
  public:
        Viewer();
        ~Viewer();
  
        // function that actually displays the images
        //void display( const colorspaces::Image& image,const colorspaces::Image& image2 );    
	void display( cv::Mat image, cv::Mat image2 );
        bool isVisible(); 	
        bool on_clicked(GdkEventButton * event);

		//Filters and Feature detectors
		//void selection( const colorspaces::Image& image );	
		void selection( cv::Mat image );
		/*void canny( const colorspaces::Image& image );
		void sobel( const colorspaces::Image& image );
		void laplace( const colorspaces::Image& image );
		void hough( const colorspaces::Image& image );
		void hough_circles( const colorspaces::Image& image );
		void harris( const colorspaces::Image& image );
		void gray( const colorspaces::Image& image );
		void flow( const colorspaces::Image& image );
		void color( const colorspaces::Image& image );	
		void conv( const colorspaces::Image& image );	
		void pyramid( const colorspaces::Image& image );	
		int valuesOK(double H, double S, double V);*/
		void canny( cv::Mat image );
                void sobel( cv::Mat image );
                void laplace( cv::Mat image );
                void hough( cv::Mat image );
                void hough_circles( cv::Mat image );
                void harris( cv::Mat image );
                void gray( cv::Mat image );
                void flow( cv::Mat image );
                void color( cv::Mat image );
                void conv( cv::Mat image );
                void pyramid( cv::Mat image );
                int valuesOK(double H, double S, double V);

		
	    double getH(double r, double g, double b);
        double getS(double r, double g, double b);
        double getV(double r, double g, double b);

  private:

 		Glib::RefPtr<Gnome::Glade::Xml> refXml;

        Gtk::Image* gtkimage; // input image
        Gtk::Image* gtkimage2; // output image
        Gtk::Window* mainwindow;
        Gtk::Main gtkmain;
        
        //IplImage* imagenO;
	cv::Mat imagenO;
    	pthread_mutex_t mutex;
		
		// Horizontal slidebars
        Gtk::HScale* scale_sobel; 
        Gtk::HScale* scale_canny;
        Gtk::HScale* hough_threshold; 
        Gtk::HScale* hough_long;
        Gtk::HScale* hough_gap;

		// Vertical slidebars
		Gtk::VScale* Hmax;
		Gtk::VScale* Hmin;
		Gtk::VScale* Vmax;
		Gtk::VScale* Vmin;
		Gtk::VScale* Smax;
		Gtk::VScale* Smin;
		
		Gtk::EventBox* eventbox;

		// Check buttons that implement the filters
        Gtk::CheckButton * button_canny;
        Gtk::CheckButton * button_sobel;
        Gtk::CheckButton * button_laplace;
        Gtk::CheckButton * button_hough;
        Gtk::CheckButton * button_harris;
        Gtk::CheckButton * button_default;
        Gtk::CheckButton * button_gray;
        Gtk::CheckButton * button_flow;
        Gtk::CheckButton * button_conv;
        Gtk::CheckButton * button_pyramid;
        Gtk::CheckButton * button_color;
        Gtk::CheckButton * button_houghcircles;

		// Selection of Method
		Gtk::ComboBox * hough_combobox;
		Gtk::ComboBox * conv_combobox;

		// Labels
        Gtk::Label * label_long;
        Gtk::Label * label_gap;

    //Checks if the button has been clicked
        void button_canny_clicked();
        void button_sobel_clicked();
        void button_laplace_clicked();
        void button_hough_clicked();
        void button_hough_circles_clicked();
        void button_harris_clicked();
        void button_default_clicked();
        void button_gray_clicked();
        void button_flow_clicked();		
        void button_color_clicked();		
        void button_conv_clicked();		
        void button_pyramid_clicked();				

		// Checkbox control
		int canny_box;
		int sobel_box;
		int laplace_box;
		int harris_box;
		int hough_box;
		int houghcircles_box;
		int def_box;		
		int gray_box;		
		int flow_box;
		int color_box;
		int conv_box;
		int pyramid_box;

  };

}//namespace

#endif //OPENCVDEMO_VIEWER_H
