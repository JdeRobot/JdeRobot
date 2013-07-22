/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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

#ifndef JDEROBOTICE_COMPONENT_STATUS_H
#define JDEROBOTICE_COMPONENT_STATUS_H

#include <gbxutilacfr/trivialstatus.h>
#include <gbxutilacfr/tracer.h>

namespace jderobotice{
  
  /**
   * Use trivial implementation until we have something better
   */
  class StatusI: public gbxutilacfr::TrivialStatus{
  public:
    StatusI(gbxutilacfr::Tracer& tracer, bool stateChange=true, bool ok=false, 
	    bool warn=true, bool fault=true, bool heartbeat=false)
      : gbxutilacfr::TrivialStatus(tracer,stateChange,ok,warn,fault,heartbeat) {}
  };
}

#endif
