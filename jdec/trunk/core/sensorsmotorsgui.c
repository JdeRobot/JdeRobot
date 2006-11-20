/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "sensorsmotorsgui.h"

FD_sensorsmotorsgui *create_form_sensorsmotorsgui(void)
{
  FL_OBJECT *obj;
  FD_sensorsmotorsgui *fdui = (FD_sensorsmotorsgui *) fl_calloc(1, sizeof(*fdui));

  fdui->sensorsmotorsgui = fl_bgn_form(FL_NO_BOX, 1150, 520);
  obj = fl_add_box(FL_UP_BOX,0,0,1150,520,"");
    fl_set_object_lcolor(obj,FL_BLUE);
  fdui->hide = obj = fl_add_button(FL_NORMAL_BUTTON,10,260,50,20,"HIDE");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
    fl_set_object_lcolor(obj,FL_CYAN);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  fdui->escala = obj = fl_add_valslider(FL_VERT_NICE_SLIDER,20,30,30,150,"scale,m");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_YELLOW);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->track_robot = obj = fl_add_button(FL_PUSH_BUTTON,10,190,50,30,"track\n robot");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_BOTTOM_BCOL);
    fl_set_object_lcolor(obj,FL_YELLOW);
  fdui->ventanaA = obj = fl_add_frame(FL_ENGRAVED_FRAME,480,10,323,243,"colorA");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
  fdui->micanvas = obj = fl_add_canvas(FL_NORMAL_CANVAS,70,10,400,360,"canvas");
    fl_set_object_lcolor(obj,FL_RIGHT_BCOL);
  fdui->ventanaB = obj = fl_add_frame(FL_ENGRAVED_FRAME,810,10,323,243,"colorB");
    fl_set_object_color(obj,FL_COL1,FL_COL1);

  fdui->teleoperator = fl_bgn_group();
  fdui->joystick = obj = fl_add_positioner(FL_NORMAL_POSITIONER,120,410,110,50,"");
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
  fdui->back = obj = fl_add_button(FL_PUSH_BUTTON,180,470,50,20,"back");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKGOLD);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
  fdui->stop = obj = fl_add_button(FL_MENU_BUTTON,120,470,50,20,"stop");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKGOLD);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
  fl_end_group();


  fdui->pantiltjoystick = fl_bgn_group();
  fdui->pantilt_joystick = obj = fl_add_positioner(FL_NORMAL_POSITIONER,320,410,110,50,"");
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
  fdui->pantilt_origin = obj = fl_add_button(FL_MENU_BUTTON,380,470,50,20,"origin");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKGOLD);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
  fdui->pantilt_stop = obj = fl_add_button(FL_MENU_BUTTON,320,470,50,20,"stop");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKER_COL1,FL_DARKGOLD);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
  fdui->ptspeed = obj = fl_add_slider(FL_VERT_SLIDER,300,410,10,70,"speed");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_LIGHTER_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_slider_size(obj, 0.00);
  fl_end_group();

  fdui->ventanaC = obj = fl_add_frame(FL_ENGRAVED_FRAME,480,260,323,243,"colorC");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
  fdui->ventanaD = obj = fl_add_frame(FL_ENGRAVED_FRAME,810,260,323,243,"colorD");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
  obj = fl_add_text(FL_NORMAL_TEXT,110,380,140,20,"Base Teleoperator");
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,290,380,150,20,"Pantilt Teleoperator");
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->sensorsmotorsgui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

