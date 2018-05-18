/*
 *  Copyright (C) 1997-2013 JDE Developers TeamrgbdViz.camRGB
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */

#include "rgbdVizgui.h"
#include <jderobot/pointcloud.h>
#include <string>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <resourcelocator/gladelocator.hpp>
#include <opencv2/opencv.hpp>

namespace rgbdViz {
rgbdVizgui::rgbdVizgui(bool rgb, bool depth,bool pointCloud , std::string path, std::string path_rgb, std::string path_ir,  cv::Size sizeRGB, cv::Size sizeDEPTH, float cycle): gtkmain(0,0) {

    /*Init OpenGL*/
    if(!Gtk::GL::init_check(NULL, NULL))	{
        std::cerr << "Couldn't initialize GL\n";
        std::exit(1);
    }
    this->cycle=cycle;

    if (rgb)
        cam_rgb_active=1;
    else
        cam_rgb_active=0;
    if (depth)
        cam_depth_active=1;
    else
        cam_depth_active=0;
    modesAvalables=0;
    if (pointCloud) {

        if ((rgb)&&(depth)) {
            reconstructMode=1;
            modesAvalables=2; //only one mode
        }
        else {
            reconstructMode=1;
            modesAvalables=1; //both modes
        }
    }
    else {
        if ((rgb)&&(depth)) {
            reconstructMode=0;
            modesAvalables=1; //only point cloud mode
        }
        else {
            reconstructMode=0;
            modesAvalables=0; // no mode available
        }
    }

    reconstruct_depth_activate=false;
    lines_depth_active=false;
    lines_rgb_active=false;
    std::cout << "Loading glade\n";
    const std::string gladepath = resourcelocator::findGladeFile("rgbdVizgui.glade");
    refXml = Gnome::Glade::Xml::create(gladepath);
    myDepthSize=sizeDEPTH;
    myRGBSize=sizeRGB;




    /*Get widgets*/
    refXml->get_widget("rgbdViz", mainwindow);
    refXml->get_widget("imageRGB", w_imageRGB);
    refXml->get_widget("imageDEPTH", w_imageDEPTH);
    refXml->get_widget("eventboxRGB", w_event_rgb);
    refXml->get_widget("eventboxDEPTH", w_event_depth);
    refXml->get_widget("toggle_reconstruct", w_reconstruct);
    refXml->get_widget("toggle_camera_pos", w_camera_pos);
    refXml->get_widget("toggle_lines_rgb", w_lines_rgb);
    refXml->get_widget("toggle_lines_depth", w_lines_depth);
    refXml->get_widget("toggle_depth", w_toggle_depth);
    refXml->get_widget("toggle_rgb", w_toggle_rgb);
    refXml->get_widget("view_controller",w_view_controller);
    refXml->get_widget("window_controller",w_window_controller);
    refXml->get_widget("vbox_reconstruct_selection",w_reconstruct_selection);
    refXml->get_widget("vbox_reconstruct_mode",w_reconstruct_mode);
    refXml->get_widget("radio_mode_pointcloud",w_radio_mode_pointcloud);
    refXml->get_widget("radio_mode_image",w_radio_mode_image);
    refXml->get_widget("radio_depth",w_radio_depth);
    refXml->get_widget("radio_rgb",w_radio_rgb);
    refXml->get_widget("button_clear_lines",w_button_clear_lines);
    refXml->get_widget("window_gl",w_window_gl);
    refXml->get_widget("tg_gl",w_tg_gl);
    refXml->get_widget("vbox_gl",w_vbox_gl);
    refXml->get_widget("tg_RealDistance",w_realDistance);
    refXml->get_widget("vbox_images_tool",w_images_tool);
    refXml->get_widget("entry1",w_entry);
    refXml->get_widget("labelDistance",w_Distance);


    if (!cam_rgb_active) {
        w_toggle_rgb->hide();
    }
    if (!cam_depth_active) {
        w_toggle_depth->hide();
    }

    w_event_rgb->signal_button_press_event().connect(sigc::mem_fun(this,&rgbdVizgui::on_clicked_event_rgb));
    w_event_depth->signal_button_press_event().connect(sigc::mem_fun(this,&rgbdVizgui::on_clicked_event_depth));
    w_reconstruct->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_reconstruct_depth));
    w_camera_pos->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::add_camera_position));
    w_lines_rgb->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_lines_rgb_toggled));
    w_lines_depth->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_lines_depth_toggled));
    w_tg_gl->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_tg_gl_toggled));
    w_view_controller->signal_activate().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_view_controller_activate));
    w_radio_depth->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_radio_depth_activate));
    w_radio_rgb->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_radio_rgb_activate));
    w_radio_mode_pointcloud->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_radio_mode_pointcloud_activate));
    w_radio_mode_image->signal_toggled().connect(sigc::mem_fun(this,&rgbdVizgui::on_w_radio_mode_image_activate));
    w_button_clear_lines->signal_clicked().connect(sigc::mem_fun(this,&rgbdVizgui::on_clicked_clear_lines));

    if (modesAvalables==0) {
        w_reconstruct->hide();
    }

    // Mundo OpenGL
    refXml->get_widget_derived("gl_world",world);
    world->setCamerasResolution(myDepthSize.width,myDepthSize.height);

    std::cout << "Creating Progeos Virtual Cameras" << std::endl;
    mypro= new rgbdViz::myprogeo(2,myRGBSize.width,myRGBSize.height);
    mypro->load_cam((char*)path_rgb.c_str(),0,myRGBSize.width, myRGBSize.height);

    mypro->load_cam((char*)path_ir.c_str(),1,myDepthSize.width, myDepthSize.height);
    //util = new rgbdViz::util3d(mypro);

    /*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
    world->readFile(path);
    mainwindow->show();
    this->distance = new cv::Mat(cv::Size(myDepthSize.width, myDepthSize.height),CV_32FC1,cv::Scalar(0,0,0));

    /*for (float i=0; i< 10000; i+=1){
                    world->add_kinect_point(1,1,i,0,0,0);
                }*/
}

