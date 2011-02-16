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


#include "RecordingLog.h"
#include <jderobot/recorder.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <mysql++/datetime.h>
#include <mysql++/mysql++.h>

//BBDD
const string BBDD_RECORDINGS = "recording_recordings";
const string BBDD_RECORDINGS_EVENTS = "recording_events";

//FIELDS
const string FIELD_RECORDING_ID = "recording_id";
const string TABLE_EVENTS_FIELD_EVENT_ID = "id";


RecordingLog::RecordingLog ()
{


}

RecordingLog::~RecordingLog ()
{

	finalize();
}


bool RecordingLog::connect (string db,
							string server,
							string user,
							string pass)
{

	m_conn = new mysqlpp::Connection(false);

	if ( !m_conn->connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str()))
	{

		cerr << "DB connection failed: " << m_conn->error() << endl;

		return false;
	}


	return true;


}

bool RecordingLog::finalize ()
{

	m_conn->disconnect();
	delete (m_conn);
	return true;
}


int RecordingLog::startRecording (const jderobot::RecorderConfigPtr& recConfig)
{
	std::stringstream sid;
	mysqlpp::DateTime nowTime = mysqlpp::DateTime::now();

	sid << recConfig->id;

    string s_query = "INSERT INTO " + BBDD_RECORDINGS + " (id_rec,name,video_file,begin_time,end_time,frame_rate) VALUES ";
	s_query += "(" + sid.str() + ",'" + recConfig->name + "','" + recConfig->path + "'," + nowTime.str() + ",NULL,'" + recConfig->frameRate + "')" ;


	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::SimpleResult res = query.execute();

	if ( !res)
	{
		cerr << "Failed to insert in table: " << query.error() << endl;
		return 0;
	}

	m_recordingId = query.insert_id();

	setStatusRecording(m_recordingId,RECORDING_IN_PROGRESS);

	return m_recordingId;

}

bool RecordingLog::endRecording (int recordingId)
{

	string s_id;
	std::stringstream out;

	mysqlpp::DateTime nowTime = mysqlpp::DateTime::now();

	out << recordingId;
	s_id = out.str();

	string s_query = " UPDATE " + BBDD_RECORDINGS + " SET end_time=" + nowTime.str();
	s_query +=        " WHERE id=" + s_id;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::SimpleResult res = query.execute();


	if ( !res)
	{
		cerr << "Failed to update in table: " << query.error() << endl;
		return false;
	}

	return setStatusRecording(m_recordingId,RECORDING_FINISHED);
}


bool RecordingLog::saveRecordingEvent (const jderobot::RecordingEventPtr& recEvent, int recordingId)
{

	string s_id;
	std::stringstream out;
	out << recordingId;
	s_id = out.str();

	/*
	std::cout << recEvent->image.size() << std::endl;

	std::cout << std::hex << "0x" << (unsigned short) recEvent->image[0] << std::endl;
	std::cout << std::hex << "0x" << (unsigned short) recEvent->image[1] << std::endl;

	int bufferLength = 0;

	int dataLength = sizeof(unsigned char);
	std::cout << "dataLength is: " << dataLength << std::endl;
	char dataBuff[230400];

	for (int i=0; i<recEvent->image.size(); i++)
	{
		memcpy(dataBuff + bufferLength, &recEvent->image[i], dataLength);
		bufferLength += dataLength;
	}

	string blobBuffer (dataBuff,bufferLength);
	std::cout << "bufferLength is: " << bufferLength << std::endl;

	 */

	mysqlpp::DateTime nowTime = mysqlpp::DateTime::now();


	ostringstream s_query;
	s_query << "INSERT INTO " << BBDD_RECORDINGS_EVENTS << " (recording_id, type, time, producer, resource, comments, path_image) VALUES ";
	s_query << "('" << s_id << "','" <<
				recEvent->type << "'," <<
				nowTime.str() << ",'" <<
				recEvent->producer << "','" <<
				recEvent->resource << "','" <<
				recEvent->comment << "','" <<
				recEvent->pathImage << "')" ;

	//cout << s_query.str() << endl;

	mysqlpp::Query query = m_conn->query (s_query.str());
	mysqlpp::SimpleResult res = query.execute();

	if ( !res)
	{
		cerr << "::saveRecordingEvent:: Failed to insert in the table: " << query.error() << endl;
		return 0;
	}


	return true;

}

jderobot::RecordingSequence RecordingLog::getAllRecording(int from, int elems)
{

	jderobot::RecordingSequence recList;
	std::stringstream limit;

	std::cout << "[*] getAllRecording()" << std::endl;

	if (elems != -1)
		limit << " LIMIT " << from << "," << elems ;
	else
		limit << "";


	string s_query = "SELECT * FROM " + BBDD_RECORDINGS + limit.str() + ";";

	//std::cout << s_query << std::endl;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();

	while(mysqlpp::Row row = res.fetch_row())
	{
		jderobot::RecorderConfigPtr rec = Row2Recorder(row);
		recList.push_back(rec);
	}

	return recList;

}

