#include "control.h"
#include "API.h"
#include "gui.h"
#define cycle_control 100 //miliseconds
#define cycle_gui 50 //miliseconds


//Global Memory
mycomponent::Api *api;

void *showGui(void*){
   
   struct timeval a, b;
   long totalb,totala;
   int cont=0;
   long diff;
   mycomponent::Gui *gui;

   gui = new mycomponent::Gui(api);


   while(true){
      gettimeofday(&a,NULL);
      totala=a.tv_sec*1000000+a.tv_usec;

      gui->ShowImages(api);
      gui->display(api);


      //Sleep Algorithm
      gettimeofday(&b,NULL);
      totalb=b.tv_sec*1000000+b.tv_usec;
      diff = (totalb-totala)/1000;
      if(diff < 0 || diff > cycle_gui)
              diff = cycle_gui;
      else
              diff = cycle_gui-diff;

      /*Sleep Algorithm*/
      usleep(diff*1000);
      if(diff < 33)
              usleep(33*1000);
      //printf("GUI %.30lf seconds elapsed, %d\n", diff, cont);
      cont++;
         
   }
}

int main(int argc, char** argv){
   pthread_t thr_gui;

   mycomponent::Control *control;
   int status;
   Ice::CommunicatorPtr ic;
   struct timeval a, b;
   long diff;
   long totalb,totala;
   bool guiActivated=0;
   bool controlActivated=0;

   //---------------- INPUT ARGUMENTS ---------------//
   
   if (argc!=3){
      printf("\n");
      printf("USE: ./mycomponent --Ice.Config=mycomponent.cfg OPTION\n");      
      printf("    -G to show the GUI\n");
      printf("    -C to run IterationControl\n");
   }

   if((argc==3)&&(!strcmp(argv[2],"-G"))){
      guiActivated=1;
      controlActivated=0;
   }
   
   if((argc==3)&&(!strcmp(argv[2],"-C"))){
      controlActivated=1;
      guiActivated=0;
   }  
   //----------------END INPUT ARGUMENTS -----------------//
   
   
   api = new mycomponent::Api();
   control = new mycomponent::Control();
   //control->iterationControlActivated=false;
   pthread_mutex_init(&api->controlGui, NULL);
  
  
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
      
      //-----------------END ICE----------------//
  
      //****************************** Processing the Control ******************************///
   api->guiVisible=true;
   api->sentido=10;
   api->accion=0;
   control->UpdateSensorsICE(api); 

   if(guiActivated){
        pthread_create(&thr_gui, NULL, &showGui, NULL);
   }  

          while(api->guiVisible){
               gettimeofday(&a,NULL);
               totala=a.tv_sec*1000000+a.tv_usec;
               
               
               control->UpdateSensorsICE(api); // Update sensors
               if(controlActivated||api->iterationControlActivated)
                  api->RunNavigationAlgorithm();
               
               control->SetActuatorsICE(api); // Set actuators
               
               //Sleep Algorithm
               gettimeofday(&b,NULL);
               totalb=b.tv_sec*1000000+b.tv_usec;
               diff = (totalb-totala)/1000;
               if(diff < 0 || diff > cycle_control)
                       diff = cycle_control;
               else
                       diff = cycle_control-diff;
         
               /*Sleep Algorithm*/
               usleep(diff*1000);
               if(diff < 33)
                       usleep(33*1000);
               //printf("CONTROL %.15lf seconds elapsed\n", diff);     
         }

      //****************************** END Processing the Control ******************************///
   if(guiActivated)   
      pthread_join(thr_gui, NULL);
            
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

