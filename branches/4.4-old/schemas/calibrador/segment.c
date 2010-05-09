#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include <string.h>

#include "TAD.h"
#include "segment.h"

#define DEBUG 0
#define MAX_POINTS_IN_LINE 20
#define MIN_ANGLE 15

int isqr(int a){
  return a*a;
}

void get_vector(Tvector *v, Tpoint2D p1, Tpoint2D p2){
  v->a = p2.u - p1.u;
  v->b = p2.v - p1.v;
} 

float producto_scalar(Tvector v1, Tvector v2){
  float tmp = ((v1.a*v2.a)+(v1.b*v2.b));
  if (DEBUG)  printf("ps = %.2f\n",tmp);
  return tmp;
} 

float modulo(Tvector v){
  return sqrt(v.a*v.a+v.b*v.b);
}

int aligned_points(Tpoint2D *ref, Tpoint2D *points, int length){

  Tvector vtmp,vref;
  int i=0;
  
  get_vector(&vref, *ref, points[0]);

  for (i=1; i<length; i++){
    get_vector(&vtmp, *ref, points[i]);
    if (ang_entre_vectores(vref,vtmp)>(float)(MIN_ANGLE*3.14/180))
      return 0;
  }
  
  return 1;
}

float ang_entre_vectores(Tvector v1, Tvector v2){
  float tmp = producto_scalar(v1,v2)/(modulo(v1)*modulo(v2));
  if (tmp>1) tmp=1;
  tmp=acos(tmp);
  return tmp;
}

float ang_dirigido_entre_vectores(Tpoint2D p1, Tpoint2D center){
  
  float ftmp;
  
  ftmp = atan2f((float)(p1.v-center.v),(float)(p1.u-center.u));
  ftmp = ftmp+2*3.14159264;
  ftmp = ftmp*(180.0 /3.14159264);
  
  return ftmp;
} 

void sort_ref_axe(Tcalib_volume *vol){

  int i,j;
  Tpoint2D ptmp,center;
  float d1,d2;

  center = *vol->center;
  
  if (DEBUG){
    printf("ref_axe before sorting center is %d:%d .. \n",vol->center->u,vol->center->v);
    for (i=0; i<vol->ref_axe->num_points; i++)
      printf("%d:%d\n",vol->ref_axe->points[i].u,vol->ref_axe->points[i].v);
  }

  if (vol->center && vol->ref_axe){

    for (i=0; i<vol->ref_axe->num_points; i++){
      
      ptmp = vol->ref_axe->points[i];
      d1 = sqrt( isqr(ptmp.u-center.u) + isqr(ptmp.v-center.v));
      
      for (j=0; j<vol->ref_axe->num_points; j++){

	ptmp = vol->ref_axe->points[j];
	d2 = sqrt( isqr(ptmp.u-center.u) + isqr(ptmp.v-center.v));

	if (d1<d2)
	  {
	    if (DEBUG) printf("changing %d by %d\n",i,j);
	    ptmp = vol->ref_axe->points[j];
	    vol->ref_axe->points[j] = vol->ref_axe->points[i];
	    vol->ref_axe->points[i] = ptmp;
	  }
      }
    }
  }else if (DEBUG)
    printf("Not center or ref_axe .. \n");

  if (DEBUG){
    printf("ref_axe after sorting .. \n");
    for (i=0; i<vol->ref_axe->num_points; i++)
      printf("%d:%d\n",vol->ref_axe->points[i].u,vol->ref_axe->points[i].v);
  }

}

int get_left_side(int pos, int mod ){

  if (pos == 1 || pos == 0)
    return mod;
  else
    return pos-1;
}

Tline* sort_lines(Tline* lines, Tcalib_volume *vol){
  
  Tline *sorted_lines = (Tline*)malloc(6*sizeof(Tline));
  Tline *sorted_lines_2 = (Tline*)malloc(6*sizeof(Tline));
  Tline ltmp;
  Tpoint2D p2,p1,center;
  int i,j;
  float ang1,ang2,angref;
  int pos,ins_pos=1;
  
  center = *vol->center;

  p2 = vol->ref_axe->points[0];
  angref = ang_dirigido_entre_vectores(p2,center);
  
  memset(sorted_lines,0,6*sizeof(Tline));
  sorted_lines[0] = *vol->ref_axe;
  for (i=1; i<6; i++)
    sorted_lines[i] = lines[i-1]; 
  
  for (i=1; i<6; i++){
    for (j=1; j<5; j++){
      
      p2 = sorted_lines[j].points[0];
      ang1 = ang_dirigido_entre_vectores(p2,center);
            
      p2 = sorted_lines[j+1].points[0];
      ang2 = ang_dirigido_entre_vectores(p2,center);
            
      if (ang2<ang1){
	ltmp = sorted_lines[j];
	sorted_lines[j] = sorted_lines[j+1];
	sorted_lines[j+1] = ltmp;
      }
    }
  }
  
  for (i=1; i<5; i++){
    
    p1 = sorted_lines[i].points[0];
    ang1 = ang_dirigido_entre_vectores(p1,center);
    p2 = sorted_lines[i+1].points[0];
    ang2 = ang_dirigido_entre_vectores(p2,center);

    if ( angref >= ang1 ){
      ins_pos++;
    }
    
  }
  
  if (DEBUG) printf("-> insert in %d\n",ins_pos);

  sorted_lines_2[0]=*(vol->ref_axe);
  pos = get_left_side(ins_pos, 5 );
  for (i=1; i<6; i++){
    if (DEBUG) printf("en %d inserto desde %d\n",i,pos);
    sorted_lines_2[i] = sorted_lines[pos];
    pos = get_left_side(pos, 5 );
  }

  for (i=0; i<6; i++){
    p2 = sorted_lines_2[i].points[0];
    ang1 = ang_dirigido_entre_vectores(p2,center);
    if (DEBUG) printf("%d el angulo con %d:%d -> %.2f \n",i, p2.u,p2.v,ang1);
    printf("\n");
  }

  free(sorted_lines);
  return sorted_lines_2;
}

