/*
 *  jdeneo.c
 *
 *  Copyright (C) 2006-2007 Jesús Ruiz-Ayúcar and José María Cañas
 *  for the Universidad Rey Juan Carlos robotic group.
 *  http://www.robotica-urjc.es
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "preferences.h"
#include "jde.h"

/****************************************************
 * CALLBACKS                                        *
 ****************************************************/

void on_preferences_activate () {
   GKeyFile *key = NULL;
   gchar path[1024], **groups;
   guint i;

   sprintf(path, "%s/jdeneoc", g_get_user_config_dir());

   key = g_key_file_new();
   g_key_file_load_from_file(key, path, G_KEY_FILE_NONE, NULL);
   g_key_file_set_list_separator (key, ',');

   /* Ahora sacamos info de la configuración */
   groups = g_key_file_get_groups (key, NULL);

   for(i = 0; groups[i]; i++) {
      if (!strcmp(groups[i], name[SCH_SONARS])) {
         if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"otos")) {
            gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "sonar_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "sonar_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_LASER])) {
         if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"otos")) {
            gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "laser_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "laser_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_ENCODERS])) {
         if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"otos")==0) {
            gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "encoders_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "encoders_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_MOTORS])) {
         if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"otos")==0) {
            gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "motors_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "motors_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_IMAGEA])) {
         if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"oculo"))
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_oculo")), 
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imagea_oculo_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imagea_oculo_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imagea_oculo_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imagea_oculo_dev")),
                  g_key_file_get_integer(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"firewire"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_firewire")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imagea_firewire_frame"));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imagea_firewire_dev")),
                  g_key_file_get_integer(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"video4linux"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_v4l")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imagea_v4l_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imagea_v4l_dev")),
                  g_key_file_get_string(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"file"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_file")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imagea_file_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imagea_file_filename")),
                  g_key_file_get_string(key, groups[i], "filename", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_IMAGEB])) {
         if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"oculo"))
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_oculo")),
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imageb_oculo_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imageb_oculo_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imageb_oculo_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imageb_oculo_dev")),
                  g_key_file_get_integer(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"firewire"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_firewire")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imageb_firewire_frame"));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imageb_firewire_dev")),
                  g_key_file_get_integer(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"video4linux"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_v4l")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imageb_v4l_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imageb_v4l_dev")),
                  g_key_file_get_string(key, groups[i], "device", NULL));
         }
         else if (!strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"file"))
         {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_file")), TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "imageb_file_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "imageb_file_filename")),
                  g_key_file_get_string(key, groups[i], "filename", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_PANTILTMOTORS])) {
         if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"oculo")==0)
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pm_oculo")),
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "pm_oculo_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "pm_oculo_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "pm_oculo_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
         else if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"serial")==0)
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pm_serial")),
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "pm_serial_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "pm_serial_dev")),
                  g_key_file_get_string(key, groups[i], "device", NULL));
         }
      }
      else if (!strcmp(groups[i], name[SCH_PANTILTENCODERS])) {
         if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"oculo")==0)
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pe_oculo")),
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "pe_oculo_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "pe_oculo_host")),
                  g_key_file_get_string(key, groups[i], "host", NULL));
            gtk_spin_button_set_value(
                  GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "pe_oculo_port")),
                  g_key_file_get_integer(key, groups[i], "port", NULL));
         }
         else if (strcmp(g_key_file_get_string(key, groups[i], "type", NULL),"serial")==0)
         {
            gtk_toggle_button_set_active(
                  GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pe_serial")),
                  TRUE);
            gtk_widget_show(glade_xml_get_widget(xml, "pe_serial_frame"));
            gtk_entry_set_text(
                  GTK_ENTRY(glade_xml_get_widget(xml, "pe_serial_dev")),
                  g_key_file_get_string(key, groups[i], "device", NULL));
         }
      }
      else if (!strcmp(groups[i], "dirs")) {
         gtk_entry_set_text(
               GTK_ENTRY(glade_xml_get_widget(xml, "dirs")),
               g_key_file_get_string(key, groups[i], "value", NULL));
      }
   }

   /* Finalmente liberamos memoria */
   g_free(groups);
   g_free(key);

   gtk_widget_show(glade_xml_get_widget(xml, "winPreferences"));
}

