/** Header file generated with fdesign on Sat Sep 30 14:08:54 2006.**/

#ifndef FD_myschemagui_h_
#define FD_myschemagui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *myschemagui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *hide;
	FL_OBJECT *activation;
	FL_OBJECT *fps;
} FD_myschemagui;

extern FD_myschemagui * create_form_myschemagui(void);

#endif /* FD_myschemagui_h_ */
