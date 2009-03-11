/*
 *  Copyright (C) 2006 Jos� Maria Ca�as Plaza
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
 *  Authors : Jos� Maria Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 *          : Kachach Redouane <redo.robot [at] gmail.com>
 */

/** El formato de color que se recibe es BGRA */

#include "jde.h"
#include "forms.h"
#include "graphics_xforms.h"

#include "glib.h"
#include "calibradorgui.h"
#include "TAD.h"
#include "clasificator.h"
#include "colorspaces.h"
#include "progeo.h"
#include <assert.h>

/** Vamos a usar GSL para operaciones matriciales*/
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>
#include "forms.h"

/** OpenGL */
#include <GL/gl.h>              
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/freeglut_std.h>
#include <forms.h>
#include <glcanvas.h>

#define DEBUG 0
#define DEBUG_LINES 0

#define ANCHO_IMAGEN 320
#define LARGO_IMAGEN 240
#define CAMARA1 1
#define CAMARA2 2

#define MAX_RADIUS_VALUE 30
#define MIN_RADIUS_VALUE 0
#define WHEEL_DELTA 0.5

#define INVALID_POINT -5

/* 
   Esta constante define el rango de error que se permite
   a la hora de buscar un color
*/

#define COLOR_RANGE 3

/* ventana de visualizacion 3d */
float xcam,ycam,zcam,foax,foay,foaz;

/* Estos dos valores representan las cooredenadas de la esquina izq superior
   de la imagen dentro del canvas */
#define IMG_1_X 11
#define IMG_1_Y 13
#define IMG_2_X 12
#define IMG_2_Y 302
#define v3f glVertex3f
#define PI 3.141592654

#define notpushed(x) (obj != fd_calibradorgui->x)
#define pushed(x) (obj == fd_calibradorgui->x)
#define L(i)  matriz_calib_cam1[i-1]
#define DrawLine(A,B) XDrawLine(display,calibrador_win,calibradorgui_gc,A.u+IMG_1_X,A.v+IMG_1_Y,B.u+IMG_1_X,B.v+IMG_1_Y);
#define showField(x,arg) fl_set_object_label(fd_calibradorgui->x,arg);  

int calibrador_id=0; 
int calibrador_brothers[MAX_SCHEMAS];
arbitration calibrador_callforarbitration;
FD_calibradorgui *fd_calibradorgui;

/* exported variables */
int calibrador_cycle=10000; /* ms */

Display *display;
int  *screen;

registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

/*imported variables */
char **mycolorA;
runFn colorArun;
stopFn colorAstop;

char **mycolorB;
runFn colorBrun;
stopFn colorBstop;


/**  Variables para manejar el display */
XWindowAttributes win_attributes;
XGCValues gc_values;
Window calibrador_win;
GC calibradorgui_gc;

int hide_selected_points, hide_test_points, hide_control_points, test_calib, motion_on;
int auto_mode_active;


/** ============= Variable propios del esquema ============= */
#define PUSHED 1
#define RELEASED 0
#define REF_AXE_POINTS 5
#define NORMAL_POINTS 25 
#define NUMPUNTOS (1+REF_AXE_POINTS+NORMAL_POINTS)
/* cada punto da lugar a dos ecuaciones */
#define NUMEC  NUMPUNTOS*2
/* Puntos utlizados para verificar que la proyeccion se esta realizando
   correctamente 
*/
#define TESTPOINTS 4
#define TESTPOINTS_OFFSET NUMPUNTOS

/* Estos valores se usan para determinar si un punto hay que dibujarlo
   normal o como deshecho */
#define UNDODRAW 1
#define DRAW 2
#define SEARCH_WINDOW 10


/* almacena los puntos que han sido elegidos en la imagen de salida*/
Tpoint2D pntos_elegidos_en_img_1[NUMPUNTOS];
Tpoint2D pntos_elegidos_en_img_2[NUMPUNTOS];
Tpoint2D puntos_proyectados_1[NUMPUNTOS+TESTPOINTS];
Tpoint2D puntos_proyectados_2[NUMPUNTOS+TESTPOINTS];
 
/* almacena el conjunto de puntos predefinidos, que pertenecen a nuestro
   objeto de control (Objeto utilizado para la calibración). EL objeto tiene
   dos tipos de puntos:

   Puntos de control -> los que se utlizan para hallar la matriz de proyeccion 3D->2D
   Puntos de test -> para verificar que la matriz proyecta bien los putnos
*/

Tpoint3D pnts_en_objeto_de_control[NUMPUNTOS+TESTPOINTS] = {
  
  {10,10,10},
  
  {8,10,10},
  {6,10,10},
  {4,10,10},
  {2,10,10},
  {0,10,10},

  {8,8,10},
  {6,6,10},
  {4,4,10},
  {2,2,10},
  {0,0,10},

  {10,8,10},
  {10,6,10},
  {10,4,10},
  {10,2,10},
  {10,0,10},

  {10,8,8},
  {10,6,6},
  {10,4,4},
  {10,2,2},
  {10,0,0},


  {10,10,8},
  {10,10,6},
  {10,10,4},
  {10,10,2},
  {10,10,0},

  {8,10,8},
  {6,10,6},
  {4,10,4},
  {2,10,2},
  {0,10,0},

  {10,10,10},
  {6,10,10},
  {10,6,10},
  {10,10,6},
};

double matriz_calib_cam1[12];
double matriz_calib_cam2[12];

/* alfa es el C12 de la matriz resultado, de momento es constante !!!
   Y cuanto másgrande mejor es la precisión !! No se porque
   TODO: investigar el significado de este parametro
*/
double alfa = 100;

/** puntero al siguiente punto para almacenar*/
int next_point_1;
int next_point_2;
int new_point_A;
int new_point_B;

char *imagen_capturada_A = 0x0;
char *imagen_capturada_B = 0x0;
int capturada_A,capturada_B,foa_mode,cam_mode;

/* variables para dibujar la imagen */
char *tmp_image_A, *tmp_image_B, *source_image_A, *source_image_B;

int draw_result = 0;

/* variables para guardar las matrices KRT de las dos camaras */
gsl_matrix *K_1,*R_1;
gsl_vector* x_1;

gsl_matrix *K_2,*R_2;
gsl_vector* x_2;

/** Utilizado para registar la pos del raton sobre el canvas*/
Tpoint2D mouse_on_canvas;

Tline* detected_lines;

int semi_auto_A=0;
int semi_auto_B=1;
int draw_progeo_epipolar=0;

Tcalib_volume *vol;
float radius;

HPoint2D progtest2D;
HPoint3D progtest3D;
int progeo_demo=0;

/** ============== fin de declaracion esquema ============== */

void progeo(){

  int i,j=0;
  gsl_matrix *RT,*T,*Res;
  HPoint2D p2;
  HPoint3D p3;
  

  /** Progeo params*/
  TPinHoleCamera camera_A;
  
  RT = gsl_matrix_alloc(4,4);
  T = gsl_matrix_alloc(3,4);
  Res = gsl_matrix_alloc(3,4);
  
  gsl_matrix_set(T,0,0,1);
  gsl_matrix_set(T,1,1,1);
  gsl_matrix_set(T,2,2,1);

  gsl_matrix_set(T,0,3,gsl_vector_get(x_1,0));
  gsl_matrix_set(T,1,3,gsl_vector_get(x_1,1));
  gsl_matrix_set(T,2,3,gsl_vector_get(x_1,2));

  gsl_linalg_matmult (R_1,T,Res);  

  for (i=0;i<3;i++)
    for (j=0;j<4;j++)
      gsl_matrix_set(RT,i,j,gsl_matrix_get(Res,i,j));
  
  
  /** set 0001 in the last row of RT */
  gsl_matrix_set(RT,3,0,0);
  gsl_matrix_set(RT,3,1,0);
  gsl_matrix_set(RT,3,2,0);
  gsl_matrix_set(RT,3,3,1);

  /** Set camera position*/
  
  camera_A.position.X = gsl_vector_get(x_1,0)/10;
  camera_A.position.Y = gsl_vector_get(x_1,1)/10;
  camera_A.position.Z = gsl_vector_get(x_1,2)/10;
      
  /** Seting intrensic matrix*/
  camera_A.k11 = gsl_matrix_get(K_1,0,0);
  camera_A.k12 = gsl_matrix_get(K_1,0,1);
  camera_A.k13 = gsl_matrix_get(K_1,0,2);
  camera_A.k14 = 0;

  camera_A.k21 = gsl_matrix_get(K_1,1,0);
  camera_A.k22 = gsl_matrix_get(K_1,1,1);
  camera_A.k23 = gsl_matrix_get(K_1,1,2);
  camera_A.k24 = 0;

  camera_A.k31 = gsl_matrix_get(K_1,2,0);
  camera_A.k32 = gsl_matrix_get(K_1,2,1);
  camera_A.k33 = gsl_matrix_get(K_1,2,2);
  camera_A.k34 = 0;

  /** Seting extrensic*/
  camera_A.rt11 = gsl_matrix_get(RT,0,0);
  camera_A.rt12 = gsl_matrix_get(RT,0,1);
  camera_A.rt13 = gsl_matrix_get(RT,0,2);
  camera_A.rt14 = gsl_matrix_get(RT,0,3);
  
  camera_A.rt21 = gsl_matrix_get(RT,1,0);
  camera_A.rt22 = gsl_matrix_get(RT,1,1);
  camera_A.rt23 = gsl_matrix_get(RT,1,2);
  camera_A.rt24 = gsl_matrix_get(RT,1,3);

  camera_A.rt31 = gsl_matrix_get(RT,2,0);
  camera_A.rt32 = gsl_matrix_get(RT,2,1);
  camera_A.rt33 = gsl_matrix_get(RT,2,2);
  camera_A.rt34 = gsl_matrix_get(RT,2,3);

  camera_A.rt41 = gsl_matrix_get(RT,3,0);
  camera_A.rt42 = gsl_matrix_get(RT,3,1);
  camera_A.rt43 = gsl_matrix_get(RT,3,2);
  camera_A.rt44 = gsl_matrix_get(RT,3,3);

  p3.X = 10;
  p3.Y = 10;
  p3.Z = 10;
  p3.H = 1;

  project(p3,&p2,camera_A);
  
  if (DEBUG)
    printf("El punto project en %f:%f:%f\n",p2.x,p2.y,p2.h);

  if (backproject(&progtest3D, progtest2D, camera_A)){
    
    progtest3D.X =  progtest3D.X/progtest3D.H;
    progtest3D.Y =  progtest3D.Y/progtest3D.H;
    progtest3D.Z =  progtest3D.Z/progtest3D.H;

    if (DEBUG)
      printf("el backproject de %.2f-%.2f es %.2f-%.2f-%.2f-%.2f \n\n",
	     progtest2D.x,progtest2D.y,
	     progtest3D.X, progtest3D.Y, progtest3D.Z, progtest3D.H
	     );
  }
   
}


