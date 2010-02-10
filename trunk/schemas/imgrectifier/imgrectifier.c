/*
 *  Copyright (C) 2006 José María Cañas Plaza / Kachach Redouane
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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 *  Authors : Kachach Redouane
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include <jde.h>
#include <limits.h>
#include <graphics_gtk.h>
#include <imgrectifier.h>
#include <string.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

/** Vamos a usar GSL para operaciones matriciales*/
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>



/* Clallback definition */
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_image_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data);
void on_image_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data);
void on_descartar_imagen_clicked(GtkButton *button, gpointer user_data);
void on_rectificar_clicked(GtkButton *button, gpointer user_data);
void on_descartar_rectificada_clicked(GtkButton *button, gpointer user_data);
void on_undo_clicked(GtkButton *button, gpointer user_data);
void on_undo_2_clicked(GtkButton *button, gpointer user_data);
void on_modo_rectangulo_clicked(GtkToggleButton *button, gpointer user_data);
void on_imagen_de_entrada_release(GtkButton *button, gpointer user_data);
void on_imagen_rectificada_release(GtkButton *button, gpointer user_data);
void on_help_clicked(GtkButton *button, gpointer user_data);
gboolean on_help_destroy (GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_help_close (GtkWidget *widget, GdkEvent *event, gpointer user_data);
      
int *width=NULL;
int *height=NULL;

/*Gui callbacks*/
registerdisplay myregister_displaycallback=NULL;
deletedisplay mydelete_displaycallback=NULL;

int imgrectifier_id=0; 
int imgrectifier_brothers[MAX_SCHEMAS];
arbitration imgrectifier_callforarbitration;

enum imgrectifier_states {init,t1,r1,t2,r2,t3,r3,t4,end};
static int imgrectifier_state;

/* exported variables */
int imgrectifier_cycle=100; /* ms */

/*imported variables */
unsigned char **mycolor=NULL;
runFn myrun=NULL;
stopFn mystop=NULL;
pthread_mutex_t colorchange_mutex;

/*Para el gui con gtk*/
GladeXML *xml=NULL; /*Fichero xml*/
GtkWidget *win=NULL;

/** ============= Variable propios del esquema ============= */
#define DRAW 1
#define UNDODRAW 2
#define PUSHED 1
#define RELEASED 0
#define NUMPUNTOS 4
/* cada punto da lugar a dos ecuaciones */
#define NUMEC  NUMPUNTOS*2

typedef struct point{
  int x;
  int y;
  int draw;
} Tpoint;
   
/* Almacena los 4 puntos que han sido elegidos */
Tpoint pnts_elegidos_en_img_entrada[NUMPUNTOS];
Tpoint pnts_elegidos_en_img_rectificada[NUMPUNTOS];

/** 
    Sistema de ecuaciones lineales. Se le alojara memoria en la
    funciona init
*/
int **sistema_lineal_ecuaciones;/*[NUMEC][9]*/

/* Siempre es de este tamaño */
double matriz_solucion[9];
/* alfa es el C9 de la matriz resultado, de momento es constante !!!*/
int alfa = 1;

/** puntero al siguiente punto para almacenar*/
int next_point_en_entrada;
int next_point_en_rectificada;
int modo_rectangulo_on=0;

unsigned char *imagen_capturada;
unsigned char *imagen_rectificada;
/** ============== fin de declaracion esquema ============== */


void imgrectifier_iteration()
{  
  speedcounter(imgrectifier_id);
}

/* Dado un punto en la imagen origen, calcula su correpondiente
   en la imagen rectificada
*/
void calcular_correspondencia(int x,int y,Tpoint *p){
  
  /*
    Formulas Base para pasar de un punto (x,y) -> (x',y'). Nuestro H(3x3) en este
    caso es "matriz_solucion" que es un array plano de 9 posiciones
    
    x' = (h1*x + h2*y + h3)/(h7*x + h8*y + h9)
    y' = (h4*x + h5*y + h6)/(h7*x + h8*y + h9)
 
  */
  
  p->x = (int) (matriz_solucion[0]*x + matriz_solucion[1]*y + matriz_solucion[2])/
    (matriz_solucion[6]*x + matriz_solucion[7]*y + matriz_solucion[8]);

  p->y = (int) (matriz_solucion[3]*x + matriz_solucion[4]*y + matriz_solucion[5])/
    (matriz_solucion[6]*x + matriz_solucion[7]*y + matriz_solucion[8]);

}

/* Resuelve el sistema sobredimensioando (un sistema con más ecuaciones de las
   necesarias) dando una solución que minimize el error cometido. Quiere decir
   minimize la suma de los cuadrados del error cometido en cada punto y su correpondiente
   calculado a traves de la matriz solución
*/
void regresion_lineal_multiple(double a_data[NUMEC*8], double b_data[NUMEC]){

  int i,j,k,aux;
  double chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *c;
  gsl_multifit_linear_workspace * work;
  
  X = gsl_matrix_alloc(NUMEC,8);
  y = gsl_vector_alloc(NUMEC);
  c = gsl_vector_alloc (8);
  cov = gsl_matrix_alloc (8,8);
  
  /* Prepramos la matriz de muestras 

     NOTA: El sistema de ecuaciones a resolver contiene
     8 incognitas en vez de nueve. La novena incognita es
     igual a alfa (constante), y hay que inicializarla sino
     La solucion siempre sera el vector nulo
  */
  
  for (i=0; i<NUMEC; i++){
    for (j=0; j<8; j++)
      {
	aux = i*8+j;
	gsl_matrix_set(X,i,j,a_data[aux]);
      }
  }
  
  /* Inicializamos el verctor de muestras */
  for (k=0; k<NUMEC; k++)
    {
      gsl_vector_set(y,k,b_data[k]);
    }
  
  /* Inicializamos y resolver el sistema sobredimensioando */
  work = gsl_multifit_linear_alloc (NUMEC,8);
  gsl_multifit_linear (X,  y, c, cov, &chisq, work);
  gsl_multifit_linear_free (work);

  /* sacamor por pantalla la suma de los cuadrados del error cometido 
   a la hora de optimizar la matriz solución. */

  /** copiamos la solución */
  for (i=0; i<8; i++)
    {
      matriz_solucion[i] = gsl_vector_get(c,i);
    }
}


/* Dados dos puntos p(x,y) y p'(x',y') obtiene dos
 ecuaciones fruto de la correspondencia entre los dos pnts
*/
void obtener_ecuacion(Tpoint p , 
		      Tpoint p_prima,
		      int **ecuacion){
  
  /** 
      Rellenamos la primera fila con los siguientes coeficientes :

      p(x,y)
      p_prima(x',y')

      Ecuacion1 -> x.x'.C7 + y.x'.C8 - C1.x -C2.y + x' -C3 -C9.x'  = 0
      Ecuacion2 -> x.y'.C7 + y.y'.C8 + y' - C4.x -C5.y - C6 -C9.y' = 0
  */

  /* Ecuación 1 */
  ecuacion[0][0] = -p.x;
  ecuacion[0][1] = -p.y;
  ecuacion[0][2] = -1;
  ecuacion[0][3] = 0;
  ecuacion[0][4] = 0;
  ecuacion[0][5] = 0;
  ecuacion[0][6] = p.x*p_prima.x;
  ecuacion[0][7] = p.y*p_prima.x;
  ecuacion[0][8] = -p_prima.x*alfa;

  /* Ecuación 2 */
  ecuacion[1][0] = 0;
  ecuacion[1][1] = 0;
  ecuacion[1][2] = 0;
  ecuacion[1][3] = -p.x;
  ecuacion[1][4] = -p.y;
  ecuacion[1][5] = -1;
  ecuacion[1][6] = p_prima.y*p.x;
  ecuacion[1][7] = p_prima.y*p.y;
  ecuacion[1][8] = -p_prima.y*alfa;
}

void resolver_sistema_de_ecuaciones(){

  int k,i,j;
  int **ecuacion_lineal;
  double a_data[8*NUMEC];
  double b_data[NUMEC];

  ecuacion_lineal = malloc(2*sizeof(int*));
  ecuacion_lineal[0] = malloc(9*sizeof(int));
  ecuacion_lineal[1] = malloc(9*sizeof(int));
  
  /* recoremos los dos arrays con los putnos almacenados, y vamos obteniendo 
     las ecuaciones para cada par de puntos. Cada par de pnts da lugar a dos ecuaciones.
  */
  for (i=0; i<NUMPUNTOS; i++){
    
    obtener_ecuacion(pnts_elegidos_en_img_rectificada[i], 
		     pnts_elegidos_en_img_entrada[i],
		     ecuacion_lineal);
    
    /** copiamos la ecuacion obtenida al sistema lineal sobre dimensionado*/
    for (j=0; j<9; j++){
      sistema_lineal_ecuaciones[i*2][j] = ecuacion_lineal[0][j];
      sistema_lineal_ecuaciones[i*2+1][j] = ecuacion_lineal[1][j];
    }
  }
  
  /** vamos a resolver Ax=b*/

  /** copiamos la matriz "A" (sistema de lineal de ecuaciones) */
  k = 0;
  for (i=0; i<NUMEC; i++){
    for (j=0; j<8; j++){
      a_data[k++] = sistema_lineal_ecuaciones[i][j];
    }
  }

  /** Copiamos el vector "b" (la ultima columna)*/
  for (j=0; j<NUMEC; j++)
    b_data[j] = sistema_lineal_ecuaciones[j][8];
      
  regresion_lineal_multiple(a_data, b_data);
}


/* Este procedemiento recogera la información de los NUMPUNTOS puntos "emparejados",
   creara un sistema de ecuaciones sobredimensionado, lo resolvera, y con la matriz resultado
   pintara la imagen de salida rectificada
*/
void rectificar_imagen(){
  
  Tpoint p;
  int i,j,offset,offset2;

  resolver_sistema_de_ecuaciones();
  
  /*
    Construimos la imagen recitificada para ello:
    Recoremos la imagen rectificada, para cada punto buscamos su correspondiente
    en la imagen de entrada, si este cae fuera lo pintamos en blanco. Si cae dentro
    lo copiamos (pintamos un pixel del mismo color en la imagen rectificada).
  */
  
  for (i=0; i<(*height); i++){
    for (j=0; j<(*width); j++){
      
      calcular_correspondencia(j,i,&p);
    
      /** 
	  Si el punto resultante cae dentro de la imagen lo dibujamos
	  sino lo pintamos en blando
      */
      
      offset2 = i*(*width)+j;
      if (imagen_rectificada && imagen_capturada){
	if ((p.x>0) && (p.y>0) && (p.x<=(*width)) && (p.y<=(*height))){
	  
	  offset = p.y*(*width)+p.x;
	  
	  imagen_rectificada[offset2*3] = imagen_capturada[offset*3+2];
	  imagen_rectificada[offset2*3+1] = imagen_capturada[offset*3+1];
	  imagen_rectificada[offset2*3+2] = imagen_capturada[offset*3];
	}else{
	  /* pintamos en Blanco los pixeles "desconozidos */
	  imagen_rectificada[offset2*3] = 255;
	  imagen_rectificada[offset2*3+1] = 255;
	  imagen_rectificada[offset2*3+2] = 255;;
	  
	}
      }
    }
  }
}

void imgrectifier_stop()
{
  /* printf("imgrectifier: cojo-stop\n");*/
  pthread_mutex_lock(&(all[imgrectifier_id].mymutex));
  put_state(imgrectifier_id,slept);
  printf("imgrectifier: off\n");
  if (mystop!=NULL)
     mystop();
  pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
  /*  printf("imgrectifier: suelto-stop\n");*/
}

void imgrectifier_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[imgrectifier_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[imgrectifier_id].mymutex));

  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[imgrectifier_id].children[i]=FALSE;
  all[imgrectifier_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) imgrectifier_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {imgrectifier_brothers[i]=brothers[i];i++;}
    }
  imgrectifier_callforarbitration=fn;
  put_state(imgrectifier_id,notready);
  printf("imgrectifier: on\n");
  pthread_cond_signal(&(all[imgrectifier_id].condition));
  pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
}

