/**
 * obj_loader.h
 *
 * In this file we describe the data structures to store a 3D object
 *
 * @author José Antonio Santos Cadenas <santoscadenas@gmail.com>
 *
 * @date 2008-01-23
 *
 * @version 0.1 First version
 */


typedef struct{
   int n_vertices;
   int n_normal_vertices;
   int *vertices_list;
   int *normal_vertices_list;
}surf_type;

typedef struct{
   float x;
   float y;
   float z;
}vert_type;

typedef struct{
   int n_vertices;
   int n_normal_vertices;
   int n_surfaces;
   vert_type *vertices;
   vert_type *normal_vertices;
   surf_type *surfaces;
}obj_type;

int load_object(char *filename, obj_type* obj);
void free_object(obj_type* obj);
int show_object(obj_type* obj);
