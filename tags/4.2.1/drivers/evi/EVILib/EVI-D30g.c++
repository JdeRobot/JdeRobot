// EVI-D30g.c++
//
// Access functions to a SONY EVI-D30
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

#include "EVI-D30g.h"

//-------------------------------------------------------------------------

/*
 * Constructor
 */
EVI_D30g::EVI_D30g()
{
  ATStatus = NULL;
  MDStatus = NULL;

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
EVI_D30g::~EVI_D30g()
{
  if(ATStatus != NULL)
    {
      delete[] ATStatus;
      ATStatus = NULL;
    }
  if(MDStatus != NULL)
    {
      delete[] MDStatus;
      MDStatus = NULL;
    }
}

/*
 * Initialize the data
 * Return 1 on success
 * Return 0 on error
 */
int EVI_D30g::Init()
{
    ATStatus = new int[11];
    if(ATStatus == NULL)
    {
        cerr<<"Error EVILib Init(): ATStatus\n";
        return 0;
    }
    MDStatus = new int[9];
    if(MDStatus == NULL)
    {
        cerr<<"Error EVILib Init(): MDStatus\n";
        return 0;
    }

    for(int c = 0; c < 11; c ++)
        ATStatus[c] = 0;
    for(int c = 0; c < 9; c ++)
        MDStatus[c] = 0;

    return HiddenInit();
}

/*
 * Automatic Target Trace ability Compensation when a wide conversion lens is installed
 * setting: 0 (no conversion) to 7 (0.6 conversion)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Wide_conLensSet(int setting, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d01072600", Id_cam);
      switch(setting)
        {
        case 0: // Wide converstion lens ratio = 1.0
	  strcat(buffer, "00FF");
	  break;
        case 1: // Wide converstion lens ratio = 0.9
	  strcat(buffer, "01FF");
	  break;
        case 2: // Wide converstion lens ratio = 0.85
	  strcat(buffer, "02FF");
	  break;
        case 3: // Wide converstion lens ratio = 0.8
	  strcat(buffer, "03FF");
	  break;
        case 4: // Wide converstion lens ratio = 0.75
	  strcat(buffer, "04FF");
	  break;
        case 5: // Wide converstion lens ratio = 0.7
	  strcat(buffer, "05FF");
	  break;
        case 6: // Wide converstion lens ratio = 0.65
	  strcat(buffer, "06FF");
	  break;
        case 7: // Wide converstion lens ratio = 0.6
	  strcat(buffer, "07FF");
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
 * Target Tracking Mode ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_Mode(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070102FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070103FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070110FF", Id_cam);
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
 * Auto Exposure for the target
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_AE(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070202FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070203FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070210FF", Id_cam);
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
 * Automatic Zooming for the target
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_AutoZoom(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070302FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070303FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070310FF", Id_cam);
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
 * Sensing Frame Display ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_MD_Frame_Display(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070402FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070403FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070410FF", Id_cam);
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
 * Shifting the Sensing Frame for AR
 * For Shifting use Pan/Tilt Drive Command
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_Offset(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070502FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070503FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070510FF", Id_cam);
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
 * Tracking or Detecting Start/Stop
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_MD_StartStop(int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d01070610FF", Id_cam);
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
 * Select a Tracking Mode
 * type: EVILIB_CHASE1
 *       EVILIB_CHASE2
 *       EVILIB_CHASE3
 *       EVILIB_CHASE123
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_Chase(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_CHASE1:
	  sprintf(buffer, "8%d01070700FF", Id_cam);
	  break;
        case EVILIB_CHASE2:
	  sprintf(buffer, "8%d01070701FF", Id_cam);
	  break;
        case EVILIB_CHASE3:
	  sprintf(buffer, "8%d01070702FF", Id_cam);
	  break;
        case EVILIB_CHASE123:
	  sprintf(buffer, "8%d01070710FF", Id_cam);
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
 * Select target study mode for AT
 * type: EVILIB_ENTRY1
 *       EVILIB_ENTRY2
 *       EVILIB_ENTRY3
 *       EVILIB_ENTRY4
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_Entry(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ENTRY1:
	  sprintf(buffer, "8%d01071500FF", Id_cam);
	  break;
        case EVILIB_ENTRY2:
	  sprintf(buffer, "8%d01071501FF", Id_cam);
	  break;
        case EVILIB_ENTRY3:
	  sprintf(buffer, "8%d01071502FF", Id_cam);
	  break;
        case EVILIB_ENTRY4:
	  sprintf(buffer, "8%d01071503FF", Id_cam);
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
 * Motion Detector Mode ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_Mode(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01070802FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01070803FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01070810FF", Id_cam);
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
 * Detecting Area Set (Size or Position)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_Frame(int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d010709FF", Id_cam);
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
 * Select Detecting Frame (1 or 2 or 1 + 2)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_Detect(int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d01070A10FF", Id_cam);
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
 * Reply a completion when the camera lost the target in AT mode.
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_LostInfo(int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d0106200720FF", Id_cam);
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
 * Reply a completion when the camera detected a motion of image in MD mode.
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_LostInfo(int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d0106200721FF", Id_cam);
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
 * Set Detecting Condition
 * type: EVILIB_Y_LEVEL --> need 'setting'
 *       EVILIB_HUE_LEVEL --> need 'setting'
 *       EVILIB_SIZE --> need 'setting'
 *       EVILIB_DISPLAYTIME --> need 'setting'
 *       EVILIB_REFRESH_MODE1
 *       EVILIB_REFRESH_MODE2
 *       EVILIB_REFRESH_MODE3
 *       EVILIB_REFRESH_TIME --> need 'setting'
 * setting: 0 to 15
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_Adjust(int type, int setting, int waitC)
{
  if(power == EVILIB_ON)
    {
      if(setting >=0 && setting <= 15)
        {
	  pthread_mutex_lock(&BufferDispo);
	  switch(type)
            {
            case EVILIB_Y_LEVEL:
	      sprintf(buffer, "8%d01070B000%XFF", Id_cam, setting);
	      break;
            case EVILIB_HUE_LEVEL:
	      sprintf(buffer, "8%d01070C000%XFF", Id_cam, setting);
	      break;
            case EVILIB_SIZE:
	      sprintf(buffer, "8%d01070D000%XFF", Id_cam, setting);
	      break;
            case EVILIB_DISPLAYTIME:
	      sprintf(buffer, "8%d01070F000%XFF", Id_cam, setting);
	      break;
            case EVILIB_REFRESH_MODE1:
	      sprintf(buffer, "8%d01071000FF", Id_cam);
	      break;
            case EVILIB_REFRESH_MODE2:
	      sprintf(buffer, "8%d01071001FF", Id_cam);
	      break;
            case EVILIB_REFRESH_MODE3:
	      sprintf(buffer, "8%d01071002FF", Id_cam);
	      break;
// Seems to be a problem with the command list of the camera
//  the 'refresh time' command has the same packet as the 'Y level' command ...
// As it seems that the command and inquiry have some part of the packets in common,
//  it's seems logical to use '8x010711000ZFF' as command of the 'refresh time'
// (we have replace the '..0B..' by '..11..')
            case EVILIB_REFRESH_TIME:
	      sprintf(buffer, "8%d010711000%XFF", Id_cam, setting);
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
    }
  return 0;
}

/*
 * Target Condition Measure Mode for More Accurate Setting for Motion Detector
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Measure_Mode1(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01072702FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01072703FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01072710FF", Id_cam);
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
 * Target Condition Measure Mode for More Accurate Setting for Motion Detector
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Measure_Mode2(int type, int waitC)
{
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      switch(type)
        {
        case EVILIB_ON:
	  sprintf(buffer, "8%d01072802FF", Id_cam);
	  break;
        case EVILIB_OFF:
	  sprintf(buffer, "8%d01072803FF", Id_cam);
	  break;
        case EVILIB_ON_OFF:
	  sprintf(buffer, "8%d01072810FF", Id_cam);
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
 * lens: contain the lens No.
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Wide_ConLensInq(int &lens)
{
  lens = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090726FF", Id_cam);
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
      lens = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_NORMAL
 *                    EVILIB_ATMODE
 *                    EVILIB_MDMODE
 * Return on error: 0
 */
int EVI_D30g::ATMD_ModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090722FF", Id_cam);
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
    return EVILIB_ATMODE;
  if(c == '\x02')
    return EVILIB_MDMODE;
  return 0;
}