void *imgrectifier_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      /* printf("imgrectifier: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[imgrectifier_id].mymutex));

      if (all[imgrectifier_id].state==slept) 
	{
	  pthread_cond_wait(&(all[imgrectifier_id].condition),&(all[imgrectifier_id].mymutex));
	  /*  printf("imgrectifier: cojo tras dormirme en condicional\n");*/
	  /*printf("imgrectifier: on\n");*/
	  imgrectifier_state=init; 
	  /* esto es para la aplicación, no tiene que ver con infraestrucura */
	  pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
	  /* printf("imgrectifier: iteration-suelto1\n");*/
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[imgrectifier_id].state==notready) put_state(imgrectifier_id,ready);
	  else all[imgrectifier_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[imgrectifier_id].state==ready) put_state(imgrectifier_id,winner);


	  if (all[imgrectifier_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
	      /*printf("imgrectifier: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      imgrectifier_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = imgrectifier_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(imgrectifier_cycle*1000-diff);
	      else 
		{printf("time interval violated: imgrectifier\n"); usleep(imgrectifier_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
	      /*printf("imgrectifier: iteration-suelto3\n");*/
	      usleep(imgrectifier_cycle*1000);
	    }
	}
    }
}

void reset(){
  
  int i;
  static int init=0;
  /* ============================================================== */

  if (init==0){
     sistema_lineal_ecuaciones = malloc(NUMEC*sizeof(int*));
     for (i=0; i<NUMEC; i++)
        sistema_lineal_ecuaciones[i] = malloc(9*sizeof(int));
     init=1;
  }
  else{
     if (imagen_rectificada!=NULL)
        free(imagen_rectificada);
  }
  if (width!=NULL && height!=NULL)
     imagen_rectificada = (unsigned char*)malloc((*width)*(*height)*3*sizeof(char));
  
  /* inicializamos el array de los puntos elegidos */
  for (i=0; i<NUMPUNTOS; i++){
    pnts_elegidos_en_img_entrada[i].x = -1;
    pnts_elegidos_en_img_entrada[i].y = -1;
    pnts_elegidos_en_img_entrada[i].draw = DRAW;
    
    pnts_elegidos_en_img_rectificada[i].x =-1;
    pnts_elegidos_en_img_rectificada[i].y =-1;
    pnts_elegidos_en_img_rectificada[i].draw = DRAW;
  }
  
  /* inicialiazamos la imagen rectificada con el color gris*/
  /*TODO Mover esto cuando se pinta bien la imagen recitifacada */

  if (height!=NULL && width!=NULL){
     for(i=0; i<(*height)*(*width); i++)
     { 
        imagen_rectificada[i*3] = 120;
        imagen_rectificada[i*3+1] = 120;
        imagen_rectificada[i*3+2] = 120;
     }
  }
  next_point_en_entrada = 0;
  next_point_en_rectificada = 0;

  /* TODO: fijar el valor constante .. */
  matriz_solucion[8] = alfa;

  /* ============================================================== */
}

