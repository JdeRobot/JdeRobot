#include <forms.h>
extern Display* display;
extern int screen;

extern void jdegui_startup();
extern void jdegui_close();
extern void jdegui_resume();
extern void jdegui_suspend();
extern int jdegui_cycle;
extern float fpsgui;
extern int kgui;

extern void mastergui_resume();
extern void mastergui_suspend();

extern void sensorsmotorsgui_resume();
extern void sensorsmotorsgui_suspend();

/* for visualization of the schemas */
typedef void (*guibuttons)(FL_OBJECT *obj);
extern int register_buttonscallback(guibuttons f);
extern int delete_buttonscallback(guibuttons f);
typedef void (*guidisplay)(void);
extern int register_displaycallback(guidisplay f);
extern int delete_displaycallback(guidisplay f);

/* tabla de asociacion guientry-esquemacargado */
#define MAX_LOADEDSCHEMAS 12
extern int associated_ID[MAX_LOADEDSCHEMAS];

/* MOUSE BUTTON MAP */
#define MOUSELEFT 1
#define MOUSEMIDDLE 2
#define MOUSERIGHT 3
#define MOUSEWHEELUP 4
#define MOUSEWHEELDOWN 5

/* PI definition */
#define PI 3.141592654


