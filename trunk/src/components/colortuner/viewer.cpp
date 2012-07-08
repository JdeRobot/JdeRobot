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

#include "viewer.h" 
#include <iostream>
#include <cmath>
#include <cv.h>
#include <highgui.h>
#include <colorspaces/colorspaces.h>

namespace cameraview{

    colorTuner::colorTuner() 
            : gtkmain(0,0) { //constructor

        std::cout << "Loading interface...";

        mainwindow = new Gtk::Window();		
        gtkimage = new Gtk::Image();
        gtkimageOriginal = new Gtk::Image();
        gtkimageColorSpaceHSV = new Gtk::Image();
        gtkimageColorSpaceYUV = new Gtk::Image();
        layout = new Gtk::Table(4,2, false);
        fpslabel = new Gtk::Label();
        
        layoutType = new Gtk::Table(4, 1, false);
        radio_original = 1;
        radio_RGB = 0;
        radio_HSV = 0;
        radio_YUV = 0;
        /*
        radioButton_original = new Gtk::RadioButton("Original");
        radioButton_RGB = new Gtk::RadioButton("RGB");
        radioButton_HSV = new Gtk::RadioButton("HSV");
        radioButton_YUV = new Gtk::RadioButton("YUV");
        */
        radioButton_original = Gtk::manage(new Gtk::RadioButton(group,"Original"));
        radioButton_RGB = Gtk::manage(new Gtk::RadioButton(group,"RGB"));
        radioButton_HSV = Gtk::manage(new Gtk::RadioButton(group,"HSV"));
        radioButton_YUV = Gtk::manage(new Gtk::RadioButton(group,"YUV"));
        
        radioButton_original->signal_toggled().connect(sigc::mem_fun(*this, &colorTuner::on_active_original_toggled));
        radioButton_RGB->signal_toggled().connect(sigc::mem_fun(*this, &colorTuner::on_active_RGB_toggled));
        radioButton_HSV->signal_toggled().connect(sigc::mem_fun(*this, &colorTuner::on_active_HSV_toggled));
        radioButton_YUV->signal_toggled().connect(sigc::mem_fun(*this, &colorTuner::on_active_YUV_toggled));
               
        layoutType->attach(*radioButton_original, 0, 1, 0, 1);
        layoutType->attach(*radioButton_RGB, 0, 1, 1, 2);
        layoutType->attach(*radioButton_HSV, 0, 1, 2, 3);
        layoutType->attach(*radioButton_YUV, 0, 1, 3, 4);
                            
        cajaImage = new Gtk::EventBox;
        cajaImage->add(*gtkimageOriginal);
        cajaImage->signal_button_press_event().connect( sigc::mem_fun(*this,&colorTuner::on_eventbox1_button_press_event) );
        
        cajaSpacesHSV = new Gtk::EventBox;
        cajaSpacesHSV->add(*gtkimageColorSpaceHSV);
        
        cajaSpacesYUV = new Gtk::EventBox;
        cajaSpacesYUV->add(*gtkimageColorSpaceYUV);
        
        cajaSpacesHSV->signal_button_press_event().connect( sigc::mem_fun(*this,&colorTuner::on_hsv_image_space_button_press_event) );
        cajaSpacesHSV->signal_button_release_event().connect( sigc::mem_fun(*this,&colorTuner::on_hsv_image_space_button_release_event) );
        cajaSpacesHSV->signal_motion_notify_event().connect( sigc::mem_fun(*this,&colorTuner::on_hsv_image_space_motion_notify_event) );
        
        cajaSpacesYUV->signal_button_press_event().connect( sigc::mem_fun(*this,&colorTuner::on_yuv_image_space_button_press_event) );
        cajaSpacesYUV->signal_button_release_event().connect( sigc::mem_fun(*this,&colorTuner::on_yuv_image_space_button_release_event) );
        cajaSpacesYUV->signal_motion_notify_event().connect( sigc::mem_fun(*this,&colorTuner::on_yuv_image_space_motion_notify_event) );
        
        layoutRGB = new Gtk::Table(2,6, false);

        sliderRMax = new Gtk::VScale(0, 6.18, 0.01);
		sliderRMax->set_value(6.18);
        sliderRMin = new Gtk::VScale(0, 6.18, 0.01);
        sliderGMax = new Gtk::VScale(0, 1.1, 0.1);
		sliderGMax->set_value(1);
        sliderGMin = new Gtk::VScale(0, 1, 0.01);
        sliderBMax = new Gtk::VScale(0, 256, 0.01);
		sliderBMax->set_value(255);
        sliderBMin = new Gtk::VScale(0, 256, 0.01);
        
        sliderRMax->set_inverted(true);
        sliderRMin->set_inverted(true);
        sliderGMax->set_inverted(true);
        sliderGMin->set_inverted(true);
        sliderBMax->set_inverted(true);
        sliderBMin->set_inverted(true);
        
        rmax = sliderRMax->get_value();
        rmin = sliderRMin->get_value();
        gmax = sliderGMax->get_value();
        gmin = sliderGMin->get_value();
        bmax = sliderBMax->get_value();
        bmin = sliderBMin->get_value();
        
        labelRMax = new Gtk::Label("RMax");
        labelRMin = new Gtk::Label("RMin");
        labelGMax = new Gtk::Label("GMax");
        labelGMin = new Gtk::Label("GMin");
        labelBMax = new Gtk::Label("BMax");
        labelBMin = new Gtk::Label("BMin");
        
        layoutRGB->attach(*labelRMax, 0, 1, 0, 1);
        layoutRGB->attach(*sliderRMax, 0, 1, 1, 2);
        layoutRGB->attach(*labelRMin, 1, 2, 0, 1);
        layoutRGB->attach(*sliderRMin, 1, 2, 1, 2);
        
        layoutRGB->attach(*labelGMax, 2, 3, 0, 1);
        layoutRGB->attach(*sliderGMax, 2, 3, 1, 2);
        layoutRGB->attach(*labelGMin, 3, 4, 0, 1);
        layoutRGB->attach(*sliderGMin, 3, 4, 1, 2);
        
        layoutRGB->attach(*labelBMax, 4, 5, 0, 1);
        layoutRGB->attach(*sliderBMax, 4, 5, 1, 2);
        layoutRGB->attach(*labelBMin, 5, 6, 0, 1);
        layoutRGB->attach(*sliderBMin, 5, 6, 1, 2);
        
        layout->attach(*gtkimage, 0, 1, 0, 1);
        layout->attach(*cajaImage, 1, 2, 0, 1);
        layout->attach(*fpslabel, 0, 1, 1, 2);
        layout->attach(*layoutType , 0, 1, 2, 3);
        layout->attach(*layoutRGB, 1, 2, 1, 3);
        layout->attach(*cajaSpacesHSV, 0, 1, 3, 4);
        layout->attach(*cajaSpacesYUV, 0, 1, 3, 4);
        
        mainwindow->add(*layout);
        
		mainwindow->resize(100,100);
	    mainwindow->set_visible();
	    mainwindow->show();
	    mainwindow->show_all_children();

	    mainwindow->set_title("ColorTuner");
        mainwindow->set_border_width(20);
        
        pthread_mutex_init(&mutex, NULL);
       
        pthread_mutex_lock(&mutex);
	    
        std::cout << "correct\n";
        
        draw_hsvmap(320);
        draw_yuvmap(320);
        
        x_max=200;
        y_max=200;
        pulsada=0;
        x_min=centro_x;
        y_min=centro_y;
        
        hmax = 6.0;
        hmin = 0.0;
        smax = 0.9;
        smin = 0.1;
        vmax = 255.0;
        vmin = 0.0;
        
        ymax = 1.0;
        ymin = 0.0;
        umax = 0.2;
        umin = -0.2;
        vmax2 = 0.3;
        vmin2 = -0.3;
        
    }
    
