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
 *			  Alejandro Hernández Cordero <ahcorde@gmail.com> 
 *
 */

#include "viewer.h"  
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <cv.h>
#include <glade/glade.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
/*#include <highgui.h>*/

using namespace cv;

#define SQUARE(a) (a)*(a)
#define PI 3.141592654
namespace opencvdemo{

  const std::string gladepath = std::string("./opencvdemo.glade");

	int opflow_first=1;
	Mat previous;

  Viewer::Viewer(): gtkmain(0,0) {

    canny_box=0;
    sobel_box=0;
    laplace_box=0;
    harris_box=0;
    hough_box=0;
    def_box=1;		
    gray_box=0;		
    flow_box=0;
    color_box=0;
    conv_box=0;
    pyramid_box=0;
    houghcircles_box=0;

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);

		// Loading GUI components
    refXml->get_widget("imageI", gtkimage);
    refXml->get_widget("imageO", gtkimage2);
    refXml->get_widget("mainwindow",mainwindow);
    refXml->get_widget("scale_sobel",scale_sobel);
    refXml->get_widget("scale_canny",scale_canny);
    refXml->get_widget("hough_combobox",hough_combobox);
    refXml->get_widget("conv_combobox",conv_combobox);
    refXml->get_widget("label16",label_long);
    refXml->get_widget("label17",label_gap);
    refXml->get_widget("hough_threshold",hough_threshold);
    refXml->get_widget("hough_long",hough_long);
    refXml->get_widget("hough_gap",hough_gap);
    refXml->get_widget("Hmax",Hmax);
    refXml->get_widget("Hmin",Hmin);
    refXml->get_widget("Smax",Smax);
    refXml->get_widget("Smin",Smin);
    refXml->get_widget("Vmax",Vmax);
    refXml->get_widget("Vmin",Vmin);
    refXml->get_widget("button_harris",button_harris);
    refXml->get_widget("button_hough",button_hough);
    refXml->get_widget("button_laplace",button_laplace);
    refXml->get_widget("button_sobel",button_sobel);
    refXml->get_widget("button_canny",button_canny);
    refXml->get_widget("button_default",button_default);
    refXml->get_widget("button_gray",button_gray);
    refXml->get_widget("button_flow",button_flow);
    refXml->get_widget("button_color",button_color);
    refXml->get_widget("button_conv",button_conv);
    refXml->get_widget("button_pyramid",button_pyramid);
    refXml->get_widget("button_houghcircles",button_houghcircles);
    refXml->get_widget("eventbox1", eventbox);

		// Callbacks
    button_canny->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_canny_clicked));
    button_sobel->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_sobel_clicked));		
    button_laplace->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_laplace_clicked));	
    button_hough->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_hough_clicked));		
    button_harris->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_harris_clicked));		
    button_default->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_default_clicked));		
    button_gray->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_gray_clicked));		
    button_flow->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_flow_clicked));	
    button_color->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_color_clicked));	
    button_conv->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_conv_clicked));	
    button_pyramid->signal_clicked().connect(sigc::mem_fun(this,&Viewer::button_pyramid_clicked));	
    button_houghcircles->signal_toggled().connect(sigc::mem_fun(this,&Viewer::button_hough_circles_clicked));	
    
    eventbox->signal_button_press_event().connect(sigc::mem_fun(this, &Viewer::on_clicked));
		
		// Init some of the components
    hough_long->hide();
    hough_gap->hide();
    label_long->hide();
    label_gap->hide();
    hough_combobox->set_active(0); 
    conv_combobox->set_active(0); 
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
  }
    

  Viewer::~Viewer() {}

    bool Viewer::isVisible(){
        return mainwindow->is_visible();
    }
    
    bool Viewer::on_clicked(GdkEventButton * event)
    {
        //IplImage* hsvimage = cvCreateImage(cvGetSize(imagenO), 8, 1);
	Mat hsvimage(imagenO.size(), CV_8UC1);
        int posX;
        int posY;
        double r, g, b;
        double h, s, v;
        int indice;

        posX = (int) event->x;
        posY = (int) event->y;
                       
        pthread_mutex_lock(&mutex);

        //indice = posY*imagenO->widthStep+posX*imagenO->nChannels;
	indice = posY*imagenO.step+posX*imagenO.channels();

        //cvCopy(imagenO, hsvimage);

	imagenO.copyTo(hsvimage);
        r = (float)(unsigned int) (unsigned char)hsvimage.data[indice];
        g = (float)(unsigned int) (unsigned char)hsvimage.data[indice+1];
        b = (float)(unsigned int) (unsigned char)hsvimage.data[indice+2]; 
        pthread_mutex_unlock(&mutex); 
        //cvReleaseImage(&hsvimage);
	~hsvimage; 
        
                
        h = getH(r, g, b);
        s = getS(r, g, b);
        v = getV(r, g, b);
        
        double rmax,rmin,gmax, gmin, bmax, bmin;
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
            
        Hmax->set_value(rmax);
        Hmin->set_value(rmin);
        Smax->set_value(gmax);
        Smin->set_value(gmin);
        Vmax->set_value(bmax);
        Vmin->set_value(bmin);
        
        if(rmin > rmax) {
	        rmax = rmin;
	        Hmax->set_value(rmax);
        } 

        if(gmin > gmax) {
	        gmax = gmin;
            Smax->set_value(gmax);
        } 

        if(bmin > bmax) {
	        bmax = bmin;
            Vmax->set_value(bmax);
        }
        std::cout << event->x << " " << event->y << "\n";
        return true;
    }



