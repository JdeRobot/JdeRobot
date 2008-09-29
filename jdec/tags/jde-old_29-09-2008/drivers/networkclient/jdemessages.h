/* 
 Communications always starts with the client sending its own name, a string of MAX_CLIENT_NAME chars, to the server.

 Robot devices messages:

 NETWORKSERVER_sonars <tag,%lu> [<sensor,%d> <reading(mm),%1.1f>] ...\n 
 NETWORKSERVER_ir <tag,%lu> [<column,%d> <row,%d> <reading (ir-units),%d>] ...\n
 NETWORKSERVER_bumpers <tag,%lu> [<sensor,%d> <reading (1=contact,0),%d>] ...\n 
 NETWORKSERVER_laser <tag,%lu> [<reading(mm),%d>] ....\n
 NETWORKSERVER_battery <tag,%lu> <(volts),%1.1f>\n 
 NETWORKSERVER_encoders <tag,%lu> <x(mm),%1.1f> <y(mm),%1.1f> <theta(degrees),%1.5f> <timestamp(us),%lu>\n
 NETWORKSERVER_replay_speed <speedup_factor (1X=real speed),%f>\n
 NETWORKSERVER_drive_speed <speed (mm/s),%f>\n
 NETWORKSERVER_steer_speed <speed (deg/s),%f>\n
 NETWORKSERVER_limp\n

 Image and pantilt messages:

 NETWORKSERVER_y8bpp_sifntsc_image <tag,%lu> <devicenumber,%d> <columns,%d> <rows,%d> <bytesperpixel,%d>
 NETWORKSERVER_rgb24bpp_sifntsc_image <tag,%lu> <devicenumber,%d> <columns,%d> <rows,%d> <bytesperpixel,%d>
 */

#ifndef JDEMESSAGES
#define JDEMESSAGES

/* max characters in a non-image message */
/** Maximum number of characters in a non-image message*/
#define MAX_MESSAGE 2048
/** Maximum number of characters in the client name*/
#define MAX_CLIENT_NAME 20
/** Coment line symbol*/
#define COMMENT '#'

/** Possible messages between client and server*/
typedef enum {
   NETWORKSERVER_goodbye,

   /* robot devices messages */

   NETWORKSERVER_sonars,
   NETWORKSERVER_subscribe_us,
   NETWORKSERVER_unsubscribe_us,
   NETWORKSERVER_ir,
   NETWORKSERVER_subscribe_ir,
   NETWORKSERVER_unsubscribe_ir,
   NETWORKSERVER_bumpers,
   NETWORKSERVER_subscribe_bumpers,
   NETWORKSERVER_unsubscribe_bumpers,
   NETWORKSERVER_encoders,
   NETWORKSERVER_subscribe_encoders,
   NETWORKSERVER_unsubscribe_encoders,
   NETWORKSERVER_battery,
   NETWORKSERVER_subscribe_battery,
   NETWORKSERVER_unsubscribe_battery,
   NETWORKSERVER_sensed_drive_speed,
   NETWORKSERVER_subscribe_sensed_drive_speed,
   NETWORKSERVER_unsubscribe_sensed_drive_speed,
   NETWORKSERVER_sensed_steer_speed,
   NETWORKSERVER_subscribe_sensed_steer_speed,
   NETWORKSERVER_unsubscribe_sensed_steer_speed,

   NETWORKSERVER_drive_speed,
   NETWORKSERVER_steer_speed,
   NETWORKSERVER_limp,

   NETWORKSERVER_replay_speed,

   NETWORKSERVER_laser,
   NETWORKSERVER_subscribe_laser,
   NETWORKSERVER_unsubscribe_laser,

   /* images and pantilt messages */

   NETWORKSERVER_goodbye_images=1000,

   NETWORKSERVER_subscribe_pantilt_encoders,
   NETWORKSERVER_unsubscribe_pantilt_encoders,

   NETWORKSERVER_pantilt_encoders,
   NETWORKSERVER_pantilt_limits_query,
   NETWORKSERVER_pantilt_limits,
   NETWORKSERVER_pantilt_position,
   NETWORKSERVER_pantilt_relative_position,
   NETWORKSERVER_pantilt_reset,
   NETWORKSERVER_pantilt_origin,
   NETWORKSERVER_pantilt_halt,

   NETWORKSERVER_grey8bpp_image_query,
   NETWORKSERVER_grey8bpp_image,
   NETWORKSERVER_logpolar_image_query,
   NETWORKSERVER_logpolar_image,
   NETWORKSERVER_y8bpp_sifntsc_image_query,
   NETWORKSERVER_y8bpp_sifntsc_image,
   NETWORKSERVER_rgb24bpp_sifntsc_image_query,
   NETWORKSERVER_rgb24bpp_sifntsc_image,

   NETWORKSERVER_zoom_encoders,
   NETWORKSERVER_zoom_limits_query,
   NETWORKSERVER_zoom_limits,
   NETWORKSERVER_zoom_position,

   NETWORKSERVER_subscribe_zoom_encoders,
   NETWORKSERVER_unsubscribe_zoom_encoders,


   NETWORKSERVER_error
}JDESocket_Messages;

#endif
