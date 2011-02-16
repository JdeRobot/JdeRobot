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
 *
 */

#include "controller.h"

namespace calibrator {

	Controller::Controller(Ice::PropertiesPtr prop) {
		this->gladepath = std::string("./calibrator.glade");

		this->world = prop->getProperty("Calibrator.World.File");
		this->camOut = prop->getProperty("Calibrator.Camera.FileOut");

		this->drawCenter = false;

		/*Init world and configurations*/
		this->init(prop);
	}

  Controller::~Controller() {
		/*delete this->camera;*/
  }
  
  std::string
	Controller::getGladePath() {
		return this->gladepath;
  }

	void
	Controller::setPos(float x, float y, float z) {
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
	Controller::setFoa(float x, float y, float z) {
		this->camera.foa.X = x;
		this->camera.foa.Y = y;
		this->camera.foa.Z = z;	
	}

	void
	Controller::setFdistX(float value) {
		this->camera.fdistx = value;	
	}

	void
	Controller::setFdistY(float value) {
		this->camera.fdisty = value;	
	}

	void
	Controller::setU0(float value) {
		this->camera.u0 = value;	
	}

	void
	Controller::setV0(float value) {
		this->camera.v0 = value;	
	}

	void
	Controller::setRoll(float value) {
		this->camera.roll = value*CALIBRATION_PI/180.0;	
	}

	HPoint3D *
	Controller::getPos() {
		return &(this->camera.position);	
	}

	HPoint3D *
	Controller::getFoa() {
		return &(this->camera.foa);	
	}

	float
	Controller::getFdistX() {
		return this->camera.fdistx;	
	}

	float
	Controller::getFdistY() {
		return this->camera.fdisty;	
	}

	float
	Controller::getU0() {
		return this->camera.u0;	
	}

	float
	Controller::getV0() {
		return this->camera.v0;	
	}

	float
	Controller::getRoll() {
		return this->camera.roll*180.0/CALIBRATION_PI;	
	}

	void
	Controller::changeDrawCenter() {
		this->drawCenter = !this->drawCenter;	
	}

	void
	Controller::init(Ice::PropertiesPtr prop) {

		int i=0;
		FILE *worldconfig;

		/* initializes the 3D world */
		worldconfig=fopen(this->world.c_str(),"r");
		if(worldconfig==NULL){
			cerr << "Calibrator: World configuration configuration file " << this->world << " does not exits" << endl;
		}else{
			do{
				i=load_world_line(worldconfig);
			} while(i!=EOF);
			fclose(worldconfig);
		}

		this->load_world();

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

		/*Load parameters from file*/
		Ice::PropertyDict pd = prop->getPropertiesForPrefix("Calibrator.Config");
		this->load_camera_config(pd);
	
		update_camera_matrix(&(this->camera));

		/* for the undo operation be ready from the very beginning */
		/*save_cam(cameraOUTfile);*/ 

	}

	void
	Controller::drawWorld(const colorspaces::Image& image) {
		IplImage src;
		CvPoint p, q;
		HPoint3D p1, p2;
		int n=0;

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
	Controller::drawLine(IplImage * src, HPoint3D pini, HPoint3D pend) {
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

	int
	Controller::load_camera_config(Ice::PropertyDict pd) {

		for(Ice::PropertyDict::const_iterator it = pd.begin(); it != pd.end(); it++) {

			if((*it).first.compare("Calibrator.Config.Position.X")==0) {
				this->camera.position.X=(float)atof((*it).second.c_str());
				this->lastx = this->camera.position.X;
			}
			else if((*it).first.compare("Calibrator.Config.Position.Y")==0) {
				this->camera.position.Y=(float)atof((*it).second.c_str());
				this->lasty = this->camera.position.Y;
			}
			else if((*it).first.compare("Calibrator.Config.Position.Z")==0) {
				this->camera.position.Z=(float)atof((*it).second.c_str());
				this->lastz = this->camera.position.Z;
			}
			else if((*it).first.compare("Calibrator.Config.Position.H")==0) {
				this->camera.position.H=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.FOAPosition.X")==0) {
				this->camera.foa.X=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.FOAPosition.Y")==0) {
				this->camera.foa.Y=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.FOAPosition.Z")==0) {
				this->camera.foa.Z=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.FOAPosition.H")==0) {
				this->camera.foa.H=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Roll")==0) {
				this->camera.roll=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Fx")==0) {
				this->camera.fdistx=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Fy")==0) {
				this->camera.fdisty=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Skew")==0) {
				this->camera.skew=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.U0")==0) {
				this->camera.u0=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.V0")==0) {
				this->camera.v0=(float)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Columns")==0) {
				this->camera.columns=(int)atof((*it).second.c_str());
			} 
			else if((*it).first.compare("Calibrator.Config.Rows")==0) {
				this->camera.rows=(int)atof((*it).second.c_str());
			} 
		}
	}
	
	int
	Controller::load_world_line(FILE *myfile) {			

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

	int
	Controller::load_world() {	
/*
		std::string filepath = "./config-example/world.xml";

		try
		{
		  xmlpp::TextReader reader(filepath);

			while(reader.read())
		  {
		    int depth = reader.get_depth();
		    std::cout << "--- node ---" << std::endl;
		    std::cout << "name: " << reader.get_name() << std::endl;
		    std::cout << "depth: " << reader.get_depth() << std::endl;

		    if(reader.has_value())
		      std::cout << "value: '" << reader.get_value() << "'" << std::endl;
		    else
		      std::cout << "novalue" << std::endl;
		  }

		}
		catch(const std::exception& ex)
		{
		  std::cout << "Exception caught: " << ex.what() << std::endl;
		}
*/
	}	

} /*namespace*/