void calibrador_iteration()
{  
  speedcounter(calibrador_id);
}

/* Dado un punto en el mundo 3D origen, calcula su correpondiente
   en la imagen proyectada. Recorrre el vector de puntos 3D y calcula
   el vector 2D correpondiente
*/

double rad2deg(double alfa){
  return (alfa*180)/3.14159264;
}

double sqr(double a){
  return a*a;
}

int in_range(float value, float center, float dis){
  if (
      (value <= center+dis) 
      && 
      (value >= center-dis)
      ){
    return 1;
  }else
    return 0;
}

/** Calcula el punto correspondiente en 2D a un punto 3D */
void calcular_correspondencia_aux(Tpoint3D *p, Tpoint2D *punto_d, double* L){

  int D = 0;
  
  D = L[8]*p->x+L[9]*p->y+L[10]*p->z+alfa;
  
  punto_d->u = (int)((L[0]*p->x+L[1]*p->y+L[2]*p->z+L[3])/D);
  punto_d->v = (int)((L[4]*p->x+L[5]*p->y+L[6]*p->z+L[7])/D); 
}

void calcular_correspondencia(int num_pnts,
			      Tpoint2D * correspondencias_2D,
			      double* matriz_calibracion){

  double error=0;
  
  /*
    Formulas Base para pasar de un punto (x,y,z) -> (u,v). Nuestro H(3x4) en este
    caso es "matriz_calib_cam1" que es un array plano de 12 posiciones
    
    u = (L1.x + L2.y + L3.z + L4) / (L9.x + L10.y + L11.z + 1)
    v = (L5.x + L6.y + L7.z + L8) / (L9.x + L10.y + L11.z + 1)
    
  */
  
  double *L = matriz_calibracion;
  int i,D = 0;
  Tpoint3D p;

  for (i=0; i<num_pnts; i++){
    p = pnts_en_objeto_de_control[i];
    D = L[8]*p.x+L[9]*p.y+L[10]*p.z+alfa;
    calcular_correspondencia_aux(&p,&correspondencias_2D[i], matriz_calibracion); 
        
    /** El error se calcula solo con los puntos de control, los 
	de test no se cuentan.
    */
    if (i<NUMPUNTOS){

      error += sqrt(
		    sqr(correspondencias_2D[i].u-pntos_elegidos_en_img_1[i].u)+
		    sqr(correspondencias_2D[i].v-pntos_elegidos_en_img_1[i].v)
		    );
    }
  }
  
  /* TODO : el error no tiene en cuenta la escala, cuando la imagen se toma
     desde lejos, el error deberia ser grande aunque las distancias de diferencia
     son pequeñas
  */
  
  if (DEBUG) printf("\n== Error -> %.2f == \n",error/NUMPUNTOS);
}

int InitOGL(FL_OBJECT *ob, Window win,int w,int h, XEvent *xev, void *ud)
{
  /* La primera parte de esta funcion inicializa OpenGL con los parametros
     que diran como se visualiza. */
  fl_activate_glcanvas(fd_calibradorgui->canvas);  
  glViewport(0,0,(GLint)w,(GLint)h);
  
  /* resetea el buffer de color y el de profundidad */ 
  glDrawBuffer(GL_BACK);
  glClearColor(1.,1.,1.,0.0);  
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  
  return 0;
}

void draw_axes()
{
  int i=0;

  /* ejes X, Y, Z*/
  glLineWidth(2.3f);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glColor3f( 0.7, 0., 0. );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 10.0, 0.0, 0.0 );
  glEnd();

  glColor3f( 0.0, 0.7, 0. );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 10.0, 0.0 );
  glEnd();

  glColor3f( 0., 0., 0.7 );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 0.0, 10.0 );
  glEnd();
  glLineWidth(1.0f);
      	        
  /* suelo */ 
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f( 0.4, 0.4, 0.4 );
  glBegin(GL_LINES);
  
  for(i=0;i<61;i++)
    {
      v3f(-30.+(float)i,-30,0);
      v3f(-30.+(float)i,30.,0.);
      v3f(-30.,-30.+(float)i,0);
      v3f(30.,-30.+(float)i,0);
    }
  glEnd();
  
}

void draw_cube(){
  
  float i;

  glLoadIdentity();
  /*las 3 caras del cubo */
  
  glColor3f( 1.0, 1.0, 1.0 );
  glBegin(GL_POLYGON);
  v3f(0.98,0.98,0.98);
  v3f(0,0.98,0.98);
  v3f(0,0.98,0);
  v3f(0.98,0.98,0);
  glEnd();

  glColor3f( 1.0, 1.0, 1.0 );
  glBegin(GL_POLYGON);
  v3f(0.98,0.98,0.98);
  v3f(0.98,0,0.98);
  v3f(0.98,0,0);
  v3f(0.98,0.98,0);
  glEnd();
  
  glColor3f( 1.0, 1.0, 1.0 );
  glBegin(GL_POLYGON);
  v3f(0.98,0.98,0.98);
  v3f(0,0.98,0.98);
  v3f(0,0,0.98);
  v3f(0.98,0,0.98);
  glEnd();

  glColor3f( .0, .0, .0 );
  glBegin(GL_LINES);
  for (i=1;i>0;i=i-0.16){

    v3f(1,i,1);
    v3f(0,i,1);
    v3f(i,0,1);
    v3f(i,1,1);

    v3f(1,1,i);
    v3f(1,0,i);
    v3f(1,i,0);
    v3f(1,i,1);

    v3f(1,1,i);
    v3f(0,1,i);
    v3f(i,1,0);
    v3f(i,1,1);

  }
  glEnd();


  glColor3f( 0.0, 0.0, 1.0 );
  glBegin(GL_POLYGON);
  v3f(0.35,0.52,1);
  v3f(0.35,0.35,1);  
  v3f(0.52,0.35,1);
  v3f(0.52,0.52,1);
  glEnd();

  glColor3f( 1.0, 0.0, 0.0 );
  glBegin(GL_POLYGON);
  v3f(1,0.35,0.52);
  v3f(1,0.35,0.35);  
  v3f(1,0.52,0.35);
  v3f(1,0.52,0.52);
  glEnd();

  glColor3f( 0.0, 1.0, 0.0 );
  glBegin(GL_POLYGON);
  v3f(0.35,1,0.52);
  v3f(0.35,1,0.35);  
  v3f(0.52,1,0.35);
  v3f(0.52,1,0.52);
  glEnd();

  glFlush();

}

void draw_camera(double ppx, double ppy, int camera){
  
  /* Estos incrementos se usan para obtener el siguiente
     punto*/
  float inc_x = 1./ANCHO_IMAGEN;
  float inc_y = 1./LARGO_IMAGEN;
  float _z = -1.0;
  
  int i,j,offset,red,green,blue;

  /* dibujamos el rectangulo */
  glColor3f( 0.0, 0.0, 0.0 );  
  glScalef(0.2,0.2,0.2);
  
  glBegin(GL_LINES);
  v3f( -0.5, 0.5, -1.0 );   
  v3f( 0.5, 0.5, -1.0 );   
  glEnd();
  
  glBegin(GL_LINES);
  v3f( -0.5, 0.5, -1.0 );   
  v3f( -0.5, -0.5, -1.0 );   
  glEnd();

  glBegin(GL_LINES);
  v3f( 0.5, 0.5, -1.0 );   
  v3f( 0.5, -0.5, -1.0 );   
  glEnd();

  glBegin(GL_LINES);
  v3f( 0.5, -0.5, -1.0 );   
  v3f( -0.5, -0.5, -1.0 );   
  glEnd();

  /* pintamos la imagen*/
  if (imagen_capturada_A || imagen_capturada_B)
    for (j=0;j<LARGO_IMAGEN;j++)
      for(i=0;i<ANCHO_IMAGEN;i++)
	{
	  
	  /* obtenemos el color del pixel de la imagen de entrada */
	  offset = j*320+i;
	  if ((CAMARA1 == camera) && imagen_capturada_A) {
	    red = (unsigned char)imagen_capturada_A[offset*3+2];
	    green = (unsigned char)imagen_capturada_A[offset*3+1];
	    blue = (unsigned char)imagen_capturada_A[offset*3];
	  }else if ((CAMARA2 == camera) && imagen_capturada_B){
	    red = (unsigned char)imagen_capturada_A[offset*3+2];
	    green = (unsigned char)imagen_capturada_A[offset*3+1];
	    blue = (unsigned char)imagen_capturada_A[offset*3];
	  }
	  
	  glColor3f( (float)red/255.0, (float)green/255.0, (float)blue/255 );  
	  
	  /* pintamos el punto correspondiente en el
	     escenario pintado con OGL */
	  glBegin(GL_POINTS);
	  v3f(0.5-(inc_x*i),0.5-(inc_y*j),-1 );   
	  glEnd();
	}
  
  /* fin dibujamos el rectangulo */

  glColor3f( 1.0, 0.0, 0.0 );  
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.5, 0.5, _z);   
  glEnd();

  glColor3f( 0.0, 1.0, 0.0 );  
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( -0.5, 0.5, _z );   
  glEnd();

  glColor3f( 0.0, 0.0, 1.0 );  
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( -0.5, -0.5, _z );   
  glEnd();

  glColor3f( 1.0, 1.0, 0.0 );  
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.5, -0.5, _z);   
  glEnd();

  glBegin(GL_LINES);
  glColor3f( 0.0, 1.0, 0.0 );  
  v3f( 0.0, 0.0, 0.0 );   
  v3f( -0.5+ppx, -0.5+ppy, _z );   
  glEnd();

  /* Por alguna razon, el tama�o maximo que se puede utilizar para un punto
     es de un solo pixel que es practicamente invisible para un ser humano.
     Visto esto, se ha decidido usar un cubo de un lado muy pequeño para 
     dibujar "puntos grandes
  */

  glLoadIdentity();
  glColor3f( 1.0, 0.0, 0.0 );
  if (CAMARA1 == camera)
    glTranslatef( 
		 -(float)gsl_vector_get(x_1,0)/10.0,
		 -(float)gsl_vector_get(x_1,1)/10.0,
		 -(float)gsl_vector_get(x_1,2)/10.0);
  else
    glTranslatef(
		 -(float)gsl_vector_get(x_2,0)/10.0,
		 -(float)gsl_vector_get(x_2,1)/10.0,
		 -(float)gsl_vector_get(x_2,2)/10.0);
  
  glutSolidCube(0.01);    
}

