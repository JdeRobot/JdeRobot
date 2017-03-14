/*
*  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef poolWritePose3d_H_
#define poolWritePose3d_H_

#include <pthread.h>
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <time.h>
#include <jderobot/pose3d.h>
#include <fstream>



namespace recorder{

struct pose3d{
	float x;
	float y;
	float z;
	float h;
	float q0;
	float q1;
	float q2;
	float q3;
};


class poolWritePose3d {
public:
	poolWritePose3d(jderobot::Pose3DPrx prx, int freq, int poolSize, int pose3dID);
	virtual ~poolWritePose3d();
	bool getActive();
	//void produceImage(cv::Mat image, long long int it);
	void consumer_thread();
	void producer_thread(struct timeval inicio);


private:
	pthread_mutex_t mutex;
	std::vector<pose3d> pose3dVec;
	std::vector<long long int> its;
	int poolSize;
	int pose3dID;
	bool active;
	struct timeval lastTime;
	int freq;
	float cycle;
	jderobot::Pose3DPrx prx;
	std::ofstream outfile;


	//threads

};
} //NAMESPACE
#endif /* poolWritePose3d_H_ */
