// EVI-D100g.h
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

#ifndef __EVI_D100g_CLASS__
#define __EVI_D100g_CLASS__

//-------------------------------------------------------------------------

#define EVILIB_minFocus     4096 // 1000 (infinity)
#define EVILIB_maxFocus    33792 // 8400 (near)
#define EVILIB_min_FocusSpeed  0 // min focus speed
#define EVILIB_max_FocusSpeed  7 // max focus speed

//-------------------------------------------------------------------------
// Here you can change the camera parameters

#define EVILIB_minGain     0 // 00 -3dB
#define EVILIB_maxGain     7 // 07 18dB
#define EVILIB_minpan   -100 // -164 degrees
#define EVILIB_maxpan    100 // +164 degrees
#define EVILIB_maxPan   1440 // 05A0
#define EVILIB_min_pspeed  1 // 01 min pan speed
#define EVILIB_max_pspeed 24 // 18 max pan speed
#define EVILIB_mintilt   -25 // -30 degrees
#define EVILIB_maxtilt    25 // +30 degrees
#define EVILIB_maxTilt   360 // 0168
#define EVILIB_min_tspeed  1 // 01 min tilt speed
#define EVILIB_max_tspeed 20 // 14 max tilt speed
#define EVILIB_minzoom     0 //
#define EVILIB_maxzoom    40 // lens x40 digital zoom
#define EVILIB_maxZoom 28672 // 0 -> 16384 (4000) = optical zoom | 16385 -> 28672 (7000) = digital zoom
#define EVILIB_min_zspeed  0 // 01 min zoom speed
#define EVILIB_max_zspeed  7 // 07 max zoom speed
#define EVILIB_minIris     0 // 00 (CLOSE)
#define EVILIB_maxIris    17 // 11 (F1.8)

//-------------------------------------------------------------------------

#include "EVILib.h"

//-------------------------------------------------------------------------

