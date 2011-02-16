/*
 *
 *  Copyright (C) 1997-2010 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Author : Roberto Calvo Palomino <rocapal@gmail.com>
 *
 */
 
#ifndef RECORDER_ICE
#define RECORDER_ICE


#include <jderobot/common.ice>

module jderobot 
{
  	
  	class RecorderConfig
  	{
  		// Recording id;
  		int id;
  		
  		// Recording name
  		string name;
  		
  		// Recording camera proxy
  		string cameraProxy;
  		
  		// Protocol: v4l | v4l2 | cameraServer
  		string protocol;
  		
  		// Device: /dev/video0 | proxy to cameraserver
  		string device;
  		
  		// Height resolution
  		string height;
  		
  		// Width resolution
  		string width;
  		
  		// File system path
  		string path;
  		
  		//! TimeStamp to begin
		Time beginTimeStamp;
		
		//! TimeStamp to end
		Time endTimeStamp;
		
  		//! FrameRate of recording
		string frameRate;
		
		//! Seconds of recording
		string duration;
		
		//! Status
		// 0: In progress
		// 1: Finished
		// 2: Error
		int status;
		
  	};
  	
  	interface Recorder
  	{
		//! Returns: ID of recording if all was ok
		//           -1 in other case
		
		["ami"] RecorderConfig startRecording (RecorderConfig recConfig); 
		
		//! Returns: 0 if all was ok
		//           -1 in other case        
		int stopRecording (int idRecording);
		 
  	};

}; /* module */

#endif /*RECORDER_ICE*/
