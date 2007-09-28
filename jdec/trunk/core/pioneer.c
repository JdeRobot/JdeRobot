#include "jde.h"
#include "pioneer.h"

/* sensor and motor variables */
char *greyA; 
char greyAA[SIFNTSC_COLUMNS*SIFNTSC_ROWS*1]; /**< sifntsc image itself */

char *colorA;
char colorAA[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageA_clock;
arbitration imageA_callbacks[MAX_SCHEMAS];
int imageA_users=0;

char *colorB;
char colorBB[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageB_clock;
arbitration imageB_callbacks[MAX_SCHEMAS];
int imageB_users=0;

char *colorC;
char colorCC[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageC_clock;
arbitration imageC_callbacks[MAX_SCHEMAS];
int imageC_users=0;

char *colorD;
char colorDD[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageD_clock;
arbitration imageD_callbacks[MAX_SCHEMAS];
int imageD_users=0;

float pan_angle; /**< pan angle of the pantilt in degrees */
float tilt_angle;   /**< tilt angle of the pantilt in degrees */
unsigned long int pantiltencoders_clock;
arbitration pantiltencoders_callbacks[MAX_SCHEMAS];
int pantiltencoders_users=0;

int jde_laser[NUM_LASER];
unsigned long int laser_clock;
arbitration laser_callbacks[MAX_SCHEMAS];
int laser_users=0;

float us[NUM_SONARS];
unsigned long int us_clock[NUM_SONARS];
intcallback sonars_callbacks[MAX_SCHEMAS];
int sonars_users=0;

float jde_robot[5]; /* mm, mm, rad */
unsigned long int encoders_clock;
float tspeed, rspeed; /* mm/s, deg/s */
unsigned long lasttime; /* microsecs */
arbitration encoders_callbacks[MAX_SCHEMAS];
int encoders_users=0;

float latitude,longitude;
float latitude_speed,longitude_speed;
int pantiltmotors_cycle=50; /* ms */

float v,w;
int motors_cycle=100; /* ms */
float ac=0.;


/* sensor positions in the Robot FrameOfReference */
float laser_coord[5];
float us_coord[NUM_SONARS][5]; 
/**< Estructura para poder cambiar medidas de sensor a coordenadas locales al robot, y de estas al sist ref inicial: xsensor, ysensor, orientsensor,cossensor y sinsensor del sensor respecto del sistema solidario con el robot. Es fija. */
float camera_coord[5];


/** coordinate transformations from one FrameOfReference to another. */
void us2xy(int numsensor, float d,float phi, Tvoxel *point) 

/*  Calcula la posicion respecto de sistema de referencia inicial (sistema odometrico) del punto detectado en el sistema de coordenadas solidario al sensor. OJO depende de estructura posiciones y de por el sensor, sabiendo que:
   a) el robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia externo,
   b) que el sensor se encuentra en xsen, ysen con orientacion asen respecto del sistema centrado en el robot apuntando hacia su frente, 
   c) el punto esta a distancia d del sensor en el angulo phi 
*/ 
{
  float  Xp_sensor, Yp_sensor, Xp_robot, Yp_robot;

  Xp_sensor = d*cos(DEGTORAD*phi);
  Yp_sensor = d*sin(DEGTORAD*phi);
  /* Coordenadas del punto detectado por el US con respecto al sistema del sensor, eje x+ normal al sensor */
  Xp_robot = us_coord[numsensor][0] + Xp_sensor*us_coord[numsensor][3] - Yp_sensor*us_coord[numsensor][4];
  Yp_robot = us_coord[numsensor][1] + Yp_sensor*us_coord[numsensor][3] + Xp_sensor*us_coord[numsensor][4];
  /* Coordenadas del punto detectado por el US con respecto al robot */
  (*point).x = Xp_robot*jde_robot[3] - Yp_robot*jde_robot[4] + jde_robot[0];
  (*point).y = Yp_robot*jde_robot[3] + Xp_robot*jde_robot[4] + jde_robot[1];
  /* Coordenadas del punto con respecto al origen del SdeR */
}


void laser2xy(int reading, float d, Tvoxel *point)

/*  Calcula la posicion respecto de sistema de referencia inicial (sistema odometrico) del punto detectado en el sistema de coordenadas solidario al sensor. OJO depende de estructura posiciones y de por el sensor, sabiendo que:
   a) el robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia externo,
   b) que el sensor se encuentra en xsen, ysen con orientacion asen respecto del sistema centrado en el robot apuntando hacia su frente, 
*/ 
{
  float  Xp_sensor, Yp_sensor, Xp_robot, Yp_robot,phi;
  
  phi=-90.+180.*reading/NUM_LASER;
  Xp_sensor = d*cos(DEGTORAD*phi);
  Yp_sensor = d*sin(DEGTORAD*phi);
  Xp_robot = laser_coord[0] + Xp_sensor*laser_coord[3] - Yp_sensor*laser_coord[4];
  Yp_robot = laser_coord[1] + Yp_sensor*laser_coord[3] + Xp_sensor*laser_coord[4];
  /* Coordenadas del punto detectado por el laser con respecto al robot */
  (*point).x = Xp_robot*jde_robot[3] - Yp_robot*jde_robot[4] + jde_robot[0];
  (*point).y = Yp_robot*jde_robot[3] + Xp_robot*jde_robot[4] + jde_robot[1];
  /* Coordenadas del punto con respecto al origen del SdeR */
}


void update_greyA(void)
{
  int i;
  for(i=0; i<(SIFNTSC_COLUMNS*SIFNTSC_ROWS);i++)
    {
      /* this pixel conversion is a little bit tricky: it MUST pass
	   through the (unsigned char) in order to get the right number
	   for values over 128 in colorA[3*i]. it MUST pass through the
	   (unsigned int) in order to allow the addition of three
	   values, which may overflow the 8 bits limit of chars and
	   unsigned chars */  
      greyA[i]=(char)(((unsigned int)((unsigned char)(colorA[3*i]))+(unsigned int)((unsigned char)(colorA[3*i+1]))+(unsigned int)((unsigned char)(colorA[3*i+2])))/3);
    }
}

 /* init the pioneer robot */
void init_pioneer(void)
{
  int i,j;

/*  Posicion y orientacion de los sensores con respecto al centro del eje trasero del robot. Dado un sistema de coordenadas con la X en la direccion de movimiento del robot, los angulos se miden considerando que el eje X toma valor 0 y siendo positivos cuando se gira en sentido contrario al de movimiento de las agujas del reloj. Se utiliza para cambiar las distancias sensoriales al sistema de referencia local, solidario con el robot-enclosure. la rellena con milimetros y grados.   */ 
  us_coord[0][0]=115.; us_coord[0][1]=130.; us_coord[0][2]=90.; 
  us_coord[1][0]=155.; us_coord[1][1]=115.; us_coord[1][2]=50.; 
  us_coord[2][0]=190.; us_coord[2][1]=80.; us_coord[2][2]=30.; 
  us_coord[3][0]=210.; us_coord[3][1]=25.; us_coord[3][2]=10.; 
  us_coord[4][0]=210.; us_coord[4][1]=-25.; us_coord[4][2]=350.; 
  us_coord[5][0]=190.; us_coord[5][1]=-80.; us_coord[5][2]=330.; 
  us_coord[6][0]=155.; us_coord[6][1]=-115.; us_coord[6][2]=310;
  us_coord[7][0]=115.; us_coord[7][1]=-130.; us_coord[7][2]=270.; 
  us_coord[8][0]=-115.; us_coord[8][1]=-130.; us_coord[8][2]=270.; 
  us_coord[9][0]=-155.; us_coord[9][1]=-115.; us_coord[9][2]=230.; 
  us_coord[10][0]=-190.; us_coord[10][1]=-80.; us_coord[10][2]=210.; 
  us_coord[11][0]=-210.; us_coord[11][1]=-25.; us_coord[11][2]=190.; 
  us_coord[12][0]=-210.; us_coord[12][1]=25.; us_coord[12][2]=170.; 
  us_coord[13][0]=-190.; us_coord[13][1]=80.; us_coord[13][2]=150.;  
  us_coord[14][0]=-155.; us_coord[14][1]=115.; us_coord[14][2]=130.; 
  us_coord[15][0]=-115.; us_coord[15][1]=130.; us_coord[15][2]=90.; 

  for(j=0;j<NUM_SONARS;j++)
    { 
      us_coord[j][3]= cos(us_coord[j][2]*DEGTORAD);
      us_coord[j][4]= sin(us_coord[j][2]*DEGTORAD);
    }

  laser_coord[0]=19.; laser_coord[1]=0.; laser_coord[2]=0.; 
  laser_coord[3]=cos(laser_coord[2]*DEGTORAD);
  laser_coord[4]=sin(laser_coord[2]*DEGTORAD);

  camera_coord[0]=190.; 
  camera_coord[1]=0.; 
  camera_coord[2]=0.; 
  camera_coord[3]= cos(camera_coord[2]*DEGTORAD);
  camera_coord[4]= sin(camera_coord[2]*DEGTORAD);


  /* initial values for sensor variables*/
  colorA=colorAA;
  colorB=colorBB;
  colorC=colorCC;
  colorD=colorDD;
  greyA=greyAA;
  for(i=0; i<(SIFNTSC_COLUMNS*SIFNTSC_ROWS);i++)
    {colorA[i*3]=(char) 0; /* blue */ 
    colorA[i*3+1]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30); /* green */
    colorA[i*3+2]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30); /* red */
    colorB[i*3]= 0;/* blue */ 
    colorB[i*3+1]=0; /* green */
    colorB[i*3+2]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* red */
    colorC[i*3]= 0;/* blue */ 
    colorC[i*3+1]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* green */
    colorC[i*3+2]=0; /* red */
    colorD[i*3]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* blue */
    colorD[i*3+1]=0; /* green */
    colorD[i*3+2]= 0;/* red */ 
    }
  v=0.; w=0;
 
}
