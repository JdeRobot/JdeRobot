#include <progeo/progeo.h>

#ifndef VISUALMEMORY_STRUCTS_H
#define VISUALMEMORY_STRUCTS_H

typedef struct SoRtype{
  struct SoRtype *father;
  float posx;
  float posy;
  float posz;
  float foax;
  float foay;
  float foaz;
  float roll;
} SofReference;

struct image_struct {
	int width;
	int height;
	int bpp;	// bytes per pixel
	char *image;
};

typedef struct {
	float R;
	float G;
	float B;
} colorRGB;

typedef struct {
	HPoint3D start;
	HPoint3D end;
	int isValid;
} Segment3D;

typedef struct {
	HPoint2D start;
	HPoint2D end;
	int type;
	int isValid;
} Segment2D;

typedef struct {
	HPoint3D p1;
	HPoint3D p2;
	HPoint3D p3;
	HPoint3D p4;
	HPoint3D centroid;
	bool isValid;
} Parallelogram3D;

typedef struct {
	HPoint3D center;
	int isValid;
} Face3D;

typedef struct {
	HPoint3D start; // base de la flecha
	HPoint3D end; // extremo de la flecha (hacia donde apunta)
	colorRGB color;
	int isAttainable; // es atendible? (si está dentro de un cierto radio le prestamos atención de rumbo)
} Arrow3D;

typedef struct {
	int isValid;
	Segment3D base; // base (segmento más largo) de la flecha
	Segment3D cross; // uno de las aspas de la flecha
	colorRGB color;
} SemiArrow3D;

enum movement_pantilt {up,down,left,right};

// Parámetros relacionados con la atención visual
enum state {think, search, analizeSearch};

typedef struct {
	float pan;
	float tilt;
} scenePoint;

typedef struct {
	int x;
	int y;
} imagePoint;

typedef struct {
  int x;
  int y;
} t_vector;

struct elementStruct {
	double lastInstant; // último instante de tiempo en su detección
	double firstInstant; // primer instante de tiempo en su detección
	float latitude; // posición absoluta del pantilt, en eje tilt
	float longitude; // posición absoluta del pantilt, en eje pan
	int scenePos; // it can be left, center or right, depends on pantilt pos where face was detected
	float saliency;
	float liveliness;
	int type; // 0 = elemento virtual; 1 = rectángulo; 2 = face; 3 = arrow
	int isVisited;
	Parallelogram3D parallelogram;
	Face3D face;
	Arrow3D arrow;
	struct elementStruct* next;
};

#endif

