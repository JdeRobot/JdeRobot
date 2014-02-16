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

#ifndef LOG_H
#define LOG_H

#include <pthread.h>
#include <fstream>
#include <vector>


namespace jderobot
{
	enum Levels{

		DEBUG=0,
		INFO,
		WARNING,
		ERROR
	};


	/**
	 * Logger class implements a thread safe logger, implemented as singleton.
	 * This logger allows save traces in file and show the traces in the screen simultaneously.
	 */
	class Logger
	{

	public:

		/**
		 * \brief Singleton to get the instance. Just one object in memory for execution
		 */

		static Logger* getInstance();

		/**
		 * \brief Set the name of file to save every traces.
		 * It's not mandatory config, if you don't configure this field
		 * just see the traces in the screen
		 *
		 * @param logFile The name of file
		 *
		 */
		void setLogFile (std::string logFile);

		/**
		 * \brief Set log level for log file. Just the upper levers will be saved in file.
		 *
		 * @param levels The level of log file
		 *
		 */
		void setLogLevel (enum Levels);

		/**
		 * \brief Set log level for screen. Just the upper levers will be shown in the screen
		 *
		 * @param levels The level of screen log
		 *
		 */
		void setScreenLevel (enum Levels);

		/**
		 * \brief Save log message as debug level
		 *
		 * @param message The message saved
		 *
		 */
		void debug (std::string message);

		/**
		 * \brief Save log message as info level
		 *
		 * @param message The message saved
		 *
		 */
		void info (std::string message);

		/**
		 * \brief Save log message as warning level
		 *
		 * @param message The message saved
		 *
		 */
		void warning (std::string message);

		/**
		 * \brief Save log message as error level
		 *
		 * @param message The message saved
		 *
		 */
		void error (std::string message);

	private:

		Logger();
		~Logger();

		void trace (Levels level, std::string message);

		void trace_screen (Levels level, std::string message);

		std::string mLogFile;
		std::ofstream* mFs;

		Levels mLogLevel;
		Levels mScrenLevel;
		int mLastLevel;

		pthread_mutex_t mWriteLock;

		std::stringstream mLogDateTime;
	};
}

#endif
