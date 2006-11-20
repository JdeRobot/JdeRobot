/** Header file generated with fdesign on Sat Oct 28 13:32:50 2006.**/

#ifndef FD_sensorsmotorsgui_h_
#define FD_sensorsmotorsgui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *sensorsmotorsgui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *hide;
	FL_OBJECT *escala;
	FL_OBJECT *track_robot;
	FL_OBJECT *ventanaA;
	FL_OBJECT *micanvas;
	FL_OBJECT *ventanaB;
	FL_OBJECT *teleoperator;
	FL_OBJECT *joystick;
	FL_OBJECT *back;
	FL_OBJECT *stop;
	FL_OBJECT *pantiltjoystick;
	FL_OBJECT *pantilt_joystick;
	FL_OBJECT *pantilt_origin;
	FL_OBJECT *pantilt_stop;
	FL_OBJECT *ptspeed;
	FL_OBJECT *ventanaC;
	FL_OBJECT *ventanaD;
} FD_sensorsmotorsgui;

extern FD_sensorsmotorsgui * create_form_sensorsmotorsgui(void);

#endif /* FD_sensorsmotorsgui_h_ */
