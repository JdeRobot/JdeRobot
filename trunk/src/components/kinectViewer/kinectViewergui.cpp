

#include "kinectViewergui.h"
#include <jderobot/pointcloud.h>

namespace kinectViewer {
	kinectViewergui::kinectViewergui(jderobot::CameraPrx rgb,jderobot::CameraPrx depth,kinectViewerController::PointCloudController* pointCloud_ctr,kinectViewerController::Pose3DMotorsController* ptmc_in, kinectViewerController::LedsController* lc_in, std::string path, std::string path_rgb, std::string path_ir, int width, int height): gtkmain(0,0) {	
		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}
	cam_rgb=rgb;
	cam_depth=depth;
	if (cam_rgb==0)
		cam_rgb_active=1;
	else
		cam_rgb_active=0;
	if (cam_depth==0)
		cam_depth_active=1;
	else
		cam_depth_active=0;
	modesAvalables=0;
	if (pointCloud_ctr != NULL){
		if ((cam_rgb==0)||(cam_depth==0)){
			reconstructMode=1;
			modesAvalables=1; //only one mode
		}
		else{
			reconstructMode=0;
			modesAvalables=2; //both modes
		}
	}
	else{
		if ((cam_rgb!=0)&&(cam_depth!=0)){
			reconstructMode=1;
			modesAvalables=1; //only point cloud mode 
		}
		else{
			reconstructMode=0;
			modesAvalables=0; // no mode available
		}
	}

	reconstruct_depth_activate=false;
	lines_depth_active=false;
	lines_rgb_active=false;
	std::cout << "Loading glade\n";
	refXml = Gnome::Glade::Xml::create("./kinectViewergui.glade");
	cWidth=width;
	cHeight=height;
	
	/*Create controller*/
	if (ptmc_in != NULL){
		ptmGui = new kinectViewerGuiModules::Pose3DMotorsGui(ptmc_in, refXml);
	}
	if (lc_in){
		lGui = new kinectViewerGuiModules::ledsGui(lc_in,refXml);
	}

	/*ojo controlar si existe */
	pointCloud=pointCloud_ctr;

	

	/*Get widgets*/
	refXml->get_widget("kinectViewer", mainwindow);
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
		
	if (cam_rgb_active){
		w_toggle_rgb->hide();
	}
	if (cam_depth_active){
		w_toggle_depth->hide();
	}
	