/** Dibuja el suporte sobre el cual esta fijadas las dos camaras*/
void draw_camera_conection(gsl_vector* T1,gsl_vector* T2){

  glLoadIdentity();

  glColor3f( 0.0, 0.0, 0.0 );  
  glLineWidth(2.5f);
  glBegin(GL_LINES);
  v3f(-(float)gsl_vector_get(T1,0)/10,
      -(float)gsl_vector_get(T1,1)/10,
      -(float)gsl_vector_get(T1,2)/10
      );
  v3f(-(float)gsl_vector_get(T2,0)/10,
      -(float)gsl_vector_get(T2,1)/10,
      -(float)gsl_vector_get(T2,2)/10
      );
  glLineWidth(1.0f);
  glEnd();
}

void set_camera_pos(int camara, gsl_matrix* K, gsl_matrix* R, gsl_vector* T ){

  float ppx = gsl_matrix_get(K,0,2)/ANCHO_IMAGEN;
  float ppy = gsl_matrix_get(K,1,2)/ANCHO_IMAGEN;
  GLdouble m[16];

  memset(m, 0, 16*sizeof(GLdouble));

  /* formating the rotation matrix to be used by OGL */

  m[0] = gsl_matrix_get(R,0,0);
  m[1] = gsl_matrix_get(R,0,1);
  m[2] = gsl_matrix_get(R,0,2);
  m[3] = 0;
  
  m[4] = gsl_matrix_get(R,1,0); 
  m[5] = gsl_matrix_get(R,1,1);
  m[6] = gsl_matrix_get(R,1,2);
  m[7] = 0;
  
  m[8] = gsl_matrix_get(R,2,0);
  m[9] = gsl_matrix_get(R,2,1);
  m[10] = gsl_matrix_get(R,2,2);
  m[11] = 0; 
  
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(
	       -(float)gsl_vector_get(T,0)/10.0,
	       -(float)gsl_vector_get(T,1)/10.0,
	       -(float)gsl_vector_get(T,2)/10.0
	       );

  /* Rotation by multiplying the actual matrix by the rotation matrix m */
  glMultMatrixd(m);
  draw_camera(ppx, ppy, camara);
}

/** Dibuja el escenario con la dos camaras,
    si y solo si estas han sido calibradas */
void draw_scenario(){
  
  if (progeo_demo && x_1 && K_1 && R_1){
  
    progeo();
    
    glLineWidth(2.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f( 0, 0, 0.7 );
    glBegin(GL_LINES);
    v3f( -gsl_vector_get(x_1,0)/10, -gsl_vector_get(x_1,1)/10, -gsl_vector_get(x_1,2)/10);   
    v3f(-progtest3D.X,-progtest3D.Y,-progtest3D.Z);
    glEnd();
    
    glLineWidth(1.f);
  }

  if (NUMPUNTOS == next_point_1)
    set_camera_pos(CAMARA1, K_1,R_1,x_1);
  
  if (NUMPUNTOS == next_point_2)
    set_camera_pos(CAMARA2, K_2,R_2,x_2);
  
  /** Solo dibujamos el suporte si las dos camaras han sido
      calibradas, en otro caso solo se dibuja la camara en 
      cuestion */
  if ((NUMPUNTOS == next_point_1)&&(NUMPUNTOS == next_point_2))
    draw_camera_conection(x_1,x_2);
}

/* Redibuja el paton de calibracion,
   utilizando para ello la matriz de
   calibracion calculada
*/
void my_test_calib(){


  Tpoint3D A,B,C,D,E,F,G,H;
  Tpoint2D A2,B2,C2,D2,E2,F2,G2,H2;
  
  A.x = 8;  A.y = 8;  A.z = 10;
  B.x = 10;  B.y = 8;  B.z = 10;
  C.x = 10;  C.y = 10;  C.z = 10;
  D.x = 8;  D.y = 10;  D.z = 10;
  
  E.x = 8;  E.y = 8;  E.z = 12;
  F.x = 10;  F.y = 8;  F.z = 12;
  G.x = 10;  G.y = 10;  G.z = 12;
  H.x = 8;  H.y = 10;  H.z = 12;

  calcular_correspondencia_aux(&A,&A2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&B,&B2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&C,&C2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&D,&D2, matriz_calib_cam1); 

  calcular_correspondencia_aux(&E,&E2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&F,&F2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&G,&G2, matriz_calib_cam1); 
  calcular_correspondencia_aux(&H,&H2, matriz_calib_cam1); 

  DrawLine(A2,B2);
  DrawLine(B2,C2);
  DrawLine(C2,D2);
  DrawLine(D2,A2);

  DrawLine(E2,F2);
  DrawLine(F2,G2);
  DrawLine(G2,H2);
  DrawLine(H2,E2);

  DrawLine(A2,E2);
  DrawLine(B2,F2);
  DrawLine(G2,C2);
  DrawLine(H2,D2);
  
}

void DrawRectange (Tpoint2D c, int r){

  Tpoint2D A,B,C,D;

  A.u = c.u-r;
  A.v = c.v-r;
  
  B.u = c.u+r;
  B.v = c.v-r;

  C.u = c.u-r;
  C.v = c.v+r;

  D.u = c.u+r;
  D.v = c.v+r;

  DrawLine(A,B);
  DrawLine(A,C);
  DrawLine(B,D);
  DrawLine(D,C);

}

/* Resuelve el sistema sobredimensioando (un sistema con más ecuaciones de las
   necesarias) dando una solución que minimize el error cometido. Quiere decir
   minimize la suma de los cuadrados del error cometido en cada punto y su correpondiente
   calculado a traves de la matriz solución
*/
void regresion_lineal_multiple(double a_data[NUMEC*11], double b_data[NUMEC], double* solucion){

  int i,j,k,aux;
  double chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *c;
  gsl_multifit_linear_workspace * work;
  
  X = gsl_matrix_alloc(NUMEC,11);
  y = gsl_vector_alloc(NUMEC);
  c = gsl_vector_alloc (11);
  cov = gsl_matrix_alloc (11,11);
  
  /* Prepramos la matriz de muestras 

     NOTA: El sistema de ecuaciones a resolver contiene
     11 incognitas en vez de 12. La novena incognita es
     igual a alfa (constante), y hay que inicializarla sino
     La solucion siempre sera el vector nulo
  */
  
  for (i=0; i<NUMEC; i++){
    for (j=0; j<11; j++)
      {
	aux = i*11+j;
	gsl_matrix_set(X,i,j,a_data[aux]);
      }
  }
  
  /* Inicializamos el verctor de muestras */
  for (k=0; k<NUMEC; k++)
    {
      gsl_vector_set(y,k,b_data[k]);
    }
  
  /* Inicializamos y resolvemos el sistema sobredimensioando */
  work = gsl_multifit_linear_alloc (NUMEC,11);
  gsl_multifit_linear (X,  y, c, cov, &chisq, work);
  gsl_multifit_linear_free (work);
  
  /** copiamos la solución */
  for (i=0; i<11; i++)
    {
      solucion[i] = gsl_vector_get(c,i);
    }
  
}

/* Dados dos puntos p(x,y,z) y p'(u,v) obtiene dos
   ecuaciones fruto de la correspondencia entre los dos pnts
*/
void obtener_ecuacion(Tpoint3D p , 
		      Tpoint2D p_prima,
		      int **ecuacion){
  
  /** 
      Rellenamos la primera fila con los siguientes coeficientes Li :

      Ecuacion1 -> L1.x+L2.y+L3.z+L4+L5.0+L6.0+L7.0+L8.0-L9.x.u-L10.y.u-L11.z.u = u
      Ecuacion2 -> L1.0+L2.0+L3.0+L4.0+L5.x+L6.y+L7.z+L8-L9.x.v-L10.y.v-L11.z.v = v
  */

  /* Ecuación 1 - lado izq */
  ecuacion[0][0]  = p.x;
  ecuacion[0][1]  = p.y;
  ecuacion[0][2]  = p.z;
  ecuacion[0][3]  = 1;
  ecuacion[0][4]  = 0;
  ecuacion[0][5]  = 0;
  ecuacion[0][6]  = 0;
  ecuacion[0][7]  = 0;
  ecuacion[0][8]  = -p.x*p_prima.u;
  ecuacion[0][9]  = -p.y*p_prima.u;
  ecuacion[0][10] = -p.z*p_prima.u;

  /* Ecuación 1 - lado derecho*/
  ecuacion[0][11] = p_prima.u*alfa;

  /* Ecuación 2 - lado derecho*/
  ecuacion[1][0]  = 0;
  ecuacion[1][1]  = 0;
  ecuacion[1][2]  = 0;
  ecuacion[1][3]  = 0;
  ecuacion[1][4]  = p.x;
  ecuacion[1][5]  = p.y;
  ecuacion[1][6]  = p.z;
  ecuacion[1][7]  = 1;
  ecuacion[1][8]  = -p.x*p_prima.v;
  ecuacion[1][9]  = -p.y*p_prima.v;
  ecuacion[1][10] = -p.z*p_prima.v;

  /* Ecuación 2 - lado derecho*/
  ecuacion[1][11] = p_prima.v*alfa;
}


void show_results(gsl_matrix* K,
		  gsl_matrix* R,
		  gsl_vector* x){
		
  char text[80]="";

  sprintf(text,"%.2f",gsl_matrix_get(K,0,0));
  showField(k1, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,0,1));
  showField(k2, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,0,2));
  showField(k3, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,1,0));
  showField(k4, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,1,1));
  showField(k5, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,1,2));
  showField(k6, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,2,0));
  showField(k7, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,2,1));
  showField(k8, text);
  sprintf(text,"%.2f",gsl_matrix_get(K,2,2));
  showField(k9, text);
  
  sprintf(text,"%.2f",gsl_matrix_get(R,0,0));
  showField(r1, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,0,1));
  showField(r2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,0,2));
  showField(r3, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,1,0));
  showField(r4, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,1,1));
  showField(r5, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,1,2));
  showField(r6, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,2,0));
  showField(r7, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,2,1));
  showField(r8, text);
  sprintf(text,"%.2f",gsl_matrix_get(R,2,2));
  showField(r9, text);

  sprintf(text,"%.2f",-gsl_vector_get(x,0));
  showField(c1, text);
  sprintf(text,"%.2f",-gsl_vector_get(x,1));
  showField(c2, text);
  sprintf(text,"%.2f",-gsl_vector_get(x,2));
  showField(c3, text);

}