rgbdVizgui::~rgbdVizgui() {
    //delete this->controller;
}


void
rgbdVizgui::updateAll( cv::Mat imageRGB, cv::Mat imageDEPTH, std::vector<jderobot::RGBPoint> cloud )
{

	imageRGBlocal=imageRGB;
	imageDEPTHlocal=imageDEPTH;


    cv::Mat colorDepth(imageDEPTH.size(),imageDEPTH.type());
    CvPoint pt1,pt2;
    if (w_toggle_rgb->get_active()) {
        Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) imageRGB.data,Gdk::COLORSPACE_RGB,false,8,imageRGB.cols,imageRGB.rows,imageRGB.step);
        w_imageRGB->clear();
        /*si queremos pintar las lineas*/
        if (lines_rgb_active) {
            //util->draw_room(imageRGB,0, world->lines, world->numlines);
        }
        w_imageRGB->set(imgBuff);

    }
    if (w_toggle_depth->get_active()||((reconstruct_depth_activate)&&(reconstructMode==0))) {

        /*split channels to separate distance from image*/
        std::vector<cv::Mat> layers;
        cv::split(imageDEPTH, layers);

        cv::cvtColor(layers[0],colorDepth,CV_GRAY2RGB);
        //cv::imshow("color", colorDepth);
        //cv::waitKey(1);

        this->m_distance.lock();
        
        for (int x=0; x< layers[1].cols ; x++) {
            for (int y=0; y<layers[1].rows; y++) {
                distance->at<float>(y,x) = (((int)layers[1].at<unsigned char>(y,x)<<8)|(int)layers[2].at<unsigned char>(y,x));

            }
        }
        this->m_distance.unlock();

        if (w_toggle_depth->get_active()) {
            cv::Mat localDepth;
            Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) colorDepth.data,Gdk::COLORSPACE_RGB,false,8,colorDepth.cols,colorDepth.rows,colorDepth.step);
            w_imageDEPTH->clear();
            if (lines_depth_active) {
                //util->draw_room(colorDepth,1, world->lines, world->numlines);
            }
            w_imageDEPTH->set(imgBuff);
        }




    }
    if (reconstruct_depth_activate) {
        if (reconstructMode==0) {
            add_depth_pointsImage(imageRGB, *distance);
        }
        else{
            add_depth_pointsCloud(cloud);
        }
    }
    world->my_expose_event();
    while (gtkmain.events_pending())
        gtkmain.iteration();
}

