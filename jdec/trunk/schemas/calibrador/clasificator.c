#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glib.h"
#include "math.h"
#include "TAD.h"
#include "clasificator.h"
#include "segment.h"
#include "colorspaces.h"
#include <limits.h>
#include <sys/types.h>

#define DEBUG 0

/*
  Este parametro determina el tamaño de la ventana utilizada para buscar
  los puntos de control cuando el modo auto_detect esta activado por defecto
  es de 3x3 pixels
*/

#define SEARCH_WINDOW 9
#define MIN_PIXELS_IN_SET 1
#define MAX_CLASSES 20000
#define MAX_CLASS_FOR_REAL_TIME 10000
#define MAX_GROUPS 20000
#define NOVALID 0
#define UNIFIED -2
#define VALID 1
#define MAX_VALID_GROUPS 25

typedef struct tclass{
  GSList* list;
  int id;
  int num_elem;
  int state;
} Tclass;

typedef struct HSV HSV;

Tpoint2D auto_points[MAX_CLASSES];

int min(int a,int b,int c){
  
  int min;

  min = a;
  if (b<min) min=b;
  if (c<min) min=c;
  
  return min;
}

int max(int a,int b,int c){
  
  int max;

  max = a;
  if (b>max) max=b;
  if (c>max) max=c;
  
  return max;
}

/* A point is "in" the picture if we can establish a search window
   that has this point as a center without go out the image borders (320x240)
*/
int point_in(Tpoint2D* p){
  return (
	  (p->u >= SEARCH_WINDOW/2) && (p->u < 320-(SEARCH_WINDOW/2)) 
	  && 
	  (p->v >= SEARCH_WINDOW/2) && (p->v < 240-(SEARCH_WINDOW/2))
	  );
}

float fsqr(float a){
  return a*a;
}

float distance2D(Tpoint2D a, Tpoint2D b){
  return sqrt( fsqr(a.u-b.u) + fsqr(a.v-b.v));
}

int in_pink_range(HSV *hsv){

  if (!hsv) return 0;

  return 
    (
     (
      ((hsv->H >= 5) && (hsv->H <= 15))
      ||
      ((hsv->H >= 310) && (hsv->H <= 360))
      )
     &&
     ((hsv->S >= 0.35) && (hsv->S <= 0.8))
     );
}

int in_yellow_range(HSV *hsv){
  
  if (!hsv) return 0;

  return 
    (
     (hsv->H >= 60) && (hsv->H <= 80) 
     &&
     (hsv->S >= 0.2) && (hsv->S <= 0.8)
     );
}

int in_blue_range(HSV *hsv){
  
  if (!hsv) return 0;

  return 
    (
     (hsv->H > 220) && (hsv->H < 250) 
     &&
     (hsv->S >= 0.37) && (hsv->S <= 0.8)
     );
}

int in_white_range(HSV *hsv){
  
  if (!hsv) return 0;

  return 
    (hsv->V > 120); 
}


Tclass* new_class(){

  Tclass* class = (Tclass*) malloc(sizeof(Tclass));	
  
  if(class){

    class->state = NOVALID;
    class->list = NULL;
    class->num_elem = 0;
    class->id = -1;

  }else{
    printf("I cant create a new class, no enough memory!!!\n");
    return NULL;
  }
  
  return class;
}

Tpoint2D* get_average(Tclass* class){

  int i;
  Tpoint2D* p,*pm;
  int pmx=0;
  int pmy=0;

  if (class && (class->num_elem>0)){
    
    pm = (Tpoint2D*)malloc(sizeof(Tpoint2D));
 
    for (i=0; i<class->num_elem; i++){
      
      p = g_list_nth_data(class->list,i);
      if (p){
	pmx = p->u+pmx;
	pmy = p->v+pmy;
      }
    }
    
    pm->u = (pmx/class->num_elem);
    pm->v = (pmy/class->num_elem);
    pm->class = p->class;
   
    return pm;
  }
  else
    return NULL;
}