	w_event_rgb->signal_button_press_event().connect(sigc::mem_fun(this,&kinectViewergui::on_clicked_event_rgb));
	w_event_depth->signal_button_press_event().connect(sigc::mem_fun(this,&kinectViewergui::on_clicked_event_depth));
	w_reconstruct->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_reconstruct_depth));
	w_camera_pos->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::add_camera_position));
	w_lines_rgb->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_lines_rgb_toggled));
	w_lines_depth->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_lines_depth_toggled));
	w_tg_gl->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_tg_gl_toggled));
	w_view_controller->signal_activate().connect(sigc::mem_fun(this,&kinectViewergui::on_w_view_controller_activate));
	w_radio_depth->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_radio_depth_activate));
	w_radio_rgb->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_radio_rgb_activate));
	w_radio_mode_pointcloud->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_radio_mode_pointcloud_activate));
	w_radio_mode_image->signal_toggled().connect(sigc::mem_fun(this,&kinectViewergui::on_w_radio_mode_image_activate));
	w_button_clear_lines->signal_clicked().connect(sigc::mem_fun(this,&kinectViewergui::on_clicked_clear_lines));

	if (modesAvalables==0){
		w_reconstruct->hide();
	}

		// Mundo OpenGL
    refXml->get_widget_derived("gl_world",world);
	world->setCamerasResolution(width,height);

	std::cout << "Creating Progeos Virtual Cameras" << std::endl;
	mypro= new kinectViewer::myprogeo();
	mypro->load_cam((char*)path_rgb.c_str(),0);

	mypro->load_cam((char*)path_ir.c_str(),1);
	util = new kinectViewer::util3d(mypro);

		/*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
		world->readFile(path);
		mainwindow->show();	

	}

	kinectViewergui::~kinectViewergui() {
		//delete this->controller;
	}


void 
kinectViewergui::updateAll( const colorspaces::Image& imageRGB, const colorspaces::Image& imageDEPTH )
{
		CvPoint pt1,pt2;
			if (w_toggle_rgb->get_active()){
				colorspaces::ImageRGB8 img_rgb888(imageRGB);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageRGB->clear();
				/*si queremos pintar las lineas*/
				if (lines_rgb_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
					util->draw_room(src,0, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
	    		w_imageRGB->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			if (w_toggle_depth->get_active()){
	
				colorspaces::ImageRGB8 img_rgb888(imageDEPTH);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageDEPTH->clear();
				if (lines_depth_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
					util->draw_room(src,1, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
	    		w_imageDEPTH->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			if (reconstruct_depth_activate){
				if (reconstructMode==0)
					add_depth_pointsImage(imageDEPTH, imageRGB);
				else
					add_depth_pointsCloud();
			}
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}

void 
kinectViewergui::updateRGB( const colorspaces::Image& imageRGB)
{
		CvPoint pt1,pt2;
			if (w_toggle_rgb->get_active()){
				colorspaces::ImageRGB8 img_rgb888(imageRGB);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageRGB->clear();
				/*si queremos pintar las lineas*/
				if (lines_rgb_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
					util->draw_room(src,0, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
	    		w_imageRGB->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			if (reconstruct_depth_activate){
				if (reconstructMode!=0)
					add_depth_pointsCloud();
			}
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}

void 
kinectViewergui::updateDEPTH(const colorspaces::Image& imageDEPTH )
{
		CvPoint pt1,pt2;
			if (w_toggle_depth->get_active()){
	
				colorspaces::ImageRGB8 img_rgb888(imageDEPTH);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageDEPTH->clear();
				if (lines_depth_active){
					IplImage* src = cvCreateImage(cvSize(img_rgb888.width,img_rgb888.height), IPL_DEPTH_8U, 3);
					memcpy((unsigned char *) src->imageData, &(img_rgb888.data[0]),img_rgb888.width*img_rgb888.height * 3);
					util->draw_room(src,1, world->lines, world->numlines);
					memmove(&(img_rgb888.data[0]),(unsigned char *) src->imageData,img_rgb888.width*img_rgb888.height * 3);
					
				}
	    		w_imageDEPTH->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			if (reconstruct_depth_activate){
				if (reconstructMode!=0){
					add_depth_pointsCloud();
				}
			}
				
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}

void 
kinectViewergui::updatePointCloud( )
{
	displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			if (reconstruct_depth_activate){
				if (reconstructMode==1){
					add_depth_pointsCloud();
				}
			}
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}


	bool kinectViewergui::isClosed(){
		return false;
	}

	  bool kinectViewergui::isVisible() {
    return mainwindow->is_visible();
  }

void kinectViewergui::displayFrameRate()
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
		//fpslabel->set_label(fpsString.str());
		}
	}

bool kinectViewergui::on_clicked_event_rgb(GdkEventButton* event){
	int x,y;
	float xp,yp,zp,camx,camy,camz;
	float xu,yu,zu; 
	float k;
		
	gdk_window_at_pointer(&x,&y);
	std::cout << x << ", " << y << std::endl;
	mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,0);
	xu=(xp-camx)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
	yu=(yp-camy)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
	zu=(zp-camz)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));

	k= 5000;
	world->add_line(camx+k*xu,camy+k*yu,camz+k*zu,camx,camy,camz);
	return true;
}


bool kinectViewergui::on_clicked_event_depth(GdkEventButton* event){
	int x,y;
	float xp,yp,zp,camx,camy,camz;
	float xu,yu,zu; 
	float k;
		
	gdk_window_at_pointer(&x,&y);
	mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,1);
	xu=(xp-camx)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
	yu=(yp-camy)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));
	zu=(zp-camz)/sqrt((xp-camx)*(xp-camx) + (yp-camy)*(yp-camy) + (zp-camz)*(zp-camz));

	k= 5000;
	world->add_line(camx+k*xu,camy+k*yu,camz+k*zu,camx,camy,camz);
	
	world->add_line(k*xu + camx,k*yu + camy,k*zu + camz,camx,camy,camz);
	return true;
}