void show_results_2(gsl_matrix* R_prima,
		  gsl_matrix* Q_prima,
		  gsl_vector* x){
		
  char text[80]="";

  sprintf(text,"%.2f",gsl_matrix_get(R_prima,0,0));
  showField(k1_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,0,1));
  showField(k2_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,0,2));
  showField(k3_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,1,0));
  showField(k4_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,1,1));
  showField(k5_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,1,2));
  showField(k6_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,2,0));
  showField(k7_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,2,1));
  showField(k8_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(R_prima,2,2));
  showField(k9_2, text);
  
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,0,0));
  showField(r1_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,0,1));
  showField(r2_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,0,2));
  showField(r3_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,1,0));
  showField(r4_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,1,1));
  showField(r5_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,1,2));
  showField(r6_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,2,0));
  showField(r7_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,2,1));
  showField(r8_2, text);
  sprintf(text,"%.2f",gsl_matrix_get(Q_prima,2,2));
  showField(r9_2, text);

  sprintf(text,"%.2f",-gsl_vector_get(x,0));
  showField(c1_2, text);
  sprintf(text,"%.2f",-gsl_vector_get(x,1));
  showField(c2_2, text);
  sprintf(text,"%.2f",-gsl_vector_get(x,2));
  showField(c3_2, text);

}

void print_matrix(gsl_matrix* mat){
 
  int n,mm;

  if (!DEBUG)
    return;

  for (n=0; n<3; n++){
    for (mm=0; mm<3; mm++){
      printf("%g ",gsl_matrix_get(mat,n,mm));
    }
    printf("\n");
  }
  
  printf("\n\n");
}

/* Para llegar a la desomposicion RQ, hay que 
   invertir la matriz que queremos descomopner
   y luego aplicarle un QR, finalmente se invierte
   tanto R como Q para obtener la RQ

   1) M' = invert(M)
   2) M' = QR
   3) (M')' = (QR)' = R'Q'
   4) M = R'Q'
   
*/
void rq_decomp(double* solucion, 
	       gsl_matrix* R_prima,
	       gsl_matrix* Q_prima,
	       gsl_vector* x
	       ){

  int n,mm,s,signum ;
  gsl_matrix *M,*Q,*R;
  gsl_vector* tau;
  double tmp,det;

  /* para invertir las matriz M,Q,R */
  gsl_permutation* p = gsl_permutation_alloc (3);
  gsl_permutation* p2 = gsl_permutation_alloc (3);
  gsl_permutation* p3 = gsl_permutation_alloc (3);
  gsl_matrix* M_prima = gsl_matrix_alloc(3,3);
  gsl_matrix* Q_prima_tmp = gsl_matrix_alloc(3,3);
  
  /* para resolver el centro de la camara usando Mx=C 
     donde C es el verctor p4 de la matriz P */
  gsl_vector* p4 = gsl_vector_alloc(3);

  gsl_matrix* temp = gsl_matrix_alloc(3,3);
  gsl_matrix* I_C = gsl_matrix_alloc(3,4);
  gsl_matrix* test = gsl_matrix_alloc(3,4);

  M = gsl_matrix_alloc(3,3);
  Q = gsl_matrix_alloc(3,3);
  R = gsl_matrix_alloc(3,3);
  tau = gsl_vector_alloc(3);

  /* Copiamos la submatriz 3x3 Izq de la solucion P a la matriz M*/

  gsl_matrix_set(M,0,0,solucion[0]);
  gsl_matrix_set(M,0,1,solucion[1]);
  gsl_matrix_set(M,0,2,solucion[2]);
  
  gsl_matrix_set(M,1,0,solucion[4]);
  gsl_matrix_set(M,1,1,solucion[5]);
  gsl_matrix_set(M,1,2,solucion[6]);

  gsl_matrix_set(M,2,0,solucion[8]);
  gsl_matrix_set(M,2,1,solucion[9]);
  gsl_matrix_set(M,2,2,solucion[10]);

  /* Copiamos el vector p4*/
  gsl_vector_set(p4,0,solucion[3]);
  gsl_vector_set(p4,1,solucion[7]);
  gsl_vector_set(p4,2,solucion[11]);

  /* invertimos la matriz M */
  gsl_linalg_LU_decomp (M, p, &s);
  gsl_linalg_LU_solve(M,p,p4,x);
  gsl_linalg_LU_invert (M, p, M_prima);
  
  /* Hacemos una descomposicion a la matriz M invertida */
  gsl_linalg_QR_decomp (M_prima,tau);
  gsl_linalg_QR_unpack (M_prima,tau,Q,R);

  /* Invertimos R*/
  gsl_linalg_LU_decomp (R, p2, &s);
  gsl_linalg_LU_invert (R, p2, R_prima);
  
  /* Invertimos Q*/
  gsl_linalg_LU_decomp (Q, p3, &s);
  gsl_linalg_LU_invert (Q, p3, Q_prima);
  gsl_matrix_memcpy(Q_prima_tmp, Q_prima);


  if (DEBUG) {
    /** checking results:
	
	If the rq decompsition is correct we should obtain
	the decomposed matrix:

	orig_matrix = K*R*T

	where T = (I|C)

     */

    gsl_matrix_set(I_C,0,3,gsl_vector_get(x,0));
    gsl_matrix_set(I_C,1,3,gsl_vector_get(x,1));
    gsl_matrix_set(I_C,2,3,gsl_vector_get(x,2));
    
    gsl_matrix_set(I_C,0,0,1);
    gsl_matrix_set(I_C,0,1,0);
    gsl_matrix_set(I_C,0,2,0);
    
    gsl_matrix_set(I_C,1,0,0);
    gsl_matrix_set(I_C,1,1,1);
    gsl_matrix_set(I_C,1,2,0);
    
    gsl_matrix_set(I_C,2,0,0);
    gsl_matrix_set(I_C,2,1,0);
    gsl_matrix_set(I_C,2,2,1);
    
    gsl_linalg_matmult(R_prima,Q_prima,temp);
    gsl_linalg_matmult(temp,I_C,test);
    
    printf(" Result -> \n");
    
    for (n=0; n<3; n++){
      for (mm=0; mm<4; mm++){
	printf(" %g \t",gsl_matrix_get(test,n,mm));
      }
      printf("\n");
    }
  }
  
  /* El elemento (3,3) de la matriz R tiene que ser 1
     para ello tenemos que normalizar la matriz dividiendo
     entre este elemento
  */
  
  tmp = gsl_matrix_get(R_prima,2,2);
  for (n=0; n<3; n++)
    for (mm=0; mm<3; mm++){
      gsl_matrix_set(R_prima,n,mm, gsl_matrix_get(R_prima,n,mm)/tmp);
    }


  /*  Si obtenemos valores negativos en la
      diagonal de K tenemos que cambiar de signo la columna de K y la fila de Q
      correspondiente
  */
  
  if (DEBUG) 
    print_matrix(R_prima);
  if (DEBUG) 
    print_matrix(Q_prima);

  if (gsl_matrix_get(R_prima,0,0)<0){
  
    if (DEBUG) printf(" distancia focat 0,0 negativa\n");
    gsl_matrix_set(R_prima,0,0,
		   abs(gsl_matrix_get(R_prima,0,0))
		   );
    for (n=0;n<3;n++)
      gsl_matrix_set(Q_prima,0,n,
		     gsl_matrix_get(Q_prima,0,n)*-1
		     );
    
  }

  if (DEBUG)  printf("R_prima\n");
  print_matrix(R_prima);
  if (DEBUG) printf("Q_prima\n");
  print_matrix(Q_prima);

  if (gsl_matrix_get(R_prima,1,1)<0){
    if (DEBUG) printf(" distancia focat 1,1 negativa\n");
    for (n=0;n<3;n++){
      gsl_matrix_set(Q_prima,1,n,
		     gsl_matrix_get(Q_prima,1,n)*-1
		     );
      gsl_matrix_set(R_prima,n,1,
		     gsl_matrix_get(R_prima,n,1)*-1
		     );
    }
  }

  if (DEBUG) printf("R_prima\n");
  print_matrix(R_prima);
  if (DEBUG) printf("Q_prima\n");
  print_matrix(Q_prima);
  
  
  /*Finalmente, si Q queda con determinante -1 cambiamos de signo
    todos sus elementos para obtener una rotación sin "reflexion".
    
    NOTA: Este trozo de codigo lo he desactivado debido a que si lo
    hacemos obtenemos una orientacion equivocada a la hora de dibujarla
    con OGL
  
  */
  
  gsl_linalg_LU_decomp (Q_prima_tmp, p3, &s);
  signum=1;
  det = gsl_linalg_LU_det(Q_prima_tmp,signum);
    
  if (-1 == det && 0){
    if (DEBUG) printf("Q has a negatif det");
    for (n=0;n<3;n++)
      for (mm=0;mm<3;mm++)
	gsl_matrix_set(Q_prima,n,mm,gsl_matrix_get(Q_prima,n,mm)*-1);
    
  }  
  
}

void resolver_sistema_de_ecuaciones(Tpoint2D* pnts_elegidos, 
				    double* matriz_calibracion,
				    gsl_matrix* K,
				    gsl_matrix* R,
				    gsl_vector* x
				    ){

  /** 
      Sistema de ecuaciones lineales. Se le alojara memoria en la funcion init

  */
  int **sistema_lineal_ecuaciones;/*[NUMEC][12]*/
 
  int k,i,j;
  int **ecuacion_lineal;
  double a_data[11*NUMEC];
  double b_data[NUMEC];
  
  k=i=j=0;

  sistema_lineal_ecuaciones = malloc(NUMEC*sizeof(int*));
  if (sistema_lineal_ecuaciones){
    for (i=0; i<NUMEC; i++){
      sistema_lineal_ecuaciones[i] = malloc(12*sizeof(int));
      if (!sistema_lineal_ecuaciones[i]){
	printf("I can't reserve memory ...\n");
	assert(0);
	return;
      }
    }
  }
  else{
    printf("I can't reserve memory .. \n");
    assert(0);
  }
  
  ecuacion_lineal = malloc(2*sizeof(int*));
  ecuacion_lineal[0] = malloc(12*sizeof(int));
  ecuacion_lineal[1] = malloc(12*sizeof(int));
  
  /* recoremos los dos arrays con los putnos almacenados, y vamos obteniendo 
     las ecuaciones para cada par de puntos. Cada par de pnts da lugar a dos ecuaciones.
  */
  for (i=0; i<NUMPUNTOS; i++){
    
    obtener_ecuacion(pnts_en_objeto_de_control[i], 
		     pnts_elegidos[i],
		     ecuacion_lineal);
    
    /** copiamos la ecuacion obtenida al sistema lineal sobre dimensionado*/
    for (j=0; j<12; j++){
      sistema_lineal_ecuaciones[i*2][j] = ecuacion_lineal[0][j];
      sistema_lineal_ecuaciones[i*2+1][j] = ecuacion_lineal[1][j];
    }
  }
  
  /** vamos a resolver Ax=b*/

  /** copiamos la matriz "A" */
  k = 0;
  for (i=0; i<NUMEC; i++){
    for (j=0; j<11; j++){
      a_data[k++] = sistema_lineal_ecuaciones[i][j];
    }
  }

  /** Copiamos el vector "b" (la ultima columna)*/
  for (j=0; j<NUMEC; j++)
    b_data[j] = sistema_lineal_ecuaciones[j][11];


  /** Liberamos la memoria utilizada*/
  for (i=0; i<NUMEC; i++)
    free(sistema_lineal_ecuaciones[i]);
  free(sistema_lineal_ecuaciones);

  free(ecuacion_lineal[0]);
  free(ecuacion_lineal[1]);
  free(ecuacion_lineal);
  
  regresion_lineal_multiple(a_data, b_data, matriz_calibracion);
  
  if (DEBUG)
    for (i=0; i<12; i++)
      printf("calibration matrix -> %g\n",matriz_calibracion[i]);

  rq_decomp(matriz_calibracion, K, R, x);
}


