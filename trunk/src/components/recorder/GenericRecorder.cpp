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

#include "GenericRecorder.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>



int GenericRecorder::startRecording()
{

	pid_t p_rec, p_surv;

	int descPipe [2];
	pipe (descPipe);

	p_surv = fork();

	if ( p_surv == 0 )
	{
		p_rec = fork();
		if (p_rec == 0)
		{
			// This process execute the recording command
			doRecording();
			exit(-1);
		}
		else
		{
			// This process waits to end of recording process

			/* 0 it's read side */
			close (descPipe[0]);

			write (descPipe[1], &p_rec, sizeof(int));

			int status = 0;
			mStatus = RECORDING_IN_PROGRESS;

			std::cout << "Waiting for end recording (" << p_rec << ") .... " <<  std::endl;
			int ret = wait (&status);

			std::cout << "End recording process (" << p_rec << "): " << status << " - errno: " << errno << " - ret = " << ret << " - " << WIFEXITED(status) << std::endl;

			if (WIFEXITED(status) != 0)
			{
				std::cout << "Finished recording with error." << std::endl;
				mStatus = RECORDING_ERROR;
			}

			exit(0);
		}
	}
	else
	{
		// This process waits the PID of recording process and return.

		int pid_child = 0;

		/* 1 it's write side */
		close (descPipe[1]);

		// Read the PID
		read (descPipe[0], &pid_child, sizeof(int));

		std::cout << "Recording PID: " << pid_child << std::endl;
		setId(pid_child);

		return pid_child;
	}

}


int GenericRecorder::stopRecording()
{

	std::cout << "Killing recording process (" << getId() << ") ... " << std::endl;

	int ret = kill(getId(), SIGKILL );

	std::cout << "Ret = " << ret << std::endl;

	return ret;

}
