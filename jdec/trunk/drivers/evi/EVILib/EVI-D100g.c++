// EVI-D100g.c++
//
// Access functions to a SONY EVI-D100
//
// RS-232C Serial Port Communication by Max Lungarella.
// Email: max.lungarella@aist.go.jp
//
// Copyright (c) 2004 Pic Mickael
//
//----------------------------------------------------------------------------
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//----------------------------------------------------------------------------
//
// Author: Pic Mickael, AIST Japan, 2004.
// Email: mickael.pic@aist.go.jp

#include "EVI-D100g.h"

//-------------------------------------------------------------------------

/*
 * Constructor
 */
EVI_D100g::EVI_D100g()
{
  minGain = EVILIB_minGain;
  maxGain = EVILIB_maxGain;
  minpan = EVILIB_minpan;
  maxpan = EVILIB_maxpan;
  maxPan = EVILIB_maxPan;
  min_pspeed = EVILIB_min_pspeed;
  max_pspeed = EVILIB_max_pspeed;
  mintilt = EVILIB_mintilt;
  maxtilt = EVILIB_maxtilt;
  maxTilt = EVILIB_maxTilt;
  min_tspeed = EVILIB_min_tspeed;
  max_tspeed = EVILIB_max_tspeed;
  minzoom = EVILIB_minzoom;
  maxzoom = EVILIB_maxzoom;
  maxZoom = EVILIB_maxZoom;
  min_zspeed = EVILIB_min_zspeed;
  max_zspeed = EVILIB_max_zspeed;
  min_iris = EVILIB_minIris;
  max_iris = EVILIB_maxIris;
}

/*
 *
 */
EVI_D100g::~EVI_D100g()
{
}

/*
 * Wide mode setting
 * type: EVILIB_OFF
 *       EVILIB_CINEMA
 *       EVILIB_16_9_FULL
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Wide(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046000FF", Id_cam);
	  break;
        case EVILIB_CINEMA:
	  sprintf(buffer, "8%d01046001FF", Id_cam);
	  break;
        case EVILIB_16_9_FULL:
	  sprintf(buffer, "8%d01046002FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Digital effect setting
 * type: EVILIB_OFF
 *       EVILIB_STILL
 *       EVILIB_FLASH
 *       EVILIB_LUMI
 *       EVILIB_TRAIL
 *       EVILIB_EFFECTLEVEL -> need 'effect'
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * effect: effect level 0 (00) to 24 (18) (flash, trail), 0 (00) to 32 (20) (still, lumi)
 */