/* Este procedemiento recogera la información de los NUMPUNTOS puntos "emparejados",
   creara un sistema de ecuaciones sobredimensionado, lo resolvera, y con la matriz resultado
   pintara la imagen de salida rectificada
*/
void calibrar(){

  K_1 = gsl_matrix_alloc(3,3);
  R_1 = gsl_matrix_alloc(3,3);
  x_1 = gsl_vector_alloc(3);

  K_2 = gsl_matrix_alloc(3,3);
  R_2 = gsl_matrix_alloc(3,3);
  x_2 = gsl_vector_alloc(3);

  /* Camara 1 */
  if (NUMPUNTOS == next_point_1){
    resolver_sistema_de_ecuaciones(pntos_elegidos_en_img_1, matriz_calib_cam1,K_1,R_1,x_1);
    calcular_correspondencia(NUMPUNTOS+TESTPOINTS, puntos_proyectados_1, matriz_calib_cam1);
    show_results(K_1,R_1,x_1);
  }
  
  /* Camara 2 */
  if (NUMPUNTOS == next_point_2){
    resolver_sistema_de_ecuaciones(pntos_elegidos_en_img_2, matriz_calib_cam2,K_2,R_2,x_2);
    calcular_correspondencia(NUMPUNTOS+TESTPOINTS, puntos_proyectados_2, matriz_calib_cam2);
    show_results_2(K_2,R_2,x_2);
  }
  
  fl_set_foreground(calibradorgui_gc,FL_BLUE);
  draw_result = 1;
}


void reset(){
  
  int i;
  /* ============================================================== */

  /* inicializamos el array de los puntos elegidos */
  for (i=0; i<NUMPUNTOS; i++){
    
    pntos_elegidos_en_img_1[i].u = -1;
    pntos_elegidos_en_img_1[i].v = -1;
    pntos_elegidos_en_img_1[i].class = DRAW;

    pntos_elegidos_en_img_2[i].u = -1;
    pntos_elegidos_en_img_2[i].v = -1;
    pntos_elegidos_en_img_2[i].class = DRAW;
  }
  
  /** Control Points && images*/
  next_point_1 = 0;
  next_point_2 = 0;
  capturada_B = 0;
  capturada_A = 0;
  new_point_A=1;
  new_point_B=1;

  /** Reservamos memoria si todavia no lo hemos hecho */
  if (!imagen_capturada_A)
    imagen_capturada_A = (char*)malloc(ANCHO_IMAGEN*LARGO_IMAGEN*4);
  else
    memset(imagen_capturada_A, 0,ANCHO_IMAGEN*LARGO_IMAGEN*4);
  
  if (!imagen_capturada_B)
    imagen_capturada_B = (char*)malloc(ANCHO_IMAGEN*LARGO_IMAGEN*4);
  else
    memset(imagen_capturada_B, 0,ANCHO_IMAGEN*LARGO_IMAGEN*4);
  
  if (!tmp_image_B)
    tmp_image_B = (char *) malloc(ANCHO_IMAGEN*LARGO_IMAGEN*4);    
  else
    memset(tmp_image_B, 0,ANCHO_IMAGEN*LARGO_IMAGEN*4);

  if (!tmp_image_A)
    tmp_image_A = (char *) malloc(ANCHO_IMAGEN*LARGO_IMAGEN*4);
  else
    memset(tmp_image_A, 0,ANCHO_IMAGEN*LARGO_IMAGEN*4);
  
  if (x_1)   gsl_vector_free(x_1);
  x_1 = 0;
  if (x_2)   gsl_vector_free(x_2);
  x_2 = 0;

  /* TODO fijar el valor constante .. Hemos observado que cuanto
     mas gande es este valor, mas precisa es la matriz de calibracion*/
  matriz_calib_cam1[11] = alfa;
  matriz_calib_cam2[11] = alfa;

  /* ============================================================== */
}

float distance(Tpoint2D p,int x,int y){
  return  sqrt((p.u-x)*(p.u-x) + (p.v-y)*(p.v-y)); 
}

int drop_point(Tcalib_volume* vol, int x, int y, char* imagen){

  int i,j;
  Tcolor BLACK;
  int borrado=0;

  BLACK.c1=0;
  BLACK.c2=0;
  BLACK.c3=0;

  if (!vol)
    return borrado;
  
  if (vol->center)
    if (distance(*vol->center,x,y)<10)
      {
	vol->center=NULL;
	borrado=1;
      }
  
  if (!borrado && vol->ref_axe)
    for (i=0; i<vol->ref_axe->num_points; i++){
      
      if (distance(vol->ref_axe->points[i],x,y)<10)
	{
	  draw_rectangle(imagen, &vol->ref_axe->points[i], BLACK);
	  if (DEBUG)
	    printf("borrando el punto %d:%d desde el eje %d\n",
		   vol->ref_axe->points[i].u,
		   vol->ref_axe->points[i].v,i);
	  
	  for (j=i; j<vol->ref_axe->num_points-1; j++)
	    vol->ref_axe->points[j] = vol->ref_axe->points[j+1];
	  
	  vol->ref_axe->num_points--;
	  
	  borrado=1;
	  break;
	}
    }
  
  if (!borrado && vol->normal_points)
    for (i=0; i<vol->num_of_points; i++){
      if (distance(vol->normal_points[i],x,y) < 10)
	{
	  draw_rectangle(imagen, &vol->normal_points[i], BLACK);
	  if (DEBUG)
	    printf("borrando el punto normal %d:%d \n",
		 vol->normal_points[i].u,
		 vol->normal_points[i].v);

	  for (j=i; j<vol->num_of_points-1; j++)
	    vol->normal_points[j]=vol->normal_points[j+1];
	  
	  vol->num_of_points--;
	  borrado=1;
	  break;
	}
    }

  new_point_A=1;
  new_point_B=1;

  return borrado;
}

/* Callback que captura el evento de pulsar sobre la imagen,
   capturando el (x,y) del punto que ha sido elegido
*/
int freeobj_imagen_de_entrada_1_handle(FL_OBJECT* obj, int event, 
				       FL_Coord mouse_x, FL_Coord mouse_y,
				       int key, void* xev1){
  
  int r,g,b,offset;

  /* variables para manejar el evento */  
  int punto_pulsado_x = 0;
  int punto_pulsado_y = 0;
  Tpoint2D *p = (Tpoint2D*)malloc(sizeof(Tpoint2D));
  Tcolor RED,GREEN,BLUE;
  struct HSV *hsi;
  XKeyEvent *xev = (XKeyPressedEvent*)xev1;

  RED.c1=0;
  RED.c2=0;
  RED.c3=255;
  
  GREEN.c1=0;
  GREEN.c2=255;
  GREEN.c3=0;
  
  BLUE.c1=255;
  BLUE.c2=0;
  BLUE.c3=0;

  punto_pulsado_x = mouse_x-IMG_1_X;
  punto_pulsado_y = mouse_y-IMG_1_Y;
  
  /** Si pulsamos el button centro y con el modo progeo activado
      entonces activamos/desactivamos el dibujado de la linea.
      Esto es asi para que el usuario pueda para cuando quiera
      de dibujar de manera continua el eje epiplar de progeo

      Nota: event = 2 -> button pressed
  */
  if (xev && (2 == event) && ( 2 == key)){
    draw_progeo_epipolar = !draw_progeo_epipolar;

    if (draw_progeo_epipolar)
      fl_set_object_color(fd_calibradorgui->progdraw_active,FL_GREEN, FL_GREEN);
    else
      fl_set_object_color(fd_calibradorgui->progdraw_active,FL_TOMATO, FL_TOMATO);
  }
  
  if (draw_progeo_epipolar){
    progtest2D.x = punto_pulsado_x;
    progtest2D.y = punto_pulsado_y;
    progtest2D.h = 1;
  }

  if ((x_1 || x_2) && (next_point_1==NUMPUNTOS)) return -1;
    
  if (event == FL_PUSH){
    
    /* 
       las restas que hacen son hard-coded y se obtienen viendo en que sitio cae en la
       ventana la imagen, cogiendo las coordenedas de la esquina superior izquierda 
    */
    if (xev && (3 == xev->keycode)){
      if (drop_point(vol, punto_pulsado_x, punto_pulsado_y, imagen_capturada_A))
	next_point_1--;
      return 0;
    }

    p->u = punto_pulsado_x;
    p->v = punto_pulsado_y;

    offset = punto_pulsado_y*320+punto_pulsado_x;
    r = (unsigned char)imagen_capturada_A[offset*3+2];
    g = (unsigned char)imagen_capturada_A[offset*3+1];
    b = (unsigned char)imagen_capturada_A[offset*3];
    
    hsi = (struct HSV *)RGB2HSV_getHSV(r,g,b);
    if (DEBUG)
      printf ("%d-%d  HSV (%.2f,%.2f,%.2f)\n",punto_pulsado_x,punto_pulsado_y,hsi->H,hsi->S,hsi->V);
            
    progtest2D.x = punto_pulsado_x;
    progtest2D.y = punto_pulsado_y;
    progtest2D.h = 1;

    if (semi_auto_A && vol){

      if (is_normal_point(*p,imagen_capturada_A)){
	draw_rectangle(imagen_capturada_A,p,RED);	 
	vol->normal_points[vol->num_of_points++]=*p;
      }
      else if (is_the_center(*p,imagen_capturada_A) ){

	/* The center is unique, if there's more than one
	   that mean some problem in the color filtering
	*/
	if (!vol->center){
	  draw_rectangle(imagen_capturada_A,p,GREEN);
	  vol->center = (Tpoint2D*)malloc(sizeof(Tpoint2D));
	  *vol->center = *p;
	}else
	  return 0;
      }
      else if (is_ref_axe_point(*p,imagen_capturada_A)){

	if (!vol->ref_axe ){
	  vol->ref_axe = (Tline*)malloc(sizeof(Tline));
	  if(vol->ref_axe){
	    vol->ref_axe->num_points=0;
	    vol->ref_axe->points=(Tpoint2D*)malloc(sizeof(Tpoint2D)*REF_AXE_POINTS);
	  }
	}
	draw_rectangle(imagen_capturada_A,p,BLUE);
	vol->ref_axe->points[vol->ref_axe->num_points++] = *p;
		
      }else
	return 0;
    }
    
    pntos_elegidos_en_img_1[next_point_1].u = punto_pulsado_x;
    pntos_elegidos_en_img_1[next_point_1].v = punto_pulsado_y;
    pntos_elegidos_en_img_1[next_point_1].class = DRAW;

    next_point_1++;
    new_point_A=1;
  }

  free(p);

  return 1;
}