//void Viewer::laplace( const colorspaces::Image& image ){

void Viewer::laplace( Mat image )
{
  
			int aperture=scale_sobel->get_value();
			if(aperture%2==0){aperture++;}
			//IplImage src=image;
			Mat src;
			image.copyTo(src); 
			//IplImage *gray;
			Mat gray(image.size(), CV_8UC1);
			//IplImage *dst;
			Mat dst(image.size(), CV_16SC1);
			//IplImage *gaux;
			Mat gaux(image.size(), CV_8UC1);

			//gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			//dst = cvCreateImage(cvSize(image.width,image.height), 16, 3);
			//gaux = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			std::cout << aperture << std::endl;
			//cvCvtColor(&src, gray, CV_RGB2GRAY);
			cvtColor(src, gray, CV_RGB2GRAY); 
			//cvLaplace(gray, dst, aperture);
			Laplacian(gray, dst, gray.depth(), aperture);
			//cvConvertScale(dst,gaux,1,0);
			dst.convertTo(gaux,-1,1,0);
			//cvCvtColor(gaux, &src, CV_GRAY2RGB);
			cvtColor(gaux,image,CV_GRAY2RGB);
			
			//cvReleaseImage(&gray);
			~gray;
			//cvReleaseImage(&dst);
			~dst;
			//cvReleaseImage(&gaux);
			~gaux;
			~src;

}


int Viewer::valuesOK(double H, double S, double V) 
	{

		if(!((S <= Smax->get_value()) && (S >=  Smin->get_value()) && (V <=  Vmax->get_value()) && (V >=  Vmin->get_value())))
			return 0;

		H = H*PI/180.0;

		if( Hmin->get_value() <  Hmax->get_value()) {
			if((H <= Hmax->get_value()) && (H >= Hmin->get_value()))
				return 1;
		} else {
			if(((H >= 0.0) && (H <= Hmax->get_value())) || ((H <= 2*PI) && (H >= Hmin->get_value()))) 
				return 1;
		}

		return 0;
	}
	
	 double Viewer::getH(double r, double g, double b)
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
    double Viewer::getS(double r, double g, double b)
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
    double Viewer::getV(double r, double g, double b)
    {
        if(r >= g && r >= b)
            return  r;
        if( g >= r && g >= b )    
            return  g;
        if(b >= r && b >= g)
            return b;
            
        return 0;
    }

//void Viewer::color( const colorspaces::Image& image )

void Viewer::color( Mat image )
{
        //IplImage src = image;
	Mat src;
	image.copyTo(src);
	    
        //IplImage* cvResultado = cvCreateImage(cvGetSize(&src), 8, 3);
	Mat cvResultado(src.size(), CV_8UC1);
        //cvCopy(&src, cvResultado);
	src.copyTo(cvResultado);
        
        double r,g,b;
	    int i;
	    double h,s,v;
	    Size size=cvResultado.size();

	    for (i=0;i< size.width*size.height; i++){
			    //r = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3];
			      r= (float)(unsigned int)(unsigned char) cvResultado.data[i*3];
			   // g = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+1];
			      g = (float)(unsigned int)(unsigned char) cvResultado.data[i*3+1];
			    //b = (float)(unsigned int)(unsigned char) cvResultado->imageData[i*3+2];
			    b = (float)(unsigned int)(unsigned char) cvResultado.data[i*3+2];
			
                h = getH(r, g, b);
                s = getS(r, g, b);
                v = getV(r, g, b);
             
			    if( Hmax->get_value()>=h*DEGTORAD && Hmin->get_value()<=h*DEGTORAD 
			        && Smax->get_value()>=s && Smin->get_value()<=s 
			        && Vmax->get_value()>=v && Vmin->get_value()<=v ){
				    //hsv->imageData[i*3]   = hsv->imageData[i*3];
				    //hsv->imageData[i*3+1] = hsv->imageData[i*3+1];
				    //hsv->imageData[i*3+2] = hsv->imageData[i*3+2];
			    }  else {
	        		/* Gray Scale */
				    //cvResultado->imageData[i*3]   = 0;//(unsigned char) (v*100/255);
					cvResultado.data[i*3] = 0;
				    //cvResultado->imageData[i*3+1] = 0;//(unsigned char) (v*100/255);
					cvResultado.data[i*3+1] = 0;
				    //cvResultado->imageData[i*3+2] = 0;//(unsigned char) (v*100/255);
				    cvResultado.data[i*3+2] = 0;//(unsigned char) (v*100/255);
			    }
	    }
		//cvCopy(cvResultado,&src);
		cvResultado.copyTo(image);
		//cvReleaseImage(&cvResultado);
		~cvResultado;
		~src;
}
  
