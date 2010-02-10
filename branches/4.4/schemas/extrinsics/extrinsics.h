/* image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320

/* PI definition */
#define PI 3.141592654

/* button states */
#define PUSHED 1
#define RELEASED 0

extern void extrinsics_init(char *configfile);
extern void extrinsics_terminate();

extern void extrinsics_stop();
extern void extrinsics_run(int father, int *brothers, arbitration fn);

extern void extrinsics_show();
extern void extrinsics_hide();

extern int extrinsics_id; /* schema identifier */
extern int extrinsics_cycle; /* ms */


