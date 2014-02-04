/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *             Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#include "module_extrinsics.h"

namespace calibrator {

	Module_Extrinsics::Module_Extrinsics(Ice::PropertiesPtr prop) {
		std::cout << std::string(GLADE_DIR) + std::string("/calibrator.glade") << std::endl;
		this->gladepath = std::string(std::string(GLADE_DIR) + "/calibrator.glade");

		this->world = prop->getProperty("Calibrator.World.File");
		//this->camOut = prop->getProperty("Calibrator.Camera.FileOut");

		this->drawCenter = false;

		/*Init world and configurations*/
		this->init(prop);
	}

  Module_Extrinsics::~Module_Extrinsics() {
		/*delete this->camera;*/
  }
  
  std::string
	Module_Extrinsics::getGladePath() {
		return this->gladepath;
  }

	void
	Module_Extrinsics::setPos(float x, float y, float z) {
		this->camera.position.X = x;
		this->camera.position.Y = y;
		this->camera.position.Z = z;

		this->camera.foa.X += this->camera.position.X - lastx;
		this->camera.foa.Y += this->camera.position.Y - lasty;
		this->camera.foa.Z += this->camera.position.Z - lastz;

		this->lastx = x;
		this->lasty = y;
		this->lastz = z;
	}

	void
	Module_Extrinsics::setFoa(float x, float y, float z) {
		this->camera.foa.X = x;
		this->camera.foa.Y = y;
		this->camera.foa.Z = z;	
	}

	void
	Module_Extrinsics::setFdistX(float value) {
		this->camera.fdistx = value;	
	}

	void
	Module_Extrinsics::setFdistY(float value) {
		this->camera.fdisty = value;	
	}

	void
	Module_Extrinsics::setU0(float value) {
		this->camera.u0 = value;	
	}

	void
	Module_Extrinsics::setV0(float value) {
		this->camera.v0 = value;	
	}

	void
	Module_Extrinsics::setRoll(float value) {
		this->camera.roll = value*CALIBRATION_PI/180.0;	
	}

	HPoint3D *
	Module_Extrinsics::getPos() {
		return &(this->camera.position);	
	}

	HPoint3D *
	Module_Extrinsics::getFoa() {
		return &(this->camera.foa);	
	}

	float
	Module_Extrinsics::getFdistX() {
		return this->camera.fdistx;	
	}

	float
	Module_Extrinsics::getFdistY() {
		return this->camera.fdisty;	
	}

	float
	Module_Extrinsics::getU0() {
		return this->camera.u0;	
	}

	float
	Module_Extrinsics::getV0() {
		return this->camera.v0;	
	}

	float
	Module_Extrinsics::getRoll() {
		return this->camera.roll*180.0/CALIBRATION_PI;	
	}

	TPinHoleCamera Module_Extrinsics::getCam(){
		return this->camera;
	}

	void Module_Extrinsics::setCam(TPinHoleCamera camera){

		setPos(camera.position.X, camera.position.Y, camera.position.Z);
		setFoa(camera.foa.X, camera.foa.Y, camera.foa.Z);
		setFdistX(camera.fdistx);
		setFdistY(camera.fdisty);
		setU0(camera.u0);
		setV0(camera.v0);	
		setRoll(camera.roll);

		update_camera_matrix(&(this->camera));

		vscale_pos_x->set_value((double)this->getPos()->X);
		vscale_pos_y->set_value((double)this->getPos()->Y);
		vscale_pos_z->set_value((double)this->getPos()->Z);
		vscale_foa_x->set_value((double)this->getFoa()->X);
		vscale_foa_y->set_value((double)this->getFoa()->Y);
		vscale_foa_z->set_value((double)this->getFoa()->Z);		
		vscale_fx->set_value((double)this->getFdistX());
		vscale_fy->set_value((double)this->getFdistY());
		vscale_u0->set_value((double)this->getU0());
		vscale_v0->set_value((double)this->getV0());
		vscale_roll->set_value((double)this->getRoll());

	}

	void
	Module_Extrinsics::changeDrawCenter() {
		this->drawCenter = !this->drawCenter;	
	}

	void
	Module_Extrinsics::init(Ice::PropertiesPtr prop) {
		std::cout << "init\n";
		int i=0;
		FILE *worldconfig;

		/* initializes the 3D world */
		worldconfig=fopen(this->world.c_str(),"r");
		if(worldconfig==NULL){
			cerr << "Calibrator: World configuration file " << this->world << " does not exits" << endl;
		}else{
			do{
				i=load_world_line(worldconfig);
			} while(i!=EOF);
			fclose(worldconfig);
		}

		

		/*Default values for camera parameters*/
		this->camera.position.X = 0.;
		this->camera.position.Y = 0.;
		this->camera.position.Z = 0.;
		this->camera.position.H = 1.;
		this->camera.foa.X = 0.;
		this->camera.foa.Y = 0.;
		this->camera.foa.Z = 0.;
		this->camera.foa.H = 1.;
		this->camera.fdistx = 300.0;
		this->camera.fdisty = 300.0;
		this->camera.v0 = IMAGE_WIDTH/2;
		this->camera.u0 = IMAGE_HEIGHT/2;
		this->camera.roll = 0.0;
		this->camera.columns = IMAGE_WIDTH;
		this->camera.rows = IMAGE_HEIGHT;
		this->camera.skew = 0.0;

		this->lastx=0.;
		this->lasty=0.;
		this->lastz=0.;

		update_camera_matrix(&(this->camera));

	}
	