//void Viewer::conv( const colorspaces::Image& image ){
void Viewer::conv( Mat image ){
  
			int sizekernel;
			int offset;
			int modulo;	
			float* kernel;

			int effect=conv_combobox->get_active_row_number();

			if (effect==0){//sharpenning

					sizekernel=3;
					offset=1;
					modulo=0;
					kernel=(float *)malloc(sizeof(float)*sizekernel*sizekernel);
					memset(kernel, 0, sizeof(float)*sizekernel*sizekernel);
					kernel=(float *)malloc(sizeof(float)*3*3);

					kernel[0]=0;kernel[1]=-1;kernel[2]=0;
					kernel[3]=-1;	kernel[4]=5;kernel[5]=-1;
					kernel[6]=0;kernel[7]=-1;	kernel[8]=0;						
			}else{
				if (effect==1){//Gaussian Blur
							sizekernel=3;
							offset=5;
							modulo=0;
							kernel=(float *)malloc(sizeof(float)*sizekernel*sizekernel);
							memset(kernel, 0, sizeof(float)*sizekernel*sizekernel);
							kernel=(float *)malloc(sizeof(float)*3*3);

							kernel[0]=0;kernel[1]=1;kernel[2]=0;	
							kernel[3]=1;kernel[4]=1;kernel[5]=1;		
							kernel[6]=0;kernel[7]=1;kernel[8]=0;		
	
					}else{
						if(effect==2){// Embossing
								sizekernel=3;
								offset=1;
								modulo=0;
								kernel=(float *)malloc(sizeof(float)*sizekernel*sizekernel);
								memset(kernel, 0, sizeof(float)*sizekernel*sizekernel);
								kernel=(float *)malloc(sizeof(float)*3*3);

								kernel[0]=-2;kernel[1]=-1;kernel[2]=0;
								kernel[3]=-1;kernel[4]=1;kernel[5]=1;
								kernel[6]=0;kernel[7]=1;kernel[8]=2;		
					/*	}else{
							if (effect==3){// Edge Detection
									sizekernel=3;
									offset=1;
									modulo=128;
									kernel=(float *)malloc(sizeof(float)*sizekernel*sizekernel);
									memset(kernel, 0, sizeof(float)*sizekernel*sizekernel);
									kernel=(float *)malloc(sizeof(float)*3*3);
									kernel[0]=0;kernel[1]=-1;kernel[2]=0;
									kernel[3]=-1;kernel[4]=4;kernel[5]=-1;
									kernel[6]=0;kernel[7]=-1;	kernel[8]=0;		
							}*/
						}
					}									
			}
			
		//CvMat *mask;
		//IplImage src=image;
		Mat src;
		image.copyTo(src);
		//IplImage *tmp, *dst;
		int i, h, w;
		int elems = sizekernel*sizekernel;

		//tmp = cvCreateImage(cvSize(image.width, image.height), 8, 1);
		Mat tmp (src.size(), CV_8UC3);
		//dst = cvCreateImage(cvSize(image.width, image.height), 8, 1);
		Mat dst (src.size(), CV_8UC1);
		//mask = cvCreateMat(sizekernel, sizekernel, CV_32FC1);
		Mat mask(sizekernel, sizekernel, CV_32FC1);
		/* Save mask in CvMat format*/
		for(i=0;i<elems;i++) {
			h = i/sizekernel;
			w = i%sizekernel;
			
			if(modulo > 1)
				//cvSet2D(mask,h, w, cvScalar(kernel[i]/modulo,0,0,0));
				mask.at<float>(h,w)=kernel[i]/modulo;

//Scalar(kernel[i]/modulo,0,0,0);
			else
				//cvSet2D(mask,h, w, cvScalar(kernel[i],0,0,0));
				mask.at<float>(h,w)= kernel[i];

//Scalar(kernel[i],0,0,0);
		}

		if (offset != 0) {
			//cvFilter2D(&src, tmp, mask, cvPoint(-1,-1));
			filter2D(src, tmp, -1, mask, Point(-1,-1));
			//cvAddS(tmp, cvScalarAll(offset), dst, 0);
			add(tmp, Scalar(offset, offset, offset, offset), dst);
			
		} else{
			//cvFilter2D(&src, dst, mask, cvPoint(-1,-1));
			filter2D(src, dst, -1, mask, Point(-1,-1));
		}

		//cvCopy(dst,&src);
		dst.copyTo(image);
		
		//cvReleaseImage(&dst);
		~dst;
		//cvReleaseImage(&tmp);
		~tmp;
		~src;
		~mask;
}


