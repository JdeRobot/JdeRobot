/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Eduardo Perdices <eperdes@gsyc.es>
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */


#include "adapter.h"
#include "alproxy.h"
#include "alvisionimage.h"


/*#############################################################################################
										Camera
################################################################################################*/

	/*Funciones de la clase camera en C++*/
	Camera::Camera(int width, int height, int fps) {
		this->data = 0;
		this->dataAux = 0;
		this->IP = "127.0.0.1";
		this->PORT = 9559;
		this->colorSpace = AL::kRGBColorSpace;
		this->fps = fps;
		this->name = "jderobot";
		this->time = 0;
		this->updated = 0;

		/*Image format and size*/
		this->getSizeValues(width, height);
	}

	void Camera::getSizeValues(int width, int height) {
		if(width == 640 && height == 480) {
			this->width = 640;
			this->height = 480;
			this->channels = 3;
			this->format = AL::kVGA;
		} else if(width == 320 && height == 240) {
			this->width = 320;
			this->height = 240;
			this->channels = 3;
			this->format = AL::kQVGA;
		} else if(width == 160 && height == 120) {
			this->width = 160;
			this->height = 120;
			this->channels = 3;
			this->format = AL::kQQVGA;
		} else {
			/*Default 320x240*/
			this->width = 320;
			this->height = 240;
			this->channels = 3;
			this->format = AL::kQVGA;
		}
	}

	int Camera::init() {
		try {
			/*Get proxy*/
			this->cameraProxy = new AL::ALProxy("NaoCam", IP, PORT);
			/*Nos registramos*/
			this->name = this->cameraProxy->call<std::string>("register", this->name, this->format, this->colorSpace, this->fps);
			std::cout << "Registrado en naoqi con el nombre: " << this->name << std::endl;

			/*Get memory*/
			this->dataAux = (unsigned char*)malloc(this->width*this->height*this->channels*sizeof(unsigned char));
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al conectar con naoqi: "<<e.toString()<< std::endl;
			return -1;
		}
		
		return 0;
	}

	void Camera::terminate() {
		try {
			/*Free memory*/
			if(this->dataAux != NULL)
				free(this->dataAux);
			/*Quitamos nuestro registro*/
			this->cameraProxy->callVoid("unRegister", this->name);
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al desregistrarnos de naoqi: "<<e.toString()<< std::endl;
		}
	}

	int Camera::updateImage() {
		struct timeval tmp;
		unsigned long newtime;
		long freq;

		pthread_mutex_lock(&(this->cammutex));
		/*Check last time updated*/
		freq = 1000000/(float)(this->fps);
		gettimeofday(&tmp,NULL);
		newtime = tmp.tv_sec*1000000+tmp.tv_usec;
		if((newtime - this->time) < (unsigned long)freq) {
			pthread_mutex_unlock(&(this->cammutex));
			return 0;
		}
		this->time = newtime;

		/*Get image from naoqi*/
		AL::ALValue newImage;
		newImage.arraySetSize(7);
		try{
			newImage = cameraProxy->call<AL::ALValue>("getImageRemote", name);
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al recuperar la imagen: " << e.toString() << std::endl;
		}

		/*Copy image in our buffer, to avoid naoqi to modify it*/
		this->data = (unsigned char*) static_cast<const unsigned char*>(newImage[6].GetBinary());
		memcpy(this->dataAux, this->data, this->width*this->height*this->channels*sizeof(unsigned char));
		this->channels = newImage[2];
		this->width = newImage[0];
		this->height = newImage[1];
		this->updated = 1;
		pthread_mutex_unlock(&(this->cammutex));

		return 0;
	}

	void Camera::getImage(unsigned char * data) {
		int i;
		pthread_mutex_lock(&(this->cammutex));
		/*If image was updated*/
		if(this->updated) {
			if(this->dataAux != 0 && data!=0) {
				for(i=0;i<this->width * this->height;i++) {
					/*Update RGB values*/
					data[i*3+2] = this->dataAux[i*3];
					data[i*3+1] = this->dataAux[i*3+1];
					data[i*3] = this->dataAux[i*3+2];
				}
			} 
			this->updated = 0;
		}
		pthread_mutex_unlock(&(this->cammutex));
	}

	int Camera::getWidth() {
		int width;
		pthread_mutex_lock(&(this->cammutex));
		width = this->width;
		pthread_mutex_unlock(&(this->cammutex));		
		return width; 
	}

	int Camera::getHeight() {
		int height;
		pthread_mutex_lock(&(this->cammutex));
		height = this->height;
		pthread_mutex_unlock(&(this->cammutex));		
		return height; 
	}

	/*Funciones exportadas a C*/
	Camera* newCamera(int width, int height, int fps) {
		Camera * c = new Camera(width, height, fps);
		if(initCamera(c) < 0) {
			delete c;
			return NULL;
		}			
		return c;
	}

	int initCamera(Camera* c) {
		return c->init();
	}

	void terminateCamera(Camera* c) {
		c->terminate();
	}

	void deleteCamera(Camera* c) {
		terminateCamera(c);
		delete c;
	}

	int updateImageCamera(Camera* c){
		return c->updateImage();
	}

	void getImageCamera(Camera* c, unsigned char * data){
		c->getImage(data);
	}

	int getWidthCamera(Camera* c){
		return c->getWidth();
	}

	int getHeightCamera(Camera* c){
		return c->getHeight();
	}