void imgrectifier_guiinit(){
   if (myregister_displaycallback==NULL){
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_gtk", "register_displaycallback"))==NULL){
         printf ("I can't fetch register_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
      }
   }
}

void imgrectifier_terminate()
{
  printf ("imgrectifier terminate\n");
}


void imgrectifier_init(char *configfile)
{
  pthread_mutex_lock(&(all[imgrectifier_id].mymutex));
  printf("imgrectifier schema started up\n");
  imgrectifier_guiinit();

  myexport("imgrectifier","id",&imgrectifier_id);
  myexport("imgrectifier","cycle",&imgrectifier_cycle);
  myexport("imgrectifier","run",(void *)imgrectifier_run);
  myexport("imgrectifier","stop",(void *)imgrectifier_stop);
  put_state(imgrectifier_id,slept);

  /* == inicializamos las variables globales y el display== */
  pthread_mutex_init(&colorchange_mutex,NULL);

  reset();

  modo_rectangulo_on = 0;
  
  pthread_create(&(all[imgrectifier_id].mythread),NULL,imgrectifier_thread,NULL);
  pthread_mutex_unlock(&(all[imgrectifier_id].mymutex));
}

/*Callbacks*/

gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   imgrectifier_hide();
   gdk_threads_enter();
   return TRUE;
}

