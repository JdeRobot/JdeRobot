// EVI-D70g.c++
//
// Access functions to a SONY EVI-D70
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

#include "EVI-D70g.h"

//-------------------------------------------------------------------------

/*
 * Constructor
 */
EVI_D70g::EVI_D70g()
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
  maxTilt = EVILIB_maxTiltOFF;
  min_tspeed = EVILIB_min_tspeed;
  max_tspeed = EVILIB_max_tspeed;
  min_iris = EVILIB_minIris;
  max_iris = EVILIB_maxIris;
}

/*
 *
 */
EVI_D70g::~EVI_D70g()
{
}

//-------------------------------------------------------------------------

/*
 * Night Power Off
 * timer = power off timer parameter 0000 (timer off) to 65535 (FFFF) minutes
 * A setting of 0 (zero min.) is equivalent to OFF, and the smallest value that
 *  can be set is 1 min.
 * When the Day/Night function is in effect, abd the Night setting as been made,
 *  if an operation is not attempted via either a VISCA command or the remote
 *  comtroller, the unit will continue to operate for the time set in the timer,
 *  and will then shut off automatically.
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::NightPowerOff(int timer, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      if(timer >= 0 && timer <= 65535)
        {
	  sprintf(buffer, "8%d010441", Id_cam);
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
 * AF Sensitivity
 * type: EVILIB_NORMAL 
 *       EVILIB_LOW
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AF_Sensitivity(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_NORMAL:
	  sprintf(buffer, "8%d01045802FF", Id_cam);
	  break;
        case EVILIB_LOW:
	  sprintf(buffer, "8%d01045803FF", Id_cam);
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
 * AF Mode
 * type: EVILIB_NORMAL
 *       EVILIB_INTERVAL
 *       EVILIB_ZOOM_TRIGGER
 *       EVILIB_AI_TIME --> need 'movementTime' & 'interval'
 * movementTime: EVILIB_minMovementTime to EVILIB_maxMovementTime
 * interval: EVILIB_minInterval to EVILIB_maxInterval
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * return 1 on success, 0 on error
 */