void kinectViewergui::add_cameras_position(){
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
kinectViewergui::on_reconstruct_depth(){
	if (w_reconstruct->get_active()){
		reconstruct_depth_activate=true;
		world->draw_kinect_points=true;
		w_reconstruct_selection->show();
		if (modesAvalables==2)
			w_reconstruct_mode->show();
	}
	else{
		reconstruct_depth_activate=false;
		world->draw_kinect_points=false;
		w_reconstruct_selection->hide();
	}
}

void 
kinectViewergui::add_depth_pointsImage(const colorspaces::Image& imageDEPTH, const colorspaces::Image& imageRGB){
	float distance;

		world->clear_points();
		//std::cout << "inicio reconstrucción" << std::endl;
		for( unsigned int i = 0 ; i < cWidth*cHeight ; i++) {
			if (((int)imageDEPTH.data[3*i+0]==255) && ( ((int)imageDEPTH.data[3*i+1] >0 ) && ((int)imageDEPTH.data[3*i+1] < 255)) && (((int)imageDEPTH.data[3*i+2] > 0 ) && ((int)imageDEPTH.data[3*i+2] < 255))){
					distance = -((int)imageDEPTH.data[3*i+1] - 255);
				}
				else if (((int)imageDEPTH.data[3*i+0]==255) && (((int)imageDEPTH.data[3*i+1] >= 0) && ((int)imageDEPTH.data[3*i+1]<255)) && ((int)imageDEPTH.data[3*i+2] == 0)){
					distance = 255+ (int)imageDEPTH.data[3*i+1];
				}
				else if ((((int)imageDEPTH.data[3*i+0] >= 0 ) && ((int)imageDEPTH.data[3*i+0] < 255))  && ((int)imageDEPTH.data[3*i+1]==255) && ((int)imageDEPTH.data[3*i+2] == 0)){
					distance = 2*255 - ((int)imageDEPTH.data[3*i+0] - 255);
				}
				else if (((int)imageDEPTH.data[3*i+0] == 0) && ((int)imageDEPTH.data[3*i+1]==255) && (((int)imageDEPTH.data[3*i+2] >= 0 ) && ((int)imageDEPTH.data[3*i+2] < 255))){
					distance = 3*255 + (int)imageDEPTH.data[3*i+2];
				}
				else if (((int)imageDEPTH.data[3*i+0] == 0) && ((int)imageDEPTH.data[3*i+1] >= 0 ) && ((int)imageDEPTH.data[3*i+1] < 255) && ((int)imageDEPTH.data[3*i+2]==255)){
					distance = 4*255 - ((int)imageDEPTH.data[3*i+1] - 255);
				}
				else if (((int)imageDEPTH.data[3*i+0] == 0) && ((int)imageDEPTH.data[3*i+1] == 0) && ((int)imageDEPTH.data[3*i+2] > 0 ) && ((int)imageDEPTH.data[3*i+2] < 255)){
					distance = 5*255 - ((int)imageDEPTH.data[3*i+2] - 255);
				}
				else{
					distance=0;
				}

				if (distance != 0 ){
					//PRUEBA DE DISTANCIA		
					/*if (i==153600 + 320){
						std::cout << distance << std::endl;
					}*/

				distance = distance *10;
			
				float xp,yp,zp,camx,camy,camz;
				float ux,uy,uz; 
				float x,y;
				float k;
				float c1x, c1y, c1z;
				float fx,fy,fz;
				float fmod;
				float t;
				float Fx,Fy,Fz;
			
		
			
				mypro->mybackproject(i % cWidth, i / cWidth, &xp, &yp, &zp, &camx, &camy, &camz,0);
			
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

				Fx= distance*fx + camx;
				Fy= distance*fy + camy;
				Fz= distance*fz + camz;

				/* calculamos el punto real */
				t = (-(fx*camx) + (fx*Fx) - (fy*camy) + (fy*Fy) - (fz*camz) + (fz*Fz))/((fx*ux) + (fy*uy) + (fz*uz));

			

				/*world->points[i][0]=distance*ux+camx;
				world->points[i][1]=distance*uy+camy;
				world->points[i][2]=distance*uz+camz;*/
				/*std::cout << xp << "," << yp << "," << zp << "," << modulo << std::endl;
				std::cout << xp-camx << "," << yp-camy<< "," << zp-camz << std::endl;
				std::cout << ux << "," << uy<< "," << uz << std::endl;*/
				//k= (80-yp)/uy;
				//std::cout << "distancia" << distance << std::endl;
			
				world->add_kinect_point(t*ux + camx,t*uy+ camy,t*uz + camz,(int)imageRGB.data[3*i],(int)imageRGB.data[3*i+1],(int)imageRGB.data[3*i+2]);

				//world->add_line(distance*ux + camx,distance*uy+ camy,distance*uz + camz,camx,camy,camz);
				}
		}

	//std::cout << "fin reconstrucción" << std::endl;
}

void 
kinectViewergui::add_depth_pointsCloud(){
	world->clear_points();
	std::vector<jderobot::RGBPoint> cloud;
	cloud=pointCloud->getCloud();

	for (std::vector<jderobot::RGBPoint>::iterator it = cloud.begin(); it != cloud.end(); ++it){
		world->add_kinect_point(it->x,it->y,it->z,(int)it->r,(int)it->g,(int)it->b);
	}
}

void 
kinectViewergui::add_camera_position(){
	float c1x, c1y, c1z, c2x, c2y, c2z, c3x, c3y, c3z, c4x, c4y,c4z;
	float camx, camy, camz;
	float w,h;
	float modulo,distance;
	

	if (w_camera_pos->get_active()){
		distance=300;
		mypro->mygetcamerasize(&w,&h,1);
		mypro->mybackproject(0,0,&c1x,&c1y,&c1z,&camx, &camy, &camz,1);
		mypro->mybackproject(0,cWidth,&c2x,&c2y,&c2z,&camx, &camy, &camz,1);
		mypro->mybackproject(cHeight,0,&c3x,&c3y,&c3z,&camx, &camy, &camz,1);
		mypro->mybackproject(cHeight,cWidth,&c4x,&c4y,&c4z,&camx, &camy, &camz,1);
		
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
	}
	else{
		world->clear_camera_lines();
	}


	//mypro->mybackproject(x, y, &xp, &yp, &zp, &camx, &camy, &camz,1);
}

void 
kinectViewergui::on_w_lines_rgb_toggled(){
	if (w_lines_rgb->get_active()){
		lines_rgb_active=true;
	}
	else
		lines_rgb_active=false;
}

void 
kinectViewergui::on_w_lines_depth_toggled(){
	if (w_lines_depth->get_active()){
		lines_depth_active=true;
	}
	else
		lines_depth_active=false;
}

void
kinectViewergui::on_w_view_controller_activate(){
	w_window_controller->show();
}

void
kinectViewergui::on_w_radio_depth_activate(){
	if (w_radio_depth->get_active())
		world->draw_kinect_with_color=false;
}

void
kinectViewergui::on_w_radio_mode_pointcloud_activate(){
	if (w_radio_mode_pointcloud->get_active())
		reconstructMode=0;
}

void
kinectViewergui::on_w_radio_mode_image_activate(){
	if (w_radio_mode_image->get_active())
		reconstructMode=1;
}

void
kinectViewergui::on_w_radio_rgb_activate(){
	if (w_radio_rgb->get_active())
		world->draw_kinect_with_color=true;
}

void
kinectViewergui::on_clicked_clear_lines(){
	world->clearExtraLines();
}

void 
kinectViewergui::on_w_tg_gl_toggled(){
	if (w_tg_gl->get_active()){
		w_window_gl->show();
		w_vbox_gl->show();
	}
	else{
		w_window_gl->hide();
		w_vbox_gl->hide();
	}
}


} // namespace
