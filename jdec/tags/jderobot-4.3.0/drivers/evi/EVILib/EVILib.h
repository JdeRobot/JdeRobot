// EVILib.h
//
// Access functions to a SONY EVI camera
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
// Author: Pic Mickael, AIST Japan, 2002.
// Email: mickael.pic@aist.go.jp

#ifndef __EVILIB__
#define __EVILIB__

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <pthread.h>

//----------------------------

using namespace std;

//--------------------------------------------------------

// Because the camera can only store two commands at one time,
//  we do not need a waiting time.
// The commands are regulated by the time needed by the camera to complete them.
#define __EVILIB_Waiting_Time__ 50

// Home position of the camera
#define EVILIB_HomeX 0
#define EVILIB_HomeY 0
#define EVILIB_HomeZ 5.0

#define EVILIB_OFF                   1
#define EVILIB_ON                    2
#define EVILIB_PAN                   3
#define EVILIB_TILT                  4
#define EVILIB_ZOOM                  5
#define EVILIB_RESET                 6
#define EVILIB_UP                    7
#define EVILIB_DOWN                  8
#define EVILIB_DIRECT                9
#define EVILIB_STOP                 10
#define EVILIB_TELE_S               11
#define EVILIB_WIDE_S               12
#define EVILIB_FAR                  13
#define EVILIB_NEAR                 14
#define EVILIB_AUTO                 15
#define EVILIB_MANUAL               16
#define EVILIB_AUTO_MANUAL          17
#define EVILIB_INDOOR               18
#define EVILIB_OUTDOOR              19
#define EVILIB_ONEPUSH_MODE         20
#define EVILIB_ONEPUSH_TRIGGER      21
#define EVILIB_SHUTTER_PRIO         22
#define EVILIB_IRIS_PRIO            23
#define EVILIB_BRIGHT               24
#define EVILIB_SET                  25
#define EVILIB_RECALL               26
#define EVILIB_ON_OFF               27
#define EVILIB_LEFT                 28
#define EVILIB_RIGHT                29
#define EVILIB_UPLEFT               30
#define EVILIB_UPRIGHT              31
#define EVILIB_DOWNLEFT             32
#define EVILIB_DOWNRIGHT            33
#define EVILIB_ABSOLUTE             34
#define EVILIB_RELATIVE             35
#define EVILIB_HOME                 36
#define EVILIB_CLEAR                37
#define EVILIB_CHASE1               39
#define EVILIB_CHASE2               40
#define EVILIB_CHASE3               41
#define EVILIB_CHASE123             42
#define EVILIB_ENTRY1               43
#define EVILIB_ENTRY2               44
#define EVILIB_ENTRY3               45
#define EVILIB_ENTRY4               46
#define EVILIB_Y_LEVEL              47
#define EVILIB_HUE_LEVEL            48
#define EVILIB_SIZE                 49
#define EVILIB_DISPLAYTIME          50
#define EVILIB_REFRESH_MODE1        51
#define EVILIB_REFRESH_MODE2        52
#define EVILIB_REFRESH_MODE3        53
#define EVILIB_REFRESH_TIME         54
#define EVILIB_NTSC                 55
#define EVILIB_PAL                  56
#define EVILIB_NORMAL               57
#define EVILIB_ATMODE               58
#define EVILIB_MDMODE               59
#define EVILIB_SETTING              60
#define EVILIB_TRACKING             61
#define EVILIB_LOST                 62
#define EVILIB_UNDETECT             63
#define EVILIB_DETECTED             64
#define EVILIB_POWER_ON_OFF         65
#define EVILIB_ZOOM_TELE_WIDE       66
#define EVILIB_AF_ON_OFF            67
#define EVILIB_CAM_BACKLIGHT        68
#define EVILIB_CAM_MEMORY           69
#define EVILIB_PAN_TILT_DRIVE       70
#define EVILIB_AT_MODE_ON_OFF       71
#define EVILIB_MD_MODE_ON_OFF       72
#define EVILIB_TELE_V               73
#define EVILIB_WIDE_V               74
#define EVILIB_ATW                  75
#define EVILIB_FAR_V                76
#define EVILIB_NEAR_V               77
#define EVILIB_INFINITY             78
#define EVILIB_AF_SENS_HIGH         79
#define EVILIB_AF_SENS_LOW          80
#define EVILIB_NEAR_LIMIT           81
#define EVILIB_DZOOM_ON             82
#define EVILIB_DZOOM_OFF            83
#define EVILIB_GAIN_PRIO            84
#define EVILIB_SHUTTER_AUTO         85
#define EVILIB_IRIS_AUTO            86
#define EVILIB_GAIN_AUTO            87
#define EVILIB_CINEMA               88
#define EVILIB_16_9_FULL            89
#define EVILIB_PASTEL               90
#define EVILIB_NEGART               91
#define EVILIB_SEPIA                92
#define EVILIB_BW                   93
#define EVILIB_SOLARIZE             94
#define EVILIB_MOSAIC               95
#define EVILIB_SLIM                 96
#define EVILIB_STRETCH              97
#define EVILIB_STILL                98
#define EVILIB_FLASH                99
#define EVILIB_LUMI                100
#define EVILIB_TRAIL               101
#define EVILIB_EFFECTLEVEL         102
#define EVILIB_COMBINE_MODE        103
#define EVILIB_SEPARATE_MODE       104
#define EVILIB_x1_MAX              105
#define EVILIB_LOW                 106
#define EVILIB_INTERVAL            107
#define EVILIB_ZOOM_TRIGGER        108
#define EVILIB_AI_TIME             109
#define EVILIB_LENS                110
#define EVILIB_COMP_SCAN           111
#define EVILIB_POSITION            112
#define EVILIB_TITLE_SET1          113
#define EVILIB_TITLE_SET2          114
#define EVILIB_TITLE_SET3          115
#define EVILIB_TITLE_CLEAR         116
#define EVILIB_SET_MODE            117
#define EVILIB_SET_DAY_NIGHT_LEVEL 118
#define EVILIB_HIGH                119