int EVI_D70g::AFMode(int type, int movementTime, int interval, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_NORMAL:
	  sprintf(buffer, "8%d01045700FF", Id_cam);
	  break;
        case EVILIB_INTERVAL:
	  sprintf(buffer, "8%d01045701FF", Id_cam);
	  break;
        case EVILIB_ZOOM_TRIGGER:
	  sprintf(buffer, "8%d01045702FF", Id_cam);
	  break;
        case EVILIB_AI_TIME:
	  if(movementTime >= EVILIB_minMovementTime && movementTime <= EVILIB_maxMovementTime &&
	     interval >= EVILIB_minInterval && interval <= EVILIB_maxInterval)
            {
	      sprintf(buffer, "8%d010427", Id_cam);
	      make0XString(movementTime, &buffer[strlen(buffer)], 2);
	      make0XString(interval, &buffer[strlen(buffer)], 2);
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
 * Initialize
 * type: EVILIB_LENS      : Lens Initialization Start
 *       EVILIB_COMP_SCAN : Correction of CCD pixel blemishes
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Initialize(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_LENS:
	  sprintf(buffer, "8%d01041901FF", Id_cam);
	  break;
        case EVILIB_COMP_SCAN:
	  sprintf(buffer, "8%d01041902FF", Id_cam);
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
 * Spot Automatic Exposure setting
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_POSITION -> need 'x' & 'y'
 * x: 0 to 15 (F)
 * y: 0 to 15 (F)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::SpotAE(int type, int x, int y, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01045902FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01045903FF", Id_cam);
	  break;
        case EVILIB_POSITION:
	  if(x >= 0 && x <= 15 &&
	     y >= 0 && y <= 15)
            {
	      sprintf(buffer, "8%d010429", Id_cam);
	      make0XString(x, &buffer[strlen(buffer)], 2);
	      make0XString(y, &buffer[strlen(buffer)], 2);
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
 * Infrared mode (some models do not support Infrared Mode)
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::ICR(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01040102FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01040103FF", Id_cam);
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
 * Auto Infrared mode (some models do not support Infrared Mode)
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AutoICR(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01045102FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01045103FF", Id_cam);
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
 * Display
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Display(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01041502FF", Id_cam);
//	  sprintf(buffer, "8%d01060602FF", Id_cam); // seems there is 2 code possible...
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01041503FF", Id_cam);
//	  sprintf(buffer, "8%d01060603FF", Id_cam); // seems there is 2 code possible...
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01041510FF", Id_cam);
//	  sprintf(buffer, "8%d01060610FF", Id_cam); // seems there is 2 code possible...
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
 * Title
 * type: EVILIB_TITLE_SET1 -> need 'data[0]' Vposition, 'data[1]' Hposition, 'data[2]' Color, 'data[3]' Blink
 *       EVILIB_TITLE_SET2 -> need 'data' contain 1st to 10th characters to display
 *       EVILIB_TITLE_SET3 -> need 'data' contain 11st to 20th characters to display
 *       EVILIB_TITLE_CLEAR
 *       EVILIB_ON
 *       EVILIB_OFF
 *   Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Title(int type, int data[10], int waitC)
{
  int x, y;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_TITLE_SET1:
	  if(data[0] >= 0 && data[0] <= 10 && // 00 to 0A Vposition
	     data[1] >= 0 && data[1] <= 23 && // 00 to 17 Hposition
	     data[2] >= 0 && data[2] <= 6 &&  // 00 to 01 Blink
	     data[3] >= 0 && data[3] <= 1)    // 00 to 06 Color
            {
	      sprintf(buffer, "8%d01047300%02X%02X%02X%02X000000000000FF", Id_cam, data[0], data[1], data[2], data[3]);
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_TITLE_SET2:
	  y = 0;
	  for(x = 0; x < 10; x ++)
	    if(data[x] >= 0 && data[x] <= 79) // character 00 to 4F
	      y++;
	  if(y == 10)
            {
	      sprintf(buffer, "8%d01047301%02X%02X%02X%02X%02X%02X%02X%02X%02X%02XFF", Id_cam, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_TITLE_SET3:
	  y = 0;
	  for(x = 0; x < 10; x ++)
	    if(data[x] >= 0 && data[x] <= 79) // character 00 to 4F
	      y++;
	  if(y == 10)
            {
	      sprintf(buffer, "8%d01047302%02X%02X%02X%02X%02X%02X%02X%02X%02X%02XFF", Id_cam, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_TITLE_CLEAR:
	  sprintf(buffer, "8%d01047400FF", Id_cam);
	  break;
        case EVILIB_ON:
	  sprintf(buffer, "8%d01047402FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01047403FF", Id_cam);
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
 * Mute
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Mute(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01047502FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01047503FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01047510FF", Id_cam);
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
 * Camera ID
 * id: 0 (0000) to 65535 (FFFF)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::IDWrite(int id, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      if(id >= 0 && id <= 65535)
	{
	  sprintf(buffer, "8%d010422", Id_cam);
	  make0XString(id, &buffer[strlen(buffer)], 4);
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
 * Camera Alarm
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_SET_MODE --> data[0]: 0 to 13 (0C)
 *       EVILIB_SET_DAY_NIGHT_LEVEL --> data[0]: 0 to 4095 Day setting AE level (no maximum value given in documentation, we assumme FFF maximum value)
 *                                      data[1]: 0 to 4095 Night setting AE level (no maximum value given in documentation, we assumme FFF maximum value)
 *  also, this can be called SetTime. The guy who made the technical doc should be more carefull...
 *   Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Alarm(int type, int data[2], int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01046B02FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046B03FF", Id_cam);
	  break;
        case EVILIB_SET_MODE:
	  if(data[0] >= 0 && data[0] <= 13)
            {
	      sprintf(buffer, "8%d01046C%02XFF", Id_cam, data[0]);
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
	  break;
        case EVILIB_SET_DAY_NIGHT_LEVEL:
	  if(data[0] >= 0 && data[0] <= 4095 &&
	     data[1] >= 0 && data[1] <= 4095)
            {
	      sprintf(buffer, "8%d01046D", Id_cam);
	      make0XString(data[0], &buffer[strlen(buffer)], 3);
	      make0XString(data[1], &buffer[strlen(buffer)], 3);
	      strcat(buffer, "FF");
            }
	  else
            {
	      pthread_mutex_unlock(&BufferDispo);
	      return 0;
            }
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
 * timer contain the power off timer
 * Return 1 on success, 0 on error
 */
int EVI_D70g::NightPowerOffInq(int &timer)
{
  timer = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090441FF", Id_cam);
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
 * Return on success: EVILIB_COMBINE_MODE
 *                    EVILIB_SEPARATE_MODE
 * Return on error: 0
 */
int EVI_D70g::DZoomCSModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090436FF", Id_cam);
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
    {
      combined = EVILIB_COMBINE_MODE;
      return EVILIB_COMBINE_MODE;
    }
  if(c == '\x01')
    {
      combined = EVILIB_SEPARATE_MODE;
      return EVILIB_SEPARATE_MODE;
    }
  return 0;
}

/*
 * Return on success: EVILIB_NORMAL
 *                    EVILIB_LOW
 * Return on error: 0
 */
int EVI_D70g::AF_SensitivityInq()
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
  if(c == '\x00')
    return EVILIB_NORMAL;
  if(c == '\x01')
    return EVILIB_LOW;
  return 0;
}

/*
 * movementTime contain the Movement Time
 * interval contain the interval
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AFTimeSettingInq(int &movementTime, int &interval)
{
  movementTime = -1;
  interval = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090427FF", Id_cam);
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
      movementTime = int(deconvert(2, 2));
      interval = int(deconvert(4, 2));
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
int EVI_D70g::SpotAEModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090459FF", Id_cam);
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
 * x contain the X position
 * y contain the Y position
 * Return 1 on success, 0 on error
 */
int EVI_D70g::SpotAEPosInq(int x, int y)
{
  x = 0;
  y = 0;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090429FF", Id_cam);
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
      x = int(deconvert(2, 2));
      y = int(deconvert(4, 2));
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
int EVI_D70g::PictureFlipModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090466FF", Id_cam);
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
int EVI_D70g::ICRModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090401FF", Id_cam);
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
int EVI_D70g::AutoICRModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090451FF", Id_cam);
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
int EVI_D70g::DisplayModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090415FF", Id_cam);
//  sprintf(buffer, "8%d090606FF", Id_cam); // seems there is 2 code possible...
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
int EVI_D70g::TitleDisplayModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090474FF", Id_cam);
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
int EVI_D70g::MuteModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090475FF", Id_cam);
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
 * model contain the Model ID
 * rom contain the ROM version
 * socket contain the Socket numer (=2)
 * Return 1 on success, 0 on error
 */
int EVI_D70g::DeviceTypeInq(int &model, int &rom, int &socket)
{
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
  model = int(deconvert(2, 2));
  rom = int(deconvert(4, 2));
  socket = int(deconvert(6, 1));
  pthread_mutex_unlock(&AccessBuffer3);
  return 1;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D70g::AlarmInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09046BFF", Id_cam);
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
 * mode contain the alarm mode
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AlarmModeInq(int &mode)
{
  mode = -1;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09046CFF", Id_cam);
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
  mode = int(deconvert(2, 1));
  pthread_mutex_unlock(&AccessBuffer3);
  return 1;
}

/*
 * day contain the Day setting AE level
 * night contain the Night setting AE level
 * level contain the current AE level
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AlarmDayNightLevelInq(int &day, int &night, int &level)
{
  day = -1;
  night = -1;
  level = -1;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09046DFF", Id_cam);
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
  day = int(deconvert(2, 3));
  night = int(deconvert(5, 3));
  level = int(deconvert(8, 3));
  pthread_mutex_unlock(&AccessBuffer3);
  return 1;
}

/*
 * Return on success: EVILIB_HIGH
 *                    EVILIB_LOW
 * Return on error: 0
 */
int EVI_D70g::AlarmDetectLevelInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d09046EFF", Id_cam);
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
  if(c == '\x01')
    return EVILIB_HIGH;
  if(c == '\x00')
    return EVILIB_LOW;
  return 0;
}

//-------------------------------------------------------------------------

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
int EVI_D70g::AutoPowerOff(int timer, int waitC)
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
 * gain: R Gain 0000 to 255 (FF), 256 steps
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::RGain(int type, int gain, int waitC)
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
 * gain: B Gain 0000 to 255 (FF), 256 steps
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::BGain(int type, int gain, int waitC)
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
int EVI_D70g::SlowShutter(int type, int waitC)
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
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT
 * cmd: ExpComp Position 0 (-7, -10.5 dB) to 14 (0E, 7 10.5dB)
 *       Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::ExpComp(int type, int cmd, int waitC)
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
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT
 * cmd: Aperture Gain 0 to 15 (F), 16 steps
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Aperture(int type, int cmd, int waitC)
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
int EVI_D70g::LR_Reverse(int type, int waitC)
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
int EVI_D70g::Freeze(int type, int waitC)
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
 *       EVILIB_NEGART
 *       EVILIB_BW
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::PictureEffect(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01046300FF", Id_cam);
	  break;
        case EVILIB_NEGART:
	  sprintf(buffer, "8%d01046302FF", Id_cam);
	  break;
        case EVILIB_BW:
	  sprintf(buffer, "8%d01046304FF", Id_cam);
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
 * Enable/Disable for RS-232C and key control
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::KeyLock(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01041700FF", Id_cam);
	  break;
        case EVILIB_ON:
	  sprintf(buffer, "8%d01041702FF", Id_cam);
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
 * timer contain the power off timer
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AutoPowerOffInq(int &timer)
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
int EVI_D70g::DZoomModeInq()
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
 * focus contain the Focus limit position
 * Return 1 on success, 0 on error
 */
int EVI_D70g::FocusNearLimitInq(int &focus)
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
 * Return on success: EVILIB_NORMAL
 *                    EVILIB_INTERVAL
 *                    EVILIB_ZOOM_TRIGGER
 * Return 0 on error
 */
int EVI_D70g::AFModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090457FF", Id_cam);
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
    return EVILIB_NORMAL;
  if(c == '\x01')
    return EVILIB_INTERVAL;
  if(c == '\x02')
    return EVILIB_ZOOM_TRIGGER;
  return 0;
}

/*
 * gain contain the R Gain
 * Return 1 on success, 0 on error
 */
int EVI_D70g::RGainInq(int &gain)
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
int EVI_D70g::BGainInq(int &gain)
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
int EVI_D70g::SlowShutterModeInq()
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
 * bright contain the bright value of the camera
 * Return 1 on success, 0 on error
 */
int EVI_D70g::BrightPosInq(int &bright)
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
 * Return on success: EVILIB_ON
 *                   EVILIB_OFF
 * Return on error: 0
 */
int EVI_D70g::ExpCompModeInq()
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
 * ExpComp containt the ExpComp position
 * Return 1 on success, 0 on error
 */
int EVI_D70g::ExpCompPosInq(int &ExpComp)
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

/*
 * aperture contain the Aperture Gain
 * Return 1 on success, 0 on error
 */
int EVI_D70g::ApertureInq(int &aperture)
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
int EVI_D70g::LR_ReverseModeInq()
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
int EVI_D70g::FreezeModeInq()
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

/* Return on success: EVILIB_OFF
 *                    EVILIB_NEGART
 *                    EVILIB_BW
 * Return on error: 0
 */
int EVI_D70g::PictureEffectModeInq()
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
  if(c == '\x02')
    return EVILIB_NEGART;
  if(c == '\x04')
    return EVILIB_BW;
  return 0;
}

/*
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D70g::KeyLockInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090417FF", Id_cam);
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
  if(c == '\x02')
    return EVILIB_ON;
  return 0;
}

/*
 * id: contain the ID of the camera
 * Return 1 on success, 0 on error
 */
int EVI_D70g::IDInq(int &id)
{
  id = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090422FF", Id_cam);
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
      id = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

//-------------------------------------------------------------------------

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
 *       EVILIB_NEAR_LIMIT -> focus: focus near limit position EVILIB_min_Focus (far) to EVILIB_max_Focus (near)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Focus(int type, int focus, int waitC)
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
 * type: EVILIB_AUTO            : Normal Auto
 *       EVILIB_INDOOR          : Indoor mode
 *       EVILIB_OUTDOOR         : Outdoor mode
 *       EVILIB_ONEPUSH_MODE    : One Push WB mode
 *       EVILIB_ATW             : Auto Tracing White Balance
 *       EVILIB_MANUAL          : Manual Control mode
 *       EVILIB_ONEPUSH_TRIGGER : One Push WB Trigger
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::WB(int type, int waitC)
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
 * type: EVILIB_AUTO         : Auto Exposure Mode
 *       EVILIB_MANUAL       : Manual control mode
 *       EVILIB_SHUTTER_PRIO : Shutter priority automatic exposure mode
 *       EVILIB_IRIS_PRIO    : Iris priority automatic exposure mode
 *       EVILIB_BRIGHT       : Bright mode (Manual control)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::AE(int type, int waitC)
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
        case EVILIB_BRIGHT:
	  sprintf(buffer, "8%d0104390DFF", Id_cam);
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
 *       EVILIB_DIRECT
 * cmd: 0 (00, close, 0 dB) to 31 (1F, F1.4, 28 dB)
 *       Check camera documentation for values
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D70g::Bright(int type, int cmd, int waitC)
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
	  if(cmd >= 0 && cmd <= 31)
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