void on_image_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
   /*Check value*/
   void *value;
   char **color;
   value=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);

   color=myimport(value,value);
   if (color!=NULL){
      pthread_mutex_lock(&colorchange_mutex);
      if (mystop!=NULL)
         mystop();
      mycolor=(unsigned char **)color;
      myrun=(runFn)myimport(value, "run");
      mystop=(runFn)myimport(value, "stop");
      width=(int *)myimport(value,"width");
      height=(int *)myimport(value,"height");
      myrun(imgrectifier_id,NULL,NULL);
      if (imagen_capturada)
         free(imagen_capturada);
      imagen_capturada=NULL;
      reset();
      on_descartar_rectificada_clicked(NULL, NULL);
      on_descartar_imagen_clicked(NULL, NULL);
      pthread_mutex_unlock(&colorchange_mutex);
   }
}

void on_capturar_imagen_clicked(GtkButton *button, gpointer user_data){
   int i;
   /** Reservamos memoria si todavia no lo hemos hecho */
   if (!imagen_capturada){
      imagen_capturada = (unsigned char*)malloc((*width)*(*height)*3*sizeof(char));
   }
   memcpy(imagen_capturada, (unsigned char *)*mycolor, (*width)*(*height)*3);

   /** inicializamos la imagen rectificada tambien */
   for(i=0; i<(*height)*(*width); i++){
      imagen_rectificada[i*3] = 120;
      imagen_rectificada[i*3+1] = 120;
      imagen_rectificada[i*3+2] = 120;
   }
}

