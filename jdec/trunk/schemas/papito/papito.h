extern void papito_startup();
extern void papito_suspend();
extern void papito_resume(int father, int *brothers, arbitration fn);
extern void papito_guiresume();
extern void papito_guisuspend();

extern int papito_id; /* schema identifier */
extern int papito_cycle; /* ms */