	void Module_Extrinsics::resetLines(){
        this->lines.clear();	
	}

	void
	Module_Extrinsics::display(const colorspaces::Image& image) {
		IplImage src;
		CvPoint p, q;
		HPoint3D p1, p2;
		int n=0;

		Glib::RefPtr<Gdk::Pixbuf> imgBuff_extrinsics = Gdk::Pixbuf::create_from_data((const guint8*)image.data,
			Gdk::COLORSPACE_RGB,
			false,
			8,
			image.width,
			image.height,
			image.step);
		gtk_image->clear();
		gtk_image->set(imgBuff_extrinsics);

		/*Update camera with current values*/
		update_camera_matrix(&(this->camera));

		src=image;

		for(vector<HPoint3D>::iterator it = this->lines.begin(); it != this->lines.end(); it++) {
			if(n%2==0) {
				p1 = (*it);
			} else {
				p2 = (*it);
				this->drawLine(&src, p1, p2);
			}
			n++;
		}

		/*Draw central point*/
		if(this->drawCenter) {
			p.x = IMAGE_WIDTH/2;
			p.y = 0;
			q.x = IMAGE_WIDTH/2;
			q.y = IMAGE_HEIGHT-1;
			cvLine(&src, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);
			p.x = 0;
			p.y = IMAGE_HEIGHT/2;
			q.x = IMAGE_WIDTH-1;
			q.y = IMAGE_HEIGHT/2;
			cvLine(&src, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);
		}

	}

	void
	Module_Extrinsics::drawLine(IplImage * src, HPoint3D pini, HPoint3D pend) {
		HPoint2D p1, p2;
		HPoint2D gooda,goodb;
		CvPoint pt1, pt2;

		project(pini,&p1,this->camera);
    		project(pend,&p2,this->camera);

		if(displayline(p1,p2,&gooda,&goodb,this->camera)==1) {

			/*From optical to pixels*/
			pt1.x=(int)gooda.y;
			pt1.y=this->camera.rows-1-(int)gooda.x;
			pt2.x=(int)goodb.y;
			pt2.y=this->camera.rows-1-(int)goodb.x;
      
			cvLine(src, pt1, pt2, cvScalar(255, 0, 255, 0), 2, 8, 0);
    		}

	}
	
	int Module_Extrinsics::load_world_line(FILE *myfile) {			

		int limit = 256;		
		char word1[limit],word2[limit],word3[limit],word4[limit],word5[limit];
		char word6[limit],word7[limit],word8[limit];
		char word[limit];
		int i=0;
		char buffer_file[limit]; 
		HPoint3D p3d;  

		buffer_file[0]=fgetc(myfile);
		if (feof(myfile)) return EOF;
		if (buffer_file[0]==(char)255) return EOF; 
		if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
		if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
		if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

		/* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
		while((buffer_file[i]!='\n') && 
		(buffer_file[i] != (char)255) &&  
		(i<limit-1) ) {
			buffer_file[++i]=fgetc(myfile);
		}

		if (i >= limit-1) { 
			printf("%s...\n", buffer_file); 
			printf ("Line too long in config file!\n"); 
			exit(-1);
		}
		buffer_file[++i]='\0';


		if (sscanf(buffer_file,"%s",word)!=1) return 0; 
		/* return EOF; empty line*/
		else {
			if(strcmp(word,"worldline")==0){
				sscanf(buffer_file,"%s %s %s %s %s %s %s %s %s",word,word1,word2,word3,word4,word5,word6,word7,word8);

				p3d.X = (float)atof(word1); p3d.Y = (float)atof(word2); p3d.Z = (float)atof(word3); p3d.H = (float)atof(word4);
				this->lines.push_back(p3d);
				p3d.X = (float)atof(word5); p3d.Y = (float)atof(word6); p3d.Z = (float)atof(word7); p3d.H = (float)atof(word8);
				this->lines.push_back(p3d);
			}/*
			else if (strcmp(word,"max_room_x")==0){
				sscanf(buffer_file,"%s %s",word,word1);
				max_room_x=(float)atof(word1);
			}
			else if (strcmp(word,"max_room_y")==0){
				sscanf(buffer_file,"%s %s",word,word1);
				max_room_y=(float)atof(word1);
			}
			else if (strcmp(word,"max_room_z")==0){
				sscanf(buffer_file,"%s %s",word,word1);
				max_room_z=(float)atof(word1);
			}*/
			return 1;
		}

	}


/** Extrinsics **/
void Module_Extrinsics::pos_x_changed(){
		this->setPos((float)vscale_pos_x->get_value(), this->camera.position.Y, this->camera.position.Z); 
		vscale_foa_x->set_value((double)this->getFoa()->X);
		vscale_foa_y->set_value((double)this->getFoa()->Y);
		vscale_foa_z->set_value((double)this->getFoa()->Z);
		
  }

