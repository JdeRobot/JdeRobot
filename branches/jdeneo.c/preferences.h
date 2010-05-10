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

#include "gui.h"

/****************************************************
 * CALLBACKS                                        *
 ****************************************************/
void on_preferences_activate ();

void on_preferences_clicked ( GtkToolButton *tool ,
                              gpointer user_data );

void on_preferences_apply_clicked ( GtkButton *button ,
                              gpointer user_data );

void on_preferences_ok_clicked ( GtkButton *button ,
                              gpointer user_data );

void on_winPreferences_delete_event();

void on_preferences_cancel_clicked ( GtkButton *button ,
                              gpointer user_data );

void on_pe_oculo_activate ( GtkButton *button ,
                              gpointer user_data );

void on_pe_serial_activate ( GtkButton *button ,
                              gpointer user_data );

void on_pm_oculo_activate ( GtkButton *button ,
                              gpointer user_data );

void on_pm_serial_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imagea_oculo_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imagea_firewire_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imagea_video4linux_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imagea_file_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imageb_oculo_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imageb_firewire_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imageb_video4linux_activate ( GtkButton *button ,
                              gpointer user_data );

void on_imageb_file_activate ( GtkButton *button ,
                              gpointer user_data );
