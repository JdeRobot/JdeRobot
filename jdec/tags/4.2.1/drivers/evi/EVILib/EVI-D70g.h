// EVI-D70g.h
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

#ifndef __EVI_D70g_CLASS__
#define __EVI_D70g_CLASS__

//-------------------------------------------------------------------------

#define EVILIB_minFocus     4096 // 1000 (infinity)
#define EVILIB_maxFocus    49152 // C000 (1.0cm)
#define EVILIB_min_FocusSpeed  0 // min focus speed
#define EVILIB_max_FocusSpeed  7 // max focus speed

//-------------------------------------------------------------------------

#define EVILIB_minGain         0 // 00  0dB
#define EVILIB_maxGain        15 // 0F 28dB
#define EVILIB_minpan       -170 // -170 degrees
#define EVILIB_maxpan        170 // +170 degrees
#define EVILIB_maxPan       2059 // 080B
#define EVILIB_min_pspeed      1 // min pan speed
#define EVILIB_max_pspeed     24 // max pan speed
#define EVILIB_mintilt       -30 //  -30 degrees
#define EVILIB_maxtilt        90 //  +90 degrees
#define EVILIB_maxTiltOFF   1200 // 04B0 (Image Flip: OFF)
#define EVILIB_maxTiltON     400 // 0190 (Image Filp: ON)
#define EVILIB_min_tspeed      1 // min tilt speed
#define EVILIB_max_tspeed     23 // max tilt speed
#define EVILIB_minIris         0 // 00 (CLOSE)
#define EVILIB_maxIris        17 // 11 (F1.4)

#define EVILIB_minMovementTime   0 // 00 (not minimum value given in tech doc)
#define EVILIB_maxMovementTime 255 // FF (not maximum value given in tech doc)
#define EVILIB_minInterval       0 // 00 (not minimum value given in tech doc)
#define EVILIB_maxInterval     255 // FF (not maximum value given in tech doc)

//-------------------------------------------------------------------------

#include "EVILib.h"

//-------------------------------------------------------------------------

class EVI_D70g : virtual public EVILib
{
 protected:
// Keep track of the combined/separated mode for the zoom, digital zoom
  int combined;