void
rgbdVizgui::updateRGB(cv::Mat imageRGB)
{
    CvPoint pt1,pt2;
    if (w_toggle_rgb->get_active()) {
        Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) imageRGB.data,Gdk::COLORSPACE_RGB,false,8,imageRGB.cols,imageRGB.rows,imageRGB.step);
        w_imageRGB->clear();
        /*si queremos pintar las lineas*/
        if (lines_rgb_active) {
            //util->draw_room(imageRGB,0, world->lines, world->numlines);

        }
        w_imageRGB->set(imgBuff);
        displayFrameRate();
        while (gtkmain.events_pending())
            gtkmain.iteration();
    }
    if (reconstruct_depth_activate) {
        if (reconstructMode!=0) {
            //add_depth_pointsCloud();	
        }
    }
    world->my_expose_event();
    while (gtkmain.events_pending())
        gtkmain.iteration();
}

void
rgbdVizgui::updateDEPTH(cv::Mat imageDEPTH )
{
	imageDEPTHlocal=imageDEPTH;
    CvPoint pt1,pt2;
    if (w_toggle_depth->get_active()) {
        Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) imageDEPTH.data,Gdk::COLORSPACE_RGB,false,8,imageDEPTH.cols,imageDEPTH.rows,imageDEPTH.step);
        w_imageDEPTH->clear();
        if (lines_depth_active) {
            //util->draw_room(imageDEPTH,1, world->lines, world->numlines);

        }
        w_imageDEPTH->set(imgBuff);
    }
    if (reconstruct_depth_activate) {
        if (reconstructMode!=0) {
            //add_depth_pointsCloud();
        }
    }
    world->my_expose_event();
    while (gtkmain.events_pending())
        gtkmain.iteration();
}

void
rgbdVizgui::updatePointCloud(std::vector<jderobot::RGBPoint> cloud )
{
    displayFrameRate();
    while (gtkmain.events_pending())
        gtkmain.iteration();
    if (reconstruct_depth_activate) {
        if (reconstructMode==1) {
            add_depth_pointsCloud(cloud);
        }
    }
    world->my_expose_event();
    while (gtkmain.events_pending())
        gtkmain.iteration();
}


bool rgbdVizgui::isClosed() {
    return false;
}

bool rgbdVizgui::isVisible() {
    return mainwindow->is_visible();
}

void rgbdVizgui::displayFrameRate()
{
    double diff;
    IceUtil::Time diffT;

    currentFrameTime = IceUtil::Time::now();
    diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
    if (diff < 1000.0)
        frameCount++;
    else {
        oldFrameTime = currentFrameTime;
        fps = frameCount*1000.0/diff;
        frameCount=0;
        // Display the frame rate
        std::stringstream fpsString;
        fpsString << "fps = " << int(fps);
        //fpslabel->set_label(fpsString.str());
    }
}

bool rgbdVizgui::on_clicked_event_rgb(GdkEventButton* event) {
    int x,y;
    float xp,yp,zp,camx,camy,camz;
    float xu,yu,zu;

    gdk_window_at_pointer(&x,&y);
    mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,0);
    xu=(xp-camx)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    yu=(yp-camy)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    zu=(zp-camz)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));

    this->m_distance.lock();
	double d=this->distance->at<float>(y,x);
	this->m_distance.unlock();
	if (d!=0) {
		//std::cout << d << std::endl;
		//d=d*10;
		float xp,yp,zp,camx,camy,camz;
		float ux,uy,uz;
		float c1x, c1y, c1z;
		float fx,fy,fz;
		float fmod;
		float Fx,Fy,Fz;

		mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,0);



		//vector unitario
		float modulo;

		modulo = sqrt(1/(((camx-xp)*(camx-xp))+((camy-yp)*(camy-yp))+((camz-zp)*(camz-zp))));
		mypro->mygetcamerafoa(&c1x, &c1y, &c1z, 0);


		fmod = sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
		fx = (c1x - camx)*fmod;
		fy = (c1y - camy)*fmod;
		fz = (c1z - camz) * fmod;
		ux = (xp-camx)*modulo;
		uy = (yp-camy)*modulo;
		uz = (zp-camz)*modulo;

		Fx= d*fx + camx;
		Fy= d*fy + camy;
		Fz= d*fz + camz;


		/* calculamos el punto real */
		d = (-(fx*camx) + (fx*Fx) - (fy*camy) + (fy*Fy) - (fz*camz) + (fz*Fz))/((fx*ux) + (fy*uy) + (fz*uz));
		//imprimos la información
		std::stringstream ss;
		ss << d;
		w_Distance->set_text(ss.str());

	}
	else{

		d= 5000;
		w_Distance->set_text("no measure");
	}




    world->add_line(camx+d*xu,camy+d*yu,camz+d*zu,camx,camy,camz);
    return true;
}


