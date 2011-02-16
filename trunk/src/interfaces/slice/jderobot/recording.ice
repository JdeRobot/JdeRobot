

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

#ifndef RECORDING_ICE
#define RECORDING_ICE


#include <jderobot/common.ice>
#include <jderobot/recorder.ice>

module jderobot {

	class Recording
	{
	
		//! Identifier of recording
		int id;
		
		//! Name of recording
		string name;
		
		//! Path to file video
		string pathFileVideo;
		
		//! TimeStamp to begin
		Time beginTimeStamp;
		
		//! TimeStamp to end
		Time endTimeStamp;
	
		//! FrameRate of recording
		string frameRate;
	};
	
	//! A sequence of recordings
  	sequence<RecorderConfig> RecordingSequence;
	
	//! Description of the recording's event
	class RecordingEvent
	{
		//! Identifier of event
		int id;
		
		//! Type of event
		string type;
		
		//! TimeStamp of event
	    Time timeStamp;
	    
	    //! What producer the alarm
	    string producer;
	    
	    //! Resource from the alarm
	    string resource;
	    
	    //! Path to image
	    string pathImage;
	    
	    //! Image associated to event
	    ByteSeq image;
	    
	    //! Comment
	    string comment; 
	};
	
	//! A sequence of events/alarms of the recording
  	sequence<RecordingEvent> EventSequence;
  	
  	interface RecordingManager
  	{
		//! Returns the list of recordings
		idempotent RecordingSequence getRecordings(int from, int elems);
		
		//! Returns the list of recordings by date
		// Format Date = "YYYY-MM-DD" - "2010-05-28"
		idempotent RecordingSequence getRecordingsByDate(string date, int from, int elems);
		
		
		//! Return the event list of recording
		EventSequence getEventsOfRecording (int recordingId);
		
		//! Set an alarm event
		int setEvent (RecordingEvent recEvent, int recordingId);
		
		//! Get event
		RecordingEvent getEvent (int eventId);
		
		//! Start recording
		int startRecording (RecorderConfig recConfig);
		
		//! Returns: 0 if all was ok
		//           -1 in other case        
		int stopRecording (int recordingId);
		
		//! Return: url to listen the streaming
		//			null in other case
		string startStreaming (int id); 
		
		//! Return: bytes of thumb
		//			null in other case	
		ByteSeq getThumbRecording (int id);
		
  	};

}; /* module */

#endif /*RECORDING_ICE*/
