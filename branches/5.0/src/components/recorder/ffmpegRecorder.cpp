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

#include "ffmpegRecorder.h"
#include <unistd.h>

void *record_function( void *ptr );

ffmpegRecorder::ffmpegRecorder(const jderobotice::Context& context) : GenericRecorder(context)
{

}

int ffmpegRecorder::startRecording()
{


	getContext().tracer().info ( "starting recording: Path = " +
								 getConfig()->path + " - FrameRate = " +
								 getConfig()->frameRate + " fps - ");


	//char *const parmList[] = {"mencoder","tv://","-tv", "driver=v4l:width=320:height=240:device=/dev/video0", "-ovc", "lavc", "-fps", "15.0", "-o", "webcam-15.mpg", NULL};

	std::string v4lVersion, resolution;

	if (getConfig()->v4lVersion == "v4l")
		v4lVersion = "video4linux";
	else
		v4lVersion = "video4linux2";

	resolution = getConfig()->width + "x" + getConfig()->height;

	char *parmList[] = {"ffmpeg","-y","-f","alsa","-r","16000","-f",
						(char*) v4lVersion.c_str(),"-s",(char*) resolution.c_str(),
						"-i",(char*) (getConfig()->v4lDevice).c_str(),
						"-r",(char*) getConfig()->frameRate.c_str(),
						"-f","avi","-vcodec","mpeg4","-vtag","xvid","-sameq","-acodec","libmp3lame","-ab","96k",
						(char*) (getConfig()->path).c_str(),NULL};

	pid_t pid;

	int descPipe [2];
	pipe (descPipe);
	char buffer[100];

	if ( fork()==0 )
	{
		/* 0 it's read side */
		close (descPipe[0]);

		// Obtain the PID
		int mypid = getpid();
		std::cout << "PID= " << mypid << std::endl;

		// Send my PID to father
		write (descPipe[1], &mypid, sizeof(int));

		// Execute command
		execv("/usr/bin/ffmpeg", parmList);
		return -1;
	}
	else
	{
		int pid_child = 0;

		/* 0 it's write side */
		close (descPipe[1]);

		// Read the PID
		read (descPipe[0], &pid_child, sizeof(int));

		std::cout << "Soy el padre:" << pid_child << std::endl;
		setId(pid_child);

		return pid_child;
	}


}

int ffmpegRecorder::stopRecording()
{

	return 0;
}




