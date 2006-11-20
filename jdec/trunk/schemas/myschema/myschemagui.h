/** Header file generated with fdesign on Wed Nov  1 17:10:13 2006.**/

#ifndef FD_myschemagui_h_
#define FD_myschemagui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *myschemagui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *fps;
	FL_OBJECT *text;
} FD_myschemagui;

extern FD_myschemagui * create_form_myschemagui(void);

#endif /* FD_myschemagui_h_ */
