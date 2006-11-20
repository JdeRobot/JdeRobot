extern void opengldemo_startup();
extern void opengldemo_suspend();
extern void opengldemo_resume(int father, int *brothers, arbitration fn);
extern void opengldemo_guiresume();
extern void opengldemo_guisuspend();

extern int opengldemo_id; /* schema identifier */
extern int opengldemo_cycle; /* ms */
