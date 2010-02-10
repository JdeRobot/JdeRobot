// EVI-D100.c++
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

#include "EVI-D100.h"

//-------------------------------------------------------------------------

/*
 * Constructor
 */
EVI_D100::EVI_D100()
{
  strcpy(camera_type, cam_type);
}

/*
 *
 */
EVI_D100::~EVI_D100()
{
}

/*
 * Electronic Shutter Setting
 * Enable on AE_Manual, Shutter_Priority
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'speed'
 * speed: 1/4 to 1/10000 second
 * Authorized values: 4 - 8 - 15 - 30 - 60 - 90 - 100 - 125 - 180 - 250 - 350 - 500 -
 *   725 - 1000 - 1500 - 2000 - 3000 - 4000 - 6000 - 10000
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100::Shutter(int type, int speed, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_RESET:
	  sprintf(buffer, "8%d01040A00FF", Id_cam);
	  break;
        case EVILIB_UP:
	  sprintf(buffer, "8%d01040A02FF", Id_cam);
	  break;
        case EVILIB_DOWN:
	  sprintf(buffer, "8%d01040A03FF", Id_cam);
	  break;
// How to compute the speed automatically ?
        case EVILIB_DIRECT:
	  sprintf(buffer, "8%d01044A0000", Id_cam);
	  switch(speed)
            {
            case 4:
	      strcat(buffer, "0000FF");
	      break;
            case 8:
	      strcat(buffer, "0001FF");
	      break;
            case 15:
	      strcat(buffer, "0002FF");
	      break;
            case 30:
	      strcat(buffer, "0003FF");
	      break;
            case 60:
	      strcat(buffer, "0004FF");
	      break;
            case 90:
	      strcat(buffer, "0005FF");
	      break;
            case 100:
	      strcat(buffer, "0006FF");
	      break;
            case 125:
	      strcat(buffer, "0007FF");
	      break;
            case 180:
	      strcat(buffer, "0008FF");
	      break;
            case 250:
	      strcat(buffer, "0009FF");
	      break;
            case 350:
	      strcat(buffer, "000AFF");
	      break;
            case 500:
	      strcat(buffer, "000BFF");
	      break;
            case 725:
	      strcat(buffer, "000CFF");
	      break;
            case 1000:
	      strcat(buffer, "000DFF");
	      break;
            case 1500:
	      strcat(buffer, "000EFF");
	      break;
            case 2000:
	      strcat(buffer, "000FFF");
	      break;
            case 3000:
	      strcat(buffer, "0100FF");
	      break;
            case 4000:
	      strcat(buffer, "0101FF");
	      break;
            case 6000:
	      strcat(buffer, "0102FF");
	      break;
            case 10000:
	      strcat(buffer, "0103FF");
	      break;
            default:
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
 * shutter: contain the shutter position of the camera
 * Return 1 on success, 0 on error
 */
int EVI_D100::ShutterPosInq(int &shutter)
{
  shutter = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09044AFF", Id_cam);
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
      int s = int(deconvert(2, 4));
      pthread_mutex_unlock(&AccessBuffer3);
      switch(s)
        {
        case 0:
	  shutter = 4;
	  break;
        case 1:
	  shutter = 8;
	  break;
        case 2:
	  shutter = 15;
	  break;
        case 3:
	  shutter = 30;
	  break;
        case 4:
	  shutter = 60;
	  break;
        case 5:
	  shutter = 90;
	  break;
        case 6:
	  shutter = 100;
	  break;
        case 7:
	  shutter = 125;
	  break;
        case 8:
	  shutter = 180;
	  break;
        case 9:
	  shutter = 250;
	  break;
        case 10:
	  shutter = 350;
	  break;
        case 11:
	  shutter = 500;
	  break;
        case 12:
	  shutter = 725;
	  break;
        case 13:
	  shutter = 1000;
	  break;
        case 14:
	  shutter = 1500;
	  break;
        case 15:
	  shutter = 2000;
	  break;
        case 16:
	  shutter = 3000;
	  break;
        case 17:
	  shutter = 4000;
	  break;
        case 18:
	  shutter = 6000;
	  break;
        case 19:
	  shutter = 10000;
	  break;
        default:
	  return 0;
        }
      return 1;
    }
  return 0;
}
