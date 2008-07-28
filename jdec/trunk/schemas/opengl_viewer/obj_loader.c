/**
 * obj_loader.c
 *
 * In this file we implement the functions described at obj_loader.h
 *
 * @author José Antonio Santos Cadenas <santoscadenas@gmail.com>
 *
 * @date 2008-01-23
 *
 * @version 0.1 First version
 */

#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <GL/gl.h>

#define BUFF_SIZE 512

int show_object(obj_type* obj){

   int i, j;
   int v_id;

   for (i=0; i< obj->n_surfaces; i++){
      glBegin (GL_POLYGON);
      for (j = 0; j < obj->surfaces[i].n_vertices; j++)
      {
         if (obj->surfaces[i].normal_vertices_list !=NULL){
            float x,y,z;
            v_id = obj->surfaces[i].normal_vertices_list[j];
            if (v_id < obj->n_normal_vertices){
               x=obj->normal_vertices[v_id].x;
               y=obj->normal_vertices[v_id].y;
               z=obj->normal_vertices[v_id].z;
            }
            glNormal3f(x, y, z);
         }
         if (obj->surfaces[i].vertices_list != NULL)
         {
            float x,y,z;
            v_id = obj->surfaces[i].vertices_list[j];
            if (v_id < obj->n_vertices){
               x=obj->vertices[v_id].x;
               y=obj->vertices[v_id].y;
               z=obj->vertices[v_id].z;
            }
            glVertex3f (x, y, z);
         }
      }
      glEnd ();
   }
   return 0;
}

int read_obj(FILE* obj_file, obj_type* obj){
   char buff[BUFF_SIZE];
   int actual_vertex=0;
   int actual_surface=0;
   int actual_normal_vertex=0;
   float x,y,z;
   char *pointer;

   while (fgets(buff, BUFF_SIZE, obj_file)){
      if (buff[0]=='v'){
         if (buff[1]==' '){
            pointer=buff;
            pointer++;
            while (pointer[0]==' ' || pointer[0]=='\t')
               pointer ++;
            if (sscanf(pointer, "%f %f %f", &x, &y, &z)!=3){
               return 1;
            }
            obj->vertices[actual_vertex].x=x;
            obj->vertices[actual_vertex].y=y;
            obj->vertices[actual_vertex].z=z;
            actual_vertex++;
            printf ("Vertex %d of %d loaded\n", actual_vertex, obj->n_vertices);
         }
         else if (buff[1]=='n'){
            if (buff[2]==' '){
               pointer=buff;
               pointer+=2;
               while (pointer[0]==' ' || pointer[0]=='\t')
                  pointer ++;
               if (sscanf(pointer, "%f %f %f", &x, &y, &z)!=3){
                  return 1;
               }
               obj->normal_vertices[actual_normal_vertex].x=x;
               obj->normal_vertices[actual_normal_vertex].y=y;
               obj->normal_vertices[actual_normal_vertex].z=z;
               actual_normal_vertex++;
               printf ("Normal vertex %d of %d loaded\n", actual_normal_vertex, obj->n_normal_vertices);
            }
         }
      }
      else if (buff[0]=='f'){
         if (buff[1]==' '){
            pointer=buff;
            pointer++;
            while (pointer[0]==' ' || pointer[0]=='\t')
               pointer ++;
            /*How many vertices?*/
            {
               char *counter;
               counter=pointer;
               obj->surfaces[actual_surface].n_vertices=0;
               while (counter[0]!='\0' && counter[0]!='\n'){
                  while (isdigit(counter[0]) || counter[0]=='-')
                     counter++;
                  obj->surfaces[actual_surface].n_vertices++;
                  if (counter[0]=='/'){
                     /*There are more than just spacial vertices*/
                     counter++;
                     while (isdigit(counter[0]) || counter[0]=='-')
                        counter++;
                     /*Count the texture vertices if necessary*/
                     if (counter[0]=='/'){
                        counter++;
                        while (isdigit(counter[0]) || counter[0]=='-')
                           counter++;
                        obj->surfaces[actual_surface].n_normal_vertices++;
                     }
                     else{
                        return 1;
                     }
                  }
                  while (counter[0]==' ' || counter[0]=='\t')
                     counter++;
               }
            }
            /*Now we read each vertex*/
            if (obj->surfaces[actual_surface].n_vertices>0){
               obj->surfaces[actual_surface].vertices_list=(int *)
                     malloc(sizeof(int)*obj->surfaces[actual_surface].n_vertices);
            }
            else{
               obj->surfaces[actual_surface].vertices_list=NULL;
            }
            if (obj->surfaces[actual_surface].n_normal_vertices){
               obj->surfaces[actual_surface].normal_vertices_list=(int *)
                     malloc(sizeof(int)*obj->surfaces[actual_surface].n_normal_vertices);
            }
            else{
               obj->surfaces[actual_surface].normal_vertices_list=NULL;
            }
            {
               int i;
               int *list;
               int *normal_list;
               list=obj->surfaces[actual_surface].vertices_list;
               normal_list=obj->surfaces[actual_surface].normal_vertices_list;
               for (i=0; i< obj->surfaces[actual_surface].n_vertices; i++){
                  if (sscanf(pointer, "%d", &list[i])<1){
                     fprintf (stderr, "opengl_viewer: malformed object file, wrong value >%s<\n", pointer);
                     return 1;
                  }
                  /*If the number refer the vertex down-up*/
                  if (list[i]<0){
                     list[i]=actual_vertex-list[i];
                  }
                  else{
                     list[i]=list[i]-1;
                  }
                  if (list[i]<0){
                     fprintf (stderr, "opengl_viewer: malformed object file, not valid data %d\n", list[i]);
                     return 1;
                  }
                  /*hop to the next type of vertex*/
                  while (isdigit(pointer[0]) || pointer[0]=='-')
                     pointer++;
                  if (pointer[0]=='/'){
                     pointer++;
                     /*This is the case of texture vertex not implemented yet*/
                  }
                  while (isdigit(pointer[0]) || pointer[0]=='-')
                     pointer++;
                  if (pointer[0]=='/'){
                     pointer++;
                     /*This is the case of normal vertex*/
                     if (sscanf(pointer, "%d", &normal_list[i])<1){
                        fprintf (stderr, "opengl_viewer: malformed object file, wrong value >%s<\n", pointer);
                        return 1;
                     }
                     if (normal_list[i]<0){
                        normal_list[i]=actual_normal_vertex-normal_list[i];
                     }
                     else{
                        normal_list[i]=normal_list[i]-1;
                     }
                     if (normal_list[i]<0){
                        fprintf (stderr, "opengl_viewer: malformed object file, not valid data %d\n", normal_list[i]);
                        return 1;
                     }
                  }
                  /*Hop to the next vertex*/
                  while (isdigit(pointer[0]) || pointer[0]=='-')
                     pointer++;
                  while (pointer[0]==' ' || pointer[0]=='\t')
                     pointer++;
               }
            }
            actual_surface++;
            printf ("Surface %d of %d loaded\n", actual_surface, obj->n_surfaces);
         }
      }
      else{
         printf (">%c<\n", buff[0]);
      }
   }
   return 0;
}