int freeobj_imagen_de_entrada_2_handle(FL_OBJECT* obj, int event, 
				     FL_Coord mouse_x, FL_Coord mouse_y,
				     int key, void* xev){
  
  /* variables para manejar el evento */  
  int punto_pulsado_x = 0;
  int punto_pulsado_y = 0;

  if (next_point_2==NUMPUNTOS)
    return -1;

  if (event == FL_PUSH){
    
    /* printf (" %d -- %d \n",mouse_x,mouse_y); */
    /* 
       las restas que hacen son hard-coded y se obtienen viendo en que sitio cae en la
       ventana la imagen, cogiendo las coordenedas de la esquina superior izquierda 
    */
    punto_pulsado_x = mouse_x-IMG_2_X;
    punto_pulsado_y = mouse_y-IMG_2_Y;
    
    pntos_elegidos_en_img_2[next_point_2].u = punto_pulsado_x;
    pntos_elegidos_en_img_2[next_point_2].v = punto_pulsado_y;
    pntos_elegidos_en_img_2[next_point_2].class = DRAW;
    next_point_2++;
    new_point_B=1;
  }
  
  return 1;
}


void calibrador_stop()
{
  /* printf("calibrador: cojo-stop\n");*/
  pthread_mutex_lock(&(all[calibrador_id].mymutex));
  put_state(calibrador_id,slept);
  printf("calibrador: off\n");
  pthread_mutex_unlock(&(all[calibrador_id].mymutex));
  /*  printf("calibrador: suelto-stop\n");*/
}

void calibrador_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[calibrador_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }
  
  pthread_mutex_lock(&(all[calibrador_id].mymutex));
  
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[calibrador_id].children[i]=FALSE;
  all[calibrador_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) calibrador_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {calibrador_brothers[i]=brothers[i];i++;}
    }


  if ((screen=(int *)myimport("graphics_xforms", "screen"))==NULL){
     fprintf (stderr, "teleoperator: I can't fetch screen from graphics_xforms\n");
     jdeshutdown(1);
  }
  if ((display=(Display *)myimport("graphics_xforms", "display"))==NULL){
     fprintf (stderr, "teleoperator: I can't fetch display from graphics_xforms\n");
     jdeshutdown(1);
  }

  /* Importamos colorA and launch the colorA child schema */
  mycolorA=myimport ("colorA", "colorA");
  colorArun=myimport("colorA", "run");
  colorAstop=myimport("colorA", "stop");  
  if (colorArun!=NULL)
    colorArun(calibrador_id, NULL, NULL);

  /* Importamos colorB and launch the colorB child schema */
  mycolorB=myimport ("colorB", "colorB");
  colorBrun=myimport("colorB", "run");
  colorBstop=myimport("colorB", "stop");  
  if (colorBrun!=NULL)
    colorBrun(calibrador_id, NULL, NULL);

  mycolorB = mycolorA;

  calibrador_callforarbitration=fn;
  put_state(calibrador_id,notready);
  printf("calibrador: on\n");
  pthread_cond_signal(&(all[calibrador_id].condition));
  pthread_mutex_unlock(&(all[calibrador_id].mymutex));
}

void *calibrador_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      /* printf("calibrador: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[calibrador_id].mymutex));

      if (all[calibrador_id].state==slept) 
	{
	  /*printf("calibrador: off\n");*/
	  /*  printf("calibrador: suelto para dormirme en condicional\n");*/
	  pthread_cond_wait(&(all[calibrador_id].condition),&(all[calibrador_id].mymutex));
	  /*  printf("calibrador: cojo tras dormirme en condicional\n");*/
	  /*printf("calibrador: on\n");*/
	  /* esto es para la aplicación, no tiene que ver con infraestrucura */
	  pthread_mutex_unlock(&(all[calibrador_id].mymutex));
	  /* printf("calibrador: iteration-suelto1\n");*/
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[calibrador_id].state==notready) put_state(calibrador_id,ready);
	  else all[calibrador_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[calibrador_id].state==ready) put_state(calibrador_id,winner);


	  if (all[calibrador_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[calibrador_id].mymutex));
	      /*printf("calibrador: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      calibrador_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = calibrador_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(calibrador_cycle*1000-diff);
	      else 
		{printf("time interval violated: calibrador\n"); usleep(calibrador_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[calibrador_id].mymutex));
	      /*printf("calibrador: iteration-suelto3\n");*/
	      usleep(calibrador_cycle*1000);
	    }
	}
    }
}

void calibrador_init(char *configfile)
{
  int argc=1;
  char **argv;
  char *aa;
  char a[]="myjde";
 
  aa=a;
  argv=&aa;

  /** Esto es para inicilizar glut*/
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  /* preparamos el display */

  pthread_mutex_lock(&(all[calibrador_id].mymutex));
  printf("calibrador schema started up\n");
  
  myexport("calibrador","calibrador_cycle",&calibrador_cycle);
  myexport("calibrador","calibrador_run",(void *)calibrador_run);
  myexport("calibrador","calibrador_stop",(void *)calibrador_stop);

  if (myregister_buttonscallback==NULL){
    if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
      printf ("I can't fetch register_buttonscallback from graphics_xforms\n");
      jdeshutdown(1);
    }
    if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
      printf ("I can't fetch delete_buttonscallback from graphics_xforms\n");
      jdeshutdown(1);
    }
    if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
      printf ("I can't fetch register_displaycallback from graphics_xforms\n");
      jdeshutdown(1);
    }
    if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
      jdeshutdown(1);
      printf ("I can't fetch delete_displaycallback from graphics_xforms\n");
    }
  }
  
  put_state(calibrador_id,slept);

  /** == inicializamos las variables globales y el display == */
  reset();
  
  pthread_create(&(all[calibrador_id].mythread),NULL,calibrador_thread,NULL);
  pthread_mutex_unlock(&(all[calibrador_id].mymutex));
}

void copy_lines_points_to_control_points(Tcalib_volume *vol){
  
  int i,j;

  if (!vol) return;

  next_point_1 = 0;
  
  if (vol->center){
    pntos_elegidos_en_img_1[next_point_1] = *vol->center;
    next_point_1++;
  }
  
  if (vol->sorted_lines)

    for (i=0; i<6; i++){
      
      if (DEBUG_LINES)
	printf("linea %d : pnts == %d \n",i,vol->sorted_lines[i].num_points);
      
      for (j=0; j<vol->sorted_lines[i].num_points; j++){

	if (DEBUG_LINES)
	  printf("lineas[%d][%d] ___ %d-%d\n",i,j,
		 vol->sorted_lines[i].points[j].u,vol->sorted_lines[i].points[j].v);
	
	vol->sorted_lines[i].points[j].class = -1;
	pntos_elegidos_en_img_1[next_point_1++] = vol->sorted_lines[i].points[j];
      }
      printf("\n");

    }
}

void capturar_imagen(){

  memcpy(imagen_capturada_A, (*mycolorA), ANCHO_IMAGEN*LARGO_IMAGEN*3);
  memcpy(imagen_capturada_B, (*mycolorA), ANCHO_IMAGEN*LARGO_IMAGEN*3);

  capturada_A=1;
  capturada_B=1;
  new_point_A = 1;
  new_point_B = 1;
  semi_auto_A=0;
  draw_result=0;
}


/** By default we can use the auto-detection in two ways:
    
    1. auto-detecting in real time using the image from colorA
    2. capturing the image and the auto-detecting the points
    
    In the case of using the captured image, we can active the
    semi-auto_detection that's if we miss some point we give the
    user the choice to select the undetected points. That may help
    the user to continue even if we havent auto-detect all the points
*/

void calib_auto_detect_control_points(int capturada, 
				      int *semi_auto,  
				      char* cap_image, 
				      char* real_image){

  if (!*semi_auto){
    
    if (capturada){
      
      vol = auto_detect_control_points(cap_image,
				       LARGO_IMAGEN, 
				       ANCHO_IMAGEN);
      
      if (is_valid_vol(vol, NORMAL_POINTS, REF_AXE_POINTS)){
	copy_lines_points_to_control_points(vol);
	fl_set_button(fd_calibradorgui->auto_mode,0);
	*semi_auto=1;
      }
      else if (vol){
	next_point_1 = get_num_of_points(vol);	
	*semi_auto=1;
      }else{
	*semi_auto=0;
	fl_set_button(fd_calibradorgui->auto_mode,0);
      }
	    
    }else{

      vol = auto_detect_control_points(real_image,
				       LARGO_IMAGEN, 
				       ANCHO_IMAGEN);
      if (DEBUG)
	print_volume(vol);
      
      /* If the returned calib_volume has the needed points and the
	 lines contained in were sorted correctly then we can proceed
	 with the calibration. Some times, even if all the points were
	 detected, if some point was detected out of the correct region
	 that make imposible to sort the lines and by the way we can't
	 use the data of this execution to calibrate the camera
      */
    
      if (is_valid_vol(vol, NORMAL_POINTS, REF_AXE_POINTS)) {
	capturar_imagen();
	copy_lines_points_to_control_points(vol);
	*semi_auto = 0;
	free_volume(&vol);
	fl_set_button(fd_calibradorgui->auto_mode,0);
	auto_mode_active = 0;
      }
    }
    
  }else if (*semi_auto && (NUMPUNTOS == next_point_1)){
    
    semi_auto_detect_control_points(vol,
				    cap_image,
				    LARGO_IMAGEN, 
				    ANCHO_IMAGEN
				    );
    
    if (DEBUG)
      print_volume(vol);
    
    if (is_valid_vol(vol, NORMAL_POINTS, REF_AXE_POINTS)) {
      copy_lines_points_to_control_points(vol);
      free_volume(&vol);
      *semi_auto = 0;
    }else
      free_volume(&vol);

    fl_set_button(fd_calibradorgui->auto_mode,0);
  }

  if (*semi_auto){
    fl_set_object_color(fd_calibradorgui->auto_mode,FL_TOMATO,FL_TOMATO);
    fl_deactivate_object(fd_calibradorgui->auto_mode);    
    fl_set_object_color(fd_calibradorgui->semi_auto,FL_GREEN,FL_GREEN);
  }

  new_point_A=1;

}

