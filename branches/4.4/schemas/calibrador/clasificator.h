
#define NORMAL_POINT 0
#define AXE_POINT 1
#define CENTER 2
#define NONCLASSIFIED 3

#define NORMAL_CLASS_COLOR_CENTER 350
#define CENTER_COLOR_CENTER 50
#define AXE_CLASS_COLOR_CENTER 50


#define CLASS_DIAMETER 20

extern Tcalib_volume* auto_detect_control_points(char *imagen,
						 int LARGO_IMAGEN, 
						 int ANCHO_IMAGEN
						 );
	
extern Tcalib_volume* semi_auto_detect_control_points(Tcalib_volume *vol,
						      char *imagen,
						      int LARGO_IMAGEN, 
						      int ANCHO_IMAGEN
						      );

extern int is_normal_point(Tpoint2D point, char* imagen);
extern int is_the_center(Tpoint2D point, char* imagen);
extern int is_ref_axe_point(Tpoint2D point, char* imagen);
extern void print_volume(Tcalib_volume *vol);
extern int get_num_of_points(Tcalib_volume* vol);
extern void free_volume(Tcalib_volume **vol);
extern int is_valid_vol(Tcalib_volume *vol, int normal_points, int axe_points);
extern int is_partialy_valid(Tcalib_volume *vol, int axe_points);
extern void draw_rectangle(char* imagen, Tpoint2D* pm, Tcolor color);
extern Tcalib_volume* createVolume(int normal_pnts, int axe_pnts);
