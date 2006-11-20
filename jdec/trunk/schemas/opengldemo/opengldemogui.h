/** Header file generated with fdesign on Fri Aug 25 21:16:49 2006.**/

#ifndef FD_opengldemogui_h_
#define FD_opengldemogui_h_

/** Callbacks, globals and object handlers **/
extern void Rotar(FL_OBJECT *, long);
extern void Detener(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *opengldemogui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *boton_rotar;
	FL_OBJECT *boton_detener;
	FL_OBJECT *canvas;
	FL_OBJECT *camX;
	FL_OBJECT *camY;
	FL_OBJECT *camZ;
	FL_OBJECT *camR;
	FL_OBJECT *camOrigin;
	FL_OBJECT *camlatlong;
	FL_OBJECT *foaOrigin;
	FL_OBJECT *foaX;
	FL_OBJECT *foaY;
	FL_OBJECT *foaZ;
	FL_OBJECT *canvas2;
} FD_opengldemogui;

extern FD_opengldemogui * create_form_opengldemogui(void);

#endif /* FD_opengldemogui_h_ */
