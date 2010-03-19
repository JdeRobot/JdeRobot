#include <jde.h>
#include <hierarchy.h>
#include <stdio.h>
#include <signal.h>
#include <laser.h>
#include <motors.h>
#include <unistd.h>
#include <math.h>


JDEHierarchy *myhierarchy;
int shutdown = 0;


void signal_handler(int sig){
  shutdown = sig;
}

int main(int argc, char *argv[]){
  JDESchema *root_schema;
  LaserPrx *l;
  MotorsPrx *m;
  char *cf = 0;

  signal(SIGTERM, &signal_handler); /* kill interrupt handler */
  signal(SIGINT, &signal_handler); /* control-C interrupt handler */
  signal(SIGABRT, &signal_handler); /* failed assert handler */
  signal(SIGPIPE, SIG_IGN);

  if (argc > 1)
    cf = strdup(argv[1]);

  myhierarchy = new_JDEHierarchy(argc,argv,cf);
  if (myhierarchy==0){
    fprintf(stdout,"Initialization failed...\n");
    exit(-1);
  }
  /*it will be the user for all the interface prxs*/
  root_schema = JDEHierarchy_root_schema_get(myhierarchy);
  
  l = new_LaserPrx(myhierarchy,"laser",root_schema);
  if (l == 0){
    fprintf(stdout,"Can't get laser prx\n");
    exit(-1);
  }
  LaserPrx_run(l);
  
  m = new_MotorsPrx(myhierarchy,"motors",root_schema);
  if (m == 0){
    fprintf(stdout,"Can't get motors prx\n");
    exit(-1);
  }
  MotorPrx_run(m);
  
  {
    unsigned int left_laser,front_laser,right_laser,i;
    unsigned int threshold = 500;
    unsigned int left_obst,right_obst;

    right_laser = 0;
    front_laser = LaserPrx_number_get(l)/2;
    left_laser = LaserPrx_number_get(l)-1;

    while(shutdown==0){
      left_obst = right_obst = threshold;
      for (i=0;i<left_laser;i++){
	if (i<front_laser)
	  right_obst = fmin(LaserPrx_laser_get(l)[i],right_obst);
	else
	  left_obst = fmin(LaserPrx_laser_get(l)[i],left_obst);
      }
	
      if ((right_obst < threshold) || (left_obst < threshold)){/*turn*/
	if (right_obst<left_obst){/*obstacle approaching on right*/
	  MotorsPrx_v_set(m,0);
	  MotorsPrx_w_set(m,50);/*turn left 1s*/
	  
	}else{/*obstacle approaching on left*/
	  MotorsPrx_v_set(m,0);
	  MotorsPrx_w_set(m,-50);/*turn right 1s*/
	  
	}
      }else{/*no obstacle approaching*/
	MotorsPrx_v_set(m,100);
	MotorsPrx_w_set(m,0);
      }
      printf("L: %d, F: %d, R: %d\n",
	     LaserPrx_laser_get(l)[left_laser],
	     LaserPrx_laser_get(l)[front_laser],
	     LaserPrx_laser_get(l)[right_laser]);
      usleep(300000);/*~3 times/s*/
    }
  }
  MotorsPrx_v_set(m,0);
  MotorsPrx_w_set(m,0);
  delete_MotorsPrx(m);
  delete_LaserPrx(l);
  delete_JDEHierarchy(myhierarchy);
  return shutdown;
}