//void Viewer::pyramid( const colorspaces::Image& image ){
void Viewer::pyramid( Mat image ){

		//IplImage src=image;
		Mat src;
		image.copyTo(src);
		//IplImage *div2;
		//IplImage *div4;
		//IplImage *div8;
		//IplImage *div16;
		//IplImage *dst;
		int i,w,w2,tmp;
		Size imasize;

		imasize=image.size();

		//div2 = cvCreateImage(cvSize(image.width/2,image.height/2), 8, 3);
		Mat div2(Size(imasize.width/2, imasize.height/2), CV_8UC3); 
		//div4 = cvCreateImage(cvSize(image.width/4,image.height/4), 8, 3);
		Mat div4(Size(imasize.width/4, imasize.height/4), CV_8UC3);
		//div8 = cvCreateImage(cvSize(image.width/8,image.height/8), 8, 3);
		Mat div8(Size(imasize.width/8, imasize.height/8), CV_8UC3);
		//div16 = cvCreateImage(cvSize(image.width/16,image.height/16), 8, 3);
		Mat div16(Size(imasize.width/16, imasize.height/16), CV_8UC3);
		//dst = cvCreateImage(cvSize(image.width, image.height), 8, 3);
		Mat dst(imasize,CV_8UC3);

		//cvPyrDown(&src, div2, CV_GAUSSIAN_5x5);
		pyrDown(src, div2);
		//cvPyrDown(div2, div4, CV_GAUSSIAN_5x5);
		pyrDown(div2, div4);
		//cvPyrDown(div4, div8, CV_GAUSSIAN_5x5);
		pyrDown(div4, div8);
		//cvPyrDown(div8, div16, CV_GAUSSIAN_5x5);
		pyrDown(div8, div16);

	 	dst= Mat::zeros(dst.size(),CV_8UC3);
		//w = image.width*3;
		w=imasize.width*3;
		Size div2size=div2.size();
		Size div4size=div4.size();
		Size div8size=div8.size();
		Size div16size=div16.size();
		//for(i=0; i<image.height/2; i++) {
		for(i=0; i<imasize.height/2; i++) {
			//w2 = div2->width*div2->nChannels;
			w2=div2size.width*div2.channels();
			memcpy((dst.data)+w*i, (div2.data)+w2*i, w2);
			//if(i<image.height/4) {
			if(i<imasize.height/4) {
				//tmp = (image.width/2)*3;
				tmp= (imasize.width/2)*3;
				w2 = div4size.width*div4.channels();
				//memcpy((dst->imageData)+w*i+tmp, (div4->imageData)+w2*i, w2);
				memcpy((dst.data)+w*i+tmp, (div4.data)+w2*i, w2);
			}
			if(i<imasize.height/8) {
				tmp = (imasize.width/2 + imasize.width/4)*3;
				w2 = div8size.width*div8.channels();
				//memcpy((dst->imageData)+w*i+tmp, (div8->imageData)+w2*i, w2);
				memcpy((dst.data)+w*i+tmp, (div8.data)+w2*i, w2);
			}
			if(i<imasize.height/16) {
				tmp = (imasize.width/2 + imasize.width/4 + imasize.width/8)*3;
				w2 = div16size.width*div16.channels();
				//memcpy((dst->imageData)+w*i+tmp, (div16->imageData)+w2*i, w2);
				memcpy((dst.data)+w*i+tmp, (div16.data)+w2*i, w2);
			}
		}
		//cvCopy(dst,&src);
		dst.copyTo(image);
		
		//cvReleaseImage(&div2);
		~div2;
		//cvReleaseImage(&div4);
		~div4;
		//cvReleaseImage(&div8);
		~div8;
		//cvReleaseImage(&div16);
		~div16;
		//cvReleaseImage(&dst);
		~dst;

}
  
//void Viewer::sobel( const colorspaces::Image& image ){
void Viewer::sobel( Mat image ){
  
			int aperture=scale_sobel->get_value();
			if(aperture%2==0){aperture++;}
			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *gray; 
			//IplImage *dst;
			//IplImage *gaux;

			//gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			Mat gray(image.size(), CV_8UC1);
			//dst = cvCreateImage(cvSize(image.width,image.height), 16, 3);
			Mat dst(image.size(), CV_16SC1);
			//gaux = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			Mat gaux(image.size(), CV_8UC1);

			//cvCvtColor(&src, gray, CV_RGB2GRAY); 
			cvtColor(src,gray, CV_RGB2GRAY);
			//cvSobel(gray, dst, 0, 1, aperture);
			Sobel(gray, dst, dst.depth(), 0, 1, aperture);
			//cvConvertScale(dst,gaux,1,0);
			dst.convertTo(gaux, gaux.type(),1,0);
			//cvCvtColor(gaux, &src, CV_GRAY2RGB);
			cvtColor(gaux, src, CV_GRAY2RGB);

			src.copyTo(image);
			
			//cvReleaseImage(&gray);
			~gray;
			//cvReleaseImage(&dst);
			~dst;
			//cvReleaseImage(&gaux);
			~gaux;

			~src;

	
}

// void Viewer::canny( const colorspaces::Image& image )
void Viewer::canny( Mat image )
  {

			int aperture=scale_sobel->get_value();
			if(aperture%2==0){aperture++;}
			if (aperture<3)
				aperture=3;
			else if (aperture>7)
				aperture=7;
			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *gray; 
			//IplImage *dst;

			//gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			Mat gray(image.size(), CV_8UC1);
		 	//dst = cvCreateImage (cvSize(image.width,image.height), 8, 3);
			Mat dst(image.size(), CV_8UC1);
			
			//cvCvtColor(&src, gray, CV_RGB2GRAY);
			cvtColor(src,gray, CV_RGB2GRAY);
			//cvCanny( gray, dst, scale_canny->get_value(),scale_canny->get_value()*3,  aperture);
			Canny(gray, dst, scale_canny->get_value(),scale_canny->get_value()*3, aperture);
			//cvCvtColor(dst, &src, CV_GRAY2RGB);	
			cvtColor(dst, src, CV_GRAY2RGB);

			src.copyTo(image);

//colorspaces::ImageRGB8 img_rgb8(image); 
			
			//cvReleaseImage(&gray);
			~gray;
			//cvReleaseImage(&dst);
			~dst;

			~src;
}


// void Viewer::gray( const colorspaces::Image& image )

