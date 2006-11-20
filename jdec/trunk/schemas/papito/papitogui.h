/** Header file generated with fdesign on Sun Oct 29 23:53:47 2006.**/

#ifndef FD_papitogui_h_
#define FD_papitogui_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *papitogui;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *hide;
	FL_OBJECT *activation;
	FL_OBJECT *fps;
} FD_papitogui;

extern FD_papitogui * create_form_papitogui(void);

#endif /* FD_papitogui_h_ */