	void Module_Extrinsics::pos_y_changed(){
		this->setPos(this->camera.position.X, (float)vscale_pos_y->get_value(), this->camera.position.Z); 
		vscale_foa_x->set_value((double)this->getFoa()->X);
		vscale_foa_y->set_value((double)this->getFoa()->Y);
		vscale_foa_z->set_value((double)this->getFoa()->Z);
		
  }

	void Module_Extrinsics::pos_z_changed(){
		this->setPos(this->camera.position.X, this->camera.position.Y, (float)vscale_pos_z->get_value());
		vscale_foa_x->set_value((double)this->getFoa()->X);
		vscale_foa_y->set_value((double)this->getFoa()->Y);
		vscale_foa_z->set_value((double)this->getFoa()->Z); 
  }

	void Module_Extrinsics::foa_x_changed(){
		this->setFoa((float)vscale_foa_x->get_value(), this->camera.foa.Y, this->camera.foa.Z);
  }

	void Module_Extrinsics::foa_y_changed(){
		this->setFoa(this->camera.foa.X, (float)vscale_foa_y->get_value(), this->camera.foa.Z); 
  }

	void Module_Extrinsics::foa_z_changed(){
		this->setFoa(this->camera.foa.X, this->camera.foa.Y, (float)vscale_foa_z->get_value()); 
  }

	void Module_Extrinsics::fx_changed(){
		this->setFdistX((float)vscale_fx->get_value()); 
  }

	void Module_Extrinsics::fy_changed(){
		this->setFdistY((float)vscale_fy->get_value()); 		
  }

	void Module_Extrinsics::u0_changed(){
		this->setU0((float)vscale_u0->get_value()); 
  }

	void Module_Extrinsics::v0_changed(){
		this->setV0((float)vscale_v0->get_value()); 
  }

	void Module_Extrinsics::roll_changed(){
		this->setRoll((float)vscale_roll->get_value()); 
  }

	void Module_Extrinsics::button_center_clicked(){
		this->changeDrawCenter();
  }
  
    void Module_Extrinsics::button_load_clicked(const char* filename)
    {

		int i=0;
		FILE *worldconfig;


                worldconfig=fopen(filename,"r");
		//worldconfig=fopen(dialog.get_filename().data(),"r");
                
                if(worldconfig==NULL){
                    cerr << "Calibrator: World configuration configuration file error: " << filename << endl;
                }else{
                    this->resetLines();
                    do{
                        i=this->load_world_line(worldconfig);
                    } while(i!=EOF);
                fclose(worldconfig);
                }

                
    }

void Module_Extrinsics::get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml){

		refXml->get_widget("pos_x",vscale_pos_x);
		refXml->get_widget("pos_y",vscale_pos_y);
		refXml->get_widget("pos_z",vscale_pos_z);
		refXml->get_widget("foa_x",vscale_foa_x);
		refXml->get_widget("foa_y",vscale_foa_y);
		refXml->get_widget("foa_z",vscale_foa_z);
		refXml->get_widget("fx",vscale_fx);
		refXml->get_widget("fy",vscale_fy);
		refXml->get_widget("u0",vscale_u0);
		refXml->get_widget("v0",vscale_v0);
		refXml->get_widget("roll",vscale_roll);

		refXml->get_widget("image_extrinsics",gtk_image);	
		refXml->get_widget("button_center",button_center);	
		refXml->get_widget("button_save",button_save);	
				
		/*Create callbacks*/
		vscale_pos_x->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::pos_x_changed));
		vscale_pos_y->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::pos_y_changed));
		vscale_pos_z->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::pos_z_changed));
		vscale_foa_x->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::foa_x_changed));
		vscale_foa_y->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::foa_y_changed));
		vscale_foa_z->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::foa_z_changed));
		vscale_fx->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::fx_changed));
		vscale_fy->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::fy_changed));
		vscale_u0->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::u0_changed));
		vscale_v0->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::v0_changed));
		vscale_roll->signal_value_changed().connect(sigc::mem_fun(this,&Module_Extrinsics::roll_changed));
		button_center->signal_clicked().connect(sigc::mem_fun(this,&Module_Extrinsics::button_center_clicked));
}

void Module_Extrinsics::set_mainwindow(Gtk::Window* mainwindow){
	this->mainwindow = mainwindow;
}


} /*namespace*/

