// EVI-D30g.h
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

#ifndef __EVI_D30g_CLASS__
#define __EVI_D30g_CLASS__

//-------------------------------------------------------------------------

#define EVILIB_minFocus    4096 // 1000 (infinity)
#define EVILIB_maxFocus   40959 // 9000 (close)
#define EVILIB_min_FocusSpeed 0 // min focus speed
#define EVILIB_max_FocusSpeed 7 // max focus speed

//-------------------------------------------------------------------------
// Here you can change the camera parameters

#define EVILIB_minGain     0 // 00 -3dB
#define EVILIB_maxGain     7 // 07 18dB
#define EVILIB_minpan   -100 // -100 degrees
#define EVILIB_maxpan   +100 // +100 degrees
#define EVILIB_maxPan    880 // 0370
#define EVILIB_min_pspeed  1 // 01 min pan speed
#define EVILIB_max_pspeed 24 // 18 max pan speed
#define EVILIB_mintilt   -25 // -25 degrees
#define EVILIB_maxtilt   +25 // +25 degrees
#define EVILIB_maxTilt   300 // 012C
#define EVILIB_min_tspeed  1 // 01 min tilt speed
#define EVILIB_max_tspeed 20 // 14 max tilt speed
#define EVILIB_minzoom     0 //
#define EVILIB_maxzoom    12 // lens x12 power zoom
#define EVILIB_maxZoom  1023 // 03FF (tele)
#define EVILIB_min_zspeed  2 // 02 min zoom speed
#define EVILIB_max_zspeed  7 // 07 max zoom speed
#define EVILIB_minIris     0 // 00 (CLOSE)
#define EVILIB_maxIris    17 // 11 (F1.8)

//-------------------------------------------------------------------------

#include "EVILib.h"

//-------------------------------------------------------------------------

class EVI_D30g : virtual public EVILib
{

 private:
// Store the status codes of the AT
  int *ATStatus;
// Store the status code of the MD
  int *MDStatus;

//-------------------------------------------------------------------------

