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

class GenericRecorder
{

	public:
		/// \brief Constructor
		GenericRecorder(const jderobotice::Context& context) : mContext(context),mRecConfig(NULL), mId() {};


		void setId (int id) {mId = id;};

		int getId () { return mId; };

		/// \brief Get the config about the recording
		jderobot::RecorderConfigPtr getConfig() { return mRecConfig; };

		/// \brief Set recording config
		void setConfig (const jderobot::RecorderConfigPtr& recConfig) { mRecConfig = recConfig; };


		jderobotice::Context getContext(){ return mContext; };


		virtual int endRecording() {};



	private:

		virtual int starRecording() {};

		jderobot::RecorderConfigPtr mRecConfig;
		jderobotice::Context mContext;

		int mId;
};

#endif GENERIC_RECORDER_H