void Viewer::gray( Mat image )
  {
			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *gray; 
			//IplImage *dst;

			//gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			Mat gray(image.size(), CV_8UC1);
		 	//dst = cvCreateImage (cvSize(image.width,image.height), 8, 3);
			Mat dst(image.size(), CV_8UC1);
			//cvCvtColor(&src, gray, CV_RGB2GRAY);
			cvtColor(src, gray, CV_RGB2GRAY);
			//cvCvtColor(gray, &src, CV_GRAY2RGB);	
			cvtColor(gray, src, CV_GRAY2RGB);

			src.copyTo(image);
			
			//cvReleaseImage(&gray);
			~gray;
			//cvReleaseImage(&dst);
			~dst;
			~src;
}

 //void Viewer::harris( const colorspaces::Image& image )
void Viewer::harris( Mat image )
  {
			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *gray; 
			//IplImage *dst;
			//IplImage *gaux;
		int aperture=scale_sobel->get_value();
		if (aperture%2==0){aperture+=1;}
            //gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
	    Mat gray(image.size(), CV_8UC1);
            //dst = cvCreateImage(cvSize(image.width,image.height), 32, 3);
	    Mat dst(image.size(), CV_32FC1);
            //gaux = cvCreateImage(cvSize(image.width,image.height), 8, 3);
	    Mat gaux(image.size(), CV_8UC1);

            //cvCvtColor(&src, gray, CV_RGB2GRAY);
	    cvtColor(src, gray, CV_RGB2GRAY);
            //cvCornerHarris(gray, dst, 5, scale_sobel->get_value(), 0.04); 
	    cornerHarris(gray, dst, 5, aperture, 0.04);
            //cvConvertScale(dst,gaux,1,0);
	    dst.convertTo(gaux, gaux.type(), 1, 0);
            //cvCvtColor(gaux, &src, CV_GRAY2RGB);
	    cvtColor(gaux, src, CV_GRAY2RGB);

	    src.copyTo(image);
		  
            //cvReleaseImage(&gray);
	    ~gray;
            //cvReleaseImage(&dst);
	    ~dst;
            //cvReleaseImage(&gaux);
	    ~gaux;

	    ~src;
}


 //void Viewer::hough_circles( const colorspaces::Image& image )
 void Viewer::hough_circles( Mat image )
  {
 		//IplImage src=image;
		Mat src;
		image.copyTo(src);
		//IplImage *gray; 

		//gray = cvCreateImage(cvSize(image.width,image.height), 8, 3);
		Mat gray(image.size(), CV_8UC1);
		//CvMemStorage* storage = cvCreateMemStorage(0);        
      std::vector<Vec3f> circles;
		//cvCvtColor( &src, gray, CV_BGR2GRAY );
		cvtColor(src, gray, CV_BGR2GRAY);
		//cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); 
		Size graysize=gray.size();
		GaussianBlur(gray, gray, Size(9,9), 0, 0);// smooth it, otherwise a lot of false circles may be detected        
		//CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
		HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, graysize.height/4, 200, 100);

    size_t i;

    //for( i = 0; i < circles->total; i++ )
      for (i = 0; i< circles.size(); i++ )
    {
        //float* p = (float*)cvGetSeqElem( circles, i );
	Point p(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius=cvRound(circles[i][2]);
        //cvCircle( gray, cvPoint(cvRound(p[0]),cvRound(p[1])), 3, CV_RGB(255,255,0), -1, 8, 0 );
	circle(gray, p, 3, Scalar(255,255,0), -1, 8, 0);
        //cvCircle( gray, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,255,0), 3, 8, 0 );
	circle(gray, p, radius, Scalar(255,255,0), 3, 8, 0);
    }
		//cvCvtColor(gray, &src, CV_GRAY2RGB);
		cvtColor(gray, src, CV_GRAY2RGB);
	
		src.copyTo(image);
		
		//cvReleaseImage(&gray);
		~gray;
		//cvReleaseMemStorage(&storage);
		~src;
}

 //void Viewer::hough( const colorspaces::Image& image )
  void Viewer::hough( Mat image )
  {
			int aperture=scale_sobel->get_value();
			if(aperture%2==0){aperture++;}
			if(aperture<3)
				aperture=3;
			else if(aperture>7)
				aperture=7;

			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *color_dst; 
			//IplImage *dst;
			//IplImage *gray;

			int method=hough_combobox->get_active_row_number();

			//color_dst = cvCreateImage(cvSize(image.width,image.height), 8, 3);
			Mat color_dst(image.size(), CV_8UC3);
		 	//dst = cvCreateImage (cvSize(image.width,image.height), 8, 3);
			Mat dst(image.size(), CV_8UC1);
			//gray = cvCreateImage (cvSize(image.width,image.height), 8, 3);      
			Mat gray(image.size(), CV_8UC1);
			//CvMemStorage* storage = cvCreateMemStorage(0);
      //CvSeq* lines = 0;
      std::vector<Vec2f> lines;
      size_t i;

			//cvCvtColor(&src, gray, CV_RGB2GRAY);	
			cvtColor(src, gray, CV_RGB2GRAY);
			//cvCanny( gray, dst, scale_canny->get_value(), scale_canny->get_value()*3, aperture);
			Canny(gray, dst,scale_canny->get_value(), scale_canny->get_value()*3, aperture);
			//cvCvtColor( dst, color_dst, CV_GRAY2BGR );
			cvtColor(dst, color_dst, CV_GRAY2BGR);

			if (method==0){
						hough_long->hide();
            hough_gap->hide();
						label_long->hide();
            label_gap->hide();
					//lines = cvHoughLines2( dst,
                               //storage,
                               //CV_HOUGH_STANDARD,
                               //1,
                               //CV_PI/180,
                               //hough_threshold->get_value(),
                               //0,
                               //0 );
				HoughLines(dst, lines, 1, CV_PI/180,
					   hough_threshold->get_value(),
					   0, 0);

        		//for( i = 0; i < MIN(lines->total,100); i++ )
			for( i = 0; i < MIN(lines.size(), 100); i++ )
        		{

            		//float* line = (float*)cvGetSeqElem(lines,i);
				        //float rho = line[0];
					float rho = lines[i][0];
				        //float theta = line[1];
					float theta = lines[i][1];
				        //CvPoint pt1, pt2;
				        double a = cos(theta), b = sin(theta);
				        double x0 = a*rho, y0 = b*rho;
				        //pt1.x = cvRound(x0 + 1000*(-b));
				        //pt1.y = cvRound(y0 + 1000*(a));
					Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
				        //pt2.x = cvRound(x0 - 1000*(-b));
				        //pt2.y = cvRound(y0 - 1000*(a));
					Point pt2(cvRound(cvRound(x0 - 1000*(-b))),cvRound( y0 - 1000*(a)));
				        //cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
					line( color_dst, pt1, pt2, Scalar(255,0,0), 3, 8);
      		}
		}else{
			if(method==1){				
				hough_long->show();
        hough_gap->show();
				label_long->show();
        label_gap->show();

                std::vector<Vec4i> linesp;
				
				//lines = cvHoughLines2( dst,
                               //storage,
                               //CV_HOUGH_PROBABILISTIC,
                               //1,
                               //CV_PI/180,
                               //hough_threshold->get_value(),
                               //hough_long->get_value(),
                               //hough_gap->get_value());
				HoughLinesP( dst, linesp, 1, CV_PI/180, 
				hough_threshold->get_value(),
				hough_long->get_value(),
				hough_gap->get_value());

        //for( i = 0; i < lines->total; i++ )
	for( i=0; i< linesp.size(); i++ )
        {
            //CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
	    line(color_dst, Point(linesp[i][0], linesp[i][1]), 
	    Point(linesp[i][2],linesp[i][3]), Scalar(255,0,0), 3, 8);    
            //cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
        }
			}
		}		
				//cvCopy(color_dst,&src,0);
				color_dst.copyTo(src);
				src.copyTo(image);
				
		//cvReleaseImage(&color_dst);
		~color_dst;
		//cvReleaseImage(&dst);
		~dst;
		//cvReleaseImage(&gray);
		~gray;
 		//cvReleaseMemStorage(&storage);
		~src;
}