bool rgbdVizgui::on_clicked_event_depth(GdkEventButton* event) {
    int x,y;
    float xp,yp,zp,camx,camy,camz;
    float xu,yu,zu;

    gdk_window_at_pointer(&x,&y);
    mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,1);
    xu=(xp-camx)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    yu=(yp-camy)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    zu=(zp-camz)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));

    this->m_distance.lock();
	double d=this->distance->at<float>(y,x);
	this->m_distance.unlock();
	if (d!=0) {
		//std::cout << d << std::endl;
		//d=d*10;
		float xp,yp,zp,camx,camy,camz;
		float ux,uy,uz;
		float x=0,y =0;
		float c1x, c1y, c1z;
		float fx,fy,fz;
		float fmod;
		float Fx,Fy,Fz;

		mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,0);



		//vector unitario
		float modulo;

		modulo = sqrt(1/(((camx-xp)*(camx-xp))+((camy-yp)*(camy-yp))+((camz-zp)*(camz-zp))));
		mypro->mygetcamerafoa(&c1x, &c1y, &c1z, 0);


		fmod = sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
		fx = (c1x - camx)*fmod;
		fy = (c1y - camy)*fmod;
		fz = (c1z - camz) * fmod;
		ux = (xp-camx)*modulo;
		uy = (yp-camy)*modulo;
		uz = (zp-camz)*modulo;

		Fx= d*fx + camx;
		Fy= d*fy + camy;
		Fz= d*fz + camz;


		/* calculamos el punto real */
		d = (-(fx*camx) + (fx*Fx) - (fy*camy) + (fy*Fy) - (fz*camz) + (fz*Fz))/((fx*ux) + (fy*uy) + (fz*uz));
		//imprimos la información
		std::stringstream ss;
		ss << d;
		w_Distance->set_text(ss.str());

	}
	else{

		d= 5000;
		w_Distance->set_text("no measure");
	}





    world->add_line(camx+d*xu,camy+d*yu,camz+d*zu,camx,camy,camz);

    world->add_line(d*xu + camx,d*yu + camy,d*zu + camz,camx,camy,camz);
    return true;
}

void rgbdVizgui::add_cameras_position() {
    int x,y;
    float xp,yp,zp,camx,camy,camz;
    float xu,yu,zu;
    float k;

    gdk_window_at_pointer(&x,&y);
    mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,0);
	
    xu=(xp-camx)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    yu=(yp-camy)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
    zu=(zp-camz)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));

    k= 500;
    world->add_line(camx+k*xu,camy+k*yu,camz+k*zu,camx,camy,camz);
    world->add_line(1905,800,1240,camx,camy,camz);
}

void
rgbdVizgui::on_reconstruct_depth() {
    std::cout << std::endl<< std::endl<< std::endl<< w_reconstruct->get_active() << std::endl<< std::endl<< std::endl<< std::endl;
    if (w_reconstruct->get_active()) {
        reconstruct_depth_activate=true;
        world->draw_kinect_points=true;
        w_reconstruct_selection->show();
        if (modesAvalables==2)
            w_reconstruct_mode->show();
    }
    else {
        reconstruct_depth_activate=false;
        world->draw_kinect_points=false;
        w_reconstruct_selection->hide();
    }
}

