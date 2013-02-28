/*
 *
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *
 */



#include "control_class.h"
#include "gui.h"

mycomponent::Control *control;

//int accion;
//int sentido;

typedef enum Estados_1 {
		avanzando,
		parando,
		marcha_atras,
		girando
}Estados_1;

typedef enum Estados_2 {
		A,
		A_ghost,
		B,
		B_ghost,
		C,
		C_ghost
}Estados_2;

typedef enum Estados_3 {
		X,
		X_ghost,
		Y,
		Y_ghost,
		Z,
		Z_ghost
}Estados_3;

//Estructura para pasar argumentos a los hilos de control
typedef struct
{
  int cycle;
  int th_id;
} th_data; 


//Variables para los automatas

int giro = (1 + rand() % 40);
int seg_giro = (1 + rand() % 10);
int variable_temp_prueba = 0;

Estados_1 padre=avanzando;
Estados_2 hijo=A_ghost;
Estados_3 nieto=X_ghost;


namespace mycomponent{
   
   /*
    ITERATION Control -> Here we can put a code to navegation.
    The laser class contains 2 attributes: distanceData(an array with the distance for all their angles (0-180º)) and numLaser (the number of angles -> 180)
    The laser interface only contains 1 method: getLaserData (returns a LaserData object with the 2 attributes updated by gazeboserver)
    mprx is the motors interface and contains 6 methods: set/getV, set/getW, set/getL
    The encoders class contains 5 attributes: robotx, roboty, robottheta, robotcos, robotsin (contains the robot coordinates) 
    The encoders interface contains 1 method: getEncodersData (returns a EncodersData object with the 5 attributes updated by gazeboserver)
   */
   
   void Control::iterationControl(int th_id){
      float v, w, l;    
   
      switch(th_id){
              
              case 1:	
				{	
                    printf("HILO 1\n");

                  break;
				}

              case 2:
				{     
					if(padre==girando)               
						printf("HILO 2\n");

                  break;
				}

              case 3:		
				{
					if(padre==girando && hijo==A)
	                   printf("HILO 3\n");

                 break;
				}

		}//Switch seleccion de hilo
            
   }//Iteration control

   // HANDLECAMERAS -> In this function is processed the cameras data that will send to the GUI or they will be used to depurate
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

   gui = new mycomponent::Gui(control);