void on_preferences_clicked ( GtkToolButton *tool , gpointer user_data ) {
   on_preferences_activate ();
}

void on_preferences_ok_clicked ( GtkButton *button , gpointer user_data ) {
   GKeyFile *key = NULL;
   gchar path[1024], **groups;

   sprintf(path, "%s/jdeneoc", g_get_user_config_dir());

   key = g_key_file_new();
   g_key_file_set_list_separator (key, ',');

   /* Ahora sacamos la nueva info de la configuración */
   groups = g_key_file_get_groups (key, NULL);

   /* Sonar */
   g_key_file_set_string ( key, name[SCH_SONARS],
                           "type",
                           "otos");
   g_key_file_set_string ( key, name[SCH_SONARS], 
                           "host",
                           gtk_entry_get_text(
                              GTK_ENTRY(glade_xml_get_widget(xml, "sonar_host"))));
   g_key_file_set_integer( key, name[SCH_SONARS],
                           "port",
                           gtk_spin_button_get_value_as_int( 
                              GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "sonar_port"))));
   /* Laser */
   g_key_file_set_string ( key, name[SCH_LASER],
                           "type",
                           "otos");
   g_key_file_set_string ( key, name[SCH_LASER],
                           "host",
                           gtk_entry_get_text(
                              GTK_ENTRY(glade_xml_get_widget(xml, "laser_host"))));
   g_key_file_set_integer( key, name[SCH_LASER],
                           "port",
                           gtk_spin_button_get_value_as_int( 
                              GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "laser_port"))));

   /* Encoders */
   g_key_file_set_string ( key, name[SCH_ENCODERS],
                           "type",
                           "otos");
   g_key_file_set_string ( key, name[SCH_ENCODERS],
                           "host",
                           gtk_entry_get_text(
                              GTK_ENTRY(glade_xml_get_widget(xml, "encoders_host"))));
   g_key_file_set_integer( key, name[SCH_ENCODERS],
                           "port",
                           gtk_spin_button_get_value_as_int( 
                              GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "encoders_port"))));

   /* Motors */
   g_key_file_set_string ( key, name[SCH_MOTORS],
                           "type",
                           "otos");
   g_key_file_set_string ( key, name[SCH_MOTORS],
                           "host",
                           gtk_entry_get_text(
                              GTK_ENTRY(glade_xml_get_widget(xml, "motors_host"))));
   g_key_file_set_integer( key, name[SCH_MOTORS],
                           "port",
                           gtk_spin_button_get_value_as_int(
                              GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "motors_port"))));
   /* Pantilt encoders */
   if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pe_oculo")))) {
      g_key_file_set_string ( key, name[SCH_PANTILTENCODERS],
                              "type",
                              "oculo");
      g_key_file_set_string ( key, name[SCH_PANTILTENCODERS],
                              "host",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "pe_oculo_host"))));
      g_key_file_set_integer( key, name[SCH_PANTILTENCODERS],
                              "port",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "pe_oculo_port"))));

   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pe_serial")))) {
      g_key_file_set_string ( key, name[SCH_PANTILTENCODERS],
                              "type",
                              "serial");
      g_key_file_set_string ( key, name[SCH_PANTILTENCODERS],
                              "device",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "pe_serial_dev"))));
   }

   /* Pantilt motors */
   if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pm_oculo")))) {
      g_key_file_set_string ( key, name[SCH_PANTILTMOTORS],
                              "type",
                              "oculo");
      g_key_file_set_string ( key, name[SCH_PANTILTMOTORS],
                              "host",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "pm_oculo_host"))));
      g_key_file_set_integer( key, name[SCH_PANTILTMOTORS],
                              "port",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "pm_oculo_port"))));

   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "pm_serial")))) {
      g_key_file_set_string ( key, name[SCH_PANTILTMOTORS],
                              "type",
                              "serial");
      g_key_file_set_string ( key, name[SCH_PANTILTMOTORS],
                              "device",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "pm_serial_dev"))));
   }

   /* ImageA */
   if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_oculo")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "type",
                              "oculo");
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "host",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imagea_oculo_host"))));
      g_key_file_set_integer( key, name[SCH_IMAGEA],
                              "port",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imagea_oculo_port"))));
      g_key_file_set_integer( key, name[SCH_IMAGEA],
                              "device",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imagea_oculo_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_firewire")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "type",
                              "firewire");
      g_key_file_set_integer( key, name[SCH_IMAGEA],
                              "device",
                              gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(
                                 glade_xml_get_widget(xml, "imagea_firewire_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_v4l")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "type",
                              "video4linux");
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "device",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imagea_v4l_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imagea_file")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "type",
                              "file");
      g_key_file_set_string ( key, name[SCH_IMAGEA],
                              "filename",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imagea_file_filename"))));
   }

   /* ImageB */
   if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_oculo")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "type",
                              "oculo");
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "host",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imageb_oculo_host"))));
      g_key_file_set_integer( key, name[SCH_IMAGEB],
                              "port",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imageb_oculo_port"))));
      g_key_file_set_integer( key, name[SCH_IMAGEB],
                              "device",
                              gtk_spin_button_get_value_as_int(
                                 GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "imageb_oculo_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_firewire")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "type",
                              "firewire");
      g_key_file_set_integer( key, name[SCH_IMAGEB],
                              "device",
                              gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(
                                 glade_xml_get_widget(xml, "imageb_firewire_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_v4l")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "type",
                              "video4linux");
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "device",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imageb_v4l_dev"))));
   }
   else if (gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(glade_xml_get_widget(xml, "imageb_file")))) {
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "type",
                              "file");
      g_key_file_set_string ( key, name[SCH_IMAGEB],
                              "filename",
                              gtk_entry_get_text(
                                 GTK_ENTRY(glade_xml_get_widget(xml, "imageb_file_filename"))));
   }

   /* Directories */
   g_key_file_set_string ( key, "dirs",
                           "value",
                           gtk_entry_get_text(
                              GTK_ENTRY(glade_xml_get_widget(xml, "dirs"))));

   /* Saving */
   if (g_file_set_contents(path,g_key_file_to_data(key, NULL, NULL), -1, NULL))
      g_print("Configuration file sucessfully saved\n");
   else
      g_warning("Error while saving configuration file");
   gtk_widget_hide(glade_xml_get_widget(xml, "winPreferences"));
}