/*
 * After the completion of ATModeInq, you can check the status with the functions
 *  provided
 * Return 1 on success, 0 on error
 */
int EVI_D30g::ATModeInq()
{
  unsigned char alpha;
  unsigned char beta;
  int low[8];
  int high[8];
  int ct;

  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090723FF", Id_cam);
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
  alpha = buffer3[2];
  beta = buffer3[3];
  pthread_mutex_unlock(&AccessBuffer3);

  bitGenerator(alpha, low);
  bitGenerator(beta, high);

  for(ct = 0; ct < 11; ct ++)
    ATStatus[ct] = 0;

  if(low[0] == 0 && high[0] == 0 && high[6] == 0 && high[7] == 0)
    ATStatus[0] = 1;
  if(low[0] == 0 && high[0] == 0 && high[6] == 0 && high[7] == 1)
    ATStatus[1] = 1;
  if(low[0] == 0 && high[0] == 0 && high[6] == 1 && high[7] == 0)
    ATStatus[2] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 1)
    ATStatus[3] = 1;
  if(low[0] == 0 && high[0] == 0 && high[4] == 1)
    ATStatus[4] = 1;
  if(low[0] == 0 && high[0] == 0 && high[3] == 1)
    ATStatus[5] = 1;
  if(low[0] == 0 && high[0] == 1 && high[2] == 1)
    ATStatus[6] = 1;
  if(low[0] == 0 && low[6] == 0 && low[7] == 0 && high[0] == 0)
    ATStatus[7] = 1;
  if(low[0] == 0 && low[6] == 0 && low[7] == 1 && high[0] == 0)
    ATStatus[8] = 1;
  if(low[0] == 0 && low[6] == 1 && low[7] == 0 && high[0] == 0)
    ATStatus[9] = 1;
  if(low[0] == 0 && low[6] == 1 && low[7] == 1 && high[0] == 0)
    ATStatus[10] = 1;

  return 1;
}

