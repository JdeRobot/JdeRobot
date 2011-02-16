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


#ifndef GENERIC_RECORDER_H
#define GENERIC_RECORDER_H

#include <jderobot/recorder.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <IceUtil/Thread.h>

//Constants for protocols
const std::string RECORDING_PROTOCOL_V4L = "v4l";
const std::string RECORDING_PROTOCOL_CAMERASERVER = "cameraServer";

// Constants for providers
const int RECORDING_PROVIDER_FFMPEG = 50;
const int RECORDING_PROVIDER_MENCODER = 51;
const int RECORDING_PROVIDER_MPLAYER = 52;
const int RECORDING_PROVIDER_VLC = 53;

// Constants for status of recording
const int RECORDING_IN_PROGRESS = 0;
const int RECORDING_FINISHED = 1;
const int RECORDING_ERROR = 2;

class GenericRecorder
{

	public:
		/// \brief Constructor
		GenericRecorder(const jderobotice::Context& context,const jderobot::RecorderConfigPtr& recConfig, int recordingProvider) : mContext(context),
																				      mRecConfig(recConfig),
																				      mId(),
																				      mStatus(),
																				      mProvider(recordingProvider) {};

		void setId (int id) {mId = id;};

		int getId () { return mId; };

		/// \brief Get the config about the recording
		jderobot::RecorderConfigPtr getConfig() { return mRecConfig; };

		/// \brief Set recording config
		void setConfig (const jderobot::RecorderConfigPtr& recConfig) { mRecConfig = recConfig; };

		/// \brief Get recording provider
		int getProvider() { return mProvider; };

		jderobotice::Context getContext(){ return mContext; };

		/// \brief Return the status of the recording
		int getStatus();

		/// \brief Start the recording process
		/// \return PID if all was OK. -1 if an error ocurred
		int startRecording();

		/// \brief Start the recording process
		/// \return 0 if all was OK. -1 if an error ocurred
		int stopRecording();



	private:

		virtual int doRecording() {};

		jderobot::RecorderConfigPtr mRecConfig;
		jderobotice::Context mContext;

		int mId;

		int mStatus;

		int mProvider;
};

#endif GENERIC_RECORDER_H