void insert_in_class(Tclass* class, Tpoint2D point){

  Tpoint2D* p;
  
  if (!class) return;
  
  p = (Tpoint2D*) malloc(sizeof(Tpoint2D));
  if (p){
    p->u = point.u;
    p->v = point.v;
    p->class = point.class;
    class->list = g_slist_append(class->list,p);
    class->num_elem++;
    class->state = VALID;
  }
}

Tclass* fusion_2_classes(Tclass* class1, Tclass* class2){
  
  int i;

  if (!class1 || !class2) return NULL;
  if (class1->id == class2->id) return NULL;
  
  class1->num_elem = class1->num_elem + class2->num_elem;
  if (class2->list)
    for (i=0; i<g_slist_length(class2->list); i++)
      class1->list = g_slist_append(class1->list,g_slist_nth_data(class2->list,i));
  
  return class1;
}

void invalid_class(Tclass* class){
  if (class)
    class->state = NOVALID;
}

void unified_group(Tclass* class){
  if (class)
    class->state = UNIFIED;
}

void valid_group(Tclass* class){
  if (class)
    class->state = VALID;
}

void free_class(Tclass* class){
  if (class){
    if (class->list)
      g_slist_free(class->list);
    class->list = NULL;
    free(class);
    class = NULL;
  }
}

int get_white_pixels_of_rectangle(char* imagen, Tpoint2D *pm){

  u_int8_t r,g,b;
  int sum,l,offset;
  HSV* hsv;

  if (!pm || !point_in(pm) || (pm->class<0))
    return 0;

  sum = 0;
  
  for (l=(pm->u-SEARCH_WINDOW/2);l<(pm->u+SEARCH_WINDOW/2);l++){
    
    offset = (pm->v-SEARCH_WINDOW/2)*320+l;
    r = (unsigned char)imagen[offset*3+2];
    g = (unsigned char)imagen[offset*3+1];
    b = (unsigned char)imagen[offset*3];
    hsv = (HSV*)RGB2HSV_getHSV(r,g,b);

    if (in_white_range(hsv))	sum++;
    
    offset = (pm->v+SEARCH_WINDOW/2)*320+l;
    r = (unsigned char)imagen[offset*3+2];
    g = (unsigned char)imagen[offset*3+1];
    b = (unsigned char)imagen[offset*3];
    
    hsv = (HSV*)RGB2HSV_getHSV(r,g,b);
    if (in_white_range(hsv))	sum++;
  }
  
  for (l=(pm->v-SEARCH_WINDOW/2);l<(pm->v+SEARCH_WINDOW/2);l++){
    
    offset = l*320+pm->u+SEARCH_WINDOW/2;
    
    r = (unsigned char)imagen[offset*3+2];
    g = (unsigned char)imagen[offset*3+1];
    b = (unsigned char)imagen[offset*3];

    hsv = (HSV*)RGB2HSV_getHSV(r,g,b);
    if (in_white_range(hsv))	sum++;
    
    offset = l*320+pm->u-SEARCH_WINDOW/2;
    
    r = (unsigned char)imagen[offset*3+2];
    g = (unsigned char)imagen[offset*3+1];
    b = (unsigned char)imagen[offset*3];

    hsv = (HSV*)RGB2HSV_getHSV(r,g,b);
    if (in_white_range(hsv))	sum++;
  }

  return sum;
}

void get_color_region(Tpoint2D point, char* imagen, int *pink, int *blue,int* green){

  int l,m,r,g,b,offset;
  HSV* hsv;

  *pink=0;
  *blue=0;
  *green=0;
  
  for (l=(point.u - SEARCH_WINDOW); l<(point.u + SEARCH_WINDOW); l++){
    for (m=(point.v - SEARCH_WINDOW); m<(point.v + SEARCH_WINDOW); m++){
      
      offset = (m-SEARCH_WINDOW)*320+l;
      r = (unsigned char)imagen[offset*3+2];
      g = (unsigned char)imagen[offset*3+1];
      b = (unsigned char)imagen[offset*3];

      hsv = (HSV*)RGB2HSV_getHSV(r,g,b);
      
      if (in_pink_range(hsv)) 	(*pink)++;
      if (in_yellow_range(hsv)) 	(*green)++;
      if (in_blue_range(hsv)) 	(*blue)++;
      
    }
  }
  
  if (DEBUG)
    printf("valor devuelto %d-%d-%d \n",*pink,*green,*blue);
  
}