void
rgbdVizgui::add_depth_pointsImage(cv::Mat imageRGB, cv::Mat distance) {
    float d;

    world->clear_points();
    //std::cout << "inicio reconstrucción" << std::endl;

    int sampling= atoi(this->w_entry->get_buffer()->get_text().c_str());

    if (sampling<=0){
    	sampling=1;
    }
    for (int xIm=0; xIm< myDepthSize.width; xIm+=sampling) {
        for (int yIm=0; yIm<myDepthSize.height ; yIm+=sampling) {
            d=distance.at<float>(yIm,xIm);
            if (d!=0) {
                //std::cout << d << std::endl;
                //d=d*10;
                float xp,yp,zp,camx,camy,camz;
                float ux,uy,uz;
                float c1x, c1y, c1z;
                float fx,fy,fz;
                float fmod;
                float t;
                float Fx,Fy,Fz;

                mypro->mybackproject(xIm, yIm, &xp, &yp, &zp, &camx, &camy, &camz,0);

			

                //vector unitario	
                float modulo;

                modulo = sqrt(1/(((camx-xp)*(camx-xp))+((camy-yp)*(camy-yp))+((camz-zp)*(camz-zp))));
                mypro->mygetcamerafoa(&c1x, &c1y, &c1z, 0);


                fmod = sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
                fx = (c1x - camx)*fmod;
                fy = (c1y - camy)*fmod;
                fz = (c1z - camz) * fmod;
                ux = (xp-camx)*modulo;
                uy = (yp-camy)*modulo;
                uz = (zp-camz)*modulo;

                Fx= d*fx + camx;
                Fy= d*fy + camy;
                Fz= d*fz + camz;

                /* calculamos el punto real */
                t = (-(fx*camx) + (fx*Fx) - (fy*camy) + (fy*Fy) - (fz*camz) + (fz*Fz))/((fx*ux) + (fy*uy) + (fz*uz));


                cv::Scalar color(0,0,0);
                if (myDepthSize == myRGBSize){
                	color(0)= (int)imageRGB.data[3*(yIm*myDepthSize.width+xIm)];
                	color(1)= (int)imageRGB.data[3*(yIm*myDepthSize.width+xIm)+1];
                	color(2)= (int)imageRGB.data[3*(yIm*myDepthSize.width+xIm)+2];
                }


                if (w_realDistance->get_active()){
                	world->add_kinect_point(t*ux + camx,t*uy+ camy,t*uz + camz,color(0),color(1),color(2));
                }
                else{
                	world->add_kinect_point(d*ux + camx,d*uy+ camy,d*uz + camz,color(0),color(1),color(2));
                }

            }
        }
    }

    //std::cout << "fin reconstrucción" << std::endl;
}

void
rgbdVizgui::add_depth_pointsCloud(std::vector<jderobot::RGBPoint> cloud) {
    world->clear_points();
    for (std::vector<jderobot::RGBPoint>::iterator it = cloud.begin(); it != cloud.end(); ++it) {
        world->add_kinect_point(it->x,it->y,it->z,(int)it->r,(int)it->g,(int)it->b);
    }
}