void on_winPreferences_delete_event() {
   gtk_widget_hide(glade_xml_get_widget(xml, "winPreferences"));
}

void on_preferences_cancel_clicked ( GtkButton *button , gpointer user_data ) {
   on_winPreferences_delete_event();
}

void on_pe_oculo_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "pe_oculo_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "pe_serial_frame"));
}

void on_pe_serial_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_show(glade_xml_get_widget(xml, "pe_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "pe_serial_frame"));
}

void on_pm_oculo_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_show(glade_xml_get_widget(xml, "pm_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "pm_serial_frame"));
}

void on_pm_serial_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "pm_oculo_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "pm_serial_frame"));
}

void on_imagea_oculo_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_show(glade_xml_get_widget(xml, "imagea_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_file_frame"));
}

void on_imagea_firewire_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_oculo_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imagea_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_file_frame"));
}

void on_imagea_video4linux_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_firewire_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imagea_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_file_frame"));
}

void on_imagea_file_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imagea_v4l_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imagea_file_frame"));
}

void on_imageb_oculo_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_show(glade_xml_get_widget(xml, "imageb_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_file_frame"));
}

void on_imageb_firewire_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_oculo_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imageb_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_file_frame"));
}

void on_imageb_video4linux_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_firewire_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imageb_v4l_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_file_frame"));
}

void on_imageb_file_activate ( GtkButton *button , gpointer user_data ) {
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_oculo_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_firewire_frame"));
   gtk_widget_hide(glade_xml_get_widget(xml, "imageb_v4l_frame"));
   gtk_widget_show(glade_xml_get_widget(xml, "imageb_file_frame"));
}