 public:
// Constructor
  EVI_D70g();
// Destructor
  ~EVI_D70g();

//-------------------------------------------------------------------------

// Night Power Off
// timer = power off timer parameter 0000 (timer off) to 65535 (FFFF) minutes
// A setting of 0 (zero min.) is equivalent to OFF, and the smallest value that
//  can be set is 1 min.
// When the Day/Night function is in effect, abd the Night setting as been made,
//  if an operation is not attempted via either a VISCA command or the remote
//  comtroller, the unit will continue to operate for the time set in the timer,
//  and will then shut off automatically.
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int NightPowerOff(int timer, int waitC);

// AF Sensitivity
// type: EVILIB_NORMAL 
//       EVILIB_LOW
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AF_Sensitivity(int type, int waitC);

// AF Mode
// type: EVILIB_NORMAL
//       EVILIB_INTERVAL
//       EVILIB_ZOOM_TRIGGER
//       EVILIB_AI_TIME --> need 'movementTime' & 'interval'
// movementTime: EVILIB_minMovementTime to EVILIB_maxMovementTime
// interval: EVILIB_minInterval to EVILIB_maxInterval
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// return 1 on success, 0 on error
  int AFMode(int type, int movementTime, int interval, int waitC);

// Initialize
// type: EVILIB_LENS      : Lens Initialization Start
//       EVILIB_COMP_SCAN : Correction of CCD pixel blemishes
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Initialize(int type, int waitC);

// Spot Automatic Exposure setting
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_POSITION -> need 'x' & 'y'
// x: 0 to 15 (F)
// y: 0 to 15 (F)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int SpotAE(int type, int x, int y, int waitC);

// Infrared mode (some models do not support Infrared Mode)
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int ICR(int type, int waitC);

// Auto Infrared mode (some models do not support Infrared Mode)
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AutoICR(int type, int waitC);

// Display
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Display(int type, int waitC);

// Title
// type: EVILIB_TITLE_SET1 -> need 'data[0]' Vposition, 'data[1]' Hposition, 'data[2]' Color, 'data[3]' Blink
//       EVILIB_TITLE_SET2 -> need 'data' contain 1st to 10th characters to display
//       EVILIB_TITLE_SET3 -> need 'data' contain 11st to 20th characters to display
//       EVILIB_TITLE_CLEAR
//       EVILIB_ON
//       EVILIB_OFF
//   Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Title(int type, int data[10], int waitC);

// Mute
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Mute(int type, int waitC);

// Camera ID
// id: 0 (0000) to 65535 (FFFF)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int IDWrite(int id, int waitC);

// Camera Alarm
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_SET_MODE --> data[0]: 0 to 13 (0C)
//       EVILIB_SET_DAY_NIGHT_LEVEL --> data[0]: 0 to 4095 Day setting AE level (no maximum value given in documentation)
//                                      data[1]: 0 to 4095 Night setting AE level (no maximum value given in documentation)
//  also, this can be called SetTime. The guy who made the technical doc should be more carefull...
//   Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Alarm(int type, int data[2], int waitC);

// timer contain the power off timer
// Return 1 on success, 0 on error
  int NightPowerOffInq(int &timer);

// Return on success: EVILIB_COMBINE_MODE
//                    EVILIB_SEPARATE_MODE
// Return on error: 0
  int DZoomCSModeInq();

// Return on success: EVILIB_NORMAL
//                    EVILIB_LOW
// Return on error: 0
  int AF_SensitivityInq();

// movementTime contain the Movement Time
// interval contain the interval
// Return 1 on success, 0 on error
  int AFTimeSettingInq(int &movementTime, int &interval);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int SpotAEModeInq();

// x contain the X position
// y contain the Y position
// Return 1 on success, 0 on error
  int SpotAEPosInq(int x, int y);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int PictureFlipModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int ICRModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int AutoICRModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int DisplayModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int TitleDisplayModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int MuteModeInq();

// model contain the Model ID
// rom contain the ROM version
// socket contain the Socket numer (=2)
// Return 1 on success, 0 on error
  int DeviceTypeInq(int &model, int &rom, int &socket);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int AlarmInq();

// mode contain the alarm mode
// Return 1 on success, 0 on error
  int AlarmModeInq(int &mode);

// day contain the Day setting AE level
// night contain the Night setting AE level
// level contain the current AE level
// Return 1 on success, 0 on error
  int AlarmDayNightLevelInq(int &day, int &night, int &level);

// Return on success: EVILIB_HIGH
//                    EVILIB_LOW
// Return on error: 0
  int AlarmDetectLevelInq();

//-------------------------------------------------------------------------

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
// cmd: Aperture Gain 0 to 15 (F), 16 steps
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
//       EVILIB_NEGART
//       EVILIB_BW
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int PictureEffect(int type, int waitC);

// Enable/Disable for RS-232C and key control
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int KeyLock(int type, int waitC);

// timer contain the power off timer
// Return 1 on success, 0 on error
  int AutoPowerOffInq(int &timer);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int DZoomModeInq();

// focus contain the Focus limit position
// Return 1 on success, 0 on error
  int FocusNearLimitInq(int &focus);

// Return on success: EVILIB_AF_SENS_HIGH
//                    EVILIB_AF_SENS_LOW
// Return 0 on error
  int AFModeInq();

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
//                    EVILIB_NEGART
//                    EVILIB_BW
// Return on error: 0
  int PictureEffectModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int KeyLockInq();

// id: contain the ID of the camera
// Return 1 on success, 0 on error
  int IDInq(int &id);

//-------------------------------------------------------------------------

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
//       EVILIB_NEAR_LIMIT -> focus: focus near limit position EVILIB_min_Focus (far) to EVILIB_max_Focus (near)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Focus(int type, int focus, int waitC);

// White Balance Setting.
// type: EVILIB_AUTO            : Normal Auto
//       EVILIB_INDOOR          : Indoor mode
//       EVILIB_OUTDOOR         : Outdoor mode
//       EVILIB_ONEPUSH_MODE    : One Push WB mode
//       EVILIB_ATW             : Auto Tracing White Balance
//       EVILIB_MANUAL          : Manual Control mode
//       EVILIB_ONEPUSH_TRIGGER : One Push WB Trigger
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int WB(int type, int waitC);

// type: EVILIB_AUTO         : Auto Exposure Mode
//       EVILIB_MANUAL       : Manual control mode
//       EVILIB_SHUTTER_PRIO : Shutter priority automatic exposure mode
//       EVILIB_IRIS_PRIO    : Iris priority automatic exposure mode
//       EVILIB_BRIGHT       : Bright mode (Manual control)
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
// cmd: 0 (00, close, 0 dB) to 31 (1F, F1.4, 28 dB)
//       Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Bright(int type, int cmd, int waitC);

//-------------------------------------------------------------------------

// Defined in EVI-D70 & EVI-D70P
  virtual int Zoom(int type, int speed, float zoom, int waitC) = 0;
  virtual int ZoomPosInq(float &zoom) = 0;
  virtual int DZoom(int type, int speed, float zoom, int waitC) = 0;
  virtual int ZoomFocus(float zoom, int focus, int waitC) = 0;
  virtual int DZoomPosInq(float &zoom) = 0;
  virtual int Shutter(int type, int speed, int waitC) = 0;

};

//-------------------------------------------------------------------------

#endif // __EVI_D70g_CLASS__