jderobot::RecordingSequence RecordingLog::getRecordingsByDate (std::string date, int from, int elems)
{

	std::cout << "[*] getRecordingsByDate()" << std::endl;

	jderobot::RecordingSequence recList;
	std::stringstream limit;

	if (elems != -1)
		limit << " LIMIT " << from << "," << elems ;
	else
		limit << "";


	string s_query = "SELECT * FROM " + BBDD_RECORDINGS + " WHERE DATE_FORMAT(begin_time,'%Y-%m-%d')='" + date + "'" + limit.str() + ";";

	//std::cout << s_query << std::endl;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();

	if (!res)
		// return empty list
		return recList;

	while(mysqlpp::Row row = res.fetch_row())
	{
		jderobot::RecorderConfigPtr rec = Row2Recorder(row);
		recList.push_back(rec);
	}

	return recList;
}

jderobot::RecorderConfigPtr RecordingLog::getRecording(int recordingId)
{

	std::stringstream id;
	id << recordingId;

	string s_query = "SELECT * FROM " + BBDD_RECORDINGS + " WHERE id=" + id.str() + ";";

	//std::cout << s_query << std::endl;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();


	if (mysqlpp::Row row = res.fetch_row())
	{
		return Row2Recorder(row);
	}

	return NULL;

}


int RecordingLog::getRecordingPID (int recordingId)
{

	std::stringstream id;
	id << recordingId;

	string s_query = "SELECT * FROM " + BBDD_RECORDINGS + " WHERE id=" + id.str() + ";";

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();


	if (mysqlpp::Row row = res.fetch_row())
	{
		return static_cast<int>(row["id_rec"]);
	}

	return -1;

}


bool RecordingLog::setStatusRecording (int recordingId, int recordingStatus)
{
	std::stringstream id, status;
	id << recordingId;
	status << recordingStatus;

	string s_query = " UPDATE " + BBDD_RECORDINGS + " SET status=" + status.str();
	s_query +=        " WHERE id=" + id.str();

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::SimpleResult res = query.execute();


	if (!res)
	{
		cerr << "RecordingLog::setStatusRecording:: Failed to update in table: " << query.error() << endl;
		return true;
	}

	return false;

}

jderobot::RecordingEventPtr RecordingLog::getEvent (int eventId)
{

	std::cout << "getImageEvent" << std::endl;

	std::stringstream eventIdStream;
	eventIdStream << eventId;

	string s_query = "SELECT * FROM " + BBDD_RECORDINGS_EVENTS + " WHERE " + TABLE_EVENTS_FIELD_EVENT_ID + "=" + eventIdStream.str();

	std::cout << s_query << std::endl;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();

	mysqlpp::Row row = res.fetch_row();

	return Row2RecordingEvent(row, true);

}

jderobot::RecorderConfigPtr RecordingLog::Row2Recorder (mysqlpp::Row row)
{

	jderobot::RecorderConfigPtr rec = new jderobot::RecorderConfig();

	rec->id = static_cast<int>(row["id"]);
	rec->name = static_cast<std::string>(row["name"]);
	rec->path =  static_cast<std::string>(row["video_file"]);

	rec->beginTimeStamp.seconds = ((time_t) ((mysqlpp::DateTime) row["begin_time"]));
	rec->endTimeStamp.seconds = ((time_t) ((mysqlpp::DateTime) row["end_time"]));

	rec->frameRate = static_cast<std::string>(row["frame_rate"]);

	rec->status = static_cast<int>(row["status"]);

	return rec;

}

jderobot::RecordingEventPtr RecordingLog::Row2RecordingEvent (mysqlpp::Row row, bool addImage)
{

	jderobot::RecordingEventPtr event = new jderobot::RecordingEvent();

	event->id = static_cast<int>(row["id"]);
	event->type = static_cast<string>(row["type"]);
	event->producer = static_cast<string>(row["producer"]);
	event->resource = static_cast<string>(row["resource"]);
	event->pathImage = static_cast<string> (row["path_image"]);
	event->comment = static_cast<string>(row["comments"]);

    // Read the image
	if (addImage)
	{
		jderobot::ByteSeq imageVector(230400);
		Ice::Byte *buffer = new Ice::Byte[1];
		int i=0;

		ifstream file ( event->pathImage.c_str()  , ios::in | ios::binary);

		file.read (reinterpret_cast<char *>(&imageVector[0]), imageVector.size () * sizeof(Ice::Byte));

		std::cout << std::hex << "0x" << (unsigned short) imageVector[0] << std::endl;
		std::cout << std::hex << "0x" << (unsigned short) imageVector[1] << std::endl;

		std::cout << "Size: " << imageVector.size() << std::endl;

		event->image = imageVector;
	}

	return event;
}

jderobot::EventSequence RecordingLog::getEventsOfRecording (int recordingId)
{

	std::cout << "getEventsOfRecording" << std::endl;

	jderobot::EventSequence eventList;
	std::stringstream recordingIdStream;

	recordingIdStream << recordingId;

	string s_query = "SELECT * FROM " + BBDD_RECORDINGS_EVENTS + " WHERE " + FIELD_RECORDING_ID + "=" + recordingIdStream.str() +
			         " ORDER BY time;";

	std::cout << s_query << std::endl;

	mysqlpp::Query query = m_conn->query (s_query);
	mysqlpp::UseQueryResult res = query.use();

	while(mysqlpp::Row row = res.fetch_row())
	{
		jderobot::RecordingEventPtr event = Row2RecordingEvent(row, false);
		eventList.push_back(event);
	}


	return eventList;
}