int is_normal_point(Tpoint2D point, char* imagen){

  int pink,blue,green;
  
  get_color_region(point,imagen, &pink, &blue, &green);

  return (pink != 0) && (max(pink,blue,green) == pink);
}

int is_the_center(Tpoint2D point, char* imagen){

  int pink,blue,green;
  get_color_region(point,imagen, &pink, &blue, &green);

  return (green != 0) && (max(pink,blue,green) == green);
}

int is_ref_axe_point(Tpoint2D point, char* imagen){

  int pink,blue,green;
  get_color_region(point,imagen, &pink, &blue, &green);

  return (blue != 0) && (max(pink,blue,green) == blue);
}

void draw_rectangle(char* imagen, Tpoint2D* pm, Tcolor color){
  
  int l,offset;

  if (!point_in(pm))
    return;
  
  for (l=(pm->u-SEARCH_WINDOW/2); l<(pm->u+SEARCH_WINDOW/2); l++){
    
    offset = (pm->v-SEARCH_WINDOW / 2)*320+l;
    imagen[offset*3] = color.c1;
    imagen[offset*3+1] = color.c2;
    imagen[offset*3+2]   = color.c3;
    
    offset = (pm->v+SEARCH_WINDOW / 2)*320+l;
    imagen[offset*3] = color.c1;
    imagen[offset*3+1] = color.c2;
    imagen[offset*3+2]   = color.c3;
  }
  
  for (l=(pm->v-SEARCH_WINDOW/2); l<(pm->v+SEARCH_WINDOW/2); l++){
    
    offset = l*320+pm->u+SEARCH_WINDOW/2;
    imagen[offset*3] = color.c1;
    imagen[offset*3+1] = color.c2;
    imagen[offset*3+2]   = color.c3;
        
    offset = l*320+pm->u-SEARCH_WINDOW/2;
    imagen[offset*3] = color.c1;
    imagen[offset*3+1] = color.c2;
    imagen[offset*3+2]   = color.c3;
  }
}

void draw_volume(Tcalib_volume* vol, char* imagen){

  int i;
  Tcolor RED,GREEN,BLUE;

  RED.c1=0;
  RED.c2=0;
  RED.c3=255;
  
  GREEN.c1=0;
  GREEN.c2=255;
  GREEN.c3=0;
  
  BLUE.c1=255;
  BLUE.c2=0;
  BLUE.c3=0;
  
  if (vol){

    if (vol->center)
      draw_rectangle(imagen, vol->center, GREEN);
    
    for (i=0; i<vol->num_of_points; i++)
      draw_rectangle(imagen, &vol->normal_points[i], RED);
    
    if (vol->ref_axe){
      if (DEBUG)
	printf("Ref axe tiene %d puntos\n",vol->ref_axe->num_points);
      for (i=0; i<vol->ref_axe->num_points; i++)
	draw_rectangle(imagen, &vol->ref_axe->points[i], BLUE); 
    }
  }else
    printf("vol is NULL \n");
}

void free_line(Tline* line){
  if (line){
    if (line->points)
      free(line->points);
    line->points = NULL;
    free(line);
    line=NULL;
  }
}

