#include <forms.h>

/* ARRAY coordenates to GRAPHIC coordenates and backwards */
#define ARRAY_TO_GRAPHIC_ROW(value) ((value/3)/SIFNTSC_COLUMNS)
#define ARRAY_TO_GRAPHIC_COL(value) ((value/3)%SIFNTSC_COLUMNS)
#define GRAPHIC_TO_ARRAY(row,col) ((col*SIFNTSC_COLUMNS+row)*3)

/* GRAPHIC coordenates to OPTICAL coordenates */
#define GRAPHIC_TO_OPTICAL_X(x,y) (SIFNTSC_ROWS-1-y)
#define GRAPHIC_TO_OPTICAL_Y(x,y) (x)
#define OPTICAL_TO_GRAPHIC_X(x,y) (y)
#define OPTICAL_TO_GRAPHIC_Y(x,y) (SIFNTSC_ROWS-1-x)

/* hsi value table.
   note:

   if wanted to transform from RGB to HSI, then double H=hsi[3*c+0] double S=hsi[3*c+1] and double I=hsi[3*c+2]
   where int c=((0xfc& R )<<10)+((0xfc& G )<<4)+((0xfc & B)>>2); R, G and B must be int also.

   a call to build_hsi_table() is needed at the start of the program
*/

extern double hsi[256*1024*3];
extern int lineinimage(char *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor,int columns,int rows);
extern void reset_buffer(char *buffer, double R, double G, double B,int columns,int rows);
extern void drawimage_area(char *buffer,int tleft_point, int tright_point, int bleft_point, int bright_point);

/* it builds the hsi table. this function must be call if you want to transform from RGB TO HSI */
extern void build_hsi_table();