    colorTuner::~colorTuner() {} //destructor

        ////////////////////////////////////////////////////// HSV ///////////////////////////////////////////////////////////
        
    bool colorTuner::on_hsv_image_space_button_release_event (GdkEventButton *event)
    {
        /*Stop drawing cheese*/
        pulsada=0;
        return true;
    }
    
    int colorTuner::on_hsv_image_space_motion_notify_event (GdkEventMotion *event)
  //  void colorTuner::on_hsv_image_space_motion_notify_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
    {
	    int coordx = event->x;
	    int coordy = event->y;

	    if (pulsada>0){
		     xsoltada=coordx;ysoltada=coordy;
		     if (sqrt((xsoltada-centro_x)*(xsoltada-centro_x)+(ysoltada-centro_y)*(ysoltada-centro_y))<=radio_hsi_map){
		        xsoltada=xsoltada-centro_x;
		        ysoltada=centro_y-ysoltada;
		        switch(pulsada){
		           case 1:					
			      smax=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			      if (xsoltada==0){	
			         if (ysoltada<0){
				    hmax=3*3.1416/2.;
			         }else{
				    hmax=3.1416/2.;
			         }
			      }else{
			         if (ysoltada==0){
				    if (xsoltada<0){
				       hmax=3.1416;
				    }else{
				       hmax=0.;
				    }
			         }else{
				    if (xsoltada>0){
				       hmax=atan((float)ysoltada/(float)xsoltada);
				       if(ysoltada<0){
				          hmax=hmax+2*3.1416;
				       }
				    }else{
				       hmax=atan((float)ysoltada/(float)xsoltada)+3.1416;
				    }
			         }
			      }
			      break;
		           case 2:
			      smax=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			      if (xsoltada==0){	
			         if (ysoltada<0){
				    hmin=3*3.1416/2.;
			         }else{
				    hmin=3.1416/2.;
			         }
			      }else{
			         if (ysoltada==0){
				    if (xsoltada<0){
				       hmin=3.1416;
				    }else{
				       hmin=0.;
				    }
			         }else{
				    if (xsoltada>0){
				       hmin=atan((float)ysoltada/(float)xsoltada);
				       if(ysoltada<0){
				          hmin=hmin+2*3.1416;
				       }
				    }else{
				       hmin=atan((float)ysoltada/(float)xsoltada)+3.1416;
				    }
			         }
			      }
			      break;
		           case 3:
			      smin=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;	
			      if (xsoltada==0){	
			         if (ysoltada<0){
				    hmax=3*3.1416/2.;
			         }else{
				    hmax=3.1416/2.;
			         }
			      }else{
			         if (ysoltada==0){
				    if (xsoltada<0){
				       hmax=3.1416;
				    }else{
				       hmax=0.;
				    }
			         }else{
				    if (xsoltada>0){
				       hmax=atan((float)ysoltada/(float)xsoltada);
				       if(ysoltada<0){
				          hmax=hmax+2*3.1416;
				       }
				    }else{
				       hmax=atan((float)ysoltada/(float)xsoltada)+3.1416;
				    }
			         }
			      }

			      break;
		           case 4:	
			      smin=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			      if (xsoltada==0){
			         if (ysoltada<0){
				    hmin=3*3.1416/2.;
			         }else{
				    hmin=3.1416/2.;
			         }
			      }else{
			         if (ysoltada==0){
				    if (xsoltada<0){
				       hmin=3.1416;
				    }else{
				       hmin=0.;
				    }
			         }else{
				    if (xsoltada>0){
				       hmin=atan((float)ysoltada/(float)xsoltada);
				       if(ysoltada<0){
				          hmin=hmin+2*3.1416;
				       }
				    }else{
				       hmin=atan((float)ysoltada/(float)xsoltada)+3.1416;
				    }
			         }
			      }
			      break;
		           default:
			      break;
		        }
		        
            sliderRMax->set_value(hmax);
            sliderRMin->set_value(hmin);
            sliderGMax->set_value(smax);
            sliderGMin->set_value(smin);    
            if(hmin > hmax){
                sliderRMax->set_value(hmin);
                sliderRMin->set_value(hmax);
            }
		     }
	          }
	          return true;

    }
    
