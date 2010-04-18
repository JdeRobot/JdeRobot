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

#ifndef RECORDING_LOG_H
#define RECORDING_LOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <mysql++/mysql++.h>

#include <jderobot/recording.h>
#include <jderobot/recorder.h>

using namespace std;

/**
   \class RecordingLog
   \brief This class proved a mecanish to log the recording process
   \author Roberto Calvo <rocapal@gsyc.es>
   \date  12/04/2009
**/


class RecordingLog
{

public:

	/// \brief
	RecordingLog();

	/// \brief Destructor, mysql session is free when this method is called.
	~RecordingLog();

	/// \brief Set the connect with BBDD
	bool connect (string db,
				  string server,
				  string user,
				  string pass);

	/// \brief Save in bbdd information when recording begins
	/// \return Identifier of bbdd (use it when saveEndRecording is called)
	int startRecording (const jderobot::RecorderConfigPtr& recConfig);


	/// \brief Save in bbdd information when recording stops
	/// \param id
	bool endRecording (int recordingId);


	/// \brief Save in bbdd the information about an event
	/// \param recEvent The information about the event
	/// \param recordingId The recordingId assigned this event
	bool saveRecordingEvent ( const jderobot::RecordingEventPtr& recEvent, int recordingId);



	/// \brief Get all recordings
	jderobot::RecordingSequence getAllRecording();

	/// \brief Get Events of Recording
	jderobot::EventSequence getEventsOfRecording (int recordingId);

	/// \brief Get Event
	jderobot::RecordingEventPtr getEvent (int eventId);

private:

	jderobot::RecordingEventPtr Row2RecordingEvent (mysqlpp::Row row, bool addImage);

	jderobot::RecorderConfigPtr Row2Recorder (mysqlpp::Row row);

	long Date2TimeStamp(std::string date);

	mysqlpp::Connection* m_conn;

	bool finalize ();

	int m_recordingId;

};



#endif