/*
 * Return on success: EVILIB_ENTRY1
 *                    EVILIB_ENTRY2
 *                    EVILIB_ENTRY3
 *                    EVILIB_ENTRY4
 * Return on error: 0
 */
int EVI_D30g::AT_EntryInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090715FF", Id_cam);
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
    return EVILIB_ENTRY1;
  if(c == '\x01')
    return EVILIB_ENTRY2;
  if(c == '\x02')
    return EVILIB_ENTRY3;
  if(c == '\x02')
    return EVILIB_ENTRY4;
  return 0;
}

/*
 * After the completion of MDModeInq, you can check the status with the functions
 *  provided
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MDModeInq()
{
  unsigned char alpha;
  unsigned char beta;
  int low[8];
  int high[8];
  int ct;

  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090724FF", Id_cam);
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
  alpha = buffer3[2];
  beta = buffer3[3];
  pthread_mutex_unlock(&AccessBuffer3);

  bitGenerator(alpha, low);
  bitGenerator(beta, high);

  for(ct = 0; ct < 9; ct ++)
    MDStatus[ct] = 0;

  if(low[0] == 0 && high[0] == 0 && high[5] == 0 && high[6] == 0 && high[7] == 0)
    MDStatus[0] = 1;
  PanTiltStatus[0] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 0 && high[6] == 0 && high[7] == 1)
    MDStatus[1] = 1;
  PanTiltStatus[1] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 0 && high[6] == 1)
    MDStatus[2] = 1;
  PanTiltStatus[2] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 1 && high[6] == 0)
    MDStatus[3] = 1;
  PanTiltStatus[3] = 1;
  if(low[0] == 0 && high[0] == 0 && high[5] == 1 && high[6] == 1)
    MDStatus[4] = 1;
  PanTiltStatus[4] = 1;
  if(low[0] == 0 && high[0] == 0 && high[3] == 0 && high[4] == 1)
    MDStatus[5] = 1;
  PanTiltStatus[5] = 1;
  if(low[0] == 0 && high[0] == 0 && high[3] == 1 && high[4] == 0)
    MDStatus[6] = 1;
  PanTiltStatus[6] = 1;
  if(low[0] == 0 && high[0] == 0 && high[3] == 1 && high[4] == 1)
    MDStatus[7] = 1;
  PanTiltStatus[7] = 1;
  if(low[0] == 0 && high[0] == 0 && high[2] == 1 && high[4] == 0 && high[6] == 1)
    MDStatus[8] = 1;

  return 1;
}

/*
 * Dividing a scene by 48*30 pixels, Return the center position of the detecting Frame.
 * x: 4 to 42
 * y: 3 to 27
 * status: EVILIB_SETTING
 *         EVILIB_TRACKING
 *         EVILIB_LOST
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AT_ObjetPosInq(int &x, int &y, int &status)
{
  int n;
  x = 0;
  y = 0;
  status = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090720FF", Id_cam);
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
      x = int(deconvert(2, 1));
      y = int(deconvert(3, 1));
      n = int(deconvert(4, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      switch(n)
        {
        case 0:
	  status = EVILIB_SETTING;
	  break;
        case 1:
	  status = EVILIB_TRACKING;
	  break;
        case 2:
	  status = EVILIB_LOST;
	  break;
        default:
	  return 0;
        }
      return 1;
    }
  return 0;
}

/*
 * Dividing a sceen by 48*30 pixles, Return the center position of the detecting Frame.
 * x: 4 to 42
 * y: 3 to 27
 * status: EVILIB_SETTING
 *         EVILIB_TRACKING
 *         EVILIB_LOST
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_ObjetPosInq(int &x, int &y, int &status)
{
  int n;
  x = -1;
  y = -1;
  status = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090721FF", Id_cam);
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
      x = int(deconvert(2, 1));
      y = int(deconvert(3, 1));
      n = int(deconvert(4, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      switch(n)
        {
        case 1:
	  status = EVILIB_UNDETECT;
	  break;
        case 2:
	  status = EVILIB_DETECTED;
	  break;
        default:
	  return 0;
        }
      return 1;
    }
  return 0;
}

/*
 * level: 0 to 15
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_YLevelInq(int &level)
{
  level = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09070BFF", Id_cam);
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
      level = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * level: 0 to 15
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_HueLevelInq(int &level)
{
  level = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09070CFF", Id_cam);
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
      level = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * size: 0 to 15
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_SizeInq(int &size)
{
  size = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09070DFF", Id_cam);
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
      size = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * dt: 0 to 15
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_DispTimeInq(int &dt)
{
  dt = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d09070FFF", Id_cam);
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
      dt = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_REFRESH_MODE1
 *                    EVILIB_REFRESH_MODE2
 *                    EVILIB_REFRESH_MODE3
 * Return on error: 0
 */