void calibrador_guibuttons(FL_OBJECT *obj)
{

  progeo_demo = 0;

  if (pushed(capturar_imagen)){
    
    capturar_imagen();
    fl_set_object_color(fd_calibradorgui->descartar_imagen,FL_GREEN,FL_GREEN);
    
  }else if (pushed(descartar_imagen)){
    
    /* Si test calib esta pulsado lo desactivamos */
    if (pushed(test_calib))
      fl_set_button(fd_calibradorgui->test_calib,0);
    
    capturada_A = 0;
    capturada_B = 0;
    semi_auto_A=0;
    draw_result=0;
    reset();

    if (vol) free_volume(&vol);

    /* Si el auto_mode esta activado, lo deasctivamos*/
    fl_set_object_color(fd_calibradorgui->descartar_imagen,FL_TOMATO, FL_TOMATO);
    fl_set_object_color(fd_calibradorgui->auto_mode,FL_GREEN,FL_GREEN);
    fl_activate_object(fd_calibradorgui->auto_mode);    
    fl_set_button(fd_calibradorgui->auto_mode,0);
    fl_set_object_color(fd_calibradorgui->semi_auto,FL_TOMATO,FL_TOMATO);
  }

  if (pushed(undo_select_1)){

    if (next_point_1>0){
      /* Deshacemos el ultimo punto seleccionado */
      next_point_1--;
      pntos_elegidos_en_img_1[next_point_1].class = UNDODRAW;
    }
    
  }else if (pushed(undo_select_2)){
    
    if (next_point_2>0){
      /* Deshacemos el ultimo punto seleccionado */
      next_point_2--;
      pntos_elegidos_en_img_2[next_point_2].class = UNDODRAW;
    }
  }
  
  hide_control_points = (fl_get_button(fd_calibradorgui->hide_control_points) == PUSHED);
  hide_selected_points = (fl_get_button(fd_calibradorgui->hide_selected_points)  == PUSHED);
  hide_test_points = (fl_get_button(fd_calibradorgui->hide_test_points) == PUSHED);
  test_calib = pushed(test_calib);
  auto_mode_active = (fl_get_button(fd_calibradorgui->auto_mode) == PUSHED);
  
  if (pushed(calibrar)) calibrar();
  
  progeo_demo = (fl_get_button(fd_calibradorgui->progeo_demo) == PUSHED);

}

void show_control_points_info_1(){

  char text[80]="";

  sprintf(text,"                             ");
  fl_set_object_label(fd_calibradorgui->numpuntos_1,text);  
  fl_set_object_label(fd_calibradorgui->totalpuntos,text);  
  fl_set_object_label(fd_calibradorgui->sig_punto_1,text);  

  sprintf(text,"Hay que Seleccionar %d puntos",NUMPUNTOS);
  fl_set_object_label(fd_calibradorgui->totalpuntos,text);  

  sprintf(text," %d pnts",next_point_1);
  fl_set_object_label(fd_calibradorgui->numpuntos_1,text);  
  
  if (next_point_1<NUMPUNTOS){
    sprintf(text," ( %d , %d , %d )",
	    pnts_en_objeto_de_control[next_point_1].x,
	    pnts_en_objeto_de_control[next_point_1].y,
	    pnts_en_objeto_de_control[next_point_1].z
	    );
  }else{
    sprintf(text," Hecho ");	    
  }
  
  fl_set_object_label(fd_calibradorgui->sig_punto_1,text);  

}

void show_control_points_info_2(){

  char text[80]="";

  sprintf(text,"                             ");
  fl_set_object_label(fd_calibradorgui->numpuntos_2,text);  
  fl_set_object_label(fd_calibradorgui->totalpuntos,text);  
  fl_set_object_label(fd_calibradorgui->sig_punto_2,text);  

  sprintf(text,"Hay que Seleccionar %d puntos",NUMPUNTOS);
  fl_set_object_label(fd_calibradorgui->totalpuntos,text);  

  sprintf(text," %d pnts",next_point_2);
  fl_set_object_label(fd_calibradorgui->numpuntos_2,text);  
  
  if (next_point_2<NUMPUNTOS){
    sprintf(text," ( %d , %d , %d )",
	    pnts_en_objeto_de_control[next_point_2].x,
	    pnts_en_objeto_de_control[next_point_2].y,
	    pnts_en_objeto_de_control[next_point_2].z
	    );
  }else{
    sprintf(text," Hecho ");	    
  }
  
  fl_set_object_label(fd_calibradorgui->sig_punto_2,text);  

}

void draw_all(int pnts_offset,
	      int numpnts,
	      char* imagen_capturada,
	      Tpoint2D* puntos_proyectados,
	      Tpoint2D* pntos_elegidos_en_img,
	      char* tmp_image
	      ){
  
  int ancho_del_cruz = 5;
  Tpoint2D p;
  int i,j,k,offset,point;

  i=j=k=0;


  if (draw_result){ 
    
  
    /*
      Repintamos los puntos 3D del objeto de control incluyendo los puntos proyectados
      de proyeccion que hemos calculado en el paso anterior. En rojo estan los puntos
      predeterminados, y en verde los puntos de chequeo y en azul los de test
    */
    
    for (i=pnts_offset; i<numpnts; i++){
      
      p = puntos_proyectados[i];
      offset = p.v*320+p.u;
    
      if ((imagen_capturada) && (p.u>0) && (p.v>0)){
	
	/* dibujamos una linea horizontal */
	for (j=-ancho_del_cruz; j<ancho_del_cruz; j++){
	  
	  /** Comprobamos que el punto a dibujar cae en la imagen **/
	  if (((offset+j)*4) < (ANCHO_IMAGEN*LARGO_IMAGEN*4 - 3)){
	    
	    if (i<NUMPUNTOS){
	      tmp_image[(offset+j)*4]   = 0;
	      tmp_image[(offset+j)*4+1] = 255;
	      tmp_image[(offset+j)*4+2] = 0;
	      tmp_image[(offset+j)*4+3] = 255;
	    }else{
	      tmp_image[(offset+j)*4]   = 130;
	      tmp_image[(offset+j)*4+1] = 130;
	      tmp_image[(offset+j)*4+2] = 130;
	      tmp_image[(offset+j)*4+3] = 255;
	    }
	    
	  }
	}
	
	k=0;
	/* dibujamos una linea vertical */
	for (j=-ancho_del_cruz*320; k<2*ancho_del_cruz; j+=320){
	  k++;
	  
	  if (((offset+j)*4) < (ANCHO_IMAGEN*LARGO_IMAGEN*4 - 3)){
	    if (i<NUMPUNTOS){
	      tmp_image[(offset+j)*4]   = 0;
	      tmp_image[(offset+j)*4+1] = 255;
	      tmp_image[(offset+j)*4+2] = 0;
	      tmp_image[(offset+j)*4+3] = 255;
	    }else{
	      tmp_image[(offset+j)*4]   = 130;
	      tmp_image[(offset+j)*4+1] = 130;
	      tmp_image[(offset+j)*4+2] = 130;
	      tmp_image[(offset+j)*4+3] = 255;
	    }
	  }
	}
      }
    }
  }
  
  if (!hide_selected_points){
    
    for (j=0; j<NUMPUNTOS; j++)
      {
	
	point = pntos_elegidos_en_img[j].v*320 + pntos_elegidos_en_img[j].u;
	
	/* point > 0 si ha sido actualizado por lo menos una vez */
	if ((point > 0) && (point < 77120)){
	  
	  if (pntos_elegidos_en_img[j].class == UNDODRAW){
	    
	    /* dibujamos una linea horizontal */
	    for (i=-ancho_del_cruz; i<ancho_del_cruz; i=i+2){
	      tmp_image[(point+i)*4] = 150;
	      tmp_image[(point+i)*4+1] = 150;
	      tmp_image[(point+i)*4+2] = 150;
	      tmp_image[(point+i)*4+3] = 255;
	    }
	    
	    k=0;
	    /* dibujamos una linea vertical */
	    for (i=-ancho_del_cruz*320; k<2*ancho_del_cruz; i+=320){
	      k++;
	      tmp_image[(point+i)*4] = 150;
	      tmp_image[(point+i)*4+1] = 150;
	      tmp_image[(point+i)*4+2] = 150;
	      tmp_image[(point+i)*4+3] = 255;
	    }
	    
	  }else if(pntos_elegidos_en_img[j].class == DRAW){
	    
	    /* dibujamos una linea horizontal */
	    for (i=-ancho_del_cruz; i<ancho_del_cruz; i++){
	      tmp_image[(point+i)*4]   = 0;
	      tmp_image[(point+i)*4+1] = 0;
	      tmp_image[(point+i)*4+2] = 255;
	      tmp_image[(point+i)*4+3] = 255;
	    }
	    
	    k=0;
	    /* dibujamos una linea vertical */
	    for (i=-ancho_del_cruz*320; k<2*ancho_del_cruz; i+=320){
	      k++;
	      tmp_image[(point+i)*4]   = 0;
	      tmp_image[(point+i)*4+1] = 0;
	      tmp_image[(point+i)*4+2] = 255;
	      tmp_image[(point+i)*4+3] = 255;
	    }	    
	  }
	}
      }
  }
}  