void
rgbdVizgui::add_camera_position() {
    float c1x, c1y, c1z, c2x, c2y, c2z, c3x, c3y, c3z, c4x, c4y,c4z;
    float camx, camy, camz;
    float w,h;
    float modulo,distance;


    	if (w_camera_pos->get_active()){
		distance=300;
		mypro->mygetcamerasize(&w,&h,1);
		mypro->mybackproject(0,0,&c1x,&c1y,&c1z,&camx, &camy, &camz,1);
		mypro->mybackproject(myDepthSize.width,0,&c2x,&c2y,&c2z,&camx, &camy, &camz,1);
		mypro->mybackproject(0,myDepthSize.height,&c3x,&c3y,&c3z,&camx, &camy, &camz,1);
		mypro->mybackproject(myDepthSize.width,myDepthSize.height,&c4x,&c4y,&c4z,&camx, &camy, &camz,1);
		
		modulo = 	sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
		c1x = (c1x-camx)*modulo;
		c1y = (c1y-camy)*modulo;
		c1z = (c1z-camz)*modulo;
	
		modulo = 	sqrt(1/(((camx-c2x)*(camx-c2x))+((camy-c2y)*(camy-c2y))+((camz-c2z)*(camz-c2z))));
		c2x = (c2x-camx)*modulo;
		c2y = (c2y-camy)*modulo;
		c2z = (c2z-camz)*modulo;
	
		modulo = 	sqrt(1/(((camx-c3x)*(camx-c3x))+((camy-c3y)*(camy-c3y))+((camz-c3z)*(camz-c3z))));
		c3x = (c3x-camx)*modulo;
		c3y = (c3y-camy)*modulo;
		c3z = (c3z-camz)*modulo;
	
		modulo = 	sqrt(1/(((camx-c4x)*(camx-c4x))+((camy-c4y)*(camy-c4y))+((camz-c4z)*(camz-c4z))));
		c4x = (c4x-camx)*modulo;
		c4y = (c4y-camy)*modulo;
		c4z = (c4z-camz)*modulo;
	
		
	
		world->add_camera_line(distance*c1x + camx,distance*c1y+ camy,distance*c1z + camz,camx,camy,camz);
		world->add_camera_line(distance*c2x + camx,distance*c2y+ camy,distance*c2z + camz,camx,camy,camz);
		world->add_camera_line(distance*c3x + camx,distance*c3y+ camy,distance*c3z + camz,camx,camy,camz);
		world->add_camera_line(distance*c4x + camx,distance*c4y+ camy,distance*c4z + camz,camx,camy,camz);
		world->add_camera_line(distance*c1x + camx,distance*c1y+ camy,distance*c1z + camz,distance*c2x + camx,distance*c2y+ camy,distance*c2z + camz);
		world->add_camera_line(distance*c4x + camx,distance*c4y+ camy,distance*c4z + camz,distance*c2x + camx,distance*c2y+ camy,distance*c2z + camz);
		world->add_camera_line(distance*c3x + camx,distance*c3y+ camy,distance*c3z + camz,distance*c4x + camx,distance*c4y+ camy,distance*c4z + camz);
		world->add_camera_line(distance*c1x + camx,distance*c1y+ camy,distance*c1z + camz,distance*c3x + camx,distance*c3y+ camy,distance*c3z + camz);
	
		mypro->mygetcamerafoa(&c1x, &c1y, &c1z, 1);




		modulo = 	sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
		c1x = (c1x-camx)*modulo;
		c1y = (c1y-camy)*modulo;
		c1z = (c1z-camz)*modulo;
		distance=distance*3;
		world->add_camera_line(distance*c1x + camx,distance*c1y+ camy,distance*c1z + camz,camx,camy,camz);

		mypro->mybackproject(320,240,&c1x,&c1y,&c1z,&camx, &camy, &camz,1);
		modulo = 	sqrt(1/(((camx-c1x)*(camx-c1x))+((camy-c1y)*(camy-c1y))+((camz-c1z)*(camz-c1z))));
		c1x = (c1x-camx)*modulo;
		c1y = (c1y-camy)*modulo;
		c1z = (c1z-camz)*modulo;
		distance=distance*3;
		world->add_camera_line(distance*c1x + camx,distance*c1y+ camy,distance*c1z + camz,camx,camy,camz);




	}
    else {
        world->clear_camera_lines();
    }


    //mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,1);
}

void
rgbdVizgui::on_w_lines_rgb_toggled() {
    if (w_lines_rgb->get_active()) {
        lines_rgb_active=true;
    }
    else
        lines_rgb_active=false;
}

void
rgbdVizgui::on_w_lines_depth_toggled() {
    if (w_lines_depth->get_active()) {
        lines_depth_active=true;
    }
    else
        lines_depth_active=false;
}

void
rgbdVizgui::on_w_view_controller_activate() {
    w_window_controller->show();
}

void
rgbdVizgui::on_w_radio_depth_activate() {
    if (w_radio_depth->get_active())
        world->draw_kinect_with_color=false;
}

void
rgbdVizgui::on_w_radio_mode_pointcloud_activate() {
    if (w_radio_mode_pointcloud->get_active()){
        reconstructMode=1;
        w_images_tool->hide();
    }
}

void
rgbdVizgui::on_w_radio_mode_image_activate() {
    if (w_radio_mode_image->get_active()){
        reconstructMode=0;
        w_images_tool->show();
    }
}

void
rgbdVizgui::on_w_radio_rgb_activate() {
    if (w_radio_rgb->get_active())
        world->draw_kinect_with_color=true;
}

void
rgbdVizgui::on_clicked_clear_lines() {
    world->clearExtraLines();
}

void
rgbdVizgui::on_w_tg_gl_toggled() {
    if (w_tg_gl->get_active()) {
        w_window_gl->show();
        w_vbox_gl->show();
    }
    else {
        w_window_gl->hide();
        w_vbox_gl->hide();
    }
}

float
rgbdVizgui::getCycle() {
    return this->cycle;
}

} // namespace
