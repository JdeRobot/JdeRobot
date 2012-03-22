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

/** \file cameraAdapter.cpp
 * \brief cameraAdapter definitios
 */


#include "cameraAdapter.h"

#define REALNAO 0
#define CAMERA 0

namespace NaoAdapter{ 
	/**
	* \brief NaoCamera class constructor
	* \param width the width of the image captured with the camera
	* \param hight the height of the image captured with the camera
	* \param fps the frames per second that the camera will capture
	* \param IP the ip of the robot
	* \param port the port of the robot
	*/
	NaoCamera::NaoCamera(int width, int height, int fps, char* IP, int port) {

		std::stringstream namestr;
		srand(time);
		namestr << "jderobot" << rand();
		this->data = 0;
		this->dataAux = 0;
		this->IP = IP;
		this->PORT = port;
		this->colorSpace = AL::kRGBColorSpace;
		this->fps = fps;
		this->name = namestr.str();
		this->time = 0;
		this->updated = 0;

		/*Image format and size*/
		this->getSizeValues(width, height);
	}

	/**
	* \brief Funtion that sets the format of the image depending on its dimensions.
	* \param width the width of the image captured with the camera
	* \param hight the height of the image captured with the camera
	*/
	void NaoCamera::getSizeValues(int width, int height) {
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

	/**
	* \brief Funtion that create the proxy to the robot camera.
	* \return 0 if ok, otherwise -1
	*/
	int NaoCamera::init() {
		try {
			/*Get proxy*/
			this->NaoCameraProxy = new AL::ALProxy("ALVideoDevice", IP, PORT);
			/*Nos registramos*/
			this->name = this->NaoCameraProxy->call<std::string>("subscribe", this->name, this->format, this->colorSpace, this->fps);
			std::cout << "Registrado en naoqi con el nombre: " << this->name << std::endl;
			this->NaoCameraProxy->callVoid("setParam", AL::kCameraSelectID, CAMERA); /*###################*/
			/*Get memory*/
			this->dataAux = (unsigned char*)malloc(this->width*this->height*this->channels*sizeof(unsigned char));
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al conectar con naoqi: "<<e.toString()<< std::endl;
			return -1;
		}
		pthread_mutex_init(&this->cammutex,NULL);
		std::cerr << "Módulo registrado y conectado correctamente" << std::endl;
		return 0;
	}

	/**
	* \brief Funtion that deletes the proxy to the robot camera.
	*/
	void NaoCamera::terminate() {
		try {
			/*Free memory*/
			if(this->dataAux != NULL)
				free(this->dataAux);
			/*Quitamos nuestro registro*/
			this->NaoCameraProxy->callVoid("unsubscribe", this->name);
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al desregistrarnos de naoqi: "<<e.toString()<< std::endl;
		}
	}

	/**
	* \brief Funtion that updates the image.
	* \return 0 if ok, otherwise -1
	*/
	int NaoCamera::updateImage() {
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
			newImage = NaoCameraProxy->call<AL::ALValue>("getImageRemote", name);
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

	/**
	* \brief Funtion that gets the image from the camera.
	* \param data the reference to locate the image
	*/
	void NaoCamera::getImage(unsigned char * data) {
		int i;
		pthread_mutex_lock(&(this->cammutex));
		/*If image was updated*/
		if(this->updated) {
			if(this->dataAux != 0 && data!=0) {
				for(i=0;i<this->width * this->height;i++) {
					/*Update RGB values*/
					data[i*3+2] = this->dataAux[i*3+2];
					data[i*3+1] = this->dataAux[i*3+1];
					data[i*3] = this->dataAux[i*3];
				}
			} 
			this->updated = 0;
		}
		pthread_mutex_unlock(&(this->cammutex));
	}

	/**
	* \brief Funtion that return the width of the camera.
	* \return the widht of the image
	*/
	int NaoCamera::getWidth() {
		int width;
		pthread_mutex_lock(&(this->cammutex));
		width = this->width;
		pthread_mutex_unlock(&(this->cammutex));		
		return width; 
	}

	/**
	* \brief Funtion that return the height of the camera.
	* \return the height of the image
	*/
	int NaoCamera::getHeight() {
		int height;
		pthread_mutex_lock(&(this->cammutex));
		height = this->height;
		pthread_mutex_unlock(&(this->cammutex));		
		return height; 
	}
}