class EVI_D100g : virtual public EVILib
{

//-------------------------------------------------------------------------
 public:
// Constructor
  EVI_D100g();
// Destructor
  ~EVI_D100g();

// Wide mode setting
// type: EVILIB_OFF
//       EVILIB_CINEMA
//       EVILIB_16_9_FULL
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Wide(int type, int waitC);

// Digital effect setting
// type: EVILIB_OFF
//       EVILIB_STILL
//       EVILIB_FLASH
//       EVILIB_LUMI
//       EVILIB_TRAIL
//       EVILIB_EFFECTLEVEL -> need 'effect'
// effect: effect level 0 (00) to 24 (18) (flash, trail), 0 (00) to 32 (20) (still, lumi)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int DigitalEffect(int type, int level, int waitC);

// Return on success: EVILIB_OFF
//                    EVILIB_STILL
//                    EVILIB_FLASH
//                    EVILIB_LUMI
//                    EVILIB_TRAIL
// Return on error: 0
  int DigitalEffectModeInq();

// level contain the Effect level
// Return 1 on success, 0 on error
  int DigitalEffectLevelInq(int &level);

// Return on success: EVILIB_OFF
//                    EVILIB_CINEMA
//                    EVILIB_16_9_FULL
// Return on error: 0
    int WideModeInq();

// vender contain the Vender ID (1: Sony)
// model contain the Model ID
// rom contain the ROM version
// socket contain the Socket numer (=2)
// Return 1 on success, 0 on error
  int DeviceTypeInq(int &vender, int &model, int &rom, int &socket);

//-------------------------------------------------------------------------

// Zoom control.
// type: EVILIB_STOP
//       EVILIB_TELE_S (Standard)
//       EVILIB_WIDE_S (Standard)
//       EVILIB_TELE_V (Variable) --> need 'speed'
//       EVILIB_WIDE_V (Variable) --> need 'speed'
//       EVILIB_DIRECT --> need 'zoom'
//       EVILIB_DZOOM_ON
//       EVILIB_DZOOM_OFF
// speed: speed parameter EVILIB_min_zspeed (Low) to EVILIB_max_zspeed (High)
// zoom: EVILIB_minzoom (Wide) to EVILIB_maxzoom (Tele)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Zoom(int type, int speed, float zoom, int waitC);

// Focus control.
// When adjust the focus, change the mode to Manual the send Far/Near or Direct command.
// type: EVILIB_STOP
//       EVILIB_FAR
//       EVILIB_NEAR
//       EVILIB_FAR_V  -> focus: speed parameter, EVILIB_min_FocusSpeed (low) to EVILIB_max_FocusSpeed (high)
//       EVILIB_NEAR_V -> focus: speed parameter, EVILIB_min_FocusSpeed (low) to EVILIB_max_FocusSpeed (high)
//       EVILIB_DIRECT -> focus: infinity = EVILIB_minFocus, close = EVILIB_maxFocus
//       EVILIB_AUTO
//       EVILIB_MANUAL
//       EVILIB_AUTO_MANUAL
//       EVILIB_ONEPUSH_TRIGGER
//       EVILIB_INFINITY
//       EVILIB_AF_SENS_HIGH
//       EVILIB_AF_SENS_LOW
//       EVILIB_NEAR_LIMIT -> focus: focus near limit position EVILIB_min_Focus (far) to EVILIB_max_Focus (near)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Focus(int type, int focus, int waitC);

// White Balance Setting.
// type: EVILIB_AUTO         : Trace the light source automatically
//       EVILIB_INDOOR       : fixed at factory
//       EVILIB_OUTDOOR      : fixed at factory
//       EVILIB_ONEPUSH_MODE : Pull-in to White with a Trigger then hold the data until next Trigger comming
//       EVILIB_ATW          : Auto tracing white balance
//       EVILIB_MANUAL       : Manual control mode
//       EVILIB_ONEPUSH_TRIGGER
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int WB(int type, int waitC);

// type: EVILIB_AUTO         : Auto Exposure Mode
//       EVILIB_MANUAL       : Manual control mode
//       EVILIB_SHUTTER_PRIO : Shutter priority automatic exposure mode
//       EVILIB_IRIS_PRIO    : Iris priority automatic exposure mode
//       EVILIB_GAIN_PRIO    : Gain priority automatic Exposure Mode
//       EVILIB_BRIGHT       : Bright mode (Manual control)
//       EVILIB_SHUTTER_AUTO : Automatic shutter mode
//       EVILIB_IRIS_AUTO    : Automatic iris mode
//       EVILIB_GAIN_AUTO    : Automatic gain mode
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AE(int type, int waitC);

// When turning on to Bright Mode, Iris, Gain and Shutter at the time then increase or decrease
// 3 dB/step using UP/DOWN command
// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT
// cmd: 0 (00, close, 0 dB) to 23 (17, F1.8, 18 dB)
//       Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Bright(int type, int cmd, int waitC);

//-------------------------------------------------------------------------

// On screen Data Display ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Datascreen(int type, int waitC);

// Return on success: EVILIB_NTSC
//                    EVILIB_PAL
// Return on error: 0
  int VideoSystemInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int DatascreenInq();

// Auto Power Off
// timer = power off timer parameter 0000 (timer off) to 65535 (FFFF) minutes
// Initial value: 0000
// The power automatically turns off if the camera does not receive any VISCA commands
//  or signals from the Remote Commander for the duration you set in the timer
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AutoPowerOff(int timer, int waitC);

// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT
// gain: R Gain 0000 to 255 (FF), 256 steps
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int RGain(int type, int gain, int waitC);

// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT
// gain: B Gain 0000 to 255 (FF), 256 steps
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int BGain(int type, int gain, int waitC);

// type: EVILIB_AUTO
//       DERVERV_MANUAL
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int SlowShutter(int type, int waitC);

// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT
// cmd: ExpComp Position 0 (-7, -10.5 dB) to 14 (0E, 7 10.5dB)
//       Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int ExpComp(int type, int cmd, int waitC);

// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT
// cmd: Aperture Gain 0 to 15 (F), 16 steps, Initial value: 5
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Aperture(int type, int cmd, int waitC);

// Mirror image ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int LR_Reverse(int type, int waitC);

// Still image ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Freeze(int type, int waitC);

// Picture effect setting
// type: EVILIB_OFF
//       EVILIB_PASTEL
//       EVILIB_NEGART
//       EVILIB_SEPIA
//       EVILIB_BW
//       EVILIB_SOLARIZE
//       EVILIB_MOSAIC
//       EVILIB_SLIM
//       EVILIB_STRETCH
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int PictureEffect(int type, int waitC);

// timer: contain the power off timer
// Return 1 on success, 0 on error
  int AutoPowerOffInq(int &timer);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int DZoomModeInq();

// Return on success: EVILIB_AF_SENS_HIGH
//                    EVILIB_AF_SENS_LOW
// Return 0 on error
  int AFModeInq();

// focus contain the Focus limit position
// Return 1 on success, 0 on error
  int FocusNearLimitInq(int &focus);

// gain contain the R Gain
// Return 1 on success, 0 on error
  int RGainInq(int &gain);

// gain contain the B Gain
// Return 1 on success, 0 on error
  int BGainInq(int &gain);

// Return on success: EVILIB_AUTO
//                    EVILIB_MANUAL
// Return 0 on error
  int SlowShutterModeInq();

// bright contain the bright value of the camera
// Return 1 on success, 0 on error
  int BrightPosInq(int &bright);

// Return on success: EVILIB_ON
//                   EVILIB_OFF
// Return on error: 0
  int ExpCompModeInq();

// ExpComp containt the ExpComp position
// Return 1 on success, 0 on error
  int ExpCompPosInq(int &ExpComp);

// aperture contain the Aperture Gain
// Return 1 on success, 0 on error
  int ApertureInq(int &aperture);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int LR_ReverseModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int FreezeModeInq();

// Return on success: EVILIB_OFF
//                    EVILIB_PASTEL
//                    EVILIB_NEGART
//                    EVILIB_SEPIA
//                    EVILIB_BW
//                    EVILIB_SOLARIZE
//                    EVILIB_MOSAIC
//                    EVILIB_SLIM
//                    EVILIB_STRETCH
// Return on error: 0
  int PictureEffectModeInq();

//-------------------------------------------------------------------------

// Defined in EVI-D100 & EVI-D100P
  virtual int Shutter(int type, int speed, int waitC) = 0;
  virtual int ShutterPosInq(int &shutter) = 0;

};

//-------------------------------------------------------------------------

#endif // __EVI_D100g_CLASS__
