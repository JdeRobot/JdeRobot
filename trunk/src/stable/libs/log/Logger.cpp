/*
 *  Copyright (C) 2014 Jderobot Developers
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
 *  Authors : Roberto Calvo Palomino <rocapal [at] gsyc [dot] urjc [es]>
 *
 */

#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include "Logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace jderobot
{

static Logger* mInstace;
static pthread_mutex_t mSingleLock = PTHREAD_MUTEX_INITIALIZER;

const static std::string levelStr[] =  {"", "[INFO]", "[WARNING]", "[ERROR]"};
const static std::string colorsStr[] = {"\x1b[0m", "\x1b[1;32m", "\x1b[1;33m", "\x1b[1;31m"};


Logger* Logger::getInstance()
{
	if (mInstace == NULL)
	{
		pthread_mutex_lock(&mSingleLock);
		if (mInstace == NULL)
		{
			mInstace = new Logger();
		}
		pthread_mutex_unlock(&mSingleLock);
	}

	return mInstace;
}

Logger::Logger(): mLogLevel(INFO), mScrenLevel(DEBUG), mFs(NULL), mWriteLock(PTHREAD_MUTEX_INITIALIZER), mLastLevel(-1)
{
}

Logger::~Logger()
{
	mFs->close();
}


void Logger::setLogFile(std::string logFile)
{
	mLogFile = logFile;

	std::string dirName = mLogFile;
	dirName.erase(std::find(dirName.rbegin(), dirName.rend(), '/').base(), dirName.end());

	// Check dir and create it
	if ( !boost::filesystem::exists(dirName))
	{
		boost::filesystem::path dir(dirName);
		boost::filesystem::create_directories(dir);
	}

	mFs= new std::ofstream(mLogFile.c_str(), std::ofstream::app);

}

void Logger::setLogLevel(Levels level)
{
	mLogLevel = level;
}

void Logger::setScreenLevel(Levels level)
{
	mScrenLevel = level;
}

void Logger::trace (Levels level, std::string message)
{
	pthread_mutex_lock(&mWriteLock);
	boost::posix_time::ptime logTime = boost::posix_time::second_clock::local_time();

	mLogDateTime.str("");
	mLogDateTime.clear();

	mLogDateTime << logTime.date().year() << "-" << logTime.date().month().as_number() << "-"  << logTime.date().day()  << "_";
	mLogDateTime << logTime.time_of_day().hours() << ":" << logTime.time_of_day().minutes()  << ":" << logTime.time_of_day().seconds();
	mLogDateTime << " ";


	if (mFs != NULL)
	{
		*mFs << mLogDateTime.str();

		if (mLogLevel <= level)
		{
			if (mLastLevel != level)
				*mFs << levelStr[level] << " ";

			*mFs << message << std::endl;
		}

		mFs->flush();
	}
	trace_screen(level, message);
	pthread_mutex_unlock(&mWriteLock);
}

void Logger::trace_screen (Levels level, std::string message)
{

	if (level < mScrenLevel)
		return;

	std::cout << colorsStr[level];

	if (mLastLevel != level)
	{
		mLastLevel = level;
		std::cout << levelStr[level] << " ";
	}

	std::cout << message << std::endl;
	std::cout << "\x1b[0m";
}

void Logger::debug (std::string message)
{
	trace(DEBUG, message);

}

void Logger::info (std::string message)
{
	trace(INFO, message);
}

void Logger::warning (std::string message)
{
	trace(WARNING, message);
}

void Logger::error (std::string message)
{
	trace(ERROR, message);
}


}