void calibrador_guidisplay()
{
  int numpnts, pnts_offset, i;
  float longi, lati, r;
  XImage *image;

  calibrador_win = FL_ObjWin(fd_calibradorgui->imagen_de_entrada_1);
  XGetWindowAttributes(display, calibrador_win, &win_attributes);  
  XMapWindow(display, calibrador_win);
  gc_values.graphics_exposures = False;
  calibradorgui_gc = XCreateGC(display, calibrador_win, GCGraphicsExposures, &gc_values);  
  
  /* Set the OpenGL state machine to the right context for this display */
  /* reset of the depth and color buffers */
  InitOGL(fd_calibradorgui->canvas, FL_ObjWin(fd_calibradorgui->canvas),fd_calibradorgui->canvas->w,fd_calibradorgui->canvas->h,NULL,NULL);


  if (foa_mode){
    
    r = fl_get_slider_value(fd_calibradorgui->foaR);
    longi=2*PI*mouse_on_canvas.u/360.;
    lati=2*PI*mouse_on_canvas.v/360.;
    
    foax=r*cos(lati)*cos(longi);
    foay=r*cos(lati)*sin(longi);
    foaz=r*sin(lati);
    
  }else if (cam_mode){
    
    longi = 2*PI*mouse_on_canvas.u/360.;
    lati = 2*PI*mouse_on_canvas.v/360.;
    
    xcam = radius*cos(lati)*cos(longi);
    ycam = radius*cos(lati)*sin(longi);
    zcam = radius*sin(lati);

    foax=0;
    foay=0;
    foaz=0;
  }

  /* proyeccion perspectiva */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.,(GLfloat)320/(GLfloat)240,1.0,100.0);
  gluLookAt(xcam,ycam,zcam,foax,foay,foaz,0.,0.,1.);

  /* pintamos todo el escenario con OGL 
     Nota: solo pintamos el escenario si ya se han hecho los calculos 
     de calibracion, para ello comprobamos si el centro de camara (vector x_1)
     ha sido calculado
  */
  draw_axes();
  draw_cube();

  /* Si hemos calibrado al menos una camara entonces dibujamos el escenario */
  if (x_1 || x_2)  draw_scenario();
  
  glXSwapBuffers(fl_display, fl_get_canvas_id(fd_calibradorgui->canvas));

  show_control_points_info_1();
  show_control_points_info_2();
   
  if ((next_point_2 == NUMPUNTOS) || (NUMPUNTOS == next_point_1)){
    fl_set_object_color(fd_calibradorgui->calibrar,FL_GREEN,FL_GREEN);
    fl_activate_object(fd_calibradorgui->calibrar);
  }
  else{
    fl_set_object_color(fd_calibradorgui->calibrar,FL_TOMATO,FL_TOMATO);
    fl_deactivate_object(fd_calibradorgui->calibrar);    
  }

  /** If the camera was calibrated then we active the option of test_calib*/
  if (K_1 && R_1 && x_1){
    fl_set_object_color(fd_calibradorgui->test_calib,FL_GREEN,FL_GREEN);
    fl_activate_object(fd_calibradorgui->test_calib);    
    fl_set_object_color(fd_calibradorgui->progeo_demo,FL_GREEN,FL_GREEN);
    fl_activate_object(fd_calibradorgui->progeo_demo);    
  }else{
    fl_set_object_color(fd_calibradorgui->test_calib,FL_TOMATO,FL_TOMATO);
    fl_deactivate_object(fd_calibradorgui->test_calib);   

    fl_set_object_color(fd_calibradorgui->progeo_demo,FL_TOMATO,FL_TOMATO);
    fl_deactivate_object(fd_calibradorgui->progeo_demo);    
  }

  /** Es un truco para dejar que se copie la imagen cuando pulsamos algun
      Buton de "hide" */
  if (hide_test_points || hide_control_points || hide_selected_points){ 
    new_point_A=1;
    new_point_B=1;
  }
  
  /** 
      Si se ha capturada una imagen, la visualizamos, sino seguirimos
      visualizando la imagen de entrada hasta que el usuario capture otra
      imagen para usarla como base de rectificacion
  */
  
  if (capturada_A){
    source_image_A = imagen_capturada_A;
  }else{
    source_image_A = (*mycolorA);
  }

  /* Optimizacion: Solo hacemos esta copia cuando el usuario ha pulsado
     en algun punto nuevo, o todavia no ha pulsado */
  
  if (new_point_A || !capturada_A){
    new_point_A=0;
    for(i=0; i<LARGO_IMAGEN*ANCHO_IMAGEN; i++) 
      { 
	tmp_image_A[i*4]   = source_image_A[i*3];   /* Blue Byte */
	tmp_image_A[i*4+1] = source_image_A[i*3+1]; /* Green Byte */
	tmp_image_A[i*4+2] = source_image_A[i*3+2]; /* Red Byte */
	tmp_image_A[i*4+3] = 255;                   /* Alfa byte */
      }
  }
      
  if (capturada_B){
    source_image_B = imagen_capturada_B;
  }else{
    source_image_B = (*mycolorB);
  }

  if (new_point_B || !capturada_B){
    new_point_B=0;
    for(i=0; i<LARGO_IMAGEN*ANCHO_IMAGEN; i++) 
      { 
	tmp_image_B[i*4+3] = 255; /* dummy byte */
	tmp_image_B[i*4]=source_image_B[i*3]; /* Red Byte */
	tmp_image_B[i*4+1]=source_image_B[i*3+1]; /* Green Byte */
	tmp_image_B[i*4+2]=source_image_B[i*3+2];   /* Blue Byte */
      }
  }
  
  /* Las lineas que viene a continuacion juegan con los offset dentro
     del array pnts_proyectados para mostrar solo los puntos de control,
     o ademas mostrar los puntos de test, o ninguno de los dos. En total
     hay cuatro combinaciones dependiendo de si los butones en cuestion han
     sido pulsados
  */
  
  if (!hide_control_points){
    if (!hide_test_points){
      
      /* Mostramos los puntos de control y de test*/
      numpnts = NUMPUNTOS+TESTPOINTS;
      pnts_offset=0;
    }else{
      /* Mostramos los puntos de control*/
      numpnts = NUMPUNTOS;
      pnts_offset=0;
    }
    
  }else{
    /* Mostramos los puntos de test*/
    if (!hide_test_points){
      numpnts = TESTPOINTS+NUMPUNTOS;
      pnts_offset=NUMPUNTOS;
    }else{
      /* No se muestra nada*/
      numpnts = -1;
      pnts_offset=0;
    }
  }
    
  /********************/
  draw_all(pnts_offset, numpnts,
	   imagen_capturada_A, puntos_proyectados_1, pntos_elegidos_en_img_1, 
	   tmp_image_A);
  
  draw_all(pnts_offset, numpnts,
	   imagen_capturada_B, puntos_proyectados_2, pntos_elegidos_en_img_2, 
	   tmp_image_B);
  /********************/
  

  if (auto_mode_active){
    calib_auto_detect_control_points(capturada_A, &semi_auto_A, imagen_capturada_A, (*mycolorA));
    
    /* TODO:
       
       test the behaviour of the auto calibration in real time of A and B at the same time
       
       calib_auto_detect_control_points(capturada_B, &semi_auto_B, imagen_capturada_B, (*mycolorB));
    */
  }


  if (!test_calib){
    
    /* Creamos y dibujamos la imagen */
    image = XCreateImage(display, DefaultVisual(display,*screen), win_attributes.depth, 
			 ZPixmap, 0, tmp_image_A ,ANCHO_IMAGEN, LARGO_IMAGEN,8,0);
    
    if (image){
	XPutImage(display,calibrador_win,calibradorgui_gc, image,0,0,
		  fd_calibradorgui->imagen_de_entrada_1->x+1, 
		  fd_calibradorgui->imagen_de_entrada_1->y+1, ANCHO_IMAGEN, LARGO_IMAGEN);
    }else
      printf("No puedo copiar la imagen A!\n");
    
    
    /* Creamos y dibujamos la imagen */
    image = XCreateImage(display, DefaultVisual(display,*screen), win_attributes.depth, 
			 ZPixmap, 0, tmp_image_B ,ANCHO_IMAGEN, LARGO_IMAGEN,8,0);
    
    if (image){
      XPutImage(display,calibrador_win,calibradorgui_gc,image,0,0,
		fd_calibradorgui->imagen_de_entrada_2->x+1, 
		fd_calibradorgui->imagen_de_entrada_2->y+1, ANCHO_IMAGEN, LARGO_IMAGEN);
    }else
      printf("No puedo copiar la imagen A!\n");
  }
    
  if (test_calib && imagen_capturada_A){
    fl_set_foreground(calibradorgui_gc,FL_GREEN);
    my_test_calib();
  }  
}

void calibrador_hide(void)
{
  delete_buttonscallback(calibrador_guibuttons);
  delete_displaycallback(calibrador_guidisplay);
  fl_hide_form(fd_calibradorgui->calibradorgui);
}

/** Capturamos los eventos del raton sobre el canvas para ofrecer
    al usuario un control intuitivo usando solo el raton, de tal
    manera que puede cambiar la posicion de la camara de OGL y el 
    foa de manera mucho más facil.

    Si el usuario pulsa el buton 1 entonces pasa a controlar la posicion
    de la camara virtual, si pulsa el buton 2 entonces lo que se controla
    es el foa. En los dos casos el usuario puede ajustar el radio de giro
    usando los sliders en la GUI.
*/
void motion_event(FL_OBJECT *ob, Window win, int win_width, 
	   int win_height, XKeyEvent *xev, void *user_data){
  
  xev = (XKeyPressedEvent*)xev;

  /** las siguiente ecuaciones sirven mapear el movimiento:
      
      Eje X :: (0,w) -> (-180,180)
      Eje Y :: (0,h) -> (-90,90)
      
      donde (h,w) es el larcho,ancho del gl_canvas
      Si el button pulsado es 2 (centro del raton) entonces no actualizamos la posicion
      
  */
  
  if (!motion_on) return;
  mouse_on_canvas.u = ((xev->x*360/fd_calibradorgui->canvas->w)-180);
  mouse_on_canvas.v = ((xev->y*-180/fd_calibradorgui->canvas->h)+90);
}

void button_press_event(FL_OBJECT *ob, Window win, int win_width, 
	   int win_height, XKeyEvent *xev, void *user_data){
  
  xev = (XKeyPressedEvent*)xev;

  /** Lod dos modos son exluyentes */
  if (2 == xev->keycode){
    motion_on = !motion_on;
    return;
  }if (1 == xev->keycode){
    cam_mode = 1;
    foa_mode = 0;  
  }else if (3 == xev->keycode){
    foa_mode = 1;
    cam_mode = 0;
  }else if (5 == xev->keycode){
    if (radius < MAX_RADIUS_VALUE)
      radius+=WHEEL_DELTA;
  }else if (4 == xev->keycode){
    if (radius>MIN_RADIUS_VALUE)
      radius-=WHEEL_DELTA;
  }
  
}

void calibrador_show(void)
{
  static int k=0;
  float r;
  
  if (k==0) /* not initialized */
    {
      k++;
      fd_calibradorgui = create_form_calibradorgui();
      fl_set_form_position(fd_calibradorgui->calibradorgui,400,50);
      fl_add_canvas_handler(fd_calibradorgui->canvas,Expose,InitOGL,0);
      fl_add_canvas_handler(fd_calibradorgui->canvas,MotionNotify,motion_event,0);
      fl_add_canvas_handler(fd_calibradorgui->canvas,ButtonPress,button_press_event,0);
      
      RGB2HSV_init();
      RGB2HSV_createTable();

      foa_mode = 0;
      cam_mode = 0;
      motion_on = 1;
      
      xcam=10;
      ycam=10;
      zcam=10;
      radius=10.0;

      r=5.0;
      fl_set_slider_bounds(fd_calibradorgui->foaR,10,0.);
      fl_set_slider_value(fd_calibradorgui->foaR,(double)r); 
    }
  
  myregister_buttonscallback(calibrador_guibuttons);
  myregister_displaycallback(calibrador_guidisplay);
  fl_show_form(fd_calibradorgui->calibradorgui,FL_PLACE_POSITION,
	       FL_FULLBORDER,"calibrador");
  
}

void calibrador_terminate()
{
  printf("calibrador terminated\n");
}