void unify_group(Tclass** groups, int classes, int* unified){
  
  Tpoint2D *p1,*p2;
  int i,j,tmp;
  
  (*unified)=0;
  tmp=classes;
  
  for (i=2; i<tmp;i++){
    
    if (groups[i]->state == VALID){
    
      p1 = get_average(groups[i]);
      
      /* Compare p1-average with the rest*/
      for (j=2; j<tmp;j++){
	if (i != j){
	  
	  if (groups[j]->state == VALID){
	    p2 = get_average(groups[j]);
	    if ( distance2D(*p1,*p2) < (sqrt(3/2)*SEARCH_WINDOW))
	      {	  
		groups[i] = fusion_2_classes(groups[i],groups[j]);
		unified_group(groups[j]);
		(*unified)++;
	      }
	    free(p2);
	  }
	}
      }
      /*----------------------*/

      free(p1);
    }
    
  }

}

void filter_noise(Tclass** groups, char* imagen, int length){
		       
  int k,sum;
  Tpoint2D* pm;

  sum = 0;
  
  for (k=0; k<length; k++){

    if (groups[k]->num_elem < MIN_PIXELS_IN_SET){
      if (DEBUG)
	printf("clase %d invalida por num_pixels (%d)\n",k,groups[k]->num_elem );
      invalid_class(groups[k]);
      continue;
    }

    pm = get_average(groups[k]);
    sum = get_white_pixels_of_rectangle(imagen, pm);
    free(pm);
    
    if (((float)sum/(SEARCH_WINDOW*4)) > 0.5){
      valid_group(groups[k]);
    }else{
      if (DEBUG)
	printf("clase %d invalida por contorno\n",k);
      invalid_class(groups[k]);
    }
  }
  
}

void get_reference_axe(Tcalib_volume *vol, Tclass* class, char* imagen){

  Tclass* groups[20];
  Tline* ref_axe = NULL;
  Tpoint2D *p1,*p2;
  int i,j,cont=2;
  int unified;
  int validos=0;
  Tcolor BLUE;  

  BLUE.c1=0;
  BLUE.c2=0;
  BLUE.c3=255;
  
  for (i=0; i<20; i++)
    groups[i] = new_class();

  for(i=0; i < class->num_elem; i++){
    
    p1 = g_slist_nth_data(class->list,i);

    if (p1->class == 1){
      
      insert_in_class(groups[cont],*p1);	  

      for(j=0; j<class->num_elem; j++){
	
	p2 = g_slist_nth_data(class->list,j);
	
	if (i!=j && (p2->class == 1)){
	  if (distance2D(*p1,*p2) <= (SEARCH_WINDOW*2)){	  
	    groups[cont]->id = cont;
	    p2->class = cont;
	    insert_in_class(groups[cont],*p2);	  
	  }
	}
      }
      
      cont++;
    }
    if (cont == 20) break;
  }

  filter_noise(groups, imagen, cont);
  unify_group(groups, cont, &unified);
  
  for (i=0; i<cont; i++){
    if (groups[i]){
      if (groups[i]->state == VALID){
	
	if (!ref_axe){
	  ref_axe = (Tline*)malloc(sizeof(Tline));
	  ref_axe->num_points = 0;
	  ref_axe->points = (Tpoint2D*)malloc(10*sizeof(Tpoint2D));
	}
	
	p1 = get_average(groups[i]);
	ref_axe->points[ref_axe->num_points] = *p1;
	free(p1);
  	ref_axe->num_points++;
	validos++;
      }
    }
  }
  
  vol->ref_axe = ref_axe;
  if (vol->ref_axe)
    vol->ref_axe->num_points = validos;

  for (i=0; i<20; i++){
    free_class(groups[i]);
    groups[i]=NULL;
  }

  if (DEBUG)
    printf("there's %d as ref_axe from which %d are valid \n",cont-2,validos);
}

