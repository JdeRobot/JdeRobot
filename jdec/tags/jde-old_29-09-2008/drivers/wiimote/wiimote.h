/* Button flags */
#define CWIID_BTN_2     0x0001
#define CWIID_BTN_1     0x0002
#define CWIID_BTN_B     0x0004
#define CWIID_BTN_A     0x0008
#define CWIID_BTN_MINUS 0x0010
#define CWIID_BTN_HOME  0x0080
#define CWIID_BTN_LEFT  0x0100
#define CWIID_BTN_RIGHT 0x0200
#define CWIID_BTN_DOWN  0x0400
#define CWIID_BTN_UP    0x0800
#define CWIID_BTN_PLUS  0x1000

#define CWIID_NUNCHUK_BTN_Z   0x01
#define CWIID_NUNCHUK_BTN_C   0x02

/* Array Index Defs */
#define CWIID_X         0
#define CWIID_Y         1
#define CWIID_Z         2

/* Acc Defs */
#define CWIID_ACC_MAX	0xFF

/* IR Defs */
#define CWIID_IR_SRC_COUNT       4
#define CWIID_IR_X_MAX           1024
#define CWIID_IR_Y_MAX           768

/* Classic Controller Maxes */
#define CWIID_CLASSIC_L_STICK_MAX   0x3F
#define CWIID_CLASSIC_R_STICK_MAX   0x1F
#define CWIID_CLASSIC_LR_MAX        0x1F

/*
unsigned short buttons;
unsigned char acc[3];
struct cwiid_ir_src ir_src[CWIID_IR_SRC_COUNT];
struct nunchuk_state nunchuck;
*/

struct nunchuk_state {
   unsigned char stick[2];
   unsigned char acc[3];
   unsigned char buttons;
};

struct cwiid_ir_src {
   char valid;
   unsigned short pos[2];
   char size;
};
