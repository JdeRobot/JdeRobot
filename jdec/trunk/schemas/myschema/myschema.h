extern void myschema_startup();
extern void myschema_suspend();
extern void myschema_resume(int father, int *brothers, arbitration fn);
extern void myschema_guiresume();
extern void myschema_guisuspend();

extern int myschema_id; /* schema identifier */
extern int myschema_cycle; /* ms */
