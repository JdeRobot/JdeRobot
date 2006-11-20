/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "opengldemogui.h"

FD_opengldemogui *create_form_opengldemogui(void)
{
  FL_OBJECT *obj;
  FD_opengldemogui *fdui = (FD_opengldemogui *) fl_calloc(1, sizeof(*fdui));

  fdui->opengldemogui = fl_bgn_form(FL_NO_BOX, 960, 500);
  obj = fl_add_box(FL_UP_BOX,0,0,960,500,"");
  fdui->boton_rotar = obj = fl_add_button(FL_NORMAL_BUTTON,860,280,60,30,"rotar");
    fl_set_object_callback(obj,Rotar,0);
  fdui->boton_detener = obj = fl_add_button(FL_NORMAL_BUTTON,860,320,60,30,"detener");
    fl_set_object_callback(obj,Detener,0);
  fdui->canvas = obj = fl_add_glcanvas(FL_NORMAL_CANVAS,10,10,640,480,"");
  fdui->camX = obj = fl_add_slider(FL_VERT_SLIDER,680,20,10,170,"X");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_slider_size(obj, 0.15);
  fdui->camY = obj = fl_add_slider(FL_VERT_SLIDER,700,20,10,170,"Y");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_slider_size(obj, 0.15);
  fdui->camZ = obj = fl_add_slider(FL_VERT_SLIDER,720,20,10,170,"Z");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_slider_size(obj, 0.15);
  obj = fl_add_slider(FL_VERT_SLIDER,740,20,10,170,"f");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_slider_size(obj, 0.15);
  fdui->camR = obj = fl_add_slider(FL_VERT_SLIDER,760,20,10,170,"R");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_slider_size(obj, 0.15);
  fdui->camOrigin = obj = fl_add_button(FL_MENU_BUTTON,780,160,50,20,"origin");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKGOLD);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
  fdui->camlatlong = obj = fl_add_positioner(FL_NORMAL_POSITIONER,780,90,80,50,"");
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
  obj = fl_add_button(FL_MENU_BUTTON,830,60,50,20,"noroll");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
  fdui->foaOrigin = obj = fl_add_button(FL_MENU_BUTTON,780,60,50,20,"origin");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
  obj = fl_add_slider(FL_HOR_SLIDER,780,40,140,10,"roll");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_slider_size(obj, 0.15);
  fdui->foaX = obj = fl_add_slider(FL_VERT_SLIDER,890,60,10,140,"x");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_slider_size(obj, 0.15);
  fdui->foaY = obj = fl_add_slider(FL_VERT_SLIDER,910,60,10,140,"y");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_slider_size(obj, 0.15);
  fdui->foaZ = obj = fl_add_slider(FL_VERT_SLIDER,930,60,10,140,"z");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_BLUE);
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_slider_size(obj, 0.15);
  fdui->canvas2 = obj = fl_add_glcanvas(FL_NORMAL_CANVAS,680,270,160,200,"");
  fl_end_form();

  fdui->opengldemogui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

