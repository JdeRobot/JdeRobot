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
 *  Authors : Alejandro Hernández Cordero <ahcorde@gmail.com>
 *
 */

#ifndef CAMERAVIEW_VIEWER_H
#define CAMERAVIEW_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <highgui.h>

#define PI 3.141592654
#define SQUARE(a) (a)*(a)
#define DEGTORAD     (3.14159264 / 180.0)
#define RADTODEG     (180.0 /3.14159264)
#define centro_x 160
#define centro_y 160
#define radio_hsi_map 160.0
#define SMAX 320
namespace cameraview{

  class colorTuner
  {
  public:
    colorTuner();
    ~colorTuner();
  
    bool isVisible();

    //! function that actually displays the image in a window
    void display( IplImage* image );

  private:
    Gtk::Image* gtkimage;
    Gtk::Image* gtkimageOriginal;
    Gtk::Image* gtkimageColorSpaceHSV;
    Gtk::Image* gtkimageColorSpaceYUV;
    Gtk::EventBox* cajaImage;
    Gtk::EventBox* cajaSpacesHSV;
    Gtk::EventBox* cajaSpacesYUV;
        
    Gtk::Window* mainwindow;
    Gtk::Label* fpslabel;
    Gtk::Main gtkmain;
    Gtk::Table* layout;
    
    Gtk::Table*  layoutRGB;
    Gtk::VScale* sliderRMax;
	Gtk::VScale* sliderRMin;
	Gtk::VScale* sliderGMax;
	Gtk::VScale* sliderGMin;
	Gtk::VScale* sliderBMax;
	Gtk::VScale* sliderBMin;
	
    Gtk::Label* labelRMax;
	Gtk::Label* labelRMin;
	Gtk::Label* labelGMax;
	Gtk::Label* labelGMin;
	Gtk::Label* labelBMax;
	Gtk::Label* labelBMin;

    Gtk::Table* layoutType;
    Gtk::RadioButton::Group group;
    Gtk::RadioButton* radioButton_original;
    Gtk::RadioButton* radioButton_RGB; 
    Gtk::RadioButton* radioButton_HSV;
    Gtk::RadioButton* radioButton_YUV;

    int radio_original;
    int radio_RGB;
    int radio_HSV;
    int radio_YUV;

	IplImage* imageDemo;
	pthread_mutex_t mutex;
	
    IceUtil::Time currentFrameTime,oldFrameTime;
        double fps;
    int frameCount;
	
	void displayFrameRate();
	
	bool on_eventbox1_button_press_event(GdkEventButton* event);
	
	void on_active_original_toggled();
    void on_active_RGB_toggled();
    void on_active_HSV_toggled();
    void on_active_YUV_toggled();
    
    void filter_RGB(IplImage* cvResultado);
    void filter_HSV(IplImage* cvResultado);
    void filter_YUV(IplImage* cvResultado);	
    
    double rmax,rmin,gmax, gmin, bmax, bmin;
    
    void draw_hsvmap(int size);
    void draw_yuvmap(int size);
    
    bool on_hsv_image_space_button_press_event (GdkEventButton *event);
    bool on_hsv_image_space_button_release_event (GdkEventButton *event);
    int on_hsv_image_space_motion_notify_event (GdkEventMotion *event);
    
    bool on_yuv_image_space_motion_notify_event (GdkEventMotion *event);
    bool on_yuv_image_space_button_press_event (GdkEventButton *event);
    bool on_yuv_image_space_button_release_event (GdkEventButton *event);
    //void on_hsv_image_space_motion_notify_event (GtkWidget *event_box,GdkEventButton *event, gpointer data);
    
    /*HSV cheese coordenates*/

    int x_pulsada,y_pulsada,xsoltada,ysoltada;
    int x_max;
    int y_max;
    int x_min;
    int y_min;

    int xquesito1;
    int xquesito2;
    int xquesito3;
    int xquesito4;

    int yquesito1;
    int yquesito2;
    int yquesito3;
    int yquesito4;

    int pulsada;
    
    /*Option Parameters HSV*/
    float hmax;
    float hmin;
    float smax;
    float smin;
    float vmax;
    float vmin;

    //HSV
    void drawcheese (char *img,int x_centro,int y_centro, double h_max, double h_min, double s_max, double s_min, int thiscolor);
    int  drawarc(char *img, int xcentro, int ycentro, int radio, int x1, int y1, int x2, int y2, int thiscolor);
    int  lineinimage(char *img, int xa, int ya, int xb, int yb, int thiscolor);
    int drawcircle(char *img, int xcentro, int ycentro, int radio, int thiscolor); 
    
    //YUV
    void drawsquare (char *img,double u_max, double u_min, double v_max, double v_min, int thiscolor);
    
    /*YUV square coordenates*/
    int xsquare1;
    int xsquare2;
    int xsquare3;
    int xsquare4;

    int ysquare1;
    int ysquare2;
    int ysquare3;
    int ysquare4;
    
    float ymax;
    float ymin;
    float umax;
    float umin;
    float vmax2;
    float vmin2;
	
	#define RED 1 
	#define WHEAT 2
	#define PALEGREEN 3
	#define BLUE 4
	#define DEEPPINK 5
	#define WHITE 6
	#define BLACK 7  
	
    IplImage* hsv;
    IplImage* yuv;
        
    double getH(double r, double g, double b);
    double getS(double r, double g, double b);
    double getV(double r, double g, double b);
    
    
  };

}//namespace

#endif //CAMERAVIEW_VIEWER_H
