// EVI-D100P.c++
//
// Access functions to a SONY EVI-D100P
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

#include "EVI-D100P.h"

//-------------------------------------------------------------------------

/*
 * Constructor
 */
EVI_D100P::EVI_D100P()
{
  strcpy(camera_type, cam_type);
}

/*
 *
 */
EVI_D100P::~EVI_D100P()
{
}

/*
 * Electronic Shutter Setting
 * Enable on AE_Manual, Shutter_Priority
 * type: EVILIB_RESET
 *       EVILIB_UP
 *       EVILIB_DOWN
 *       EVILIB_DIRECT --> need 'speed'
 * speed: 1/3 to 1/10000 second
 * Authorized values: 3 - 6 - 12 - 25 - 50 - 75 - 100 - 120 - 150 - 215 - 300 - 425 -
 *   600 - 1000 - 1250 - 1750 - 2500 - 3500 - 6000 - 10000
 * waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
 *                                        EVILIB_WAIT_COMP
 * Return 1 on success, 0 on error
 */
int EVI_D100P::Shutter(int type, int speed, int waitC)
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
            case 3:
                strcat(buffer, "0000FF");
                break;
            case 6:
                strcat(buffer, "0001FF");
                break;
            case 12:
                strcat(buffer, "0002FF");
                break;
            case 25:
                strcat(buffer, "0003FF");
                break;
            case 50:
                strcat(buffer, "0004FF");
                break;
            case 75:
                strcat(buffer, "0005FF");
                break;
            case 100:
                strcat(buffer, "0006FF");
                break;
            case 120:
                strcat(buffer, "0007FF");
                break;
            case 150:
                strcat(buffer, "0008FF");
                break;
            case 215:
                strcat(buffer, "0009FF");
                break;
            case 300:
                strcat(buffer, "000AFF");
                break;
            case 425:
                strcat(buffer, "000BFF");
                break;
            case 600:
                strcat(buffer, "000CFF");
                break;
            case 1000:
                strcat(buffer, "000DFF");
                break;
            case 1250:
                strcat(buffer, "000EFF");
                break;
            case 1750:
                strcat(buffer, "000FFF");
                break;
            case 2500:
                strcat(buffer, "0100FF");
                break;
            case 3500:
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
int EVI_D100P::ShutterPosInq(int &shutter)
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
            shutter = 3;
            break;
        case 1:
            shutter = 6;
            break;
        case 2:
            shutter = 12;
            break;
        case 3:
            shutter = 25;
            break;
        case 4:
            shutter = 50;
            break;
        case 5:
            shutter = 75;
            break;
        case 6:
            shutter = 100;
            break;
        case 7:
            shutter = 120;
            break;
        case 8:
            shutter = 150;
            break;
        case 9:
            shutter = 215;
            break;
        case 10:
            shutter = 300;
            break;
        case 11:
            shutter = 425;
            break;
        case 12:
            shutter = 600;
            break;
        case 13:
            shutter = 1000;
            break;
        case 14:
            shutter = 1250;
            break;
        case 15:
            shutter = 1750;
            break;
        case 16:
            shutter = 2500;
            break;
        case 17:
            shutter = 3500;
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