    bool colorTuner::on_hsv_image_space_button_press_event (GdkEventButton *event)
    {
        int coordx = event->x;
	    int coordy = event->y;
	    
	    if (event->button == 3){
		    /*printf("Right mouse button clicked\n");*/
	          x_pulsada=coordx;y_pulsada=coordy;
	          x_pulsada=x_pulsada-centro_x;
	          y_pulsada=centro_y-y_pulsada;

	          if (x_pulsada==0){	
		         if (y_pulsada<0){	
		            hmax=3.1416/2.;
		        }else{
		            hmax=3.*3.1416/2.;
		        }
	         }else{
		         if (y_pulsada==0){
		            if (x_pulsada<0){
		               hmax=3.1416;
		            }else{
		               hmax=0.;
		            }
		         }else{
		            if (x_pulsada>0){
		                hmax=atan((float)(y_pulsada)/(float)x_pulsada);
		                if(y_pulsada<0){
			                hmax=hmax+2*3.1416;
		                }
		            }else{
		                hmax=atan((float)y_pulsada/(float)x_pulsada)+3.1416;
		            }
		         }
	          }
	          hmax=hmax+.2;
	          hmin=hmax-.4;
	          if (hmax>(2.*3.1416)){
		        hmax=hmax-(2.*3.1416);
	          }
	          if (hmin<0){
		        hmin=hmin+(2.*3.1416);
	          }
	          smax=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
	          smax=smax+.1;
	          smin=smax-.2;
	          if (smax>1.){
		        smax=1.;
	          }
	          if (smin<0.){
		        smin=0.;
	          }
	          /*Change sliders values*/

            sliderRMax->set_value(hmax);
            sliderRMin->set_value(hmin);
            if(hmin > hmax){
                sliderRMax->set_value(hmin);
                sliderRMin->set_value(hmax);
            }
            sliderGMax->set_value(smax);
            sliderGMin->set_value(smin);
	    }
	       xquesito1=cos(hmax)*smax*radio_hsi_map+centro_x;
	       yquesito1=centro_y-sin(hmax)*smax*radio_hsi_map;
	
	       xquesito2=cos(hmin)*smax*radio_hsi_map+centro_x;
	       yquesito2=centro_y-sin(hmin)*smax*radio_hsi_map;
	
	       xquesito3=cos(hmax)*smin*radio_hsi_map+centro_x;
	       yquesito3=centro_y-sin(hmax)*smin*radio_hsi_map;
	
	       xquesito4=cos(hmin)*smin*radio_hsi_map+centro_x;
	       yquesito4=centro_y-sin(hmin)*smin*radio_hsi_map;
	         
	    if (event->button == 1){
	        if (((coordx)<=xquesito1+5) && ((coordx)>=xquesito1-5)){
		        if (((coordy)<=yquesito1+5) && ((coordy)>=yquesito1-5)){
		           x_pulsada=coordx;y_pulsada=coordy;
		           pulsada=1;
		        }   
	       }
	        if (((coordx)<=xquesito2+5) && ((coordx)>=xquesito2-5)){
		        if (((coordy)<=yquesito2+5) && ((coordy)>=yquesito2-5)){
		            x_pulsada=coordx;y_pulsada=coordy;
		            pulsada=2;
		        }
	        }
	        if (((coordx)<=xquesito3+5) && ((coordx)>=xquesito3-5)){
		        if (((coordy)<=yquesito3+5) && ((coordy)>=yquesito3-5)){
		            x_pulsada=coordx;y_pulsada=coordy;
		            pulsada=3;
		        }
	        }
	        if (((coordx)<=xquesito4+5) && ((coordx)>=xquesito4-5)){
		        if (((coordy)<=yquesito4+5) && ((coordy)>=yquesito4-5)){
		            x_pulsada=coordx;y_pulsada=coordy;
		            pulsada=4;
		        }
	        }
	        if (hmax==hmin){			
		        x_pulsada=coordx;
		        y_pulsada=coordy;
		        x_pulsada=x_pulsada-centro_x;
		        y_pulsada=centro_y-y_pulsada;
		        if ((float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map<=1){
		            smax=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
		            pulsada=0;
		            smin=0.;
		            
		            sliderGMax->set_value(smax);
                    sliderGMin->set_value(smin);
		         }
	         }
	    }
	    return true;
    }
    

    void colorTuner::draw_hsvmap(int size){
        
        hsv = cvCreateImage(cvSize(320, 320), 8, 3);
        
        int i,j,ind; 
        float x, y, H, S, scale;
        double r,g,b;
        unsigned char R,G,B;
        int grey = 175;

        for(j=0; j < size; j++){  
            for(i=0; i < size; i++){
                x = (2.0*i)/size - 1.0;
                y = 1.0 - (2.0*j)/size;
                H = atan2(y,x);
                if (H>=2*PI){
                    H-=2*PI;
                }
                if (H<0){
                    H+=2*PI;
                }
                H = RADTODEG*H;
                S = sqrt(y*y + x*x);

                if (S<1.)
                {
                    hsv2rgb(H,S,0.7,&r,&g,&b);
                    scale = 255.0;
                    R = (unsigned char) (scale * r);
                    G = (unsigned char) (scale * g);
                    B = (unsigned char) (scale * b);
                }else{
                    R=(unsigned char) grey;
                    G= (unsigned char) grey;
                    B= (unsigned char) grey;
                }
                
                ind = (size*j + i)*3;

                //printf("Valores R: %u G: %u B: %u ind: %d H: %2f S: %2f \n",R,G,B,ind,H,S);

                hsv->imageData[ind]   = R; /* Blue */
                hsv->imageData[ind+1] = G; /* Green */
                hsv->imageData[ind+2] = B; /* Red */
            }

        }
         	   
     	//cvShowImage("hsv",hsv);
        //cvReleaseImage(&hsv);
    }
    
        /*Draw hsv cheese functions*/
    int colorTuner::drawcircle(char *img, int xcentro, int ycentro, int radio, int thiscolor){

       int r,g,b;
       int x1,y1;
       float i;

       /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, they are not valid values for the pixels.  */

       if (thiscolor==BLACK) {r=0;g=0;b=0;}
       else if (thiscolor==RED) {r=255;g=0;b=0;} 
       else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
       else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
       else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
       else if (thiscolor==DEEPPINK) {r=213;g=85;b=178; }   
       else if (thiscolor==WHITE) {r=255;g=255;b=255;}
       else {r=0;g=0;b=0;}
       
       for (i=0.;i<=360;i=i+0.1){
          x1=cos(i*DEGTORAD)*radio+xcentro;
          y1=sin(i*DEGTORAD)*radio+ycentro;
          fflush (NULL);
          img[(y1*SMAX+x1)*3]=b;
          img[(y1*SMAX+x1)*3+1]=g;
          img[(y1*SMAX+x1)*3+2]=r;
       }
       return 0; 
    }

    int colorTuner::lineinimage(char *img, int xa, int ya, int xb, int yb, int thiscolor){
       float L;
       int i,imax,r,g,b;
       int lastx,lasty,thisx,thisy,lastcount;
       int threshold_line=1;
       int Xmax,Xmin,Ymax,Ymin;

       if (xa > SMAX-1){
	    xa = SMAX-1;
       }
       if (xb > SMAX-1){
	    xb = SMAX-1;
       }
       if (ya > SMAX-1){
	    ya = SMAX-1;
       }
       if (yb > SMAX-1){
	    yb = SMAX-1;
       }

       Xmin=0; Xmax=SMAX-1; Ymin=0; Ymax=SMAX-1;
       /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */

       if (thiscolor==BLACK) {r=0;g=0;b=0;}
       else if (thiscolor==RED) {r=255;g=0;b=0;} 
       else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
       else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
       else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
       else if (thiscolor==DEEPPINK) {r=213;g=85;b=178;}
       else if (thiscolor==WHITE) {r=255;g=255;b=255;}   
       else {r=0;g=0;b=0;}

       /* first, check both points are inside the limits and draw them */
       if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1) &&
	    (xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)) 
       {
          /* draw both points */
		     
          img[(SMAX*ya+xa)*3+0]=b;
          img[(SMAX*ya+xa)*3+1]=g;
          img[(SMAX*ya+xa)*3+2]=r;

          img[(SMAX*yb+xb)*3+0]=b;
          img[(SMAX*yb+xb)*3+1]=g;
          img[(SMAX*yb+xb)*3+2]=r;
		     
          L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
          imax=3*(int)L+1;
          /* if (debug==1) printf("xa=%d ya=%d xb=%d yb=%d L=%.f imax=%d\n",xa,ya,xb,yb,L,imax);  */
          lastx=xa; lasty=xb; lastcount=0;
          for(i=0;i<=imax;i++)
          {
	     thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
	     thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));
	     if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
	     else 
	     { 
	        if (lastcount>=threshold_line)
	        { /* draw that point in the image */
	           img[(SMAX*lasty+lastx)*3+0]=b;
	           img[(SMAX*lasty+lastx)*3+1]=g;
	           img[(SMAX*lasty+lastx)*3+2]=r;
	        }
	        lasty=thisy; 
	        lastx=thisx; 
	        lastcount=0;
	     }
          }
          return 0; 
       }
       else return -1;
    }

    int colorTuner::drawarc(char *img, int xcentro, int ycentro, int radio, int x1, int y1, int x2, int y2, int thiscolor)
    {
	
       int r,g,b;
       int x,y;
       float i,imax;

       /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */
       if ((x1==x2)&&(y1==y2)){
          drawcircle(img, xcentro, ycentro, radio, thiscolor);
       }else{
          if (thiscolor==BLACK) {r=0;g=0;b=0;}
          else if (thiscolor==RED) {r=255;g=0;b=0;} 
          else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
          else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
          else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
          else if (thiscolor==DEEPPINK) {r=213;g=85;b=178; }   
          else if (thiscolor==WHITE) {r=255;g=255;b=255;}
          else {r=0;g=0;b=0;}
          x1=x1-xcentro;
          y1=ycentro-y1;
          x2=x2-xcentro;
          y2=ycentro-y2;
	       
          if (x1==0){	
	     if (y1<0){							
	        i=3*3.1416/2.;
	     }else{
	        i=3.1416/2.;
	     }
          }else{
	     if (y1==0){
	        if (x1<0){
	           i=3.1416;
	        }else{
	           i=0.;
	        }
	     }else{
	        if (x1>0){
	           i=atan((float)y1/(float)x1);
	        }else{
	           i=atan((float)y1/(float)x1)+3.1416;
	        }
	     }
          }

          i=i*RADTODEG;
	       
          if (x2==0){	
	     if (y2<0){							
	        imax=3*3.1416/2.;
	     }else{
	        imax=3.1416/2.;
	     }
          }else{
	     if (y2==0){
	        if (x2<0){
	           imax=3.1416;
	        }else{
	           imax=0.;
	        }
	     }else{
	        if (x2>0){
	           imax=atan((float)y2/(float)x2);
	        }else{
	           imax=atan((float)y2/(float)x2)+3.1416;
	        }
	     }
          }
          imax=imax*RADTODEG;
          if (imax<i){
	     imax=imax+360;
          }
          for (;i<=imax;i=i+0.1){
	     x=(cos(i*DEGTORAD)*radio+xcentro);
	     y=(ycentro-sin(i*DEGTORAD)*radio);

	     img[(y*SMAX+x)*3]=b;
	     img[(y*SMAX+x)*3+1]=g;
	     img[(y*SMAX+x)*3+2]=r;
          }
       }
       return 0; 
    }


    void colorTuner::drawcheese (char *img,int x_centro,int y_centro, double h_max, double h_min, double s_max, double s_min, int thiscolor)
    {
    
       int x1,y1,x2,y2;
       s_max=s_max*radio_hsi_map;	
       s_min=s_min*radio_hsi_map;

       x1=(cos(h_max)*s_min+x_centro);
       y1=(y_centro-sin(h_max)*s_min);
       x2=(cos(h_max)*s_max+x_centro);
       y2=(y_centro-sin(h_max)*s_max);

       lineinimage(img,x1,y1,x2,y2,thiscolor);

       x1=(cos(h_min)*s_min+x_centro);
       y1=(y_centro-sin(h_min)*s_min);
       x2=(cos(h_min)*s_max+x_centro);
       y2=(y_centro-sin(h_min)*s_max);

       lineinimage(img,x1,y1,x2,y2,thiscolor);

       x1=(cos(h_min)*s_max+x_centro);
       y1=(y_centro-sin(h_min)*s_max);
       x2=(cos(h_max)*s_max+x_centro);
       y2=(y_centro-sin(h_max)*s_max);

       drawarc(img,x_centro,y_centro,s_max,x1,y1,x2,y2,thiscolor);	

       x1=(cos(h_min)*s_min+x_centro);
       y1=(y_centro-sin(h_min)*s_min);
       x2=(cos(h_max)*s_min+x_centro);
       y2=(y_centro-sin(h_max)*s_min);

       drawarc(img,x_centro,y_centro,s_min,x1,y1,x2,y2,thiscolor);
    }
    
        //////////////////////////////////////////////////////  END HSV ///////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////YUV ///////////////////////////////////////////////////////
    void colorTuner::draw_yuvmap( int size){
    
        yuv = cvCreateImage(cvSize(320, 320), 8, 3);
        float ymax = 1.0;
        float ymin = 0.0;
       int i,j,ind,ysize; 
       float scale;
       double r,g,b,U,V;
       unsigned char R,G,B;

       ysize = (size * 0.436*2)/(0.615*2);

       for(j=0; j < size; j++){
          for(i=0; i < size; i++){

	     U = 0.436 - (((size -i)*0.436*2.0)/size);
	     V = (((size-j)*0.615*2.0)/size) - 0.615;

	     ind = (size*j + i)*3;

	     yuv2rgb((ymax+ymin)/2,U,V,&r,&g,&b);

	     scale = 255.0;

	     R = (unsigned char) (unsigned int) (float) (scale * r);
	     G = (unsigned char) (unsigned int) (float) (scale * g);
	     B = (unsigned char) (unsigned int) (float) (scale * b);
	     
	     //printf("Valores R: %u G: %u B: %u\n",R,G,B);
                yuv->imageData[ind]   = R; /* Blue */
                yuv->imageData[ind+1] = G; /* Green */
                yuv->imageData[ind+2] = B; /* Red */
            }

        }
        //cvShowImage("yuv",hsv);
        //cvReleaseImage(&yuv);
    }
    
    
    void colorTuner::drawsquare (char *img,double u_max, double u_min, double v_max, double v_min, int thiscolor)
    {
       int x1,y1,x2,y2;

       u_max =  u_max + 0.436;
       u_min =  u_min + 0.436;
       v_max = 0.615 - v_max;
       v_min = 0.615 - v_min;

       x1=(u_min*SMAX)/(0.436*2);
       y1=(v_max*SMAX)/(0.615*2);
       x2=(u_max*SMAX)/(0.436*2);
       y2=(v_max*SMAX)/(0.615*2);

       lineinimage(img,x1,y1,x2,y2,thiscolor);

       x1=(u_max*SMAX)/(0.436*2);
       y1=(v_max*SMAX)/(0.615*2);
       x2=(u_max*SMAX)/(0.436*2);
       y2=(v_min*SMAX)/(0.615*2);

       lineinimage(img,x1,y1,x2,y2,thiscolor);

       x1=(u_max*SMAX)/(0.436*2);
       y1=(v_min*SMAX)/(0.615*2);
       x2=(u_min*SMAX)/(0.436*2);
       y2=(v_min*SMAX)/(0.615*2);

       lineinimage(img,x1,y1,x2,y2,thiscolor);

       x1=(u_min*SMAX)/(0.436*2);
       y1=(v_min*SMAX)/(0.615*2);
       x2=(u_min*SMAX)/(0.436*2);
       y2=(v_max*SMAX)/(0.615*2);

       lineinimage(img,x1,y1,x2,y2,thiscolor);
    }
    
    bool colorTuner::on_yuv_image_space_button_press_event (GdkEventButton *event)
{
	int coordx = event->x;
	int coordy = event->y ;
	float u1,u2,v1,v2;
	float U,V;


	if (event->button == 3){
		/*printf("Right mouse button clicked\n");*/
	        x_pulsada=coordx;
	        y_pulsada=coordy;

	        x_pulsada = SMAX - x_pulsada;
		y_pulsada = SMAX - y_pulsada; 

	      	U =  0.436 - ((x_pulsada*0.436*2.0)/SMAX);
		V = ((y_pulsada*0.615*2.0)/SMAX) - 0.615;

	        umax=U+0.1;
		umin=U-0.1;
		 if (umax>0.436){
		    umax=0.436;
		 }
		 if (umin<-0.436){
		    umin=-0.436;
		 }

		 vmax2=V+0.1;
		 vmin2=V-0.1;
		 if (vmax2>0.615){
		    vmax2=0.615;
		 }
		 if (vmin2<-0.615){
		    vmin2=-0.615;
		 }
	      	/*Change sliders values*/
	      	
            sliderGMax->set_value(umax);
            sliderGMin->set_value(umin);
            sliderBMax->set_value(vmax2);
            sliderBMin->set_value(vmin2);   
	}

	if ((coordx < SMAX)&&(coordy <SMAX)){
		
		u1 =  umax + 0.436;	
		u2 =  umin + 0.436;
		v1 = 0.615 - vmax2;
		v2 = 0.615 - vmin2;
		
		xsquare1=(u2*SMAX)/(0.436*2);
		ysquare1=(v1*SMAX)/(0.615*2);
		xsquare2=(u1*SMAX)/(0.436*2);
		ysquare2=(v1*SMAX)/(0.615*2);
		xsquare3=(u1*SMAX)/(0.436*2);
		ysquare3=(v2*SMAX)/(0.615*2);
		xsquare4=(u2*SMAX)/(0.436*2);
		ysquare4=(v2*SMAX)/(0.615*2);
		 
		if (event->button == 1){    
		      if (((coordx)<=xsquare1+5) && ((coordx)>=xsquare1-5)){
			 if (((coordy)<=ysquare1+5) && ((coordy)>=ysquare1-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=1;
			 }
		      }
		      if (((coordx)<=xsquare2+5) && ((coordx)>=xsquare2-5)){
			 if (((coordy)<=ysquare2+5) && ((coordy)>=ysquare2-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=2;
			 }
		      }
		      if (((coordx)<=xsquare3+5) && ((coordx)>=xsquare3-5)){
			 if (((coordy)<=ysquare3+5) && ((coordy)>=ysquare3-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=3;
			 }
		      }
		      if (((coordx)<=xsquare4+5) && ((coordx)>=xsquare4-5)){
			 if (((coordy)<=ysquare4+5) && ((coordy)>=ysquare4-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=4;
			 }
		      }
		}
	}
	return true;
}

bool colorTuner::on_yuv_image_space_motion_notify_event (GdkEventMotion *event)
{
	int coordx = event->x;
	int coordy = event->y;


	if ((pulsada>0)&&(coordx < SMAX)&&(coordy <SMAX)){
	
		xsoltada=coordx;
		ysoltada=coordy;

		xsoltada = SMAX - xsoltada;
		ysoltada = SMAX - ysoltada; 
		switch(pulsada){
		       case 1:		
			  umin=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmax2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 2:
			  umax=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmax2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 3:
			  umax=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmin2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 4:
			  umin=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmin2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       default:
			  break;
		    }
		    
            sliderGMax->set_value(umax);
            sliderGMin->set_value(umin);
            sliderBMax->set_value(vmax2);
            sliderBMin->set_value(vmin2);   
	      }
	      return true;
}

bool colorTuner::on_yuv_image_space_button_release_event (GdkEventButton *event)
{
	/*Stop drawing square*/
	pulsada=0;
	return true;
}
///////////////////////////////////////////////////////// END YUV //////////////////////////////////////
    
    void colorTuner::on_active_original_toggled()
    {
        if(radio_original)
            radio_original = 0;
        else 
            radio_original = 1;
    }
    
    void colorTuner::on_active_RGB_toggled()
    {
        if(radio_RGB)
            radio_RGB = 0;
        else
            radio_RGB = 1;
    }
    
    void colorTuner::on_active_HSV_toggled()
    {
        if(radio_HSV)
            radio_HSV = 0;
        else
            radio_HSV = 1;
    }
    
    void colorTuner::on_active_YUV_toggled()
    {
        if(radio_YUV)
            radio_YUV = 0;
        else
            radio_YUV = 1;
    }

   
    bool colorTuner::on_eventbox1_button_press_event(GdkEventButton* event)
    {
    
        int posX;
        int posY;
        double r, g, b;
        double h, s, v;
        double Y,U,V;
        int indice;
        
        posX = (int) event->x;
        posY = (int) event->y;
                       
        pthread_mutex_lock(&mutex);
        
        indice = posY*imageDemo->widthStep+posX*imageDemo->nChannels;
        
        r = (float)(unsigned int) (unsigned char)imageDemo->imageData[indice];
        g = (float)(unsigned int) (unsigned char)imageDemo->imageData[indice+1];
        b = (float)(unsigned int) (unsigned char)imageDemo->imageData[indice+2];   
        
        
        h = getH(r, g, b);
        s = getS(r, g, b);
        v = getV(r, g, b);
              
        pthread_mutex_unlock(&mutex);
        
        double rmax,rmin,gmax, gmin, bmax, bmin;

        std::cout << event->x << "," <<  event->y << " " << r << " " << g << " " << b << "\n";
        
        if(radio_RGB){
            rmax = r + 20.0;
            rmin = r - 20.0;
            if(rmax>255.0)
                rmax = 255.0;
            if(rmin<0.0)
                rmin = 0.0;
                
            gmax = g + 20.0;
            gmin = g - 20.0;
            if(gmax > 255.0)
                gmax = 255.0;
            if(gmin < 0.0)
                gmin = 0.0;
                
            bmax = b + 20.0;
            bmin = b - 20.0;
            if(bmax > 255.0)
                bmax = 255.0;
            if(bmin < 0.0)
                bmin = 0.0;           
                
            sliderRMax->set_value(rmax);
            sliderRMin->set_value(rmin);
            sliderGMax->set_value(gmax);
            sliderGMin->set_value(gmin);
            sliderBMax->set_value(bmax);
            sliderBMin->set_value(bmin);
            
		    if(rmin > rmax) {
			    rmax = rmin;
			    sliderRMax->set_value(rmax);
		    } 

		    if(gmin > gmax) {
			    gmax = gmin;
			    sliderGMax->set_value(gmax);
		    } 

		    if(bmin > bmax) {
			    bmax = bmin;
			    sliderBMax->set_value(bmax);
		    }
	    }
	    if(radio_HSV){
            rmax = h*DEGTORAD + 0.2;
            rmin = h*DEGTORAD - 0.2;
            if(rmax>6.28)
                rmax = 6.28;
            if(rmin<0.0)
                rmin = 0.0;
                
            gmax = s + 0.1;
            gmin = s - 0.1;
            if(gmax > 1.0)
                gmax = 1.0;
            if(gmin < 0.0)
                gmin = 0.0;
                
            bmax = v + 50.0;
            bmin = v - 50.0;
            if(bmax > 255.0)
                bmax = 255.0;
            if(bmin < 0.0)
                bmin = 0.0;           
                
            sliderRMax->set_value(rmax);
            sliderRMin->set_value(rmin);
            sliderGMax->set_value(gmax);
            sliderGMin->set_value(gmin);
            sliderBMax->set_value(bmax);
            sliderBMin->set_value(bmin);
            
            hmax = rmax;
            hmin = rmin;
            smax = gmax;
            smin = smin;
            
		    if(rmin > rmax) {
		        std::cout<< " DENTRO!!!!: "<<rmin << "\n";
			    sliderRMin->set_value(rmax);
			    sliderRMax->set_value(rmin);
		    } 

		    if(gmin > gmax) {
			    gmax = gmin;
			    sliderGMax->set_value(gmax);
		    } 

		    if(bmin > bmax) {
			    bmax = bmin;
			    sliderBMax->set_value(bmax);
		    }
	    }
	    if(radio_YUV){

            Y = 0.299*r/255.0  +0.578*g/255.0    +0.114*b/255.0 ;
            U =-0.147*r/255.0  -0.289*g/255.0    +0.436*b/255.0 ;
            V = 0.615*r/255.0  -0.515*g/255.0    -0.100*b/255.0 ;

		     rmax=Y+.2;
		     rmin=Y-.2;
		     if (rmax>1){
		        rmax=1.0;
		     }
		     if (rmin<0){
		        rmin=0.0;
		     }
		     gmax=U+0.1;
		     gmin=U-0.1;
		     if (gmax>0.436){
		        gmax=0.436;
		     }
		     if (gmin<-0.436){
		        gmin=-0.436;
		     }
		     bmax=V+0.1;
		     bmin=V-0.1;
		     if (bmax>0.615){
		        bmax=0.615;
		     }
		     if (bmin<-0.615){
		        bmin=-0.615;
		     }

            sliderRMax->set_value(rmax);
            sliderRMin->set_value(rmin);
            sliderGMax->set_value(gmax);
            sliderGMin->set_value(gmin);
            sliderBMax->set_value(bmax);
            sliderBMin->set_value(bmin);

		    if(rmin > rmax) {
			    rmax = rmin;
                sliderRMax->set_value(rmax);
		    }

		    if(gmin > gmax) {
			    gmax = gmin;
                sliderGMax->set_value(gmax);
		    } 

		    if(bmin > bmax) {
			    bmax = bmin;
                sliderBMax->set_value(bmax);
		    }
	    }
        
        std::cout << rmax << " " << rmin << " "<< gmax << " " << gmin << " " << bmax << " " << gmin << " " << "\n";
        
        return true;
    }
    
    void colorTuner::filter_RGB(IplImage* cvResultado)
    {
     //       Para color
        for(int x = 0; x< imageDemo->width; x++){
            for(int y = 0; y < imageDemo->height; y++ ){
                int indice = y*imageDemo->widthStep+x*imageDemo->nChannels;
                
                int red   = abs(( int)imageDemo->imageData[indice]) ;
                int blue  = abs(( int)imageDemo->imageData[indice+1]);
                int green = abs(( int)imageDemo->imageData[indice+2]);
                
                bool condR = ( red <= sliderRMax->get_value()) & ( red  >= sliderRMin->get_value());
                bool condG = (blue <= sliderGMax->get_value()) & (blue  >= sliderGMin->get_value());
                bool condB = (green<= sliderBMax->get_value()) & (green >= sliderBMin->get_value());
                
                if(condR & condG & condB){
                    cvResultado->imageData[indice]   = imageDemo->imageData[indice];
                    cvResultado->imageData[indice+1] = imageDemo->imageData[indice+1];
                    cvResultado->imageData[indice+2] = imageDemo->imageData[indice+2];
                }else{
                    cvResultado->imageData[indice]   = 0;
                    cvResultado->imageData[indice+1] = 0;
                    cvResultado->imageData[indice+2] = 0;
                }
            }
        } 
    }
    double colorTuner::getH(double r, double g, double b)
    {
        double max = 0.0;
        double min = 255.0;

        if(r >= g && r >= b)
            max =  r;
        if( g >= r && g >= b )    
            max =  g;
        if(b >= r && b >= g)
            max = b;
            
        if(r <= g && r <= b)
            min =  r;
        if( g <= r && g <= b )    
            min =  g;
        if(b <= r && b <= g)
            min = b;
            
        if(max == min)
            return 0;
            
        if(max == r){
            if(g>=b){
                return (60.0*(g-b)/(max-min));
            }else{
                return ((60.0*(g-b)/(max-min))+360.0);
            }
        }
        if(max == g){
            return ((60.0*(b-r)/(max-min))+120.0);
        }
        if(max == b ){
            return ((60.0*(r-g)/(max-min))+240.0);
        }    
        
        return 0;
    }
    double colorTuner::getS(double r, double g, double b)
    {
        double max = 0.0;
        double min = 255.0;

        if(r >= g && r >= b)
            max =  r;
        if( g >= r && g >= b )    
            max =  g;
        if(b >= r && b >= g)
            max = b;
        if(max == 0.0)
            return 0.0;    
        if(r <= g && r <= b)
            min =  r;
        if( g <= r && g <= b )    
            min =  g;
        if(b <= r && b <= g)
            min = b;

        return (1.0 - (min/max)) ;
    }
    double colorTuner::getV(double r, double g, double b)
    {
        if(r >= g && r >= b)
            return  r;
        if( g >= r && g >= b )    
            return  g;
        if(b >= r && b >= g)
            return b;
            
        return 0;
    }

    void colorTuner::filter_YUV(IplImage* cvResultado)
    {
        cvCopy(imageDemo, cvResultado);
        double r,g,b;
	    int i;
	    double y,u,v;

	    for (i=0;i< cvResultado->width*cvResultado->height; i++){
			    r = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3];
			    g = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+1];
			    b = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+2];
			
                y = 0.299*r/255.0  +0.578*g/255.0    +0.114*b/255.0 ;
                u =-0.147*r/255.0  -0.289*g/255.0    +0.436*b/255.0 ;
                v = 0.615*r/255.0  -0.515*g/255.0    -0.100*b/255.0 ;
			    if( sliderRMax->get_value()>=y    && sliderRMin->get_value()<=y 
			        && sliderGMax->get_value()>=u && sliderGMin->get_value()<=u 
			        && sliderBMax->get_value()>=v && sliderBMin->get_value()<=v ){
			    }  else {
	        		/* Gray Scale */
				    cvResultado->imageData[i*3]   = 0;//(unsigned char) (v*100/255);
				    cvResultado->imageData[i*3+1] = 0;//(unsigned char) (v*100/255);
				    cvResultado->imageData[i*3+2] = 0;//(unsigned char) (v*100/255);
			    }
	    }
    }


    void colorTuner::filter_HSV(IplImage* cvResultado)
    {
        cvCopy(imageDemo, cvResultado);
        
        double r,g,b;
	    int i;
	    double h,s,v;

	    for (i=0;i< cvResultado->width*cvResultado->height; i++){
			    r = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3];
			    g = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+1];
			    b = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+2];
			
                h = getH(r, g, b);
                s = getS(r, g, b);
                v = getV(r, g, b);
             
			    if( sliderRMax->get_value()>=h*DEGTORAD && sliderRMin->get_value()<=h*DEGTORAD 
			        && sliderGMax->get_value()>=s && sliderGMin->get_value()<=s 
			        && sliderBMax->get_value()>=v && sliderBMin->get_value()<=v ){
				    //hsv->imageData[i*3]   = hsv->imageData[i*3];
				    //hsv->imageData[i*3+1] = hsv->imageData[i*3+1];
				    //hsv->imageData[i*3+2] = hsv->imageData[i*3+2];
			    }  else {
	        		/* Gray Scale */
				    cvResultado->imageData[i*3]   = 0;//(unsigned char) (v*100/255);
				    cvResultado->imageData[i*3+1] = 0;//(unsigned char) (v*100/255);
				    cvResultado->imageData[i*3+2] = 0;//(unsigned char) (v*100/255);
			    }
	    }
        
    }
    
    bool colorTuner::isVisible()
    {
	    return mainwindow->is_visible();
    }

    void colorTuner::display( IplImage* image  )
    {
    
        IplImage* cvResultado;
        
        cvResultado = cvCreateImage(cvSize(image->width,image->height), 8 ,3);
    
        imageDemo =  cvCreateImage(cvSize(image->width,image->height), 8 ,3);
        cvCopy(image, imageDemo);
        pthread_mutex_unlock(&mutex);
        
        if(radio_original){
            cvCopy(image, cvResultado);
            cajaSpacesHSV->set_child_visible(false);
            cajaSpacesYUV->set_child_visible(false);
        }
        if(radio_RGB){
            cajaSpacesHSV->set_child_visible(false);
            cajaSpacesYUV->set_child_visible(false);
            filter_RGB(cvResultado);
            labelRMax->set_text("RMax");
            labelRMin->set_text("RMin");
            labelGMax->set_text("GMax");
            labelGMin->set_text("GMin");
            labelBMax->set_text("BMax");
            labelBMin->set_text("BMin");
            sliderRMax->set_range(0,255);
            sliderRMin->set_range(0,255);
            sliderGMax->set_range(0,255);
            sliderGMin->set_range(0,255);
            sliderBMax->set_range(0,255);
            sliderBMin->set_range(0,255);
        }
        if(radio_HSV){
            cajaSpacesHSV->set_child_visible(true);
            cajaSpacesYUV->set_child_visible(false);
            labelRMax->set_text("HMax");
            labelRMin->set_text("HMin");
            labelGMax->set_text("SMax");
            labelGMin->set_text("SMin");
            labelBMax->set_text("VMax");
            labelBMin->set_text("VMin");
            sliderRMax->set_range(0,6.29);
            sliderRMin->set_range(0,6.29);
            sliderGMax->set_range(0,1.1);
            sliderGMin->set_range(0,1);
            sliderBMax->set_range(0,255.1);
            sliderBMin->set_range(0,255.1);
            filter_HSV(cvResultado);
            
            IplImage* hsvAux = cvCreateImage(cvSize(hsv->width, hsv->height), 8, 3);
            cvCopy(hsv, hsvAux);
            drawcheese(hsvAux->imageData ,centro_x,centro_y,
	                    (double)sliderRMax->get_value(),(double)sliderRMin->get_value(),
	                    (double)sliderGMax->get_value(),(double)sliderGMin->get_value(),
	                    1);

            
            Glib::RefPtr<Gdk::Pixbuf> imgBuffHSV = 
            Gdk::Pixbuf:: create_from_data((const guint8*)hsvAux->imageData,
	                                Gdk::COLORSPACE_RGB,
	                                false,
	                                hsvAux->depth,
	                                hsvAux->width,
	                                hsvAux->height,
	                                hsvAux->widthStep);

            gtkimageColorSpaceHSV->clear();
            gtkimageColorSpaceHSV->set(imgBuffHSV);
            
            cvReleaseImage(&hsvAux);                 
        }
        if(radio_YUV){
        
            cajaSpacesHSV->set_child_visible(false);
            cajaSpacesYUV->set_child_visible(true);
            labelRMax->set_text("YMax");
            labelRMin->set_text("YMin");
            labelGMax->set_text("UMax");
            labelGMin->set_text("UMin");
            labelBMax->set_text("VMax");
            labelBMin->set_text("VMin");
            sliderRMax->set_range(0, 1);
            sliderRMin->set_range(0, 1);
            sliderGMax->set_range(-0.436, 0.436);
            sliderGMin->set_range(-0.436, 0.436);
            sliderBMax->set_range(-0.615, 0.615);
            sliderBMin->set_range(-0.615, 0.615); 
            filter_YUV(cvResultado);
            
            IplImage* yuvAux = cvCreateImage(cvSize(yuv->width, yuv->height), 8, 3);
            cvCopy(yuv, yuvAux);
            drawsquare(yuvAux->imageData,
						(double)sliderGMax->get_value(),(double)sliderGMin->get_value(),
						(double)sliderBMax->get_value(),(double)sliderBMin->get_value(),
						1);
            
            Glib::RefPtr<Gdk::Pixbuf> imgBuffYUV = 
            Gdk::Pixbuf:: create_from_data((const guint8*)yuvAux->imageData,
	                                Gdk::COLORSPACE_RGB,
	                                false,
	                                yuvAux->depth,
	                                yuvAux->width,
	                                yuvAux->height,
	                                yuvAux->widthStep);

            gtkimageColorSpaceYUV->clear();
            gtkimageColorSpaceYUV->set(imgBuffYUV);
            
            cvReleaseImage(&yuvAux);   
              
        }
        
        Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
				        Gdk::Pixbuf:: create_from_data((const guint8*)cvResultado->imageData,
										        Gdk::COLORSPACE_RGB,
										        false,
										        cvResultado->depth,
										        cvResultado->width,
										        cvResultado->height,
										        cvResultado->widthStep);
										        
        Glib::RefPtr<Gdk::Pixbuf> imgBuffOriginal = 
                        Gdk::Pixbuf:: create_from_data((const guint8*)image->imageData,
						                        Gdk::COLORSPACE_RGB,
						                        false,
						                        image->depth,
						                        image->width,
						                        image->height,
						                        image->widthStep);

    	displayFrameRate();
        mainwindow->resize(1,1);
        while (gtkmain.events_pending())
          gtkmain.iteration();
        gtkimage->clear();
        gtkimage->set(imgBuff);
        
        gtkimageOriginal->clear();
        gtkimageOriginal->set(imgBuffOriginal);
        
             
        
        pthread_mutex_lock(&mutex);
        cvReleaseImage(&cvResultado);
        cvReleaseImage(&imageDemo);

    }
    
    void colorTuner::displayFrameRate()
    {
        double diff;
        IceUtil::Time diffT;

        currentFrameTime = IceUtil::Time::now();
        diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
        if (diff < 1000.0)
          frameCount++;
        else{
          oldFrameTime = currentFrameTime;
          fps = frameCount*1000.0/diff;
          frameCount=0;
          // Display the frame rate
          std::stringstream fpsString;
          fpsString << "fps = " << int(fps);
          fpslabel->set_label(fpsString.str());
        }
    }

}//namespace