// void Viewer::flow( const colorspaces::Image& image )

   void Viewer::flow( Mat image )
  {
			//IplImage src=image;
			Mat src;
			image.copyTo(src);
			//IplImage *img1, *img2, *aux1, *aux2,  *aux3, *aux4;
			

			if(opflow_first) {
					//if(previous==NULL)
					if (previous.empty())
						//previous = cvCreateImage(cvSize(image.width,image.height), 8, 3);
						previous.create(image.size(), 
						CV_8UC3);
						src.copyTo(previous);
						opflow_first = 0;
						~src;
						return;
				}
				/* Images with feature points */
				//img1 = cvCreateImage(cvSize(image.width,image.height), 8, 3);
				Mat img1(image.size(),CV_8UC1);
				//img2 = cvCreateImage(cvSize(image.width,image.height), 8, 3);
				Mat img2(image.size(), CV_8UC1);

				TermCriteria criteria = TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .03);

				/*Temp images for algorithms*/
				//aux1 = cvCreateImage(cvSize(image.width,image.height), 32, 3);
				//Mat aux1(image.size(),CV_32FC1);
				//aux2 = cvCreateImage(cvSize(image.width,image.height), 32, 3);
				//Mat aux2(image.size(), CV_32FC1);
				//aux3 = cvCreateImage(cvSize(image.width,image.height), 8, 3);
				//Mat aux3(image.size(), CV_8UC1);
				//aux4 = cvCreateImage(cvSize(image.width,image.height), 8, 3);
				//Mat aux4(image.size(), CV_8UC1);

				//cvConvertImage(previous, img1);
				//previous.convertTo(img1, CV_8UC1);
				//img1.reshape(1);
				//std::cout << img1.channels() << std::endl;
				//std::cout << img2.channels() << std::endl;
				//previous.copyTo(img1);
				cvtColor(previous,img1,CV_RGB2GRAY);
				//cvConvertImage(&src, img2);
				//src.convertTo(img2, CV_8UC1);
				//img2.reshape(1);
				//src.copyTo(img2);
				cvtColor(src,img2,CV_RGB2GRAY);
				//img1.convertTo(img1,CV_8UC1);
				//img2.convertTo(img2,CV_8UC1);

				int i;
				int numpoints =90; // 300;
				//CvPoint2D32f points1[numpoints];	
      std::vector<Point2f> points[2];/*Feature points from img1*/
				//CvPoint2D32f points2[numpoints];	
				//vector<Point2f> points2; /*Location of the feature points in img2*/
      std::vector<uchar> foundPoint;
      std::vector<float> errors;
				//CvSize sizeWindow = cvSize(5,5);
				Size sizeWindow (31,31)/*(5,5)*/, pixWinSize(15,15);
				//CvTermCriteria termCriteria;
																
				//termCriteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );

				/*	Shi and Tomasi algorithm, get feature points from img1 */	
				//cvGoodFeaturesToTrack(img1, aux1, aux2, points1, &numpoints, .01,.01,NULL);
				goodFeaturesToTrack(img1, points[0], numpoints, 
				.01, .01);

			if (points[0].size() > 0)
			{

				cornerSubPix(img1, points[0], pixWinSize,
				Size(-1,-1), criteria);
				/* Pyramidal Lucas Kanade Optical Flow algorithm, search feature points in img2 */
				//cvCalcOpticalFlowPyrLK(img1, img2, aux3, aux4, points1, points2, numpoints, sizeWindow, 5, foundPoint, errors, termCriteria, 0);

                cv::calcOpticalFlowPyrLK(img1, img2, points[0],
				points[1], foundPoint, errors, sizeWindow, 5, 
				criteria);

				/*Esta funcion sirve para colorear los puntos encontrados. Reservado para depuración.*/
				/*for(int k=0; k< numpoints; k++)	{
					circle(src, points[0][k], 3, Scalar(0, 0, 255), -1); //Azul punto principio
					circle(src, points[1][k], 3, Scalar(0, 255, 0), -1); // Verde punto final
				}*/
				/* Draw arrows*/
				for(i = 0; i < numpoints; i++)	{
						if ( foundPoint[i] == 0 )	
							continue;

						int line_thickness = 1;
						//CvScalar line_color = CV_RGB(255,0,0);
						Scalar line_color(255,0,0);
	
						//CvPoint p,q;
						//p.x = (int) points1[i].x;
						//p.y = (int) points1[i].y;
						Point p((int)points[0][i].x, 
						(int)points[0][i].y);
						//q.x = (int) points2[i].x;
						//q.y = (int) points2[i].y;
						Point q((int)points[1][i].x, 
						(int)points[1][i].y);


						double angle = atan2( (double) p.y - q.y, (double) p.x - q.x );
						double hypotenuse = sqrt(SQUARE(p.y - q.y) + SQUARE(p.x - q.x));

						if(hypotenuse < 10 || hypotenuse > 40)
							continue;

						/*Line*/
						q.x = (int) (p.x - 1 * hypotenuse * cos(angle));
						q.y = (int) (p.y - 1 * hypotenuse * sin(angle));
						//cvLine(&src, p, q, line_color, line_thickness, CV_AA, 0 );
						line(src, p, q, line_color,
						line_thickness, CV_AA, 0);
		
						/*Arrow*/
						p.x = (int) (q.x + 9 * cos(angle + PI / 4));
						p.y = (int) (q.y + 9 * sin(angle + PI / 4));
						//cvLine(&src, p, q, line_color, line_thickness, CV_AA, 0 );
						line(src, p, q, line_color,
						line_thickness, CV_AA, 0);
						p.x = (int) (q.x + 9 * cos(angle - PI / 4));
						p.y = (int) (q.y + 9 * sin(angle - PI / 4));
						//cvLine(&src, p, q, line_color, line_thickness, CV_AA, 0 );
						line(src, p ,q, line_color, 
						line_thickness, CV_AA, 0);
				}
			}

				//cvCopy(&src,previous,0);
				src.copyTo(image);
				image.copyTo(previous);
				
			//cvReleaseImage(&img1);
			~img1;
			//cvReleaseImage(&img2);
			~img2;
			//cvReleaseImage(&aux1);
			//~aux1;
			//cvReleaseImage(&aux2);
			//~aux2;
			//cvReleaseImage(&aux3);
			//~aux3;
			//cvReleaseImage(&aux4);
			//~aux4;
			~src;
}

  //void Viewer::display( const colorspaces::Image& image, const colorspaces::Image& image2 )
   void Viewer::display( Mat image, Mat image2 )
  {
    //colorspaces::ImageRGB8 img_rgb8(image);
    //colorspaces::ImageRGB8 img_rgb8_2(image2);
    Mat img_mat( image.size(), CV_8UC3);
    image.copyTo(img_mat);

    Mat img_mat2( image2.size(), CV_8UC3);
    image2.copyTo(img_mat2);

    
    //IplImage img = image; 
    //Mat img;
    //image.copyTo(img);
    //imagenO = cvCreateImage(cvGetSize(&img), 8, 3);
    imagenO.create(image.size(), CV_8UC3);
    //cvCopy(&img, imagenO);
    img_mat.copyTo(imagenO);
    pthread_mutex_unlock(&mutex);

	
    /*Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
    Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step);*/

	Size img_mat_size=img_mat.size();
	Size imagesize = image.size();

	//std::cout << img_mat_size.height << std::endl;
	//std::cout << img_mat_size.width << std::endl;
	//std::cout << imagesize.height << std::endl;
	//std::cout << imagesize.width << std::endl;
	//std::cout << img_mat.type() << std::endl;
	//std::cout << "Lo siguiente es data\n";
	//std::cout << img_mat.data << std::endl;
	Glib::RefPtr<Gdk::Pixbuf> imgBuff =
    Gdk::Pixbuf::create_from_data((const guint8*)img_mat.data,
                                    Gdk::COLORSPACE_RGB,
                                    false,
                                    8,
                                    img_mat_size.width,
                                    img_mat_size.height,
                                    //img_mat_size.width*img_mat.elemSize());
				    img_mat.step);



    /*Glib::RefPtr<Gdk::Pixbuf> imgBuff2 = 
    Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8_2.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8_2.width,
				    img_rgb8_2.height,
				    img_rgb8_2.step);*/

     Size img_mat2_size = img_mat2.size();

     Glib::RefPtr<Gdk::Pixbuf> imgBuff2 =
    Gdk::Pixbuf::create_from_data((const guint8*)img_mat2.data,
                                    Gdk::COLORSPACE_RGB,
                                    false,
                                    8,
                                    img_mat2_size.width,
                                    img_mat2_size.height,
                                    //img_mat2_size.width*img_mat2.elemSize());
				    img_mat2.step);

		gtkimage->clear();
    gtkimage->set(imgBuff);


    gtkimage2->clear();
    gtkimage2->set(imgBuff2);


    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
    pthread_mutex_lock(&mutex);
        //cvReleaseImage(&imagenO);
	~imagenO;

   }


