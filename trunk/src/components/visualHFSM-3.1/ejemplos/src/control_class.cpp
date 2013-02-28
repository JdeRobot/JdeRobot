#include "control_class.h"
#include "gui.h"


mycomponent::Control *control;

typedef enum Estado_Sub_1 {
	ChocaGira,
	Vueltas
}Estado_Sub_1;

const char *Nombres_Sub_1[] = {
	"ChocaGira",
	"Vueltas"
};

typedef enum Estado_Sub_2 {
	aIzquierda,
	aIzquierda_ghost,
	aDerecha,
	aDerecha_ghost
}Estado_Sub_2;

const char *Nombres_Sub_2[] = {
	"aIzquierda",
	"aDerecha"
};

typedef enum Estado_Sub_3 {
	avanzando,
	avanzando_ghost,
	parando,
	parando_ghost,
	marcha_atras,
	marcha_atras_ghost,
	girando,
	girando_ghost
}Estado_Sub_3;

const char *Nombres_Sub_3[] = {
	"avanzando",
	"parando",
	"marcha_atras",
	"girando"
};


Estado_Sub_1 Sub_1=ChocaGira;
Estado_Sub_2 Sub_2=aIzquierda_ghost;
Estado_Sub_3 Sub_3=avanzando_ghost;


namespace mycomponent {

void Control::handleCameras(){

		this->data1 = cprx1->getImageData();
		colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(this->data1->description->format);
		if (!fmt1)
		    throw "Format not supported";

		this->image1 = new colorspaces::Image (this->data1->description->width, this->data1->description->height, fmt1, &(this->data1->pixelData[0])); // Prepare the image to use with openCV 

		this->data2 = cprx2->getImageData();
		colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(this->data2->description->format);
		if (!fmt2)
			throw "Format not supported";

		this->image2 = new colorspaces::Image (this->data2->description->width, this->data2->description->height, fmt2, &(this->data2->pixelData[0])); // Prepare the image to use with openCV

}

void Control::resetControl(){

this->mprx->setV(0);
this->mprx->setW(0);
this->mprx->setL(0);

}

Control::~Control() {}

}

void *showGui(void*){
		mycomponent::Gui *gui;
		struct timeval a, b;	
		int cycle = 100;
		long totalb,totala;
		long diff;

using namespace std;
		string tab="";

		gui = new mycomponent::Gui(control);

		while(true){

			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;

			system("clear");

			printf("%s\n", Nombres_Sub_1[Sub_1]);
			tab = tab+"   ";

			if (Sub_2%2 == 0){
				printf("%s%s\n", tab.c_str(), Nombres_Sub_2[Sub_2/2]);
				tab = tab+"   ";
			}

			if (Sub_3%2 == 0){
				printf("%s%s\n", tab.c_str(), Nombres_Sub_3[Sub_3/2]);
				tab = tab+"   ";
			}

			tab = "";
			gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;
			diff = (totalb-totala)/1000;

			if(diff < 0 || diff > cycle)
				diff = cycle;
			else
				diff = cycle-diff;

			usleep(diff*1000);
			if(diff < 33)
				usleep(33*1000);
		}
}




void *motor_sub_1(void*){

		struct timeval a, b;
		int cycle=200;
		long totalb,totala;
		long diff;
		time_t t_ini;
		time_t t_fin;
		double secs;
		bool t_activated;

		int i;


		while(true){
		gettimeofday(&a,NULL);
		totala=a.tv_sec*1000000+a.tv_usec;

		//Switch de evaluación de transiciones
		switch (Sub_1){
			case ChocaGira:
			{
					if (!t_activated){
						t_ini = time(NULL);
						t_activated = true;
					}else{
						t_fin = time(NULL);
						secs = difftime(t_fin, t_ini);
						if (secs > (double) 30){
							Sub_1=Vueltas;
							t_activated = false;
						}
					}
				break;
			}
			case Vueltas:
			{
					if (!t_activated){
						t_ini = time(NULL);
						t_activated = true;
					}else{
						t_fin = time(NULL);
						secs = difftime(t_fin, t_ini);
						if (secs > (double) 30){
							Sub_1=ChocaGira;
							t_activated = false;
						}
					}
				break;
			}
		}
		//Switch de actuacion
		switch (Sub_1){
			case ChocaGira:
			{
			i=0;
				break;
			}
			case Vueltas:
			{
			i=0;
				break;
			}
		}


//Motor temporal del subautomata

		gettimeofday(&b,NULL);
		totalb=b.tv_sec*1000000+b.tv_usec;
		diff = (totalb-totala)/1000;
		if(diff < 0 || diff > cycle)
			diff = cycle;
		else
			diff = cycle-diff;

		//Sleep Algorithm
			usleep(diff*1000);
		if(diff < 33)
			usleep(33*1000);
	 }
}



