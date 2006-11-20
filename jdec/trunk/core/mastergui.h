/** Header file generated with fdesign on Sat Oct 28 13:32:42 2006.**/

#ifndef FD_mastergui_h_
#define FD_mastergui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *mastergui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *exit;
	FL_OBJECT *schemas;
	FL_OBJECT *act0;
	FL_OBJECT *fps0;
	FL_OBJECT *vis0;
	FL_OBJECT *fps1;
	FL_OBJECT *vis1;
	FL_OBJECT *act1;
	FL_OBJECT *fps2;
	FL_OBJECT *vis2;
	FL_OBJECT *act3;
	FL_OBJECT *fps3;
	FL_OBJECT *vis3;
	FL_OBJECT *act4;
	FL_OBJECT *fps4;
	FL_OBJECT *vis4;
	FL_OBJECT *vis5;
	FL_OBJECT *vis6;
	FL_OBJECT *vis7;
	FL_OBJECT *vis8;
	FL_OBJECT *vis9;
	FL_OBJECT *fps5;
	FL_OBJECT *fps6;
	FL_OBJECT *fps7;
	FL_OBJECT *fps8;
	FL_OBJECT *fps9;
	FL_OBJECT *act5;
	FL_OBJECT *act6;
	FL_OBJECT *act7;
	FL_OBJECT *act8;
	FL_OBJECT *act9;
	FL_OBJECT *jdegui;
	FL_OBJECT *guifps;
	FL_OBJECT *fps10;
	FL_OBJECT *vis10;
	FL_OBJECT *act10;
	FL_OBJECT *actuators;
	FL_OBJECT *motors;
	FL_OBJECT *pantiltmotors;
	FL_OBJECT *fpsmotors;
	FL_OBJECT *fpspantiltmotors;
	FL_OBJECT *vismotors;
	FL_OBJECT *vispantiltmotors;
	FL_OBJECT *sensors;
	FL_OBJECT *sonars;
	FL_OBJECT *robot;
	FL_OBJECT *frame_rateA;
	FL_OBJECT *laser;
	FL_OBJECT *colorimageA;
	FL_OBJECT *frame_rateB;
	FL_OBJECT *colorimageB;
	FL_OBJECT *pantilt_encoders;
	FL_OBJECT *fpslaser;
	FL_OBJECT *fpssonars;
	FL_OBJECT *fpsencoders;
	FL_OBJECT *fpspantiltencoders;
	FL_OBJECT *vislaser;
	FL_OBJECT *visrobot;
	FL_OBJECT *vissonars;
	FL_OBJECT *vispantiltencoders;
	FL_OBJECT *viscolorimageA;
	FL_OBJECT *viscolorimageB;
	FL_OBJECT *frame_rateC;
	FL_OBJECT *frame_rateD;
	FL_OBJECT *colorimageC;
	FL_OBJECT *colorimageD;
	FL_OBJECT *viscolorimageC;
	FL_OBJECT *viscolorimageD;
	FL_OBJECT *fps11;
	FL_OBJECT *vis11;
	FL_OBJECT *act11;
	FL_OBJECT *act2;
	FL_OBJECT *hide;
	FL_OBJECT *hierarchy;
} FD_mastergui;

extern FD_mastergui * create_form_mastergui(void);

#endif /* FD_mastergui_h_ */
