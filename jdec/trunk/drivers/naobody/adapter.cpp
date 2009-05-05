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
										Head
################################################################################################*/

	/*Funciones de la clase head en C++*/
	Head::Head() {
		this->IP = "127.0.0.1";
		this->PORT = 9559;
		this->lastValue = 1000;
	}

	int Head::init() {
		try {
			/*Get proxy*/
			this->headProxy = new AL::ALProxy("ALMotion", IP, PORT);
			//this->headProxy->callVoid("gotoAngleWithSpeed","HeadYaw",1, 50, 0);
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al conectar con naoqi: "<<e.toString()<< std::endl;
			return -1;
		}
	
		return 0;
	}	

	void Head::terminate() {
		/*Void*/
	}

	void Head::moveTo(float angle) {
		float value;

		/*No actualizamos si ya tenia ese valor*/
		if(angle == this->lastValue)
			return;

		this->lastValue = angle;
		
		value = angle*PI/180.0;
		/*Check limits*/
		if(value > 1.0) 
			value = 1.0;
		if(value < -1.0)
			value = -1.0;

		try {
			this->headProxy->callVoid("gotoAngleWithSpeed","HeadYaw",value, 50, 0);		////#########Velocidad puede ser 100
		} catch(AL::ALError& e) {
			std::cerr << "Excepción al mover la cabeza: "<<e.toString()<< std::endl;
			return;
		}
	}

	/*Funciones exportadas en C*/
	Head* newHead() {
		Head * h = new Head();
		if(initHead(h) < 0) {
			delete h;
			return NULL;
		}			
		return h;
	}

	int initHead(Head* h) {
		return h->init();
	}

	void terminateHead(Head* h) {
		h->terminate();
	}	

	void deleteHead(Head* h) {
		terminateHead(h);
		delete h;
	}

	void moveToHead(Head* h, float angle) {
		h->moveTo(angle);
	}	

	