void *motor_sub_2(void*){

		struct timeval a, b;
		int cycle=100;
		long totalb,totala;
		long diff;
		time_t t_ini;
		time_t t_fin;
		double secs;
		bool t_activated;

		


		while(true){
		gettimeofday(&a,NULL);
		totala=a.tv_sec*1000000+a.tv_usec;

		if (Sub_1==Vueltas){
			if(Sub_2==aIzquierda_ghost || Sub_2==aDerecha_ghost){
			Sub_2= (Estado_Sub_2)(Sub_2-1);
		}
		//Switch de evaluación de transiciones
		switch (Sub_2){
			case aIzquierda:
			{
					if (!t_activated){
						t_ini = time(NULL);
						t_activated = true;
					}else{
						t_fin = time(NULL);
						secs = difftime(t_fin, t_ini);
						if (secs > (double) 10){
							Sub_2=aDerecha;
							t_activated = false;
						}
					}
				break;
			}
			case aDerecha:
			{
					if (!t_activated){
						t_ini = time(NULL);
						t_activated = true;
					}else{
						t_fin = time(NULL);
						secs = difftime(t_fin, t_ini);
						if (secs > (double) 10){
							Sub_2=aIzquierda;
							t_activated = false;
						}
					}
				break;
			}
		}
		//Switch de actuacion
		switch (Sub_2){
			case aIzquierda:
			{
			control->mprx->setW(20);
				break;
			}
			case aDerecha:
			{
			control->mprx->setW(-20);
				break;
			}
		}
}else{
		if(Sub_2==aIzquierda || Sub_2==aDerecha){
			Sub_2= (Estado_Sub_2)(Sub_2+1);
		}
	}


//Motor temporal del subautomata

		gettimeofday(&b,NULL);
		totalb=b.tv_sec*1000000+b.tv_usec;
		diff = (totalb-totala)/1000;
		if(diff < 0 || diff > cycle)
			diff = cycle;
		else
			diff = cycle-diff;

		//Sleep Algorithm
			usleep(diff*1000);
		if(diff < 33)
			usleep(33*1000);
	 }
}



void *motor_sub_3(void*){

		struct timeval a, b;
		int cycle=100;
		long totalb,totala;
		long diff;
		time_t t_ini;
		time_t t_fin;
		double secs;
		bool t_activated;

		float v;

v=control->mprx->getV();
control->ld=control->lprx->getLaserData();


		while(true){
		gettimeofday(&a,NULL);
		totala=a.tv_sec*1000000+a.tv_usec;

		if (Sub_1==ChocaGira){
			if(Sub_3==avanzando_ghost || Sub_3==parando_ghost || Sub_3==marcha_atras_ghost || Sub_3==girando_ghost){
			Sub_3= (Estado_Sub_3)(Sub_3-1);
		}
		//Switch de evaluación de transiciones
		switch (Sub_3){
			case avanzando:
			{
					if (control->ld->distanceData[45] <= 1000.0){
						Sub_3=parando;
					}
				break;
			}
			case parando:
			{
					if (v <= 0.1){
						Sub_3=marcha_atras;
					}
				break;
			}
			case marcha_atras:
			{
					if (control->ld->distanceData[45] > 1300.0){
						Sub_3=girando;
					}
				break;
			}
			case girando:
			{
					if (!t_activated){
						t_ini = time(NULL);
						t_activated = true;
					}else{
						t_fin = time(NULL);
						secs = difftime(t_fin, t_ini);
						if (secs > (double) 3){
							Sub_3=avanzando;
							t_activated = false;
						}
					}
				break;
			}
		}
		//Switch de actuacion
		switch (Sub_3){
			case avanzando:
			{
			control->mprx->setW(0.);
control->mprx->setV(60.);

control->ld=control->lprx->getLaserData();
				break;
			}
			case parando:
			{
			control->mprx->setV(0.);

v=control->mprx->getV();
				break;
			}
			case marcha_atras:
			{
			control->mprx->setV(-30.);

control->ld=control->lprx->getLaserData();
				break;
			}
			case girando:
			{
			control->mprx->setW(30);
				break;
			}
		}
}else{
		if(Sub_3==avanzando || Sub_3==parando || Sub_3==marcha_atras || Sub_3==girando){
			Sub_3= (Estado_Sub_3)(Sub_3+1);
		}
	}


//Motor temporal del subautomata

		gettimeofday(&b,NULL);
		totalb=b.tv_sec*1000000+b.tv_usec;
		diff = (totalb-totala)/1000;
		if(diff < 0 || diff > cycle)
			diff = cycle;
		else
			diff = cycle-diff;

		//Sleep Algorithm
			usleep(diff*1000);
		if(diff < 33)
			usleep(33*1000);
	 }
}


