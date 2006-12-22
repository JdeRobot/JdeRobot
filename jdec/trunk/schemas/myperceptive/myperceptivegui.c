/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "myperceptivegui.h"

FD_myperceptivegui *create_form_myperceptivegui(void)
{
  FL_OBJECT *obj;
  FD_myperceptivegui *fdui = (FD_myperceptivegui *) fl_calloc(1, sizeof(*fdui));

  fdui->myperceptivegui = fl_bgn_form(FL_NO_BOX, 160, 70);
  obj = fl_add_box(FL_UP_BOX,0,0,160,70,"");
    fl_set_object_lcolor(obj,FL_BLUE);
  fdui->hide = obj = fl_add_button(FL_NORMAL_BUTTON,100,10,50,20,"HIDE");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
    fl_set_object_lcolor(obj,FL_CYAN);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  fdui->activation = obj = fl_add_lightbutton(FL_PUSH_BUTTON,10,40,120,20,"myschema");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_COL1,FL_DARKTOMATO);
    fl_set_object_lcolor(obj,FL_DARKTOMATO);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_TIMESBOLD_STYLE);
  fdui->fps = obj = fl_add_text(FL_NORMAL_TEXT,30,10,50,20,"");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->myperceptivegui->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