void on_descartar_imagen_clicked(GtkButton *button, gpointer user_data){
   if (imagen_capturada!=NULL){
      free(imagen_capturada);
      imagen_capturada = 0x0;
   }
   reset();
}

void on_rectificar_clicked(GtkButton *button, gpointer user_data){
   rectificar_imagen();
}

void on_descartar_rectificada_clicked(GtkButton *button, gpointer user_data){
   int i;
   sistema_lineal_ecuaciones = malloc(NUMEC*sizeof(int*));
   for (i=0; i<NUMEC; i++)
      sistema_lineal_ecuaciones[i] = malloc(9*sizeof(int));

   if (height!=NULL && width!=NULL){
      for(i=0; i<(*height)*(*width); i++)
      {
         imagen_rectificada[i*3+0] = 120;
         imagen_rectificada[i*3+1] = 120;
         imagen_rectificada[i*3+2] = 120;
      }

      /* inicializamos el array de los puntos elegidos */
      for (i=0; i<NUMPUNTOS; i++){
         pnts_elegidos_en_img_rectificada[i].x =-1;
         pnts_elegidos_en_img_rectificada[i].y =-1;
      }
   }

   next_point_en_rectificada=0;
}

void on_undo_clicked(GtkButton *button, gpointer user_data){
   /* Deshacemos el ultimo punto seleccionado */
   if (next_point_en_entrada>0){
      next_point_en_entrada--;
      pnts_elegidos_en_img_entrada[next_point_en_entrada].draw = UNDODRAW;
   }
}

void on_undo_2_clicked(GtkButton *button, gpointer user_data){
   /* Deshacemos el ultimo punto seleccionado */
   if (next_point_en_rectificada>0){
      next_point_en_rectificada--;
      pnts_elegidos_en_img_rectificada[next_point_en_rectificada].draw = UNDODRAW;
   }
}

void on_modo_rectangulo_clicked(GtkToggleButton *button, gpointer user_data){
   int i;
   if (gtk_toggle_button_get_active(button)){
      modo_rectangulo_on = 1;
      next_point_en_rectificada = 0;
      for (i=0; i<4;i++){
         pnts_elegidos_en_img_rectificada[i].x =-1;
         pnts_elegidos_en_img_rectificada[i].y =-1;
         pnts_elegidos_en_img_rectificada[i].draw = DRAW;
      }
   }
   else{
      modo_rectangulo_on =0;
   }
}

void on_imagen_de_entrada_release(GtkButton *button, gpointer user_data){
   gint win_x, win_y;
   gdk_window_at_pointer (&win_x, &win_y); /*Posición en la ventana*/

   if (next_point_en_entrada != NUMPUNTOS){
    /*
     las restas que hacen son hard-coded y se obtienen viendo en que sitio cae en la
     ventana la imagen, cogiendo las coordenedas de la esquina superior izquierda
    */
      /* actualizamos el array de puntos elegidos, y actualizamos el next_point_en_entrada */
      pnts_elegidos_en_img_entrada[next_point_en_entrada].x = win_x;
      pnts_elegidos_en_img_entrada[next_point_en_entrada].y = win_y;
      pnts_elegidos_en_img_entrada[next_point_en_entrada].draw = DRAW;
      next_point_en_entrada++;
   }
}

