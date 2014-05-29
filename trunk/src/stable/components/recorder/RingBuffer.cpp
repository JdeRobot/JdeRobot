/*
 *  Copyright (C) 2014 JdeRobot developers
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
 *  Authors : Roberto Calvo Palomino <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include "RingBuffer.h"

namespace recorder
{

RingBuffer::RingBuffer(long int maxTime)
{
	mMaxBufferTime = maxTime;
}

RingBuffer::~RingBuffer()
{

	for (std::vector<RingNode>::iterator it = mBuffer.begin(); it < mBuffer.end(); it++ )
	{
		it->frame.release();
	}

	mBuffer.clear();
}

bool RingBuffer::addNode(RingNode node)
{

	mBuffer.push_back(node);
	return checkBuffer();
}

static void *write_thread(void* context){

	((RingBuffer *)context)->write_th();

	pthread_exit(NULL);
	return NULL;
}

void RingBuffer::write_th()
{
	boost::posix_time::ptime init = boost::posix_time::microsec_clock::local_time();
	for (std::vector<RingNode>::iterator it = mWriteBuffer.begin(); it < mWriteBuffer.end(); it++ )
	{
		std::stringstream path;
		path << "data-" << mNameLog << "/images/camera" << it->cameraId << "/" << it->relativeTime << ".png";
		cv::imwrite(path.str(), it->frame, mCompression);
	}
	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();

	boost::posix_time::time_duration total = end - init;
	std::cout << "Total milliseconds spent: " << total.total_milliseconds() << " - " << "Total Size: " << mBuffer.size() << std::endl;

	for (std::vector<RingNode>::iterator it = mWriteBuffer.begin(); it < mWriteBuffer.end(); it++ )
		it->frame.release();


	mWriteBuffer.clear();

}

void RingBuffer::write(std::string nameLog, std::vector<int> compression)
{
	mCompression = compression;
	mNameLog = nameLog;

	mWriteBuffer.resize(mBuffer.size());
	std::copy( mBuffer.begin(), mBuffer.end(), mWriteBuffer.begin() );

	//std::cout << &(mBuffer[0].frame) << std::endl;
	//std::cout << &(mWriteBuffer[0].frame) << std::endl;

	pthread_attr_init(&mAttr);
	pthread_attr_setdetachstate(&mAttr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&mThread, &mAttr, write_thread, this);


}

bool RingBuffer::checkBuffer()
{


	for (std::vector<RingNode>::iterator it = mBuffer.begin(); it < mBuffer.end(); it++ )
	{
		std::vector<RingNode>::iterator newer = mBuffer.end()-1;
		//jderobot::Logger::getInstance()->info("Current: " + boost::lexical_cast<std::string>(it->relativeTime) +
		//		"  -  Newer: " + boost::lexical_cast<std::string>(newer->relativeTime));


		if ( ( newer->relativeTime - it->relativeTime) > mMaxBufferTime )
		{
			it->frame.release();
			mBuffer.erase(it);
			return true;
		}
		else
		{
			//jderobot::Logger::getInstance()->info("Older: " + boost::lexical_cast<std::string>(mBuffer.begin()->relativeTime) +
			//			"  -  Newer: " + boost::lexical_cast<std::string>((mBuffer.end()-1)->relativeTime));

			return false;
		}
	}

	return true;

}

}
