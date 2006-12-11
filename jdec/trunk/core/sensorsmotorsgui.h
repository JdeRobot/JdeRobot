/** Header file generated with fdesign on Sat Dec  9 03:55:48 2006.**/

#ifndef FD_sensorsmotorsgui_h_
#define FD_sensorsmotorsgui_h_

/** Callbacks, globals and object handlers **/
extern int freeobj_ventanaA_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern int freeobj_ventanaB_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern int freeobj_ventanaC_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern int freeobj_ventanaD_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *sensorsmotorsgui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *hide;
	FL_OBJECT *escala;
	FL_OBJECT *track_robot;
	FL_OBJECT *micanvas;
	FL_OBJECT *teleoperator;
	FL_OBJECT *joystick;
	FL_OBJECT *back;
	FL_OBJECT *stop;
	FL_OBJECT *pantiltjoystick;
	FL_OBJECT *pantilt_joystick;
	FL_OBJECT *pantilt_origin;
	FL_OBJECT *pantilt_stop;
	FL_OBJECT *ptspeed;
	FL_OBJECT *sampleimage;
	FL_OBJECT *ventanaA;
	FL_OBJECT *ventanaB;
	FL_OBJECT *ventanaC;
	FL_OBJECT *ventanaD;
} FD_sensorsmotorsgui;

extern FD_sensorsmotorsgui * create_form_sensorsmotorsgui(void);

#endif /* FD_sensorsmotorsgui_h_ */