void on_imagen_rectificada_release(GtkButton *button, gpointer user_data){
   gint win_x, win_y;
   gdk_window_at_pointer (&win_x, &win_y); /*Posición en la ventana*/

   if (!modo_rectangulo_on){

      if (next_point_en_rectificada !=NUMPUNTOS){
         /* actualizamos el array de puntos elegidos, y actualizamos el next_point_en_entrada */
         pnts_elegidos_en_img_rectificada[next_point_en_rectificada].x = win_x;
         pnts_elegidos_en_img_rectificada[next_point_en_rectificada].y = win_y;
         pnts_elegidos_en_img_rectificada[next_point_en_rectificada].draw = DRAW;
         next_point_en_rectificada++;
      }
   }else{

      /** Si el modo "rectangulo" ha sido activado solo se eligen dos puntos:
      esquina izq sup y esquina inf derecha. De esta manera obtenemos un
      rectangulo perfecto y evitamos impreciones debidas a la eleccion de
      los cuatro puntos a mano. las dos otras esquinas queda automaticamente
      defenidas
       **/

      /* Hack un poco "feo". Si el usuario elige un par de puntos estando
      en el modo normal, y luego activa el modo rectangulo, hay que
      poner la variable next_point_en_rectificada a zero */
      if (next_point_en_rectificada!=3) next_point_en_rectificada = 0;

      pnts_elegidos_en_img_rectificada[next_point_en_rectificada].x = win_x;
      pnts_elegidos_en_img_rectificada[next_point_en_rectificada].y = win_y;
      pnts_elegidos_en_img_rectificada[next_point_en_rectificada].draw = DRAW;

      /** Solo y solo si las dos esquinas estan defenidas, las otras dos esquinas
      se auto calculan
       */

      if ((pnts_elegidos_en_img_rectificada[0].x >= 0 )
           &&
           (pnts_elegidos_en_img_rectificada[3].x >= 0 )
         ){

         pnts_elegidos_en_img_rectificada[1].x = pnts_elegidos_en_img_rectificada[3].x;
         pnts_elegidos_en_img_rectificada[1].y = pnts_elegidos_en_img_rectificada[0].y;
         pnts_elegidos_en_img_rectificada[1].draw = DRAW;

         pnts_elegidos_en_img_rectificada[2].x = pnts_elegidos_en_img_rectificada[0].x;
         pnts_elegidos_en_img_rectificada[2].y = pnts_elegidos_en_img_rectificada[3].y;
         pnts_elegidos_en_img_rectificada[2].draw = DRAW;
         }

         if (next_point_en_rectificada == 0){
            next_point_en_rectificada = 3;
         }else{
            next_point_en_rectificada = 0;
         }
   }
}

void on_help_clicked(GtkButton *button, gpointer user_data){
   gtk_widget_show(GTK_WIDGET(glade_xml_get_widget(xml, "about_imgrectifier")));
}

void on_help_close (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(GTK_WIDGET(glade_xml_get_widget(xml, "about_imgrectifier")));
   gtk_widget_queue_draw(GTK_WIDGET(glade_xml_get_widget(xml, "about_imgrectifier")));
}

gboolean on_help_destroy (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(widget);
   gtk_widget_queue_draw(widget);
   return TRUE;
}

