/** Header file generated with fdesign on Fri Dec 22 13:10:31 2006.**/

#ifndef FD_myperceptivegui_h_
#define FD_myperceptivegui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *myperceptivegui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *hide;
	FL_OBJECT *activation;
	FL_OBJECT *fps;
} FD_myperceptivegui;

extern FD_myperceptivegui * create_form_myperceptivegui(void);

#endif /* FD_myperceptivegui_h_ */