void set_sorted_lines(Tcalib_volume *vol ,
		 int num_of_lines,
		 int check_lines
		 ){
  
  Tpoint2D detected_points[vol->num_of_points];
  Tpoint2D p,w;
  Tpoint2D p1,p2;
  Tvector vectors[vol->num_of_points];
  Tvector vtmp,vref;
  int i,j,k,n;
  Tpoint2D point;
  Tline *lines = (Tline*)malloc(num_of_lines*sizeof(Tline));
  Tline* sorted_lines;
  Tpoint2D center;
  int neighbour_points_correctly_selected=0;
  int counter=0;

  if (!num_of_lines || !vol->center || !vol->normal_points || !vol->ref_axe){
    printf("get_lines: algun valor invalido en la entrada!!\n");
    return;
  }
  
  center = *vol->center;
  
  for (i=0; i<num_of_lines; i++){
    lines[i].points = (Tpoint2D*)malloc(MAX_POINTS_IN_LINE*sizeof(Tpoint2D));
    lines[i].id = -1;
    lines[i].num_points = 0;
  }
    
  p.class = -1;
  /** Copying the set of points*/
  for (i=0; i<vol->num_of_points; i++){
    if (DEBUG) printf("%d -> %d-%d\n",i,vol->normal_points[i].u,vol->normal_points[i].v);
    p.u = vol->normal_points[i].u;
    p.v = vol->normal_points[i].v;
    detected_points[i] = p;
  }

  /** geting the distance between the center and the rest of points*/
  for (i=0; i<vol->num_of_points; i++){
    p = detected_points[i];
    detected_points[i].d = sqrt((p.u-center.u)*(p.u-center.u)+
				(p.v-center.v)*(p.v-center.v));
  }

  /** sort the points by its distance with the center*/
  for (i=0; i<vol->num_of_points; i++)
    for (j=i+1; j<vol->num_of_points; j++){
      
      w = detected_points[i];
      p = detected_points[j];
      
      if (w.d>p.d){
	detected_points[j] = w;
	detected_points[i] = p;
      }
    }
  
  /** 
      Look for neigbour points, those are the first points of
      the five lines that round the center
  */
  for (j=0; j<vol->num_of_points; j++){
    
    neighbour_points_correctly_selected=1;
    get_vector(&vref, center, detected_points[j]);
    
    for (i=j; i>=0; i--){
      if (i != j){
	get_vector(&vtmp, center,detected_points[i]);
	if (ang_entre_vectores(vref,vtmp)<(MIN_ANGLE*3.14/180)){
	  neighbour_points_correctly_selected=0;
	  break;
	}
      }
    }
    
    if (neighbour_points_correctly_selected){
      if (DEBUG)
	printf("point %d -> %d:%d selected as neihgbour\n",j,
	       detected_points[j].u,detected_points[j].v);
      get_vector(&vectors[counter++], center, detected_points[j]);
    }
    
    if (counter == num_of_lines)
      break;
  }

  if (DEBUG)
    for (i=0; i<vol->num_of_points; i++){
      printf("%d:%d -> %f\n",detected_points[i].u,detected_points[i].v,detected_points[i].d);
    }
    
  for (i=0; i<num_of_lines; i++){
    for (j=0; j<vol->num_of_points; j++){

      if (DEBUG)
	printf("procesando el punto %d:%d  iter %d\n",
	     detected_points[j].u,detected_points[j].v,i+1);

      if (detected_points[j].class == -1){
	get_vector(&vtmp, center,detected_points[j]);
	if (ang_entre_vectores(vectors[i],vtmp)<(float)(MIN_ANGLE*3.14/180))
	  {
	    if (DEBUG)
	      printf("el punto %d:%d ha sido asignado a la clase %d\n",
		     detected_points[j].u,detected_points[j].v,i+1);
	    detected_points[j].class = i+1;
	    point.u = detected_points[j].u;
	    point.v = detected_points[j].v;
	    lines[i].id = i+1;
	    lines[i].points[lines[i].num_points++] = point;
	  }
      }

    }
  }
  
  /* sort points into a line*/
  for (i=0; i<num_of_lines; i++){
    
    for (n=0; n<lines[i].num_points; n++){
      for (k=n+1; k<lines[i].num_points; k++){
	
	p1 = lines[i].points[n];
	p2 = lines[i].points[k];
	if (p2.d<p1.d){
	  lines[i].points[n] = p2;
	  lines[i].points[k] = p1;
	}
      }
    }
  }
  
  sort_ref_axe(vol);
  sorted_lines = sort_lines(lines, vol);

  /** free lines sinces they was copied into sorted_lines
      NOTE: we just free the lines, the points are already
      there and its memory is needed to be returned, more
      late this will be freed by free_volume
  */
  free(lines);
  
  
  /** Check that all detected lines have 5 points including
      the axe_reference
  */
  if (check_lines){
    
    for (i=0; i<5; i++){
      if (sorted_lines[i].num_points != 5){
	sorted_lines=NULL;
	break;
      }
    }
    
    if(vol->ref_axe->num_points != 5)
      sorted_lines = NULL;
  }
  
  if (DEBUG){
    if (sorted_lines)
      printf("Lines well formed\n");
    else
      printf("Lines Bad formed\n");
  }
  
  vol->sorted_lines = sorted_lines;
}
