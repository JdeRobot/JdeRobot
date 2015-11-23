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
#include <boost/thread/thread.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <log/Logger.h>
#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

namespace recorder
{

class RingBuffer
{
	public:

		class RingNode
		{
		public:
			long long int relativeTime;
			cv::Mat frame;
			int cameraId;
		};

		RingBuffer(long int maxTime);
		~RingBuffer();

		bool addNode(RingNode node);
		void write(std::string nameLog, std::vector<int> compression);

		void write_th();

	private:

		bool checkBuffer();

		long int mMaxBufferTime;
		std::vector<RingNode> mBuffer;
		std::vector<RingNode> mWriteBuffer;
		std::vector<int> mCompression;

		pthread_t mThread;
		pthread_attr_t mAttr;

		std::string mNameLog;

};


}
