/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include "time.h"

namespace jderobotutil{
  IpsCounter::IpsCounter()
    : _ips(0.0), counter(0) { 
    gettimeofday(&timer,0);
  }

  void IpsCounter::inc(){ 
    counter++;
  }

  double IpsCounter::ips() const{
    struct timeval now;
    gettimeofday(&now,0);
    long elapsedS = now.tv_sec - timer.tv_sec;

    if (elapsedS >= countInterval){//calculate ips and reset timer
      long elapsedUS = elapsedS*1000000.0 + (now.tv_usec - timer.tv_usec);
      _ips = counter*1000000.0/elapsedUS;
      //reset timer
      counter = 0;
      timer = now;
    }
    return _ips;
  }
  
}//namespace