Tpoint2D* get_center(char* imagen, Tclass* class){
  
  Tclass* groups[50];
  Tpoint2D *p1,*p2;
  int i,j,classes=2,max;
  Tcolor GREEN;
  int unified;
  Tpoint2D* center = NULL;
    
  GREEN.c1=0;
  GREEN.c2=255;
  GREEN.c3=0;
  
  for (i=0; i<50; i++)
    groups[i] = new_class();
  
  for(i=0; i<class->num_elem; i++){
    
    p1 = g_slist_nth_data(class->list,i);
    
    if (p1->class == 0){
      
      insert_in_class(groups[classes],*p1);	  
      
      for(j=0; j<class->num_elem; j++){
	
	p2 = g_slist_nth_data(class->list,j);
	
	if (i!=j && (p2->class == 0)){
	  if (distance2D(*p1,*p2) <= (SEARCH_WINDOW)){	  
	    groups[classes]->id = classes;
	    p2->class = classes;
	    insert_in_class(groups[classes],*p2);	  
	  }
	}
      }
      
      classes++;
      if (classes == 50){
	if (DEBUG) printf("there were more center classes than the max permitted \n");
	break;
      }
    }
  }

  max = -1;
  
  filter_noise(groups, imagen, classes);
  unify_group(groups,classes, &unified);  
  
  for (i=2; i<classes;i++){
    if ((groups[i]->state == VALID) && groups[i]->num_elem > max){
      max = i;
    }
  }


  /* sometimes due to problem with filtering the center
     may not be detected, in this case a NULL is returned
  */

  if (max>0){
    p1 = get_average(groups[max]);
    if (p1){
      center = (Tpoint2D*)malloc(sizeof(Tpoint2D));
      *center=*p1;
      free(p1);
    }
  }
    
  for (i=0; i<50; i++){
    free_class(groups[i]);
    groups[i]=NULL;
  }
  
  return center;
}

Tpoint2D* get_valid_groups(Tclass** groups, int length, int* gv){
  
  int i,valid=0;
  Tpoint2D* grupos_validos;
  Tpoint2D* av;
  grupos_validos = (Tpoint2D*)malloc(length*sizeof(Tpoint2D));
  
  for (i=2; i<length; i++){

    if (groups[i]->state == VALID){
      av = get_average(groups[i]);
      grupos_validos[valid] = *av;
      free(av);
      valid++;
    }
  }

  *gv=valid;
  
  if (DEBUG) printf("valid groups == %d\n",valid);
  return grupos_validos;
}

void draw_sorted_lines(Tcalib_volume *vol, char* imagen){

  Tcolor RED,GREEN,BLUE,WHITE,BLACK,tmpcolor;
  float ftmp;
  int i,j;

  RED.c1=0;
  RED.c2=0;
  RED.c3=255;
  
  GREEN.c1=0;
  GREEN.c2=255;
  GREEN.c3=0;
  
  BLUE.c1=255;
  BLUE.c2=0;
  BLUE.c3=0;

  WHITE.c1=255;
  WHITE.c2=255;
  WHITE.c3=255;

  BLACK.c1=0;
  BLACK.c2=0;
  BLACK.c3=0;

  if (!vol->sorted_lines)
    return;

  for (i=0; i<6; i++){
    ftmp = (float)(i+1)/6.0;

     for (j=0; j<vol->sorted_lines[i].num_points; j++){
      
      tmpcolor.c1 = ceil(ftmp*255);
      tmpcolor.c1 = ceil(ftmp*255);
      tmpcolor.c3 = ceil(ftmp*255);
      if (i == 0)
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],RED);
      else if (i == 1)
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],GREEN);
      else if (i == 2)
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],BLUE);
      else if (i == 3)
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],WHITE);
      else if (i == 4)
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],BLACK);
      else
	draw_rectangle(imagen,&vol->sorted_lines[i].points[j],tmpcolor);
    }
  }
  
}