   while(true){
      gettimeofday(&a,NULL);
      totala=a.tv_sec*1000000+a.tv_usec;
      
      //Controll->update();
      control->handleCameras();
      gui->display(*control->image1,*control->image2);
      
      
      gettimeofday(&b,NULL);
      totalb=b.tv_sec*1000000+b.tv_usec;
      //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
      
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


void *motor_th1(void* data){
	//mycomponent::Control *ctrl;
   struct timeval a, b;
	th_data *datos;
   int* cycle;
   int* th_id;
   long totalb,totala;
   long diff;

	float v;

   //ctrl = new mycomponent::Control();
	datos = (th_data*)data;
	cycle = &datos->cycle;
	th_id = &datos->th_id;

	printf("Entro hilo 1 \n");


	//El siguiente while hace las veces de iterationControl, agrupa comportamiento y motor temporal
   while(true){
      gettimeofday(&a,NULL);
      totala=a.tv_sec*1000000+a.tv_usec;
      
      //Controll->update();
      
      //ctrl->iterationControl(*th_id);

      //printf("Ciclo del hilo %d: %d\n",*th_id,*cycle);

	//AUTOMATA 1*********************************************************
		//Switch de evaluación de transiciones
		switch (padre){
			case avanzando:
			{
				printf("Antes de pedir el laser\n");
				control->ld=control->lprx->getLaserData();
				printf("Despues de pedir el laser\n");
				if (control->ld->distanceData[45] <= 1000.0){
					padre = parando;
				}
				break;
			}
			case parando:
			{
				v=control->mprx->getV();
				if (v <= 0.1){
					padre = marcha_atras;
				}
				break;
			}
			case marcha_atras:
			{
				control->ld=control->lprx->getLaserData(); // Get the laser info
				if (control->ld->distanceData[45] > 1300.0){
					padre = girando;
				}
				break;
			}
			case girando:
			{
				if (variable_temp_prueba >= seg_giro){
					padre = avanzando;
					giro = (1 + rand() % 40);
					seg_giro = (1 + rand() % 10);
					variable_temp_prueba = 0;
				}
				break;
			}
		}
		  
   		//Switch de actuación
		switch (padre){
			case avanzando:
			{
				//Percepcion para actuar si es necesario
				control->mprx->setW(0.);
				control->mprx->setV(60.);
				break;
			}
			case parando:
			{
				//Percepcion para actuar si es necesario
				control->mprx->setV(0.);
				break;
			}
			case marcha_atras:
			{
				//Percepcion para actuar si es necesario
				control->mprx->setV(-30.);
				break;
			}
			case girando:
			{
				//Percepcion para actuar si es necesario
				control->mprx->setW(giro);
				variable_temp_prueba++;
				usleep(1000000);
				break;
			}
		}

	//Motor temporal del hilo

      gettimeofday(&b,NULL);
      totalb=b.tv_sec*1000000+b.tv_usec;
      //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
      
      diff = (totalb-totala)/1000;
      if(diff < 0 || diff > *cycle)
         diff = *cycle;
      else
         diff = *cycle-diff;
      
      //Sleep Algorithm
      usleep(diff*1000);
      if(diff < 33)
         usleep(33*1000);
   }
}


void *motor_th2(void* data){
	mycomponent::Control *ctrl;
   struct timeval a, b;
	th_data *datos;
   int* cycle;
   int* th_id;
   long totalb,totala;
   long diff;

   ctrl = new mycomponent::Control();
	datos = (th_data*)data;
	cycle = &datos->cycle;
	th_id = &datos->th_id;

	printf("Entro hilo 2 \n");

   while(true){
      gettimeofday(&a,NULL);
      totala=a.tv_sec*1000000+a.tv_usec;
      
      //Controll->update();
      
      //ctrl->iterationControl(*th_id);

      //printf("Ciclo del hilo %d: %d\n",*th_id,*cycle);

	//AUTOMATA 2*********************************************************

		if(padre==girando){
			if(hijo==A_ghost || hijo==B_ghost || hijo==C_ghost){
				hijo=(Estados_2)(hijo-1);
			}
			printf("HILO 2\n");
		}else{
			if(hijo==A || hijo==B || hijo==C){
				hijo=(Estados_2)(hijo+1);
			}
		}

      gettimeofday(&b,NULL);
      totalb=b.tv_sec*1000000+b.tv_usec;
      //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
      
      diff = (totalb-totala)/1000;
      if(diff < 0 || diff > *cycle)
         diff = *cycle;
      else
         diff = *cycle-diff;
      
      //Sleep Algorithm
      usleep(diff*1000);
      if(diff < 33)
         usleep(33*1000);
   }
}


void *motor_th3(void* data){
	mycomponent::Control *ctrl;
   struct timeval a, b;
	th_data *datos;
   int* cycle;
   int* th_id;
   long totalb,totala;
   long diff;

   ctrl = new mycomponent::Control();
	datos = (th_data*)data;
	cycle = &datos->cycle;
	th_id = &datos->th_id;

	printf("Entro hilo 3 \n");

   while(true){
      gettimeofday(&a,NULL);
      totala=a.tv_sec*1000000+a.tv_usec;
      
      //Controll->update();
      
      //ctrl->iterationControl(*th_id);

      //printf("Ciclo del hilo %d: %d\n",*th_id,*cycle);

	//AUTOMATA 3*********************************************************

		if(hijo==A){
			if(nieto==X_ghost || nieto==Y_ghost || nieto==Z_ghost){
				nieto=(Estados_3)(nieto-1);
			}
			printf("HILO 3\n");
		}else{
			if(nieto==X || nieto==Y || nieto==Z){
				nieto=(Estados_3)(nieto+1);
			}
		}

      gettimeofday(&b,NULL);
      totalb=b.tv_sec*1000000+b.tv_usec;
      //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
      
      diff = (totalb-totala)/1000;
      if(diff < 0 || diff > *cycle)
         diff = *cycle;
      else
         diff = *cycle-diff;
      
      //Sleep Algorithm
      usleep(diff*1000);
      if(diff < 33)
         usleep(33*1000);
   }
}


int main(int argc, char** argv){

   int accion=0;
   int sentido=10;

   int status;
   Ice::CommunicatorPtr ic;
   struct timeval a, b;
   int cycle = 300;
   long totalb,totala;
   long diff;
   bool guiActivated=0;
   bool controlActivated=0;


	th_data data_1;
	th_data data_2;
	th_data data_3;
   
   control = new mycomponent::Control();

   pthread_mutex_init(&control->controlGui, NULL);//OBJETIVO2
   
   //---------------- INPUT ARGUMENTS ---------------//

   if (argc!=3){
      printf("\n");
      printf("USE: ./mycomponent --Ice.Config=mycomponent.cfg OPTION\n");      
      printf("    -G to show the GUI\n");
      printf("    -C to run IterationControl\n");
      printf("    -GC IterationControl & GUI\n");
   }

   if((argc==3)&&(!strcmp(argv[2],"-G"))){
      guiActivated=1;
      controlActivated=0;
   }

   if((argc==3)&&(!strcmp(argv[2],"-GC"))){
      controlActivated=1;
      guiActivated=1;
   }
   
   if((argc==3)&&(!strcmp(argv[2],"-C"))){
      controlActivated=1;
      guiActivated=0;
   }
      
   
   
   if(guiActivated){
      pthread_create(&control->thr_gui, NULL, &showGui, NULL);//OBJETIVo2  
   }

   
   //---------------- INPUT ARGUMENTS -----------------//
  
   try{
      
                  //-----------------ICE----------------//
      ic = Ice::initialize(argc,argv);
      
      // Contact to MOTORS interface
      Ice::ObjectPrx baseMotors = ic->propertyToProxy("Mycomponent.Motors.Proxy");
      if (0==baseMotors)
        throw "Could not create proxy with motors";
      // Cast to motors
      control->mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
      if (0== control->mprx)
         throw "Invalid proxy Mycomponent.Motors.Proxy";
      
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
      
      // Contact to ENCODERS interface
      Ice::ObjectPrx baseEncoders = ic->propertyToProxy("Mycomponent.Encoders.Proxy");
      if (0==baseEncoders)
         throw "Could not create proxy with encoders";
      
      // Cast to encoders
      control->eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
      if (0== control->eprx)
         throw "Invalid proxy Mycomponent.Encoders.Proxy";
      
      // Contact to LASER interface
      Ice::ObjectPrx baseLaser = ic->propertyToProxy("Mycomponent.Laser.Proxy");
      if (0==baseLaser)
         throw "Could not create proxy with laser";
      
      // Cast to laser
      control->lprx = jderobot::LaserPrx::checkedCast(baseLaser);
      if (0== control->lprx)
         throw "Invalid proxy Mycomponent.Laser.Proxy";
      
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
	if(controlActivated){
		data_1.cycle = 300;
		data_1.th_id = 1;

		data_2.cycle = 200;
		data_2.th_id = 2;

		data_3.cycle = 1000;
		data_3.th_id = 3;

		pthread_create(&control->thr_control_1, NULL, &motor_th1, &data_1);
		pthread_create(&control->thr_control_2, NULL, &motor_th2, &data_2);
		pthread_create(&control->thr_control_3, NULL, &motor_th3, &data_3);
		//while(true){
		//}
 	}
    /*  if(controlActivated){     
         while(true){

			//printf("por poner algo\n");
            control->iterationControl(1);

            gettimeofday(&a,NULL);
            totala=a.tv_sec*1000000+a.tv_usec;
            gettimeofday(&b,NULL);
            totalb=b.tv_sec*1000000+b.tv_usec;
            //std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;
            
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
      }*/

            //--------------ITERATIONS CONTROL-------------//
   		if(guiActivated)   
     	   pthread_join(control->thr_gui, NULL);

		if(controlActivated){
			pthread_join(control->thr_control_1, NULL);
			pthread_join(control->thr_control_2, NULL);
			pthread_join(control->thr_control_3, NULL);
		}
      

      
   }
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
