// EVI-D31.h
//
// Access functions to a SONY EVI-D31
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

#ifndef __EVI_D31_CLASS__
#define __EVI_D31_CLASS__

//-------------------------------------------------------------------------
// Here you can change the camera parameters

#define cam_type "EVI-D31"

//-------------------------------------------------------------------------

#include "EVI-D30g.h"

//-------------------------------------------------------------------------

class EVI_D31 : virtual public EVI_D30g
{

//-------------------------------------------------------------------------

 public:
// Constructor
  EVI_D31();
// Destructor
  ~EVI_D31();

//-------------------------------------------------------------------------

// Electronic Shutter Setting
// Enable on AE_Manual, Shutter_Priority
// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT --> need 'speed'
// speed: 1/50 to 1/10000 second
// Authorized values: 50 - 75 - 90 - 100 - 120 - 150 - 180 - 215 - 250 - 300 -
//   350 - 425 - 500 - 600 - 725 - 850 - 1000 - 1250 - 1500 - 1750 - 2000 - 2500 -
//   3000 - 3500 - 4000 - 6000 - 10000
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Shutter(int type, int speed, int waitC);

// shutter: contain the shutter position of the camera
// Return 1 on success, 0 on error
  int ShutterPosInq(int &shutter);

//-------------------------------------------------------------------------

};

//-------------------------------------------------------------------------

#endif // __EVI_D30_CLASS__