/*#############################################################################################
										Motion
################################################################################################*/

	motion::motion() {
		this->IP = "127.0.0.1";
		this->PORT = 9559;
		this->lasty=0;
		this->lastp=0;
	}


	int motion::init() {

			try {
				this->motionProxy = new AL::ALMotionProxy(IP, PORT);
				//this->motionProxy = new AL::ALProxy("ALMotion", IP, PORT);
			} catch(AL::ALError& e) {
				std::cerr << "NaoBody: exception connecting to NaoQi: "<<e.toString()<< std::endl;
				return -1;
			}
			this->mysteps=0;
	
			return 0;	
	}


	void motion::terminate() {
	
	}


	int motion::walk_stop_to_process(float v, float w) {
		float myv;
		float myw;
		int steps;
		float radius;

		w=-w;

		myv=-0.83*v+100;

		myv=-0.83*v+100;
		if (w<0){
			w=-w;
			myw= (MYMAXR-MYMINR)*((w-MYMINW)/(MYMAXW-MYMINW))+MYMINR;
			myw=-myw;
			radius=-myw;
		}
		else{
			myw= (MYMAXR-MYMINR)*((w-MYMINW)/(MYMAXW-MYMINW))+MYMINR;
			radius=myw;
		}
	
		if ((w!=0)||(v!=0)){
			steps=this->motionProxy->getRemainingFootStepCount();
			if (steps!=this->mysteps){
				mysteps=steps;
				if (mysteps < 2){
					if (w==0){
						if ((v>0)&&(v<=MAXV)){
							this->motionProxy->post.walkStraight(DISTANCE_A,myv);
						}
					}
					else{
						if (v==0){
							if (w<0){
								this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, -w);
							}
							else{
								this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, w);
							}
						}
						else{
							if ((v>0)&&(v<=MAXV)){
								this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, myv);
							}
						}
					}
				}
			}
			else if (steps==0){
				if (w==0){
					this->motionProxy->post.walkStraight(DISTANCE_A,myv);
				}
				else{
					if (v==0){
						if (w<0){
							this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, -w);
						}
						else{
							this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, w);
						}
					}
					else{
						this->motionProxy->post.walkArc(DISTANCE_A/myw,radius, myv);
					}
				}
			}
		}
		else{
			this->motionProxy->post.clearFootsteps();
		}
		return 0;
	}


	int motion::walk_stop_if_changes(float v, float w) {
		float myv;
		float myw;
		float radius;
		
	
		myv=-0.83*v+100;
		if (w<0){
			w=-w;
			myw= (MYMAXR-MYMINR)*((w-MYMINW)/(MYMAXW-MYMINW))+MYMINR;
			myw=-myw;
			radius=-myw;
		}
		else{
			myw= (MYMAXR-MYMINR)*((w-MYMINW)/(MYMAXW-MYMINW))+MYMINR;
			radius=myw;
	
		}
		if ((this->lastv!=v)||(this->lastw!=w)){
			this->motionProxy->post.clearFootsteps();
			this->lastv=v;
			this->lastw=w;
			if ((w!=0)||(v!=0)){
				if (w==0){
					if (v==0)
						this->motionProxy->post.walkStraight(10,50);
					else
						this->motionProxy->post.walkStraight(10,myv);
				}
				else{
					this->motionProxy->post.walkArc(10/myw,radius, myv);
				}
			}
			else{
				this->motionProxy->post.clearFootsteps();
			}
		}
		else if ((!this->motionProxy->walkIsActive())&&((v!=0)||(w!=0))){
			if (w==0){
					if (v==0)
						this->motionProxy->post.walkStraight(10,50);
					else
						this->motionProxy->post.walkStraight(10,myv);
				}
				else{
					this->motionProxy->post.walkArc(10/myw,radius, myv);
				}
		}
		return 0;
	}
	
	
	int motion::head(float y, float p, float *posy, float *posp, float vy, float vp, unsigned long int* clock){
		float ay;
		float ap;
		int clock_aux;
		float y_aux_real;
		float p_aux_real;

	
		ay=this->motionProxy->getAngle("HeadYaw") * 180 /PI;
		ap=this->motionProxy->getAngle ("HeadPitch")* 180 /PI;
		
		y_aux_real=truncf(ay*10);
		p_aux_real=truncf(ap*10);
		if ((y_aux_real!=truncf(y*10))&&(y>(-MAXY))&&(y<=MAXY)&&(vy<=MAXV)){
			this->motionProxy->post.gotoAngleWithSpeed ("HeadYaw",-y*PI/180, vy, 0);
			//this->motionProxy->post.gotoAngleWithSpeed ("HeadYaw",-y*PI/180, 10, 0);
		}
		if ((p_aux_real!=truncf(p*10))&&(p>(-MAXY))&&(y<=MAXY)&&(vp<=MAXV)){
			this->motionProxy->post.gotoAngleWithSpeed ("HeadPitch",-p*PI/180, vp, 0);
			//this->motionProxy->post.gotoAngleWithSpeed ("HeadPitch",-p*PI/180, 10, 0);
		}
		if ((y_aux_real>this->lasty+CHANGE_RANGE)||(y_aux_real<this->lasty-CHANGE_RANGE)||(p_aux_real>this->lastp+CHANGE_RANGE)||(p_aux_real<this->lastp-CHANGE_RANGE)){
			*posy=-ay;
			*posp=-ap;
			clock_aux=*clock;
			clock_aux++;
			*clock=clock_aux;
			this->lasty=truncf(ay*10);
			this->lastp=truncf(ap*10);
		}
	
		return 0;
	}


	motion* newmotion() {
		motion *m = new motion();
		if(initmotion(m) < 0) {
			delete m;
			return NULL;
		}			
		return m;
	}

	int initmotion(motion* m) {

		return m->init();
	}

	void terminatemotion(motion* m) {
		m->terminate();
	}

	int walkprocessmotion(motion* m, float v, float w){
		return m->walk_stop_to_process(v,w); 
	}

	int walkchangesmotion(motion* m, float v, float w){
		return m->walk_stop_if_changes(v,w); 
	}

	int headmotion(motion* m, float y, float p, float *posy, float *posp, float vy, float vp, unsigned long int* clock){
		return m->head(y,p,posy,posp,vy,vp,clock); 
	}

	void deletemotion(motion* m) {
		terminatemotion(m);
		delete m;
	}

