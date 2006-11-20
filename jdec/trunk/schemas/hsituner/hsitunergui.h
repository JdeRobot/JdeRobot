/** Header file generated with fdesign on Sat Oct 14 17:53:42 2006.**/

#ifndef FD_hsitunergui_h_
#define FD_hsitunergui_h_

/** Callbacks, globals and object handlers **/
extern int handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern int handle2(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *hsitunergui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *oculo_orig;
	FL_OBJECT *histograma;
	FL_OBJECT *oculo_modif;
	FL_OBJECT *w_slider;
	FL_OBJECT *value_SMin;
	FL_OBJECT *value_SMax;
	FL_OBJECT *value_HMin;
	FL_OBJECT *value_HMax;
	FL_OBJECT *freeobject1;
	FL_OBJECT *Hmin;
	FL_OBJECT *Hmax;
	FL_OBJECT *Smin;
	FL_OBJECT *Smax;
	FL_OBJECT *Imax;
	FL_OBJECT *Imin;
	FL_OBJECT *freeobject2;
	FL_OBJECT *toblack;
} FD_hsitunergui;

extern FD_hsitunergui * create_form_hsitunergui(void);

#endif /* FD_hsitunergui_h_ */