Tcalib_volume* clasify_points(char* imagen, int classes_num){
  
  /** det_auto **/
  Tclass*  groups[MAX_GROUPS];
  int i,j,k;
  int class = 2;
  Tpoint2D p1,p2;
  int gv,unified;
  
  Tcalib_volume *vol = (Tcalib_volume*)malloc(sizeof(Tcalib_volume));
  vol->sorted_lines = NULL;
  
  /* Initializing the local variable groups */
  memset(groups, 0, MAX_GROUPS*sizeof(Tclass*));

  if (DEBUG)
    printf("-> pixels filtrados %d \n",classes_num);

  /** Obtenemos el centro y el eje principal.
      
      Las clases "0" y "1" son clases especiales y que representan
      el centro y el eje principal.
      
  */  
  groups[0] = new_class();  
  groups[1] = new_class();  
  
  for (k=0; k<classes_num; k++){
    p2 = auto_points[k];
    if (0 == p2.class)
      insert_in_class(groups[0], p2);
    else if (1 == p2.class)
      insert_in_class(groups[1], p2);
  }
  
  vol->center = get_center(imagen, groups[0]);
  get_reference_axe(vol, groups[1], imagen);
  
  
  /** Obtenemos los puntos normales*/
  for (j=0; j<classes_num; j++){
    
    p1 = auto_points[j];
    
    if (p1.class == -1){
      
      if (!groups[class]) groups[class] = new_class();
      insert_in_class(groups[class],p1);
      auto_points[j].class = class;

      for (k=0; k<classes_num; k++){
	p2 = auto_points[k];
	if (p2.class == -1)
	  if (distance2D(p1,p2) <= (SEARCH_WINDOW)){
	    insert_in_class(groups[class],p2);
	    auto_points[k].class = class;
	  }
      }
      
      class++;
    }
    
  }/** END of for*/

  filter_noise(groups, imagen, class);
  unify_group(groups,class, &unified);  

  vol->normal_points = get_valid_groups(groups, class, &gv);
  vol->num_of_points = gv;
  
  if (DEBUG)
      printf("%d validos de los cuales %d unificados\n",gv,unified);

  /** If we get more valid classes that the maximum we discard
      the filtering. That may happen when there's some problem
      in the filtering process.
  */
  if (gv > MAX_VALID_GROUPS){

    if (DEBUG) 
      printf("There's a lot of noise. I'm sorry but i can't continue with the auto-detection!!\n");

  }else if 
      (vol->center && 
       vol->ref_axe &&
       aligned_points(vol->center,vol->ref_axe->points, vol->ref_axe->num_points)&&
       /*TODO quitar el 5 y poner una constante*/
       (vol->ref_axe->num_points == 5) &&
       (vol->num_of_points == MAX_VALID_GROUPS)
       )
    {      
      set_sorted_lines(vol,5,1); 
      draw_sorted_lines(vol,imagen);
    }
  
  draw_volume(vol, imagen);
  for (i=0; i <= class; i++){
    free_class(groups[i]);
    groups[i]=NULL;
  }
  
  return vol;
}

Tcalib_volume* auto_detect_control_points(char *imagen,
					  int LARGO_IMAGEN, 
					  int ANCHO_IMAGEN
					  ) {
  
  int k,j=0;
  int r,g,b,offset=0;
  int idx=0;
  HSV* hsv;
  Tpoint2D p;

  memset(auto_points,0,MAX_CLASSES*sizeof(Tpoint2D));

  if (DEBUG)
    printf("antes -> idx %d\n",idx);
  
  for (j=0; j<LARGO_IMAGEN; j++)
    for(k=0; k<ANCHO_IMAGEN; k++)
      {
	/* obtenemos el color del pixel de la imagen de entrada */
	offset = j*320+k;
	r = (unsigned char)imagen[offset*3+2];
	g = (unsigned char)imagen[offset*3+1];
	b = (unsigned char)imagen[offset*3];
	
	hsv = (HSV*)RGB2HSV_getHSV(r,g,b);

	/* Detectamos los puntos*/
	if (in_pink_range(hsv)){

	  p.u = k;
	  p.v = j;
	  p.class = -1;
	  if (idx < MAX_CLASSES){
	    auto_points[idx++] = p;
	  }else{
	    printf("Warrning: Algunos puntos no han sido detectados\n");
	  }

	}else if (in_blue_range(hsv)){
	  p.u = k;
	  p.v = j;
	  p.class = 1;
	  if (idx < MAX_CLASSES){
	    auto_points[idx++] = p;
	  }else{
	    printf("Blue Warrning: Algunos puntos no han sido detectados\n");
	  }
	}else if (in_yellow_range(hsv)){
	  p.u = k;
	  p.v = j;
	  p.class = 0;
	  if (idx < MAX_CLASSES){
	    auto_points[idx++] = p;
	  }else{
	    printf("Green Warrning: Algunos puntos no han sido detectados\n");
	  }
	}
	
      }

  if (DEBUG)
    printf("des ->  %d\n",idx);

  if (idx < MAX_CLASS_FOR_REAL_TIME )
    return clasify_points(imagen, idx);
  else
    return NULL;
}

