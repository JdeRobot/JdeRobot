extern void hsituner_startup();
extern void hsituner_suspend();
extern void hsituner_resume(int father, int *brothers, arbitration fn);
extern void hsituner_guiresume();
extern void hsituner_guisuspend();

extern int hsituner_id; /* schema identifier */
extern int hsituner_cycle; /* ms */