void Viewer::button_pyramid_clicked(){
	
		if(pyramid_box)
			pyramid_box = 0;
		else
			pyramid_box = 1;

}


void Viewer::button_conv_clicked(){
		if(conv_box)
			conv_box = 0;
		else
			conv_box = 1;
}


void Viewer::button_gray_clicked(){
		if(gray_box)
			gray_box = 0;
		else
			gray_box = 1;
}


void Viewer::button_color_clicked(){
		if(color_box)
			color_box = 0;
		else
			color_box = 1;
}

void Viewer::button_sobel_clicked(){
		if(sobel_box)
			sobel_box = 0;
		else
			sobel_box = 1;
}

void Viewer::button_laplace_clicked(){
		if(laplace_box)
			laplace_box = 0;
		else
			laplace_box = 1;
}


void Viewer::button_default_clicked(){
		if(def_box)
			def_box = 0;
		else
			def_box = 1;
}

void Viewer::button_canny_clicked(){
		if(canny_box)
			canny_box = 0;
		else
			canny_box = 1;
}

void Viewer::button_harris_clicked(){

		if(harris_box)
			harris_box = 0;
		else
			harris_box = 1;
}

void Viewer::button_hough_clicked(){
		if(hough_box)
			hough_box = 0;
		else
			hough_box = 1;

}

