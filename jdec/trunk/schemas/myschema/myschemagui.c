/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "myschemagui.h"

FD_myschemagui *create_form_myschemagui(void)
{
  FL_OBJECT *obj;
  FD_myschemagui *fdui = (FD_myschemagui *) fl_calloc(1, sizeof(*fdui));

  fdui->myschemagui = fl_bgn_form(FL_NO_BOX, 160, 70);
  obj = fl_add_box(FL_UP_BOX,0,0,160,70,"");
    fl_set_object_lcolor(obj,FL_BLUE);
  fdui->fps = obj = fl_add_text(FL_NORMAL_TEXT,30,10,50,20,"");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->text = obj = fl_add_text(FL_NORMAL_TEXT,10,40,140,20,"this is myschema gui");
    fl_set_object_color(obj,FL_DARKCYAN,FL_CYAN);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->myschemagui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