#define EVILIB_NO_WAIT_COMP 0
#define EVILIB_WAIT_COMP    1

// Allowed range for the difference between the real position of the camera and the one asked.
#define EVILIB_RANGE_VALUE 0.1

//--------------------------------------------------------

//  * Fonction handled by a thread.
// Receive one answer from the camera.
// Return the number of bytes recevied on success, 0 on error
void *Receiver(void *arg);

//--------------------------------------------------------

class EVILib
{

//-------------------------------------------------------------------------

 protected:
// Id of the camera
  int Id_cam;

// Power state of the camera.
// If the camera is OFF, then no Command or Inq can be send to the camera.
// A power ON must be done before
  int power;

// Store the pan speed of the camera
  int panspeed;

// Store the tilt speed of the camera
  int tiltspeed;

// Store the zoom speed of the camera
  int zoomspeed;

  struct termios _oldtio;
  int _port;

// Store the command
  char *buffer;
// Store the messages from the camera
  char *buffer2;
// Store the messages from the camera.
  char *buffer3;

// Store the status codes of the Pan Tilt
  int *PanTiltStatus;

// Store the signal of End for the thread
  int End;
// Store the size of the messagea from the camera
  int answer;
// Store if a command is waiting for completion
  int waitComp;
// Thread used for receiving the messages from the camera
  pthread_t threadReceiver;
// Synchronize the acces to 'buffer3'
  pthread_mutex_t AccessBuffer3;
// Synchronize the access to the 'ACK' messages from the camera
  pthread_mutex_t TakeAck;
// Synchronize the access to the 'information return' message from the camera
  pthread_mutex_t TakeInfo;
// Synchronize the access to the 'command completion' message from the camera
  pthread_mutex_t TakeComp;
// Synchronize the access to the 'information return' message from the camera when we want to catch
// the return from the IR commander
  pthread_mutex_t TakeReturn;
// Allow only to command at the same time in the buffer.
// This is because the camera have a buffer of size 2. If more than 2 commands are send to
//  the camera, a 'command buffer full' message is send by the camera
  pthread_mutex_t BufferDispo;
// Allow to synchronize the thread that handle the return from the camera device
  pthread_mutex_t LaunchThread;
// The camera can store up to two command at the same time.
// If more than 2 commands are send, the camera send back a "Command Buffer Full" error.
  pthread_mutex_t CommandBuffer;
  pthread_cond_t CommandBufferCond;

// Store the return value of pthread_mutex_trylock
  int mutex_return;

// The camera have buffer that can contain up to  command. So we need to check if the buffer is not full
  int Nb_Command_Buffer;

// The following variables contain the parameters of the camera. Check the header of the camera file for more
//  details
  char camera_type[50];
  int minGain;
  int maxGain;
  int minpan;
  int maxpan;
  int maxPan;
  int min_pspeed;
  int max_pspeed;
  int mintilt;
  int maxtilt;
  int maxTilt;
  int min_tspeed;
  int max_tspeed;
  int minzoom;
  int maxzoom;
  int maxZoom;
  int min_zspeed;
  int max_zspeed;
  int min_iris;
  int max_iris;
  int highLowSignal;

//-------------------------------------------------------------------------

// Initialize the data
// Return 1 on success
// Return 0 on error
  int HiddenInit();

// Transform the data of Hex to the signal send to the camera
  int asciiToPackedHex(unsigned char *buf, int len);

// This function creates a string in the form: "0Y0Y0Y0Y"
//  where YYYY is the hex representation of "pos".
// This is needed for messages to the Sony EVI-D30.
  void make0XString(int pos, char *buf, int len);

// Generate a table a bits from 'x'
  void bitGenerator(unsigned char x, int *table);

// Return the value set in the range of the type
// type is PAN, TILT, ZOOM
// limit is the maximum degree of the type
// maxi is the maximum range of the type 
  int convert(int type, float value, float limit, float maxi);

// Return the value contain in 'buffer3' from 'start' for 'length' bits
// limit and maxi are used to convert the value from Hexa to float
  float deconvert(float limit, float maxi, int start, int length);

// Return the value contain in 'buffer3' from 'start' for 'length' bits
  float deconvert(int start, int length);

// The thread must have direct access to the data.
  friend void *Receiver(void *arg);

// Display the status of the mutex
  void Mutex();

//-------------------------------------------------------------------------

public:
  EVILib();
  virtual ~EVILib();

// Set, Return Id_cam
  inline void SetId_Cam(int id){Id_cam = id;};
  inline int GetId_Cam(){return Id_cam;};

// Because the camera is not Highly precise on position, small difference can appear.
// This test allow to check if the value received is near the one asked
// Return 1 on success, 0 otherwise
  inline int TestPanRange(float ask, float rec){if(ask - EVILIB_RANGE_VALUE < rec && rec < ask + EVILIB_RANGE_VALUE) return 1; else return 0;};

// Put the Camera to the Home position
// Return 1 on success, 0 on error
  inline int Home(){return Pan_TiltDrive(EVILIB_ABSOLUTE, max_pspeed, max_tspeed, EVILIB_HomeX, EVILIB_HomeY, EVILIB_WAIT_COMP);};

// Status of the High-Low signal output
// Return 0 if no change occured
// Return 1 if Low -> High edge
// Return -1 if High -> Low edge
  inline int HighLowSignal(){return highLowSignal;};

//-------------------------------------------------------------------------
// The following functions exist for all camera

// Initialize the data
// Return 1 on success
// Return 0 on error
  virtual int Init(){return HiddenInit();};

// Open Serial Port - terminal settings
// Return 1 on success, 0 on error
  int Open(int id, char *portname);

// Restore old port settings and close Serial Port
// Return 1 on success, 0 on error
  int Close();

// Send a command to the camera
// Return the number of bytes sended
  int SendCommand(unsigned char *buf, int len);

// Send AddressSet command and IF_Clear command before starting communication.
// Return 1 on success, 0 on error
  int AddressSet();

// Send AddressSet command and IF_Clear command before starting communication.
// Return 1 on success, 0 on error
  int IF_Clear();

// socket: socket number, 0 or 1
// Return 1 on success, 0 on error
  int CommandCancel(int socket);

// When camera main power is on, camera can be changed to Power Save Mode
// type: EVILIB_ON  : set power on
//       EVILIB_Off : set power off
// Return 1 on success, 0 on error
  int Power(int type);

// Iris Setting. Enable on AE_Manual or Iris_Priority
// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT --> need 'cmd'
// cmd: ExpComp Position min_iris to max_iris
//       Check camera documentation for values
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Iris(int type, int cmd, int waitC);

// Gain Setting. Enable on AE_Manual only
// type: EVILIB_RESET
//       EVILIB_UP
//       EVILIB_DOWN
//       EVILIB_DIRECT --> need 'setting'
// setting: EVILib_minGain to EVILib_maxGain
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Gain(int type, int setting, int waitC);

// Backlight compensation.
// Gain-up to 6 dB max.
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Backlight(int type, int waitC);

// Preset memory for memorize camera condition
// type: EVILIB_RESET --> need 'position'
//       EVILIB_SET --> need 'position'
//       EVILIB_RECALL -- need 'position'
// position: 0 to 5
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Memory(int type, int position, int waitC);

// Enable/Disable for IR remote commander
// type: EVILIB_ON
//       EVILIB_OFF
//       EVILIB_ON_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int IR_Receive(int type, int waitC);

// Send replies what command received from IR Commander
// type: EVILIB_ON
//       EVILIB_OFF
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int IR_ReceiveReturn(int type, int waitC);

// type: EVILIB_UP --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_DOWN --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_LEFT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_RIGHT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_UPLEFT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_UPRIGHT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_DOWNLEFT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_DOWNRIGHT --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_STOP --> need 'pan_speed' & 'tilt_speed'
//       EVILIB_ABSOLUTE --> need 'pan_speed' & 'tilt_speed' & 'pan_pos' & 'tilt_pos'
//       EVILIB_RELATIVE --> need 'pan_speed' & 'tilt_speed' & 'pan_pos' & 'tilt_pos'
//       EVILIB_HOME
//       EVILIB_RESET
// pan_speed: pan speed EVILIB_min_pspeed to EVILIB_max_pspeed
// tilt_speed: tilt speed EVILIB_min_tspeed to EVILIB_max_tspeed
// pan_pos: pan position: approx. -EVILIB_maxpan to +EVILIB_maxpan
// tilt_pos: tilt position: approx. -EVILIB_mintilt to +EVILIB_maxtilt
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Pan_TiltDrive(int type, int pan_speed, int tilt_speed, float pan_pos, float tilt_pos, int waitC);

// Pan/Tilt limit set
// type: EVILIB_SET --> need 'mode' & 'pan_pos' & 'tilt_pos'
//       EVILIB_CLEAR --> need 'mode' & 'pan_pos' & 'tilt_pos'
// mode: EVILIB_UPRIGHT
//       EVILIB_DOWNLEFT
// pan_pos: pan position: approx. -EVILIB_maxpan to +EVILIB_maxpan
// tilt_pos: tilt position: approx. -EVILIB_mintilt to +EVILIB_maxtilt
// waitC: wait for completion of command: EVILIB_NO_WAIT_COMP
//                                        EVILIB_WAIT_COMP
// Return 1 on success, 0 on error
  int Pan_TiltLimitSet(int type, int mode, float pan_pos, float tilt_pos, int waitC);

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int PowerInq();

// Return on success: EVILIB_AUTO
//                    EVILIB_MANUAL
// Return on error: 0
  int FocusModeInq();

// focus: contain the focus position of the camera
// Return 1 on success, 0 on error
  int FocusPosInq(int &focus);

// Return on success: EVILIB_AUTO
//                    EVILIB_INDOOR
//                    EVILIB_OUTDOOR
//                    EVILIB_ONEPUSH_MODE
//                    EVILIB_ATW
//                    EVILIB_MANUAL
// Return on error: 0
  int WBModeInq();

// Return on success: EVILIB_AUTO
//                    EVILIB_MANUAL
//                    EVILIB_SHUTTER_PRIO
//                    EVILIB_IRIS_PRIO
//                    EVILIB_GAIN_PRIO
//                    EVILIB_BRIGHT
//                    EVILIB_SHUTTER_AUTO
//                    EVILIB_IRIS_AUTO
//                    EVILIB_GAIN_AUTO
// Return on error: 0
  int AEModeInq();

// Return on success: EVILIB_ON
//                    EVILIB_OFF
// Return on error: 0
  int BacklightModeInq();

// iris: contain the iris position of the camera
// Return 1 on success, 0 on error
  int IrisPosInq(int &iris);

// gain: contain the iris position of the camera
// Return 1 on success, 0 on error
  int GainPosInq(int &gain);

// memory: contain the preset memory for memorize camera condition
// Return 1 on success, 0 on error
  int MemoryInq(int &memory);

// After the completion of Pan_TiltModeInq, you can check the status with the functions
//  provided
// Return 1 on success, 0 on error
  int Pan_TiltModeInq();

// Return 1 if the 'Pan Left End' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanLeftEnd(){return PanTiltStatus[0];};

// Return 1 if the 'Pan Right End' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanRightEnd(){return PanTiltStatus[1];};

// Return 1 if the 'Tilt Upper End' Pan/Tilter Status is true
// Return 0 otherwise
  inline int TiltUpperEnd(){return PanTiltStatus[2];};

// Return 1 if the 'Tilt Down End' Pan/Tilter Status is true
// Return 0 otherwise
  inline int TiltDownEnd(){return PanTiltStatus[3];};

// Return 1 if the 'Pan Normal' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanNormal(){return PanTiltStatus[4];};

// Return 1 if the 'Pan Miss Position' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanMissPosition(){return PanTiltStatus[5];};

// Return 1 if the 'Pan Mechanical Disorder' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanMechanicalDisorder(){return PanTiltStatus[6];};

// Return 1 if the 'Tilt Normal' Pan/Tilter Status is true
// Return 0 otherwise
  inline int TiltNormal(){return PanTiltStatus[7];};

// Return 1 if the 'Tilt Miss Position' Pan/Tilter Status is true
// Return 0 otherwise
  inline int TiltMissPosition(){return PanTiltStatus[8];};

// Return 1 if the 'Tilt Mechanical Disorder' Pan/Tilter Status is true
// Return 0 otherwise
  inline int TiltMechanicalDisorder(){return PanTiltStatus[9];};

// Return 1 if the 'No Drive Command' Pan/Tilter Status is true
// Return 0 otherwise
  inline int NoDriveCommand(){return PanTiltStatus[10];};

// Return 1 if the 'Pan, Tilt In-Move' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanTiltInMove(){return PanTiltStatus[11];};

// Return 1 if the 'Pan, Tilt Drive Completion' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanTiltDriveCompletion(){return PanTiltStatus[12];};

// Return 1 if the 'Pan, Tilt Drive failure' Pan/Tilter Status is true
// Return 0 otherwise
  inline int PanTiltDriveFailure(){return PanTiltStatus[13];};

// Return 1 if the 'Before Initialize' Pan/Tilter Status is true
// Return 0 otherwise
  inline int BeforeInitialize(){return PanTiltStatus[14];};

// Return 1 if the 'In-Initialize' Pan/Tilter Status is true
// Return 0 otherwise
  inline int InInitialize(){return PanTiltStatus[15];};

// Return 1 if the 'Initialize complete' Pan/Tilter Status is true
// Return 0 otherwise
  inline int InitializeComplete(){return PanTiltStatus[16];};

// Return 1 if the 'Initialize failure' Pan/Tilter Status is true
// Return 0 otherwise
  inline int InitializeFailure(){return PanTiltStatus[17];};

// pan: contain the pan max speed of the camera
// tilt: containt the tilt max speed of the camera
// Return 1 on success, 0 on error
  int Pan_TiltMaxSpeedInq(int &pan, int &tilt);

// pan: contain the pan position of the camera
// tilt: containt the tilt position of the camera
// Return 1 on success, 0 on error
  int Pan_TiltPosInq(float &pan, float &tilt);

// Return on success: EVILIB_POWER_ON_OFF
//                    EVILIB_ZOOM_TELE_WIDE
//                    EVILIB_AF_ON_OFF
//                    EVILIB_CAM_BACKLIGHT
//                    EVILIB_CAM_MEMORY
//                    EVILIB_PAN_TILT_DRIVE
//                    EVILIB_AT_MODE_ON_OFF
//                    EVILIB_MD_MODE_ON_OFF
// Return on error: 0
  int IR_ReceiveReturn();

//-------------------------------------------------------------------------
// The following functions exist for all the camera, but might be overide
//  for one of them

// Overide for EVI-D70(P)
// zoom: contain the zoom position of the camera
// Return 1 on success, 0 on error
  virtual int ZoomPosInq(float &zoom);

//-------------------------------------------------------------------------
// The following functions exist for all the camera, but the number of valid parameters
//  may change for each of them

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int Zoom(int type, int speed, float zoom, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int Focus(int type, int focus, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int WB(int type, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int AE(int type, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int Bright(int type, int cmd, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int Shutter(int type, int speed, int waitC) = 0;

// Defined in EVI-D30g & EVI-D70g & EVI-D100g
  virtual int ShutterPosInq(int &shutter) = 0;

//-------------------------------------------------------------------------
// The following functions exist for more than one camera, but not for all of them

// The following functions exist only for EVI-D70(P) and EVI-D100(P)
  virtual int AutoPowerOff(int timer, int waitC){cout<<"AutoPowerOff not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int RGain(int type, int gain, int waitC){cout<<"RGain not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int BGain(int type, int gain, int waitC){cout<<"BGain not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int SlowShutter(int type, int waitC){cout<<"SlowShutter not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int ExpComp(int type, int cmd, int waitC){cout<<"ExpComp not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int Aperture(int type, int cmd, int waitC){cout<<"Aperture not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int LR_Reverse(int type, int waitC){cout<<"LR_Reverse not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int Freeze(int type, int waitC){cout<<"Freeze not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int PictureEffect(int type, int waitC){cout<<"PictureEffect not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int AutoPowerOffInq(int &timer){cout<<"AutoPowerOffInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int DZoomModeInq(){cout<<"DZoomModeInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int AFModeInq(){cout<<"AFModeInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int FocusNearLimitInq(int &focus){cout<<"FocusNearLimitInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int RGainInq(int &gain){cout<<"RGainInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int BGainInq(int &gain){cout<<"BGainInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int SlowShutterModeInq(){cout<<"SlowShutterModeInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int BrightPosInq(int &bright){cout<<"BrightPosInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int ExpCompModeInq(){cout<<"ExpCompModeInq not defined for this camera type\n"; return 1;};
  virtual int ExpCompPosInq(int &ExpComp){cout<<"ExpCompPosInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int ApertureInq(int &aperture){cout<<"ApertureInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int LR_ReverseModeInq(){cout<<"LR_ReverseModeInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int FreezeModeInq(){cout<<"FreezeModeInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int PictureEffectModeInq(){cout<<"PictureEffectModeInq not defined for "<<camera_type<<" camera\n"; return 1;};

// The following functions exist only for EVI-D30(31) and EVI-D70(P)
  virtual int KeyLock(int type, int waitC){cout<<"KeyLock not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int KeyLockInq(){cout<<"KeyLockInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int IDInq(int &id){cout<<"IDInq not defined for "<<camera_type<<" camera\n"; return 1;};

// The following functions exist only for EVI-D30(31) and EVI-D100(P)
  virtual int Datascreen(int type, int waitC){cout<<"Datascreen not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int VideoSystemInq(){cout<<"VideoSystemInq not defined for "<<camera_type<<" camera\n"; return 1;};
  virtual int DatascreenInq(){cout<<"DatascreenInq not defined for "<<camera_type<<" camera\n"; return 1;};
};

#endif // __EVILIB__


