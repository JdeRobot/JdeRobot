/* Form definition file generated with fdesign. */

#include <X11/forms.h>
#include <stdlib.h>
#include "calibratorgui.h"

FD_calibratorgui *create_form_calibratorgui(void)
{
  FL_OBJECT *obj;
  FD_calibratorgui *fdui = (FD_calibratorgui *) fl_calloc(1, sizeof(*fdui));

  fdui->calibratorgui = fl_bgn_form(FL_NO_BOX, 760, 530);
  fdui->eldercare_form = obj = fl_add_box(FL_UP_BOX,0,0,760,530,"");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
  fdui->color_freeobject = obj = fl_add_free(FL_NORMAL_FREE,20,20,320,240,"",
			color_handle);
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
  fdui->exit_button = obj = fl_add_button(FL_NORMAL_BUTTON,680,10,70,20,"Exit");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  obj = fl_add_frame(FL_EMBOSSED_FRAME,20,20,320,240,"");
  fdui->x_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,360,120,30,340,"x");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_YELLOW);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->y_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,400,120,30,340,"y");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_YELLOW);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->z_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,440,120,30,340,"z");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_YELLOW);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->X_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,510,120,30,340,"X");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_RED);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->Y_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,550,120,30,340,"Y");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_RED);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->Z_cam_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,590,120,30,340,"Z");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_RED);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->roll_slider = obj = fl_add_valslider(FL_HOR_NICE_SLIDER,360,480,390,30,"roll");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_RED);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->save = obj = fl_add_button(FL_NORMAL_BUTTON,590,10,80,30,"save");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_PALEGREEN,FL_GREEN);
  fdui->flip_button = obj = fl_add_checkbutton(FL_PUSH_BUTTON,350,20,80,40,"flip");
  fdui->focus_slider = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,650,130,20,200,"f");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->cam_joystick = obj = fl_add_positioner(FL_NORMAL_POSITIONER,630,350,120,110,"");
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
  obj = fl_add_frame(FL_ENGRAVED_FRAME,20,270,320,240,"");
  fdui->virtual_freeobject = obj = fl_add_free(FL_NORMAL_FREE,20,270,320,240,"",
			virtual_handle);
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
  fdui->reload = obj = fl_add_button(FL_NORMAL_BUTTON,440,10,70,30,"reload");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_PALEGREEN,FL_GREEN);
  fdui->undo = obj = fl_add_button(FL_NORMAL_BUTTON,510,10,70,30,"undo");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_PALEGREEN,FL_GREEN);
  fdui->u0 = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,680,130,20,200,"u0");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->v0 = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,710,130,20,200,"v0");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BLUE);
    fl_set_slider_precision(obj, 0);
    fl_set_slider_size(obj, 0.00);
  fdui->fixedFOA = obj = fl_add_checkbutton(FL_PUSH_BUTTON,350,60,80,40,"fixed FOA");
  fl_end_form();

  fdui->calibratorgui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