int EVI_D100g::DigitalEffect(int type, int level, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046400FF", Id_cam);
	  break;
        case EVILIB_STILL:
	  sprintf(buffer, "8%d01046401FF", Id_cam);
	  break;
        case EVILIB_FLASH:
	  sprintf(buffer, "8%d01046402FF", Id_cam);
	  break;
        case EVILIB_LUMI:
	  sprintf(buffer, "8%d01046403FF", Id_cam);
	  break;
        case EVILIB_TRAIL:
	  sprintf(buffer, "8%d01046404FF", Id_cam);
	  break;
        case EVILIB_EFFECTLEVEL:
	  if(level >= 0 && level <= 32)
            {
	      if(level <= 15)
		sprintf(buffer, "8%d0104650%XFF", Id_cam, level);
	      else
		sprintf(buffer, "8%d010465%XFF", Id_cam, level);
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_OFF
 *                    EVILIB_STILL
 *                    EVILIB_FLASH
 *                    EVILIB_LUMI
 *                    EVILIB_TRAIL
 * Return on error: 0
 */
int EVI_D100g::DigitalEffectModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090464FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_OFF;
  if(c == '\x01')
    return EVILIB_STILL;
  if(c == '\x02')
    return EVILIB_FLASH;
  if(c == '\x03')
    return EVILIB_LUMI;
  if(c == '\x04')
    return EVILIB_TRAIL;
  return 0;
}

/*
 * level contain the Effect level
 * Return 1 on success, 0 on error
 */
int EVI_D100g::DigitalEffectLevelInq(int &level)
{
  level = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090465FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      level = int(deconvert(2, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_OFF
 *                    EVILIB_CINEMA
 *                    EVILIB_16_9_FULL
 * Return on error: 0
 */
int EVI_D100g::WideModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090460FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_OFF;
  if(c == '\x01')
    return EVILIB_CINEMA;
  if(c == '\x02')
    return EVILIB_16_9_FULL;
  return 0;
}

//-------------------------------------------------------------------------

/*
 * Zoom control.
 * type: EVILIB_STOP
 *       EVILIB_TELE_S (Standard)
 *       EVILIB_WIDE_S (Standard)
 *       EVILIB_TELE_V (Variable) --> need 'speed'
 *       EVILIB_WIDE_V (Variable) --> need 'speed'
 *       EVILIB_DIRECT --> need 'zoom'
 *       EVILIB_DZOOM_ON
 *       EVILIB_DZOOM_OFF
 * speed: speed parameter EVILIB_min_zspeed (Low) to EVILIB_max_zspeed (High)
 * zoom: EVILIB_minzoom (Wide) to EVILIB_maxzoom(Tele)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Zoom(int type, int speed, float zoom, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_STOP:
	  sprintf(buffer, "8%d01040700FF", Id_cam);
	  break;
        case EVILIB_TELE_S:
	  sprintf(buffer, "8%d01040702FF", Id_cam);
	  break;
        case EVILIB_WIDE_S:
	  sprintf(buffer, "8%d01040703FF", Id_cam);
	  break;
        case EVILIB_TELE_V:
	  if(speed >= EVILIB_min_zspeed && speed <= EVILIB_max_zspeed)
	    sprintf(buffer, "8%d01040720%XFF", Id_cam, speed);
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_WIDE_V:
	  if(speed >= EVILIB_min_zspeed && speed <= EVILIB_max_zspeed)
	    sprintf(buffer, "8%d01040730%XFF", Id_cam, speed);
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_DIRECT:
	  if(zoom >= EVILIB_minzoom && zoom <= EVILIB_maxzoom)
            {
	      sprintf(buffer, "8%d010447", Id_cam);
	      make0XString(convert(EVILIB_ZOOM, zoom, EVILIB_maxzoom, EVILIB_maxZoom), &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_DZOOM_ON:
	  sprintf(buffer, "8%d01040602FF", Id_cam);
	  break;
        case EVILIB_DZOOM_OFF:
	  sprintf(buffer, "8%d01040603FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Focus control.
 * When adjust the focus, change the mode to Manual the send Far/Near or Direct command.
 * type: EVILIB_STOP
 *       EVILIB_FAR
 *       EVILIB_NEAR
 *       EVILIB_FAR_V  -> focus: speed parameter, EVILIB_min_FocusSpeed (low) to EVILIB_max_FocusSpeed (high)
 *       EVILIB_NEAR_V -> focus: speed parameter, EVILIB_min_FocusSpeed (low) to EVILIB_max_FocusSpeed (high)
 *       EVILIB_DIRECT -> focus: infinity = EVILIB_minFocus, close = EVILIB_maxFocus
 *       EVILIB_AUTO
 *       EVILIB_MANUAL
 *       EVILIB_AUTO_MANUAL
 *       EVILIB_ONEPUSH_TRIGGER
 *       EVILIB_INFINITY
 *       EVILIB_AF_SENS_HIGH
 *       EVILIB_AF_SENS_LOW
 *       EVILIB_NEAR_LIMIT -> focus: focus near limit position EVILIB_minFocus (far) to EVILIB_maxFocus (near)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Focus(int type, int focus, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_STOP:
	  sprintf(buffer, "8%d01040800FF", Id_cam);
	  break;
        case EVILIB_FAR:
	  sprintf(buffer, "8%d01040802FF", Id_cam);
	  break;
        case EVILIB_NEAR:
	  sprintf(buffer, "8%d01040803FF", Id_cam);
	  break;
        case EVILIB_FAR_V:
	  if(focus >= EVILIB_min_FocusSpeed && focus <= EVILIB_max_FocusSpeed)
	    sprintf(buffer, "8%d0104082%XFF", Id_cam, focus);
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_NEAR_V:
	  if(focus >= EVILIB_min_FocusSpeed && focus <= EVILIB_max_FocusSpeed)
	    sprintf(buffer, "8%d0104083%XFF", Id_cam, focus);
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_DIRECT:
	  if(focus >= EVILIB_minFocus && focus <= EVILIB_maxFocus)
            {
	      sprintf(buffer, "8%d010448", Id_cam);
	      make0XString(focus, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_AUTO:
	  sprintf(buffer, "8%d01043802FF", Id_cam);
	  break;
        case EVILIB_MANUAL:
	  sprintf(buffer, "8%d01043803FF", Id_cam);
	  break;
        case EVILIB_AUTO_MANUAL:
	  sprintf(buffer, "8%d01043810FF", Id_cam);
	  break;
        case EVILIB_ONEPUSH_TRIGGER:
	  sprintf(buffer, "8%d01041801FF", Id_cam);
	  break;
        case EVILIB_INFINITY:
	  sprintf(buffer, "8%d01041802FF", Id_cam);
	  break;
        case EVILIB_AF_SENS_HIGH:
	  sprintf(buffer, "8%d01045802FF", Id_cam);
	  break;
        case EVILIB_AF_SENS_LOW:
	  sprintf(buffer, "8%d01045803FF", Id_cam);
	  break;
        case EVILIB_NEAR_LIMIT:
	  if(focus >= EVILIB_minFocus && focus <= EVILIB_maxFocus)
            {
	      sprintf(buffer, "8%d010428", Id_cam);
	      make0XString(focus, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * White Balance Setting.
 * type: EVILIB_AUTO         : Trace the light source automatically
 *       EVILIB_INDOOR       : fixed at factory
 *       EVILIB_OUTDOOR      : fixed at factory
 *       EVILIB_ONEPUSH_MODE : Pull-in to White with a Trigger then hold the data until next Trigger comming
 *       EVILIB_ATW          : Auto tracing white balance
 *       EVILIB_MANUAL       : Manual control mode
 *       EVILIB_ONEPUSH_TRIGGER
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::WB(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_AUTO:
	  sprintf(buffer, "8%d01043500FF", Id_cam);
	  break;
        case EVILIB_INDOOR:
	  sprintf(buffer, "8%d01043501FF", Id_cam);
	  break;
        case EVILIB_OUTDOOR:
	  sprintf(buffer, "8%d01043502FF", Id_cam);
	  break;
        case EVILIB_ONEPUSH_MODE:
	  sprintf(buffer, "8%d01043503FF", Id_cam);
	  break;
        case EVILIB_ATW:
	  sprintf(buffer, "8%d01043504FF", Id_cam);
	  break;
        case EVILIB_MANUAL:
	  sprintf(buffer, "8%d01043505FF", Id_cam);
	  break;
        case EVILIB_ONEPUSH_TRIGGER:
	  sprintf(buffer, "8%d01041005FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo); 
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_AUTO         : Auto exposure mode
 *       EVILIB_MANUAL       : Manual control mode
 *       EVILIB_SHUTTER_PRIO : Shutter priority automatic exposure mode
 *       EVILIB_IRIS_PRIO    : Iris priority automatic exposure mode
 *       EVILIB_GAIN_PRIO    : Gain priority automatic Exposure Mode
 *       EVILIB_BRIGHT       : Bright mode (Manual control)
 *       EVILIB_SHUTTER_AUTO : Automatic shutter mode
 *       EVILIB_IRIS_AUTO    : Automatic iris mode
 *       EVILIB_GAIN_AUTO    : Automatic gain mode
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::AE(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_AUTO:
	  sprintf(buffer, "8%d01043900FF", Id_cam);
	  break;
        case EVILIB_MANUAL:
	  sprintf(buffer, "8%d01043903FF", Id_cam);
	  break;
        case EVILIB_SHUTTER_PRIO:
	  sprintf(buffer, "8%d0104390AFF", Id_cam);
	  break;
        case EVILIB_IRIS_PRIO:
	  sprintf(buffer, "8%d0104390BFF", Id_cam);
	  break;
        case EVILIB_GAIN_PRIO:
	  sprintf(buffer, "8%d0104390CFF", Id_cam);
	  break;
        case EVILIB_BRIGHT:
	  sprintf(buffer, "8%d0104390DFF", Id_cam);
	  break;
        case EVILIB_SHUTTER_AUTO:
	  sprintf(buffer, "8%d0104391AFF", Id_cam);
	  break;
        case EVILIB_IRIS_AUTO:
	  sprintf(buffer, "8%d0104391BFF", Id_cam);
	  break;
        case EVILIB_GAIN_AUTO:
	  sprintf(buffer, "8%d01043901CFF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * When turning on to Bright Mode, Iris, Gain and Shutter at the time then increase or decrease
 * 3 dB/step using UP/DOWN command
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'cmd'
 * cmd: 0 (00, close, 0 dB) to 23 (17, F1.8, 18 dB)
 *       Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Bright(int type, int cmd, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040D00FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040D02FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040D03FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(cmd >= 0 && cmd <= 23)
            {
	      sprintf(buffer, "8%d01044D", Id_cam);
	      make0XString(cmd, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'cmd'
 * cmd: ExpComp Position 0 (-7, -10.5 dB) to 14 (0E, 7 10.5dB)
 *       Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::ExpComp(int type, int cmd, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01043E02FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01043E03FF", Id_cam);
	  break;
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040E00FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040E02FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040E03FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(cmd >= 0 && cmd <= 14)
            {
	      sprintf(buffer, "8%d01044E", Id_cam);
	      make0XString(cmd, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * bright contain the bright value of the camera
 * Return 1 on success, 0 on error
 */
int EVI_D100g::BrightPosInq(int &bright)
{
  bright = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09044DFF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      bright = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * ExpComp containt the ExpComp position
 * Return 1 on success, 0 on error
 */
int EVI_D100g::ExpCompPosInq(int &ExpComp)
{
  ExpComp = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09044EFF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      ExpComp = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}


//-------------------------------------------------------------------------

/*
 * On screen Data Display ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Datascreen(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01060602FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01060603FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01060610FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_unlock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_NTSC
 *                    EVILIB_PAL
 * Return on error: 0
 */
int EVI_D100g::VideoSystemInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090623FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_NTSC;
  if(c == '\x01')
    return EVILIB_PAL;
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D100g::DatascreenInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090606FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * Auto Power Off
 * timer = power off timer parameter 0000 (timer off) to 65535 (FFFF) minutes
 * Initial value: 0000
 * The power automatically turns off if the camera does not receive any VISCA commands
 *  or signals from the Remote Commander for the duration you set in the timer
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::AutoPowerOff(int timer, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      if(timer >= 0 && timer <= 65535)
        {
	  sprintf(buffer, "8%d010440", Id_cam);
	  make0XString(timer, &buffer[strlen(buffer)], 4);
	  strcat(buffer, "FF");
        }
      else
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT
 * gain: R Gain 0 to 255 (FF), 256 steps
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::RGain(int type, int gain, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040300FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040302FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040303FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(gain >= 0 && gain <= 255)
            {
	      sprintf(buffer, "8%d010443", Id_cam);
	      make0XString(gain, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT
 * gain: B Gain 0 to 255 (FF), 256 steps
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::BGain(int type, int gain, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040400FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040402FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040403FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(gain >= 0 && gain <= 255)
            {
	      sprintf(buffer, "8%d010444", Id_cam);
	      make0XString(gain, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_AUTO
 *       DERVERV_MANUAL
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::SlowShutter(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_AUTO:
	  sprintf(buffer, "8%d01045A02FF", Id_cam);
	  break;
        case EVILIB_MANUAL:
	  sprintf(buffer, "8%d01045A03FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT
 * cmd: Aperture Gain 0 to 15 (0F), 16 steps, Initial value: 5
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Aperture(int type, int cmd, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040200FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040202FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040203FF", Id_cam);
	  break;
        case EVILIB_DIRECT:
	  if(cmd >= 0 && cmd <= 15)
            {
	      sprintf(buffer, "8%d010442", Id_cam);
	      make0XString(cmd, &buffer[strlen(buffer)], 4);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Mirror image ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::LR_Reverse(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01046102FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046103FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Still image ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::Freeze(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01046202FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046203FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * Picture effect setting
 * type: EVILIB_OFF
 *       EVILIB_PASTEL
 *       EVILIB_NEGART
 *       EVILIB_SEPIA
 *       EVILIB_BW
 *       EVILIB_SOLARIZE
 *       EVILIB_MOSAIC
 *       EVILIB_SLIM
 *       EVILIB_STRETCH
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100g::PictureEffect(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046300FF", Id_cam);
	  break;
        case EVILIB_PASTEL:
	  sprintf(buffer, "8%d01046301FF", Id_cam);
	  break;
        case EVILIB_NEGART:
	  sprintf(buffer, "8%d01046302FF", Id_cam);
	  break;
        case EVILIB_SEPIA:
	  sprintf(buffer, "8%d01046303FF", Id_cam);
	  break;
        case EVILIB_BW:
	  sprintf(buffer, "8%d01046304FF", Id_cam);
	  break;
        case EVILIB_SOLARIZE:
	  sprintf(buffer, "8%d01046305FF", Id_cam);
	  break;
        case EVILIB_MOSAIC:
	  sprintf(buffer, "8%d01046306FF", Id_cam);
	  break;
        case EVILIB_SLIM:
	  sprintf(buffer, "8%d01046307FF", Id_cam);
	  break;
        case EVILIB_STRETCH:
	  sprintf(buffer, "8%d01046308FF", Id_cam);
	  break;
        default:
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);
// receiv 'ACK'
      pthread_mutex_lock(&TakeAck);
// Wait for Completion
      if(waitC == EVILIB_WAIT_COMP)
        {
	  waitComp = EVILIB_WAIT_COMP;
	  pthread_mutex_lock(&TakeComp);
        }
      if(answer < 1)
	return 0;
      return 1;
    }
  return 0;
}

/*
 * timer: contain the power off timer
 * Return 1 on success, 0 on error
 */
int EVI_D100g::AutoPowerOffInq(int &timer)
{
  timer = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090440FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      timer = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D100g::DZoomModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090406FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * Return on success: EVILIB_AF_SENS_HIGH
 *                    EVILIB_AF_SENS_LOW
 * Return 0 on error
 */
int EVI_D100g::AFModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090458FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_AF_SENS_HIGH;
  if(c == '\x03')
    return EVILIB_AF_SENS_LOW;
  return 0;
}

/*
 * focus contain the Focus limit position
 * Return 1 on success, 0 on error
 */
int EVI_D100g::FocusNearLimitInq(int &focus)
{
  focus = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090428FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      focus = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * gain contain the R Gain
 * Return 1 on success, 0 on error
 */
int EVI_D100g::RGainInq(int &gain)
{
  gain = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090443FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      gain = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * gain contain the B Gain
 * Return 1 on success, 0 on error
 */
int EVI_D100g::BGainInq(int &gain)
{
  gain = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090444FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      gain = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_AUTO
 *                    EVILIB_MANUAL
 * Return 0 on error
 */
int EVI_D100g::SlowShutterModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09045AFF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_AUTO;
  if(c == '\x03')
    return EVILIB_MANUAL;
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D100g::ExpCompModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09043EFF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * aperture contain the Aperture Gain
 * Return 1 on success, 0 on error
 */
int EVI_D100g::ApertureInq(int &aperture)
{
  aperture = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090442FF", Id_cam);
      if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
        {
	  pthread_mutex_unlock(&BufferDispo);
	  return 0;
        }
      pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
      pthread_mutex_lock(&TakeInfo);
      if(answer < 1)
	return 0;
      pthread_mutex_lock(&AccessBuffer3);
      aperture = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D100g::LR_ReverseModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090461FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D100g::FreezeModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090462FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x02')
    return EVILIB_ON;
  if(c == '\x03')
    return EVILIB_OFF;
  return 0;
}

/*
 * Return on success: EVILIB_OFF
 *                    EVILIB_PASTEL
 *                    EVILIB_NEGART
 *                    EVILIB_SEPIA
 *                    EVILIB_BW
 *                    EVILIB_SOLARIZE
 *                    EVILIB_MOSAIC
 *                    EVILIB_SLIM
 *                    EVILIB_STRETCH
 * Return on error: 0
 */
int EVI_D100g::PictureEffectModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090463FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  c = buffer3[2];
  pthread_mutex_unlock(&AccessBuffer3);
  if(c == '\x00')
    return EVILIB_OFF;
  if(c == '\x01')
    return EVILIB_PASTEL;
  if(c == '\x02')
    return EVILIB_NEGART;
  if(c == '\x03')
    return EVILIB_SEPIA;
  if(c == '\x04')
    return EVILIB_BW;
  if(c == '\x05')
    return EVILIB_SOLARIZE;
  if(c == '\x06')
    return EVILIB_MOSAIC;
  if(c == '\x07')
    return EVILIB_SLIM;
  if(c == '\x08')
    return EVILIB_STRETCH;
  return 0;
}

/*
 * vender contain the Vender ID (1: Sony)
 * model contain the Model ID
 * rom contain the ROM version
 * socket contain the Socket numer (=2)
 * Return 1 on success, 0 on error
 */
int EVI_D100g::DeviceTypeInq(int &vender, int &model, int &rom, int &socket)
{
  vender = -1;
  model = -1;
  rom = -1;
  socket = -1;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090002FF", Id_cam);
  if(SendCommand((unsigned char *)buffer, strlen(buffer)) == 0)
    {
      pthread_mutex_unlock(&BufferDispo);
      return 0;
    }
  pthread_mutex_unlock(&BufferDispo);

// receiv 'information return'
  pthread_mutex_lock(&TakeInfo);
  if(answer < 1)
    return 0;
  pthread_mutex_lock(&AccessBuffer3);
  vender = int(deconvert(2, 2));
  model = int(deconvert(4, 2));
  rom = int(deconvert(6, 2));
  socket = int(deconvert(8, 1));
  pthread_mutex_unlock(&AccessBuffer3);
  return 1;
}
