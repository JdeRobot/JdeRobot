// EVI-D70P.h
//
// Access functions to a SONY EVI-D70P
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

#ifndef __EVI_D70P_CLASS__
#define __EVI_D70P_CLASS__

//-------------------------------------------------------------------------

#define cam_type "EVI-D70P"
#define EVILIB_minzoom         0 //
#define EVILIB_maxzoomS       18 // lens x18 optical zoom
#define EVILIB_maxzoomC       30 // lens x18 + x12 optical + digital zoom in Combined mode
#define EVILIB_maxZoomS    16384 // 0 -> 16384 (4000) = optical zoom in Separated mode
#define EVILIB_maxZoomC    30656 // 0 -> 16384 (4000) = optical zoom | 16385 -> 31424 (7AC0) = digital zoom in Combined mode
#define EVILIB_mindzoom        0 // in Separated mode
#define EVILIB_maxdzoom       12 // x12 digital zoom
#define EVILIB_maxDZoom      235 // 0 -> 235 (EB) digital zoom in Separated mode
#define EVILIB_min_zspeed      0 // min zoom speed
#define EVILIB_max_zspeed      7 // max zoom speed

//-------------------------------------------------------------------------

#include "EVI-D70g.h"

//-------------------------------------------------------------------------

class EVI_D70P : virtual public EVI_D70g
{
 public:
// Constructor
  EVI_D70P();
// Destructor
  ~EVI_D70P();

// Zoom control.
// type: EVILIB_STOP
//       EVILIB_TELE_S (Standard)
//       EVILIB_WIDE_S (Standard)
//       EVILIB_TELE_V (Variable) --> need 'speed'
//       EVILIB_WIDE_V (Variable) --> need 'speed'
//       EVILIB_DIRECT --> need 'zoom'
// speed: speed parameter EVILIB_min_zspeed (Low) to EVILIB_max_zspeed (High)
// zoom: EVILIB_minzoom (Wide) to EVILIB_maxzoom (Tele)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Zoom(int type, int speed, float zoom, int waitC);

// zoom: contain the zoom position of the camera
// Return 1 on success, 0 on error
  int ZoomPosInq(float &zoom);

// Digital Zoom
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_COMBINE_MODE  : Optical/Digital Zoom Combined
//       EVILIB_SEPARATE_MODE : Optical/Digital Zoom Separate
//       EVILIB_STOP
//       EVILIB_TELE_V (variable) --> need 'speed'
//       EVILIB_WIDE_V (variable) --> need 'speed'
//       EVILIB_x1_MAX        : x1/MAX Magnification Switchover
//       EVILIB_DIRECT --> need 'zoom'
// speed: speed parameter EVILIB_min_dzspeed to EVILIB_max_dzspeed
// zoom: EVILIB_mindzoom to EVILIB_maxdzoom // you can NOT use DZoom direct in combined mode
// the 'zoom' value as different range acording to the Combined/Separated mode. Be carefull !!
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int DZoom(int type, int speed, float zoom, int waitC);

// ZoomFocus Direct
// zoom: EVILIB_minzoom (Wide) to EVILIB_maxzoom (Tele)
// focus: infinity = EVILIB_minFocus, close = EVILIB_maxFocus
// the 'zoom' value as different range acording to the Combined/Separated mode. Be carefull !!
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int ZoomFocus(float zoom, int focus, int waitC);

// zoom contain the D-Zoom position
// Return 1 on success, 0 on error
  int DZoomPosInq(float &zoom);

// Electronic Shutter Setting
// Enable on AE_Manual, Shutter_Priority
// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT --> need 'speed'
// speed: 1/1 to 1/10000 second
// Authorized values: 1 - 2 - 3 - 6 - 12 - 25 -50 -75 - 100 -
//  120 - 150 - 215 - 300 - 425 - 600 - 1000 - 1250 - 1750 -
//  2500 - 3500 - 6000 - 10000
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Shutter(int type, int speed, int waitC);

// shutter: contain the shutter position of the camera
// Return 1 on success, 0 on error
  int ShutterPosInq(int &shutter);
};

//-------------------------------------------------------------------------

#endif // __EVI_D70P_CLASS__