/* This function receive the semi-detected set of points
   so at the begening the ser is not classified, thus
   we need to classify the points to detect the reference
   axe, the center, and the remaining of points
*/
extern Tcalib_volume* semi_auto_detect_control_points(Tcalib_volume *vol,
						      char *imagen,
						      int LARGO_IMAGEN, 
						      int ANCHO_IMAGEN
						      ){
  if (!vol)
    return NULL;
  else 
    if (DEBUG)
      print_volume(vol);
  
  set_sorted_lines(vol,5,1); 
  return vol;
}
  
void print_volume(Tcalib_volume *vol){
  if (vol){    
    if(vol->center)
      printf("vol.center %d:%d\n",vol->center->u,vol->center->v);
    else
      printf("vol.center == NULL\n");
    printf("vol.num points == %d\n",vol->num_of_points);
    if (vol->ref_axe)
      printf("vol.ref_axe_len == %d\n",vol->ref_axe->num_points);
    else
      printf("vol.ref_axe == NULL");
  }else
      printf("vol ==  NULL");
}

int is_valid_vol(Tcalib_volume *vol, int normal_points, int axe_points){
  
  return ( vol 
	   && vol->center
	   && (normal_points == vol->num_of_points)
	   && vol->ref_axe
	   && (axe_points == vol->ref_axe->num_points)
	   && (vol->sorted_lines)
	   );
      
}

int is_partialy_valid(Tcalib_volume *vol, int axe_points){
  return (vol && vol->ref_axe && ( vol->ref_axe->num_points <= axe_points));
}

int get_num_of_points(Tcalib_volume* vol){
 
  int sum=0;
  
  if (vol){
    if (vol->center)  sum++;
    if (vol->ref_axe) sum = sum + vol->ref_axe->num_points;
    sum = sum + vol->num_of_points;
  }
  
  return sum;  
}

void free_volume(Tcalib_volume **pvol){
  
  Tcalib_volume *vol = *pvol;

  /* TODO: I faced a lot of double free seg faults
     and at the end this function was desactivated.
     I haven't the enough time to fix it. Please,
     run valgrind and try to fix it
  */

  if (vol){
    if (vol->center){
      free(vol->center);
      vol->center=NULL;
    }
     
    free_line(vol->ref_axe);
    free(vol->normal_points);
    vol->normal_points=NULL;
    free_line(vol->sorted_lines);
    free(vol);
    vol = NULL;
  }
}

Tcalib_volume* createVolume(int normal_pnts, int axe_pnts){

  Tcalib_volume *vol2 = (Tcalib_volume*)malloc(sizeof(Tcalib_volume));
  vol2->center = (Tpoint2D*)malloc(sizeof(Tpoint2D));
  vol2->ref_axe = (Tline*)malloc(sizeof(Tline));
  vol2->ref_axe->points = (Tpoint2D*)malloc(sizeof(Tpoint2D)*axe_pnts);
  vol2->normal_points = (Tpoint2D*)malloc(sizeof(Tpoint2D)*normal_pnts);
  vol2->num_of_points=0;
  vol2->sorted_lines=0;
  
  return vol2;
}
