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

/* 
 Communications always starts with the client sending its own name, a string of MAX_CLIENT_NAME chars, to the server.

 Supported messages by otos server:

 OTOS_sonars <tag,%lu> [<sensor,%d> <reading(mm),%1.1f>] ...\n 
 OTOS_ir <tag,%lu> [<column,%d> <row,%d> <reading (ir-units),%d>] ...\n
 OTOS_bumpers <tag,%lu> [<sensor,%d> <reading (1=contact,0),%d>] ...\n 
 OTOS_laser <tag,%lu> [<reading(mm),%d>] ....\n
 OTOS_battery <tag,%lu> <(volts),%1.1f>\n 
 OTOS_encoders <tag,%lu> <x(mm),%1.1f> <y(mm),%1.1f> <theta(degrees),%1.5f> <timestamp(us),%lu>\n
 OTOS_replay_speed <speedup_factor (1X=real speed),%f>\n
 OTOS_drive_speed <speed (mm/s),%f>\n
 OTOS_steer_speed <speed (deg/s),%f>\n
 OTOS_limp\n

 Supported messages by oculo server:

 OCULO_y8bpp_sifntsc_image <tag,%lu> <devicenumber,%d> <columns,%d> <rows,%d> <bytesperpixel,%d>
 OCULO_rgb24bpp_sifntsc_image <tag,%lu> <devicenumber,%d> <columns,%d> <rows,%d> <bytesperpixel,%d>
 */

#ifndef JDESERVERS
#define JDESERVERS

/* max characters in a non-image message */
#define MAX_MESSAGE 2048         
#define MAX_CLIENT_NAME 20
/* To record sensations in a file */
#define COMMENT '#'


typedef enum {
/* Served by otos */
OTOS_goodbye,

OTOS_sonars,
OTOS_subscribe_us,
OTOS_unsubscribe_us,
OTOS_ir,
OTOS_subscribe_ir,
OTOS_unsubscribe_ir,
OTOS_bumpers,
OTOS_subscribe_bumpers,
OTOS_unsubscribe_bumpers,
OTOS_encoders,
OTOS_subscribe_encoders,
OTOS_unsubscribe_encoders,
OTOS_battery,
OTOS_subscribe_battery,
OTOS_unsubscribe_battery,
OTOS_sensed_drive_speed,
OTOS_subscribe_sensed_drive_speed,
OTOS_unsubscribe_sensed_drive_speed,
OTOS_sensed_steer_speed,
OTOS_subscribe_sensed_steer_speed,
OTOS_unsubscribe_sensed_steer_speed,

OTOS_drive_speed,
OTOS_steer_speed,
OTOS_limp,

OTOS_replay_speed,

OTOS_laser,
OTOS_subscribe_laser,
OTOS_unsubscribe_laser,

/* Served by oculo */
OCULO_goodbye=1000,

OCULO_subscribe_pantilt_encoders,
OCULO_unsubscribe_pantilt_encoders,

OCULO_pantilt_encoders, 
OCULO_pantilt_limits_query,
OCULO_pantilt_limits,
OCULO_pantilt_position,
OCULO_pantilt_relative_position,
OCULO_pantilt_reset,
OCULO_pantilt_origin,
OCULO_pantilt_halt,
OCULO_grey8bpp_image_query,
OCULO_grey8bpp_image,
OCULO_logpolar_image_query,
OCULO_logpolar_image,
OCULO_y8bpp_sifntsc_image_query,
OCULO_y8bpp_sifntsc_image,
OCULO_rgb24bpp_sifntsc_image_query,
OCULO_rgb24bpp_sifntsc_image
}JDESocket_Messages;

#endif


