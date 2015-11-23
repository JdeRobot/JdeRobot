/*
*  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *   			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "controller.h"

namespace rgbdManualCalibrator {

  Controller::Controller(Ice::PropertiesPtr prop, int w, int h, int nCameras) {
	cameras.resize(nCameras);
    this->gladepath = std::string("./rgbdManualCalibrator.glade");

    this->world = prop->getProperty("rgbdManualCalibrator.World.File");
    //cout << "world es " << this->world << endl;
    this->camOut = prop->getProperty("rgbdManualCalibrator.Camera.FileOut");
	cWidth=w;
	cHeight=h;
    this->drawCenter = false;

    /*Init world and configurations*/
	this->nCameras=nCameras;
    this->init(prop, nCameras);
  }

  Controller::~Controller() {
    /*delete this->camera;*/
  }
  
  std::string
  Controller::getGladePath() {
    return this->gladepath;
  }

  void
  Controller::setPos(float x, float y, float z, int cam) {
    this->cameras[cam].position.X = x;
    this->cameras[cam].position.Y = y;
    this->cameras[cam].position.Z = z;

    this->cameras[cam].foa.X += this->cameras[cam].position.X - lastx;
    this->cameras[cam].foa.Y += this->cameras[cam].position.Y - lasty;
    this->cameras[cam].foa.Z += this->cameras[cam].position.Z - lastz;

    this->lastx = x;
    this->lasty = y;
    this->lastz = z;
  }

  void
  Controller::setFoa(float x, float y, float z, int cam) {
    this->cameras[cam].foa.X = x;
    this->cameras[cam].foa.Y = y;
    this->cameras[cam].foa.Z = z;	
  }

  void
  Controller::setFdistX(float value,int cam) {
    this->cameras[cam].fdistx = value;	
  }

  void
  Controller::setFdistY(float value,int cam) {
    this->cameras[cam].fdisty = value;	
  }

  void
  Controller::setU0(float value,int cam) {
    this->cameras[cam].u0 = value;	
  }

  void
  Controller::setV0(float value,int cam) {
    this->cameras[cam].v0 = value;	
  }

  void
  Controller::setRoll(float value,int cam) {
    this->cameras[cam].roll = value*CALIBRATION_PI/180.0;	
  }

  HPoint3D *
  Controller::getPos(int cam) {
    return &(this->cameras[cam].position);	
  }

  HPoint3D *
  Controller::getFoa(int cam) {
    return &(this->cameras[cam].foa);	
  }

  float
  Controller::getFdistX(int cam) {
    return this->cameras[cam].fdistx;	
  }

  float
  Controller::getFdistY(int cam) {
    return this->cameras[cam].fdisty;	
  }

  float
  Controller::getU0(int cam) {
    return this->cameras[cam].u0;	
  }

  float
  Controller::getV0(int cam) {
    return this->cameras[cam].v0;	
  }

  float
  Controller::getRoll(int cam) {
    return this->cameras[cam].roll*180.0/CALIBRATION_PI;	
  }

  void
  Controller::changeDrawCenter() {
    this->drawCenter = !this->drawCenter;	
  }

  void
  Controller::init(Ice::PropertiesPtr prop, int nCameras) {

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

	for (int cam=0; cam< nCameras; cam++){
		
    /*Default values for camera parameters*/
		this->cameras[cam].position.X = 0.;
		this->cameras[cam].position.Y = 0.;
		this->cameras[cam].position.Z = 0.;
		this->cameras[cam].position.H = 1.;
		this->cameras[cam].foa.X = 0.;
		this->cameras[cam].foa.Y = 0.;
		this->cameras[cam].foa.Z = 0.;
		this->cameras[cam].foa.H = 1.;
		this->cameras[cam].fdistx = 300.0;
		this->cameras[cam].fdisty = 300.0;
		this->cameras[cam].v0 = cWidth/2;
		this->cameras[cam].u0 = cHeight/2;
		this->cameras[cam].roll = 0.0;
		this->cameras[cam].columns = cWidth;
		this->cameras[cam].rows = cHeight;
		this->cameras[cam].skew = 0.0;

		std::ostringstream strInd;
		strInd << "rgbdManualCalibrator.Camera." << cam << ".Calibration";

		std::string camera = prop->getProperty(strInd.str());
		xmlReader(&(this->cameras[cam]), camera.c_str());


		//old calibration
	    /*Ice::PropertyDict pd = prop->getPropertiesForPrefix("rgbdManualCalibrator.Config");
    	this->load_camera_config(pd, cam);*/
		//newOne

		update_camera_matrix(&(this->cameras[cam]));
	}

    this->lastx=0.;
    this->lasty=0.;
    this->lastz=0.;

    /*Load parameters from file*/
    
	
    

    /* for the undo operation be ready from the very beginning */
    /*save_cam(cameraOUTfile);*/ 

  }

  void
  Controller::drawWorld(cv::Mat image, int cam) {
    cv::Point p, q;
    HPoint3D p1, p2;
    int n=0;

    /*Update camera with current values*/
    update_camera_matrix(&(this->cameras[cam]));


    for(vector<HPoint3D>::iterator it = this->lines.begin(); it != this->lines.end(); it++) {
      if(n%2==0) {
	p1 = (*it);
      } else {
	p2 = (*it);
	this->drawLine(image, p1, p2, cam);
      }
      n++;
    }

    /*Draw central point*/
    if(this->drawCenter) {
      p.x = cWidth/2;
      p.y = 0;
      q.x = cWidth/2;
      q.y = cHeight-1;
      cv::line(image, p, q, cv::Scalar(255,0,0), 1, CV_AA, 0);
      p.x = 0;
      p.y = cHeight/2;
      q.x = cWidth-1;
      q.y = cHeight/2;
      cv::line(image, p, q, cv::Scalar(255,0,0), 1, CV_AA, 0);
    }
  }

  void
  Controller::drawLine(cv::Mat  src, HPoint3D pini, HPoint3D pend, int cam) {
    HPoint2D p1, p2;
    HPoint2D gooda,goodb;
    cv::Point pt1, pt2;

    project(pini,&p1,this->cameras[cam]);
    project(pend,&p2,this->cameras[cam]);

    if(displayline(p1,p2,&gooda,&goodb,this->cameras[cam])==1) {

      /*From optical to pixels*/
      pt1.x=(int)gooda.y;
      pt1.y=this->cameras[cam].rows-1-(int)gooda.x;
      pt2.x=(int)goodb.y;
      pt2.y=this->cameras[cam].rows-1-(int)goodb.x;
      
      cv::line(src, pt1, pt2, cv::Scalar(255, 0, 255, 0), 0.5, 8, 0);
    }
  }

  int
  Controller::load_camera_config(Ice::PropertyDict pd, int cam) {
    for(Ice::PropertyDict::const_iterator it = pd.begin(); it != pd.end(); it++) {

		std::istringstream sTemp;
      if((*it).first.compare("rgbdManualCalibrator.Config.Position.X")==0) {
	this->cameras[cam].position.X=(float)atof((*it).second.c_str());
	this->lastx = this->cameras[cam].position.X;
      }
      else if((*it).first.compare("rgbdManualCalibrator.Config.Position.Y")==0) {
	this->cameras[cam].position.Y=(float)atof((*it).second.c_str());
	this->lasty = this->cameras[cam].position.Y;
      }
      else if((*it).first.compare("rgbdManualCalibrator.Config.Position.Z")==0) {
	this->cameras[cam].position.Z=(float)atof((*it).second.c_str());
	this->lastz = this->cameras[cam].position.Z;
      }
      else if((*it).first.compare("rgbdManualCalibrator.Config.Position.H")==0) {
	this->cameras[cam].position.H=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.FOAPosition.X")==0) {
	this->cameras[cam].foa.X=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.FOAPosition.Y")==0) {
	this->cameras[cam].foa.Y=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.FOAPosition.Z")==0) {
	this->cameras[cam].foa.Z=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.FOAPosition.H")==0) {
	this->cameras[cam].foa.H=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Roll")==0) {
	this->cameras[cam].roll=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Fx")==0) {
	this->cameras[cam].fdistx=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Fy")==0) {
	this->cameras[cam].fdisty=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Skew")==0) {
	this->cameras[cam].skew=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.U0")==0) {
	this->cameras[cam].u0=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.V0")==0) {
	this->cameras[cam].v0=(float)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Columns")==0) {
	this->cameras[cam].columns=(int)atof((*it).second.c_str());
      } 
      else if((*it).first.compare("rgbdManualCalibrator.Config.Rows")==0) {
	this->cameras[cam].rows=(int)atof((*it).second.c_str());
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


	void Controller::add_depth_pointsImage(cv::Mat distances, cv::Mat imageRGB, rgbdManualCalibrator::DrawArea* world, int cam, int scale, int colour){
	float d;

		for (int xIm=0; xIm< cWidth; xIm++){
			for (int yIm=0; yIm<cHeight ; yIm++){
				d=distances.at<float>(yIm,xIm);


				if (d != 0 ){
					float xp,yp,zp,camx,camy,camz;
					float ux,uy,uz;
					float x,y;
					float k;
					float c1x, c1y, c1z;
					float fx,fy,fz;
					float fmod;
					float t;
					float Fx,Fy,Fz;

					HPoint2D p;
					HPoint3D pro;
					p.x=GRAPHIC_TO_OPTICAL_X(xIm,yIm,cHeight);
					p.y=GRAPHIC_TO_OPTICAL_Y(xIm,yIm,cHeight);
					p.h=1;
					backproject(&pro,p,this->cameras[cam]);
					xp=pro.X;
					yp=pro.Y;
					zp=pro.Z;

					camx=this->cameras[cam].position.X;
					camy=this->cameras[cam].position.Y;
					camz=this->cameras[cam].position.Z;

			
					//vector unitario
					float modulo;
			
					modulo = sqrt(1/(((camx-xp)*(camx-xp))+((camy-yp)*(camy-yp))+((camz-zp)*(camz-zp))));

					//mypro->mygetcameras[cam]foa(&c1x, &c1y, &c1z, 0);
					c1x=this->cameras[cam].foa.X;
					c1y=this->cameras[cam].foa.Y;
					c1z=this->cameras[cam].foa.Z;

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



					/*world->points[i][0]=distance*ux+camx;
					world->points[i][1]=distance*uy+camy;
					world->points[i][2]=distance*uz+camz;*/
					/*std::cout << xp << "," << yp << "," << zp << "," << modulo << std::endl;
					std::cout << xp-camx << "," << yp-camy<< "," << zp-camz << std::endl;
					std::cout << ux << "," << uy<< "," << uz << std::endl;*/
					//k= (80-yp)/uy;
					//std::cout << "distancia" << distance << std::endl;

					if (colour==0)
						world->add_kinect_point(t*ux + camx,t*uy+ camy,t*uz + camz,(int)imageRGB.data[3*(yIm*cWidth+xIm)],(int)imageRGB.data[3*(yIm*cWidth+xIm)+1],(int)imageRGB.data[3*(yIm*cWidth+xIm)+2]);
					else{
						int r,g,b;
						if (colour==1){
							r=255;
							g=0;
							b=0;
						}
						else if (colour==2){
							r=0;
							g=0;
							b=255;
						}
						else{
							r=0;
							g=1;
							b=0;
						}
						world->add_kinect_point(t*ux + camx,t*uy+ camy,t*uz + camz,r,g,b);
					}

				//world->add_line(distance*ux + camx,distance*uy+ camy,distance*uz + camz,camx,camy,camz);
				}
			}
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
	void 
	Controller::saveCameras(int nCameras){
		
		for (int i=0; i< nCameras; i++){
			std::ostringstream sTemp;
			sTemp << "camera-" << i << ".cfg";
			std::cout << sTemp.str() << std::endl;
			xmlWriter(cameras[i],sTemp.str().c_str());
		}
	}

} /*namespace*/

