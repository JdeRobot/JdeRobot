/** Header file generated with fdesign on Sat May 29 13:21:24 2010.**/

#ifndef FD_calibratorgui_h_
#define FD_calibratorgui_h_

/** Callbacks, globals and object handlers **/
extern int color_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern int virtual_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *calibratorgui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *eldercare_form;
	FL_OBJECT *color_freeobject;
	FL_OBJECT *exit_button;
	FL_OBJECT *x_cam_slider;
	FL_OBJECT *y_cam_slider;
	FL_OBJECT *z_cam_slider;
	FL_OBJECT *X_cam_slider;
	FL_OBJECT *Y_cam_slider;
	FL_OBJECT *Z_cam_slider;
	FL_OBJECT *roll_slider;
	FL_OBJECT *save;
	FL_OBJECT *flip_button;
	FL_OBJECT *focus_slider;
	FL_OBJECT *cam_joystick;
	FL_OBJECT *virtual_freeobject;
	FL_OBJECT *reload;
	FL_OBJECT *undo;
	FL_OBJECT *u0;
	FL_OBJECT *v0;
	FL_OBJECT *fixedFOA;
} FD_calibratorgui;

extern FD_calibratorgui * create_form_calibratorgui(void);

#endif /* FD_calibratorgui_h_ */