void imgrectifier_guidisplay()
{
  /* variables para dibujar la imagen */
  static unsigned char *tmp_image=NULL;
  static unsigned char *tmp_image_rect=NULL;
  unsigned char *source_image;
  int i,j,k,point,ancho_del_cruz;
  int from,to,inc;
  int old_height, old_width;

  pthread_mutex_lock(&colorchange_mutex);
  if (width!=NULL && height!=NULL){
     if (tmp_image==NULL){
        tmp_image = (unsigned char *) malloc((*width)*(*height)*3);
        old_height=(*height);
        old_width=(*width);
     }
     if (tmp_image_rect==NULL){
        tmp_image_rect = (unsigned char *) malloc((*width)*(*height)*3);
     }

     if (old_height!=(*height) || old_width!=(*width)){
        free(tmp_image);
        tmp_image = (unsigned char *) malloc((*width)*(*height)*3);
        free(tmp_image_rect);
        tmp_image_rect = (unsigned char *) malloc((*width)*(*height)*3);
        old_height=(*height);
        old_width=(*width);
     }


     /* Creamos una copia temporal de la imagen rectificada */
     tmp_image_rect = memcpy(tmp_image_rect, imagen_rectificada, (*width)*(*height)*3);

     ancho_del_cruz = 5;
 
  /** 
     Si se ha capturada una imagen, la visualizamos, sino seguirimos
     visualizando la imagen de entrada hasta que el usuario capture otra
     imagen para usarla como base de rectificación
   */

     if (imagen_capturada){
        source_image = imagen_capturada;
     }else{
        source_image = *mycolor;
     }

     if ((NUMPUNTOS == next_point_en_entrada) && (imagen_capturada)){
        gtk_widget_set_sensitive(glade_xml_get_widget(xml, "rectificar"),TRUE);
     }
     else{
        gtk_widget_set_sensitive(glade_xml_get_widget(xml, "rectificar"),FALSE);
     }

     /* copiamos el contenido de colorA en el array-imagen tmp_image */
     for(i=0; i<(*height)*(*width); i++)
     {
        tmp_image[i*3]=source_image[i*3+2];
        tmp_image[i*3+1]=source_image[i*3+1];
        tmp_image[i*3+2]=source_image[i*3];
     }
  
     if (gtk_toggle_button_get_active((GtkToggleButton *)glade_xml_get_widget(xml,"hide_selected_points"))==FALSE){
    
        for (j=0; j<NUMPUNTOS; j++)
        {
	
           point = pnts_elegidos_en_img_entrada[j].y*(*width) + pnts_elegidos_en_img_entrada[j].x;

           /* point > 0 si ha sido actualizado por lo menos una vez */
           if (point > 0){
	  
              if (pnts_elegidos_en_img_entrada[j].draw == UNDODRAW){
	    
                 /* dibujamos una linea horizontal */
                 for (i=-ancho_del_cruz; i<ancho_del_cruz; i=i+2){
                    tmp_image[(point+i)*3] = 100;
                    tmp_image[(point+i)*3+1] = 100;
                    tmp_image[(point+i)*3+2] = 150;
                 }
	    
                 k=0;
                 /* dibujamos una linea vertical */
                 for (i=-ancho_del_cruz*(*width); k<2*ancho_del_cruz; i+=(*width)){
                    k++;
                    tmp_image[(point+i)*3] = 100;
                    tmp_image[(point+i)*3+1] = 100;
                    tmp_image[(point+i)*3+2] = 150;
                 }
	    
              }else{
	    
                 /* dibujamos una linea horizontal */
                 for (i=-ancho_del_cruz; i<ancho_del_cruz; i++){
                    tmp_image[(point+i)*3] = 0;
                    tmp_image[(point+i)*3+1] = 0;
                    tmp_image[(point+i)*3+2] = 255;
                 }
	    
                 k=0;
                 /* dibujamos una linea vertical */
                 for (i=-ancho_del_cruz*(*width); k<2*ancho_del_cruz; i+=(*width)){
                    k++;
                    tmp_image[(point+i)*3] = 0;
                    tmp_image[(point+i)*3+1] = 0;
                    tmp_image[(point+i)*3+2] = 255;
                 }
              }
           }
        }
    
        for (j=0; j<NUMPUNTOS; j++)
        {
	
           point = pnts_elegidos_en_img_rectificada[j].y*(*width) +
                 pnts_elegidos_en_img_rectificada[j].x;
	
           /* point > 0 si ha sido actualizado por lo menos una vez */
           if (point > 0){

              if (pnts_elegidos_en_img_rectificada[j].draw == UNDODRAW){
	    
                 /* dibujamos una linea horizontal */
                 for (i=-ancho_del_cruz; i<ancho_del_cruz; i=i+2){
                    tmp_image_rect[(point+i)*3] = 100;
                    tmp_image_rect[(point+i)*3+1] = 100;
                    tmp_image_rect[(point+i)*3+2] = 150;
                 }
	    
                 k=0;
                 /* dibujamos una linea vertical */
                 for (i=-ancho_del_cruz*(*width); k<2*ancho_del_cruz; i+=(*width)){
                    k++;
                    tmp_image_rect[(point+i)*3] = 100;
                    tmp_image_rect[(point+i)*3+1] = 100;
                    tmp_image_rect[(point+i)*3+2] = 150;
                 }

              }else{

                 if (pnts_elegidos_en_img_rectificada[j].x<((*width)/2)){
                    from = 0;
                    to = ((*width)-pnts_elegidos_en_img_rectificada[j].x);
                    inc=1;

                    /* dibujamos una linea horizontal */
                    for (i=from; i<to; i=i+inc){
                       tmp_image_rect[(point+i)*3] = 255;
                       tmp_image_rect[(point+i)*3+1] = 0;
                       tmp_image_rect[(point+i)*3+2] = 0;
                    }

                 }else{
                    from = (pnts_elegidos_en_img_rectificada[j].x);
                    to = 0;
                    inc=-1;

                    /* dibujamos una linea horizontal */
                    for (i=from; i>to; i=i+inc){
                       tmp_image_rect[(point-i)*3] = 255;
                       tmp_image_rect[(point-i)*3+1] = 0;
                       tmp_image_rect[(point-i)*3+2] = 0;
                    }
	      
                 }

                 if (pnts_elegidos_en_img_rectificada[j].y<((*height)/2)){
                    from = -(*width);
                    to = ((*height)-pnts_elegidos_en_img_rectificada[j].y);
                    inc=(*width);
	      
                    k=0;
                    /* dibujamos una linea vertical */
                    for (i=from; k<to; i+=inc){
                       k++;
                       tmp_image_rect[(point+i)*3] = 255;
                       tmp_image_rect[(point+i)*3+1] = 0;
                       tmp_image_rect[(point+i)*3+2] = 0;
                    }
                 }else{
	      
                    from = pnts_elegidos_en_img_rectificada[j].y*(*width);
                    inc=-(*width);
                    to = 0;

                    k=pnts_elegidos_en_img_rectificada[j].y;
                    /* dibujamos una linea vertical */
                    for (i=from; k>to; i+=inc){
                       k--;
                       tmp_image_rect[(point-i)*3] = 255;
                       tmp_image_rect[(point-i)*3+1] = 0;
                       tmp_image_rect[(point-i)*3+2] = 0;
                    }
	      
                 }
              }
           }
        }
     }

     {
        GdkPixbuf *imgBuff;
        GtkImage *image=(GtkImage *)glade_xml_get_widget(xml, "imagen_de_entrada");
        /* We read from our buffer: colorA */
     
        imgBuff = gdk_pixbuf_new_from_data((unsigned char *)tmp_image,
                                            GDK_COLORSPACE_RGB,0,8,
                                            (*width),(*height),
                                              (*width)*3,NULL,NULL);

        gtk_image_clear(image);
        gtk_image_set_from_pixbuf(image, imgBuff);
     }
     {
        GdkPixbuf *imgBuff;
        GtkImage *image=(GtkImage *)glade_xml_get_widget(xml, "imagen_rectificada");
        /* We read from our buffer: colorA */
     
        imgBuff = gdk_pixbuf_new_from_data((unsigned char *)tmp_image_rect,
                                            GDK_COLORSPACE_RGB,0,8,
                                            (*width),(*height),
                                              (*width)*3,NULL,NULL);

        gtk_image_clear(image);
        gtk_image_set_from_pixbuf(image, imgBuff);
     }
  }
  pthread_mutex_unlock(&colorchange_mutex);
  gdk_threads_enter();
  gtk_window_resize (GTK_WINDOW(win),1,1);
  gtk_widget_queue_draw(win);
  gdk_threads_leave();
}