int EVI_D30g::MD_RefModeInq()
{
  char c;
  pthread_mutex_lock(&BufferDispo);
  sprintf(buffer, "8%d090710FF", Id_cam);
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
    return EVILIB_REFRESH_MODE1;
  if(c == '\x01')
    return EVILIB_REFRESH_MODE2;
  if(c == '\x02')
    return EVILIB_REFRESH_MODE3;
  return 0;
}

/*
 * rt: 0 to 15
 * Return 1 on success, 0 on error
 */
int EVI_D30g::MD_RefTimeInq(int &rt)
{
  rt = -1;
  if(power == EVILIB_ON)
    {
      pthread_mutex_lock(&BufferDispo);
      sprintf(buffer, "8%d090711FF", Id_cam);
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
      rt = int(deconvert(3, 1));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
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
 * speed: speed parameter EVILIB_min_zspeed (Low) to EVILIB_max_zspeed (High)
 * zoom: EVILIB_minzoom (Wide) to EVILIB_maxzoom(Tele)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Zoom(int type, int speed, float zoom, int waitC)
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
	  if(speed >= min_zspeed && speed <= EVILIB_max_zspeed)
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
 *       EVILIB_DIRECT -> focus: infinity = 4096, close = EVILIB_maxFocus
 *       EVILIB_AUTO
 *       EVILIB_MANUAL
 *       EVILIB_AUTO_MANUAL
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Focus(int type, int focus, int waitC)
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
 *       EVILIB_ONEPUSH_TRIGGER
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::WB(int type, int waitC)
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
 *       EVILIB_BRIGHT       : Bright mode (Manual control)
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::AE(int type, int waitC)
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
 * cmd: not used
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Bright(int type, int cmd, int waitC)
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

//-------------------------------------------------------------------------

/*
 * Enable/Disable for RS-232C and key control
 * type: EVILIB_ON
 *       EVILIB_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::KeyLock(int type, int waitC)
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
 * Return on success: EVILIB_ON
 *                    EVILIB_OFF
 * Return on error: 0
 */
int EVI_D30g::KeyLockInq()
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
 * On screen Data Display ON/OFF
 * type: EVILIB_ON
 *       EVILIB_OFF
 *       EVILIB_ON_OFF
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D30g::Datascreen(int type, int waitC)
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
 * id: contain the ID of the camera
 * Return 1 on success, 0 on error
 */
int EVI_D30g::IDInq(int &id)
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
      id = int(deconvert(2, 2));
      pthread_mutex_unlock(&AccessBuffer3);
      return 1;
    }
  return 0;
}

/*
 * Return on success: EVILIB_NTSC
 *                    EVILIB_PAL
 * Return on error: 0
 */
int EVI_D30g::VideoSystemInq()
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
int EVI_D30g::DatascreenInq()
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