int main(int argc, char** argv){


		int status;
		Ice::CommunicatorPtr ic;
		struct timeval a, b;
		int cycle = 300;
		long totalb,totala;
		long diff;
		bool guiActivated=0;
		bool controlActivated=0;

		control = new mycomponent::Control();

		pthread_mutex_init(&control->controlGui, NULL);
		controlActivated=1;
	    guiActivated=1;
		pthread_create(&control->thr_gui, NULL, &showGui, NULL);
	  try{

		ic = Ice::initialize(argc,argv);
			//-----------------ICE----------------//
		// Contact to LASER interface
		Ice::ObjectPrx baseLaser = ic->propertyToProxy("Mycomponent.Laser.Proxy");
		if (0==baseLaser)
		throw "Could not create proxy with laser";

		// Cast to laser
		control->lprx = jderobot::LaserPrx::checkedCast(baseLaser);
		if (0== control->lprx)
			throw "Invalid proxy Mycomponent.Laser.Proxy";

		// Contact to MOTORS interface
		Ice::ObjectPrx baseMotors = ic->propertyToProxy("Mycomponent.Motors.Proxy");
		if (0==baseMotors)
			throw "Could not create proxy with motors";

		// Cast to motors
		 control->mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
		if (0== control->mprx)
			throw "Invalid proxy Mycomponent.Motors.Proxy";

		// Contact to ENCODERS interface
		Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Mycomponent.Encoders.Proxy");
		if (0==baseEncoders)
			throw "Could not create proxy with encoders";

		// Cast to encoders
		control->eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
		if (0== control->eprx)
			throw "Invalid proxy Mycomponent.Encoders.Proxy";

		// Contact to PTMOTORS interface
		Ice::ObjectPrx ptmotors1 = ic->propertyToProxy("Mycomponent.PTMotors1.Proxy");
		if (0==ptmotors1)
			throw "Could not create proxy with motors";

		// Cast to ptmotors
		control->ptmprx1 = jderobot::PTMotorsPrx::checkedCast(ptmotors1);
		if (0== control->ptmprx1)
			throw "Invalid proxy Mycomponent.PTMotors1.Proxy";

		// Contact to PTMOTORS interface
		Ice::ObjectPrx ptmotors2 = ic->propertyToProxy("Mycomponent.PTMotors2.Proxy");
		if (0==ptmotors2)
			throw "Could not create proxy with motors";
		// Cast to ptmotors

		control->ptmprx2 = jderobot::PTMotorsPrx::checkedCast(ptmotors2);
		if (0== control->ptmprx2)
			throw "Invalid proxy Mycomponent.PTMotors2.Proxy";

		// Get driver camera
		Ice::ObjectPrx camara1 = ic->propertyToProxy("Mycomponent.Camera1.Proxy");
		if (0==camara1)
			throw "Could not create proxy to camera1 server";

		// cast to CameraPrx
		control->cprx1 = jderobot::CameraPrx::checkedCast(camara1);
		if (0== control->cprx1)
			throw "Invalid proxy";

		// Get driver camera
		Ice::ObjectPrx camara2 = ic->propertyToProxy("Mycomponent.Camera2.Proxy");
		if (0==camara2)
			throw "Could not create proxy to camera2 server";

		// cast to CameraPrx
		control->cprx2 = jderobot::CameraPrx::checkedCast(camara2);
		if (0== control->cprx2)
			throw "Invalid proxy";

		// Contact to PTENCODERS interface
		Ice::ObjectPrx ptencoders1 = ic->propertyToProxy("Mycomponent.PTEncoders1.Proxy");
		if (0==ptencoders1)
			throw "Could not create proxy with encoders";

		// Cast to encoders
		control->pteprx1 = jderobot::PTEncodersPrx::checkedCast(ptencoders1);
		if (0== control->pteprx1)
			throw "Invalid proxy Mycomponent.PTEncoders1.Proxy";

		// Contact to PTENCODERS interface
		Ice::ObjectPrx ptencoders2 = ic->propertyToProxy("Mycomponent.PTEncoders2.Proxy");
		if (0==ptencoders2)
			throw "Could not create proxy with encoders";

		// Cast to encoders
		control->pteprx2 = jderobot::PTEncodersPrx::checkedCast(ptencoders2);
		if (0== control->pteprx2)
			throw "Invalid proxy Mycomponent.PTEncoders2.Proxy";

				//-----------------ICE----------------//
//****************************** Processing the Control ******************************///
			//---------------- ITERATIONS CONTROL -----------//


//Inicializacion de datos para los distintos hilos de control y creacion de hilos
		pthread_create(&control->thr_sub_1,NULL, &motor_sub_1, NULL);
		pthread_create(&control->thr_sub_2,NULL, &motor_sub_2, NULL);
		pthread_create(&control->thr_sub_3,NULL, &motor_sub_3, NULL);


		pthread_join(control->thr_gui, NULL);

		pthread_join(control->thr_sub_1,NULL);
		pthread_join(control->thr_sub_2,NULL);
		pthread_join(control->thr_sub_3,NULL);


	 }//del try
	 catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		status = 1;
	 }
  catch (const char* msg) {
		std::cerr << msg << std::endl;
		status = 1;
  }

if (ic)
	 ic->destroy();

return 0;

}

