
typedef struct point3D{
  int x;
  int y;
  int z;
} Tpoint3D;

typedef struct color{
  float c1;
  float c2;
  float c3;
} Tcolor;

typedef struct tpoint{
  int u;
  int v;
  float d;
  int class;
} Tpoint2D;

typedef struct vector{
  float a;
  float b;
} Tvector;

typedef struct tline{
  Tpoint2D *points;
  int id;
  int num_points;
} Tline;

typedef struct tcalib_volume{
  Tpoint2D *center;
  Tline *ref_axe;
  Tpoint2D *normal_points;
  int num_of_points;
  Tline* sorted_lines;
} Tcalib_volume;