int init_obj(FILE* obj_file, obj_type* obj){
   char buff[BUFF_SIZE];

   free_object(obj);

   while (fgets(buff, BUFF_SIZE, obj_file)){
      if (buff[0]=='v'){
         if (buff[1]==' '){
            obj->n_vertices++;
         }
         else if(buff[1]=='n'){
            if (buff[2]==' '){
               obj->n_normal_vertices++;
            }
         }
      }
      else if(buff[0]=='f'){
         if(buff[1]==' '){
            obj->n_surfaces++;
         }
      }
   }

   if (obj->n_vertices > 0){
      obj->vertices=(vert_type *)malloc(sizeof(vert_type)*obj->n_vertices);
   }
   if (obj->n_surfaces > 0){
      obj->surfaces=(surf_type *)malloc(sizeof(surf_type)*obj->n_surfaces);
   }
   if (obj->n_normal_vertices > 0){
      obj->normal_vertices=(vert_type *)malloc(sizeof(vert_type)*obj->n_normal_vertices);
   }

   rewind(obj_file);
   
   return 0; /*No errors*/
}

int load_object(char *filename, obj_type* obj){
   FILE* obj_file;
   obj_file=fopen(filename, "r");
   if (obj_file==NULL)
      perror("opengl_viewer");

   /*Initializes the object*/
   if (init_obj (obj_file, obj)!=0){
      /*The initialization fails*/
      fprintf (stderr, "opengl_viewer: fail while trying to initializate obj structure\n");
      return 1;
   }
   printf ("Find %d vertices and %d surfaces\n", obj->n_vertices, obj->n_surfaces);

   if (read_obj (obj_file, obj)!=0){
      fprintf (stderr, "opengl_viewer: fail while trying to read obj file\n");
      return 1;
   }
   return 0;
}

void free_object(obj_type* obj){
   int i;
   if (obj->surfaces!=NULL){
      for (i=0; i< obj->n_surfaces; i++){
         if (obj->surfaces[i].vertices_list!=NULL){
            free(obj->surfaces[i].vertices_list);
         }
         if (obj->surfaces[i].normal_vertices_list!=NULL){
            free(obj->surfaces[i].normal_vertices_list);
         }
      }
      free(obj->surfaces);
   }
   obj->n_surfaces=0;

   obj->n_vertices=0;
   if (obj->vertices!=NULL)
      free(obj->vertices);
   obj->n_normal_vertices=0;
   if (obj->normal_vertices!=NULL){
      free(obj->normal_vertices);
   }
}
