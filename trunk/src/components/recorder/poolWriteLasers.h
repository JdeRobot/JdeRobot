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

#ifndef POOLWRITELASERS_H_
#define POOLWRITELASERS_H_

#include <pthread.h>
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <time.h>
#include <jderobot/laser.h>
#include <colorspaces/colorspacesmm.h>
#include <fstream>



namespace recorder{


class poolWriteLasers {
public:
	poolWriteLasers(jderobot::LaserPrx prx, int freq, int poolSize, int laserID);
	virtual ~poolWriteLasers();
	bool getActive();
	//void produceImage(cv::Mat image, long long int it);
	void consumer_thread();
	void producer_thread(struct timeval inicio);


private:
	pthread_mutex_t mutex;
	std::vector<std::vector <int> > lasers;
	std::vector<long long int> its;
	int poolSize;
	int laserID;
	bool active;
	struct timeval lastTime;
	int freq;
	float cycle;
	jderobot::LaserPrx prx;
	std::ofstream outfile;


	//threads

};
} //NAMESPACE

#endif /* POOLWRITELASERS_H_ */