void imgrectifier_hide(void){
   mydelete_displaycallback(imgrectifier_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gtk_widget_queue_draw(win);
      gdk_threads_leave();
   }
   if (mystop!=NULL)
      mystop();
   all[imgrectifier_id].guistate=pending_off;
}

void imgrectifier_show(void){
   static int cargado=0;
   static pthread_mutex_t imgrectifier_gui_mutex;

   pthread_mutex_lock(&imgrectifier_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&imgrectifier_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("imgrectifier.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window1");
      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      /*Conectar los callbacks*/
      {
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"capturar_imagen")),
                           "clicked", G_CALLBACK (on_capturar_imagen_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"descartar_imagen")),
                           "clicked", G_CALLBACK (on_descartar_imagen_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"rectificar")),
                           "clicked", G_CALLBACK (on_rectificar_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"descartar_rectificada")),
                           "clicked", G_CALLBACK (on_descartar_rectificada_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"undo")),
                           "clicked", G_CALLBACK (on_undo_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"undo_2")),
                           "clicked", G_CALLBACK (on_undo_2_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"modo_rectangulo")),
                           "clicked", G_CALLBACK (on_modo_rectangulo_clicked), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"evento_imagen_entrada")),
                           "button_press_event", G_CALLBACK (on_imagen_de_entrada_release), NULL);
         g_signal_connect (G_OBJECT (glade_xml_get_widget(xml,"evento_imagen_rectificada")),
                           "button_press_event", G_CALLBACK (on_imagen_rectificada_release), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "image_selection")),
                          "changed", G_CALLBACK(on_image_sel_changed), NULL);
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "help")),
                          "clicked", G_CALLBACK(on_help_clicked), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "about_imgrectifier")),
                          "response", G_CALLBACK(on_help_close), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "about_imgrectifier")),
                          "delete-event", G_CALLBACK(on_help_destroy), NULL);
      }
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&imgrectifier_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   if (myrun !=NULL)
      myrun(imgrectifier_id,NULL,NULL);
   myregister_displaycallback(imgrectifier_guidisplay);
}