void openV4L_imageA ( GtkWidget *widget, gpointer user_data) {
   GtkWidget *file_selector;
   const gchar *selected_filename;
   gchar *device;

   /* Obtenemos el nombre de fichero desde el diálogo */
   file_selector = GTK_WIDGET (user_data);
   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
   device = g_strdup(selected_filename);
   gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "imagea_v4l_dev")), device);
}

void on_imagea_v4l_open_clicked ( GtkButton *button, gpointer user_data) {
   GtkWidget *pFileSelection;

    /* Creación de la ventana de selección */
   pFileSelection = gtk_file_selection_new("Abrir dispositivo video4linux");

    /* Limitar las acciones a esta ventana */
   gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);

   g_signal_connect (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                     "clicked",
                     G_CALLBACK (openV4L_imageA),
                     pFileSelection);

   /* Ensure that the dialog box is destroyed when the user clicks a button. */
   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             pFileSelection);

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->cancel_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             pFileSelection);

   gtk_widget_show(pFileSelection);
}

void openV4L_imageB ( GtkWidget *widget, gpointer user_data) {
   GtkWidget *file_selector;
   const gchar *selected_filename;
   gchar *device;

   /* Obtenemos el nombre de fichero desde el diálogo */
   file_selector = GTK_WIDGET (user_data);
   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
   device = g_strdup(selected_filename);
   gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "imageb_v4l_dev")), device);
}

void on_imageb_v4l_open_clicked ( GtkButton *button, gpointer user_data) {
   GtkWidget *pFileSelection;

    /* Creación de la ventana de selección */
   pFileSelection = gtk_file_selection_new("Abrir dispositivo video4linux");

    /* Limitar las acciones a esta ventana */
   gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);

   g_signal_connect (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                     "clicked",
                     G_CALLBACK (openV4L_imageB),
                     pFileSelection);

   /* Ensure that the dialog box is destroyed when the user clicks a button. */
   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             pFileSelection);

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->cancel_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             pFileSelection);

   gtk_widget_show(pFileSelection);
}

void openFile_imageA ( GtkWidget *widget, gpointer user_data) {
   GtkWidget *file_selector;
   const gchar *selected_filename;
   gchar *device;

   /* Obtenemos el nombre de fichero desde el diálogo */
   file_selector = GTK_WIDGET (user_data);
   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
   device = g_strdup(selected_filename);
   gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "imagea_file_filename")), device);
}

void on_imagea_file_open_clicked ( GtkButton *button, gpointer user_data) {
   GtkWidget *pFileSelection;

    /* Creación de la ventana de selección */
   pFileSelection = gtk_file_selection_new("Abrir fichero");

    /* Limitar las acciones a esta ventana */
   gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);

   g_signal_connect (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                     "clicked",
                     G_CALLBACK (openFile_imageA),
                     pFileSelection);

   /* Ensure that the dialog box is destroyed when the user clicks a button. */
   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             pFileSelection);

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->cancel_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             pFileSelection);

   gtk_widget_show(pFileSelection);
}

void openFile_imageB ( GtkWidget *widget, gpointer user_data) {
   GtkWidget *file_selector;
   const gchar *selected_filename;
   gchar *device;

   /* Obtenemos el nombre de fichero desde el diálogo */
   file_selector = GTK_WIDGET (user_data);
   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
   device = g_strdup(selected_filename);
   gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(xml, "imageb_file_filename")), device);
}

void on_imageb_file_open_clicked ( GtkButton *button, gpointer user_data) {
   GtkWidget *pFileSelection;

    /* Creación de la ventana de selección */
   pFileSelection = gtk_file_selection_new("Abrir fichero");

    /* Limitar las acciones a esta ventana */
   gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);

   g_signal_connect (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                     "clicked",
                     G_CALLBACK (openFile_imageB),
                     pFileSelection);

   /* Ensure that the dialog box is destroyed when the user clicks a button. */
   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->ok_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             pFileSelection);

   g_signal_connect_swapped (GTK_FILE_SELECTION (pFileSelection)->cancel_button,
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy),
                             pFileSelection);

   gtk_widget_show(pFileSelection);
}