void Viewer::button_flow_clicked(){
		if(flow_box)
			flow_box = 0;
		else
			flow_box = 1;
}


void Viewer::button_hough_circles_clicked(){
		if(houghcircles_box)
			houghcircles_box = 0;
		else
			houghcircles_box = 1;
}

//void Viewer::selection( const colorspaces::Image& image ){

void Viewer::selection( Mat image ){

bool INFO=true;
	
	if (laplace_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"LAPLACE\n";}		
		laplace(image);
	}

	if(sobel_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"SOBEL : aperture = "<<scale_sobel->get_value()<<"\n";}		
		sobel(image);			
	}

	if(harris_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"HARRIS CORNER\n";}		
		harris(image);			
	}

	if(hough_box){
		if (INFO){std::cout<<"**************\n";
				if(hough_combobox->get_active_row_number()==0)			
					std::cout<<"HOUGH STANDARD : threshold = "<<hough_threshold->get_value()<<"\n";
				else std::cout<<"HOUGH PROBABILISTIC : threshold = "<<scale_sobel->get_value()<<"; length = "<<hough_long->get_value()<<"; gap = "<<hough_gap->get_value()<<"\n";
		}
		hough(image);			
	}

	if(canny_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"CANNY FILTER : threshold = "<<scale_canny->get_value()<<"\n";}		
		canny(image);
	}
								
	if(gray_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"GRAY\n";}		
		gray(image);									
	}

	if(flow_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"OPTICAL FLOW\n";}		
		flow(image);									
	}

	if(color_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"COLOR FILTER : Hmax ="<<Hmax->get_value()<<"; Hmin ="<<Hmin->get_value()<<"; Smaxn ="<<Smax->get_value()<<"; Smin ="<<Smin->get_value()<<"; Vmax ="<<Vmax->get_value()<<"; Vmin ="<<Vmin->get_value()<<"\n";}		
		color(image);									
	}

	if(conv_box){
		if (INFO){std::cout<<"**************\n";
			if(conv_combobox->get_active_row_number()==0)std::cout<<"CONVOLUTION SHARPENING\n";
			if(conv_combobox->get_active_row_number()==1)std::cout<<"CONVOLUTION GAUSSIAN BLUR\n";
			if(conv_combobox->get_active_row_number()==2)std::cout<<"CONVOLUTION EMBOSSING\n";
}
		conv(image);			
	}

	if(pyramid_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"PYRAMID\n";}		
		pyramid(image);									
	}

	if(houghcircles_box){
		if (INFO){std::cout<<"**************\n";std::cout<<"HOUGH CIRCLES\n";}				
		hough_circles(image);									
	}

	if(def_box){

					
	}
}

}//namespace
