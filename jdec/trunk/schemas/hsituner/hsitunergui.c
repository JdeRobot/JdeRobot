/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "hsitunergui.h"

FD_hsitunergui *create_form_hsitunergui(void)
{
  FL_OBJECT *obj;
  FD_hsitunergui *fdui = (FD_hsitunergui *) fl_calloc(1, sizeof(*fdui));

  fdui->hsitunergui = fl_bgn_form(FL_NO_BOX, 770, 560);
  obj = fl_add_box(FL_UP_BOX,0,0,770,560,"");
  fdui->oculo_orig = obj = fl_add_frame(FL_ENGRAVED_FRAME,430,10,322,242,"");
  fdui->histograma = obj = fl_add_frame(FL_ENGRAVED_FRAME,10,10,332,332,"");
  fdui->oculo_modif = obj = fl_add_frame(FL_ENGRAVED_FRAME,430,290,322,242,"");
  fdui->w_slider = obj = fl_add_valslider(FL_VERT_BROWSER_SLIDER,370,10,40,190,"Threshold");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_bounds(obj, 100, 1);
    fl_set_slider_value(obj, 40);
  fdui->value_SMin = obj = fl_add_text(FL_NORMAL_TEXT,90,420,40,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value_SMax = obj = fl_add_text(FL_NORMAL_TEXT,90,450,40,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value_HMin = obj = fl_add_text(FL_NORMAL_TEXT,210,420,40,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value_HMax = obj = fl_add_text(FL_NORMAL_TEXT,210,450,40,20,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->freeobject1 = obj = fl_add_free(FL_NORMAL_FREE,10,10,330,330,"",
			handle);
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
  fdui->Hmin = obj = fl_add_valslider(FL_VERT_BROWSER_SLIDER,220,360,40,170,"H_min");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_precision(obj, 4);
    fl_set_slider_bounds(obj, 6, 0);
    fl_set_slider_value(obj, 0);
  fdui->Hmax = obj = fl_add_valslider(FL_VERT_BROWSER_SLIDER,270,360,40,170,"H_max");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_precision(obj, 4);
    fl_set_slider_bounds(obj, 6, 0);
    fl_set_slider_value(obj, 0);
    fl_set_slider_step(obj, 1e-04);
  fdui->Smin = obj = fl_add_valslider(FL_VERT_BROWSER_SLIDER,320,360,40,170,"S_min");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_bounds(obj, 1, 0);
    fl_set_slider_value(obj, 0);
  fdui->Smax = obj = fl_add_valslider(FL_VERT_BROWSER_SLIDER,370,360,40,170,"S_max");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_bounds(obj, 1, 0);
    fl_set_slider_value(obj, 0);
  fdui->Imax = obj = fl_add_valslider(FL_HOR_BROWSER_SLIDER,10,360,190,30,"I_max");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_precision(obj, 1);
    fl_set_slider_bounds(obj, 0, 255);
    fl_set_slider_value(obj, 230);
  fdui->Imin = obj = fl_add_valslider(FL_HOR_BROWSER_SLIDER,10,410,190,30,"I_min");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_slider_precision(obj, 1);
    fl_set_slider_bounds(obj, 0, 255);
    fl_set_slider_value(obj, 30);
  fdui->freeobject2 = obj = fl_add_free(FL_NORMAL_FREE,430,10,320,240,"",
			handle2);
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
  fdui->toblack = obj = fl_add_button(FL_PUSH_BUTTON,40,480,140,30,"Background B/BW");
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_BOTTOM_BCOL);
    fl_set_object_lcolor(obj,FL_RIGHT_BCOL);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->hsitunergui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

