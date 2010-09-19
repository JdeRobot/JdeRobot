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

#include <string>
#include <sstream>
#include <iostream>
#include "viewtext.h"
#include "model.h"

namespace bgfglab {
  ViewText::ViewText(Controller &controller) throw ()
    : View(controller) {}

  void ViewText::update(const jderobotutil::Subject* o, jderobotutil::ObserverArg* arg)
    throw (gbxutilacfr::Exception) {

    int dumpedFrames = 0;
    controller.model().isDumpingData(&dumpedFrames);

    std::cout << controller.model().bgModelIps().ips() << " iterations/s | " << dumpedFrames << " frames dumped        \r";
    
    //Actions are done here, while in update method we are in bgfglab thread
    
  }

}//namespace