 public:
// Constructor
  EVI_D30g();
// Destructor
  ~EVI_D30g();

// Initialize the data
// Return 1 on success
// Return 0 on error
  int Init();

// Automatic Target Trace ability Compensation when a wide conversion lens is installed
// setting: 0 (no conversion) to 7 (0.6 conversion)
// Return 1 on success, 0 on error
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
  int Wide_conLensSet(int setting, int waitC);

// Target Tracking Mode ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_Mode(int type, int waitC);

// Auto Exposure for the target
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_AE(int type, int waitC);

// Automatic Zooming for the target
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_AutoZoom(int type, int waitC);

// Sensing Frame Display ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_MD_Frame_Display(int type, int waitC);

// Shifting the Sensing Frame for AR
// For Shifting use Pan/Tilt Drive Command
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_Offset(int type, int waitC);

// Tracking or Detecting Start/Stop
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_MD_StartStop(int waitC);

// Select a Tracking Mode
// type: EVILIB_CHASE1
//       EVILIB_CHASE2
//       EVILIB_CHASE3
//       EVILIB_CHASE123
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_Chase(int type, int waitC);

// Select target study mode for AT
// type: EVILIB_ENTRY1
//       EVILIB_ENTRY2
//       EVILIB_ENTRY3
//       EVILIB_ENTRY4
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_Entry(int type, int waitC);

// Motion Detector Mode ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int MD_Mode(int type, int waitC);

// Detecting Area Set (Size or Position)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int MD_Frame(int waitC);

// Select Detecting Frame (1 or 2 or 1 + 2)
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int MD_Detect(int waitC);

// Reply a completion when the camera lost the target in AT mode.
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int AT_LostInfo(int waitC);

// Reply a completion when the camera detected a motion of image in MD mode.
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int MD_LostInfo(int waitC);

// Set Detecting Condition
// type: EVILIB_Y_LEVEL --> need 'setting'
//       EVILIB_HUE_LEVEL --> need 'setting'
//       EVILIB_SIZE --> need 'setting'
//       EVILIB_DISPLAYTIME --> need 'setting'
//       EVILIB_REFRESH_MODE1
//       EVILIB_REFRESH_MODE2
//       EVILIB_REFRESH_MODE3
//       EVILIB_REFRESH_TIME --> need 'setting'
// setting: 0 to 15
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int MD_Adjust(int type, int setting, int waitC);

// Target Condition Measure Mode for More Accurate Setting for Motion Detector
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Measure_Mode1(int type, int waitC);

// Target Condition Measure Mode for More Accurate Setting for Motion Detector
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Measure_Mode2(int type, int waitC);

// lens: contain the lens No.
// Return 1 on success, 0 on error
  int Wide_ConLensInq(int &lens);

// Return on success: EVILIB_NORMAL
//                    EVILIB_ATMODE
//                    EVILIB_MDMODE
// Return on error: 0
  int ATMD_ModeInq();

// After the completion of ATModeInq, you can check the status with the functions
//  provided
// Return 1 on success, 0 on error
  int ATModeInq();

// Return 1 if the 'AT frame chase' AT Status is true
// Return 0 otherwise
  inline int ATFrameChase(){return ATStatus[0];};

// Return 1 if the 'AT pan chase' AT Status is true
// Return 0 otherwise
  inline int ATPanChase(){return ATStatus[1];};

// Return 1 if the 'AT frame/pan chase' AT Status is true
// Return 0 otherwise
  inline int ATFramePanChase(){return ATStatus[2];};

// Return 1 if the 'AT offset' AT Status is true
// Return 0 otherwise
  inline int ATOffset(){return ATStatus[3];};

// Return 1 if the 'AT AE on/off' AT Status is true
// Return 0 otherwise
  inline int ATAEOnOff(){return ATStatus[4];};

// Return 1 if the 'AT zoom on/off' AT Status is true
// Return 0 otherwise
  inline int ATZoomOnOff(){return ATStatus[5];};

// Return 1 if the 'AT frame display on/off' AT Status is true
// Return 0 otherwise
  inline int ATFrameDisplayOnOff(){return ATStatus[6];};

// Return 1 if the 'AT setting' AT Status is true
// Return 0 otherwise
  inline int ATSetting(){return ATStatus[7];};

// Return 1 if the 'AT working' AT Status is true
// Return 0 otherwise
  inline int ATWorking(){return ATStatus[8];};

// Return 1 if the 'AT lost' AT Status is true
// Return 0 otherwise
  inline int ATLost(){return ATStatus[9];};

// Return 1 if the 'AT memorizing' AT Status is true
// Return 0 otherwise
  inline int ATMemorizing(){return ATStatus[10];};

// Return on success: EVILIB_ENTRY1
//                    EVILIB_ENTRY2
//                    EVILIB_ENTRY3
//                    EVILIB_ENTRY4
// Return on error: 0
  int AT_EntryInq();

// After the completion of MDModeInq, you can check the status with the functions
//  provided
// Return 1 on success, 0 on error
  int MDModeInq();

// Return 1 if the 'MD detection method' MD Status is true
// Return 0 otherwise
  inline int MDDetectionMethod(){return MDStatus[0];};

// Return 1 if the 'MD settind' MD Status is true
// Return 0 otherwise
  inline int MDSetting(){return MDStatus[1];};

// Return 1 if the 'MD undetect' MD Status is true
// Return 0 otherwise
  inline int MDUndetect(){return MDStatus[2];};

// Return 1 if the 'MD detecting' MD Status is true
// Return 0 otherwise
  inline int MDDetecting(){return MDStatus[3];};

// Return 1 if the 'MD memorizing' MD Status is true
// Return 0 otherwise
  inline int MDMemorizing(){return MDStatus[4];};

// Return 1 if the 'MD frame 1' MD Status is true
// Return 0 otherwise
  inline int MDFrame1(){return MDStatus[5];};

// Return 1 if the 'MDFrame2' MD Status is true
// Return 0 otherwise
  inline int MDFrame2(){return MDStatus[6];};

// Return 1 if the 'MD frame 1 or 2' MD Status is true
// Return 0 otherwise
  inline int MDFrame1or2(){return MDStatus[7];};

// Return 1 if the 'MD frame display' MD Status is true
// Return 0 otherwise
  inline int MDFrameDisplay(){return MDStatus[8];};

// Dividing a sceen by 48*30 pixles, Return the center position of the detecting Frame.
// x: 4 to 42
// y: 3 to 27
// status: EVILIB_SETTING
//         EVILIB_TRACKING
//         EVILIB_LOST
// Return 1 on success, 0 on error
  int AT_ObjetPosInq(int &x, int &y, int &status);

// Dividing a scene by 48*30 pixels, Return the center position of the detecting Frame.
// x: 4 to 42
// y: 3 to 27
// status: EVILIB_SETTING
//         EVILIB_TRACKING
//         EVILIB_LOST
// Return 1 on success, 0 on error
  int MD_ObjetPosInq(int &x, int &y, int &status);

// level: 0 to 15
// Return 1 on success, 0 on error
  int MD_YLevelInq(int &level);

// level: 0 to 15
// Return 1 on success, 0 on error
  int MD_HueLevelInq(int &level);

// size: 0 to 15
// Return 1 on success, 0 on error
  int MD_SizeInq(int &size);

// dt: 0 to 15
// Return 1 on success, 0 on error
  int MD_DispTimeInq(int &dt);

// Return on success: EVILIB_REFRESH_MODE1
//                    EVILIB_REFRESH_MODE2
//                    EVILIB_REFRESH_MODE3
// Return on error: 0
  int MD_RefModeInq();

// rt: 0 to 15
// Return 1 on success, 0 on error
  int MD_RefTimeInq(int &rt);

//-------------------------------------------------------------------------

// Zoom control.
// type: EVILIB_STOP
//       EVILIB_TELE (Standard)
//       EVILIB_WIDE (Standard)
//       EVILIB_TELE (Variable) --> need 'speed'
//       EVILIB_WIDE (Variable) --> need 'speed'
//       EVILIB_DIRECT --> need 'zoom'
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
//       EVILIB_AUTO
//       EVILIB_MANUAL
//       EVILIB_AUTO_MANUAL
//       EVILIB_DIRECT --> need 'focus'
// focus: infinity = 4096, close = 40959
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
    int Focus(int type, int focus, int waitC);

// White Balance Setting.
// type: EVILIB_AUTO         : Trace the light source automatically
//       EVILIB_INDOOR       : fixed at factory
//       EVILIB_OUTDOOR      : fixed at factory
//       EVILIB_ONEPUSH_MODE : Pull-in to White with a Trigger then hold the data until next Trigger comming
//       EVILIB_ONEPUSH_TRIGGER
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
// cmd: not used
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Bright(int type, int cmd, int waitC);

//-------------------------------------------------------------------------

// Enable/Disable for RS-232C and key control
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int KeyLock(int type, int waitC);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int KeyLockInq();

// On screen Data Display ON/OFF
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Datascreen(int type, int waitC);

// id: contain the ID of the camera
// Return 1 on success, 0 on error
  int IDInq(int &id);

// Return on success: EVILIB_NTSC
//                    EVILIB_PAL
// Return on error: 0
  int VideoSystemInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int DatascreenInq();

//-------------------------------------------------------------------------

// Defined in EVI-D30 & EVI-D31
  virtual int Shutter(int type, int speed, int waitC) = 0;
  virtual int ShutterPosInq(int &shutter) = 0;
};

//-------------------------------------------------------------------------

#endif // __EVI_D30g_CLASS__
