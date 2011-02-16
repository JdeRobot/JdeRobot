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


#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include "mainthread.h"

namespace bgfglab{
  class Component: public jderobotice::Component{
  public:
    Component()
      : jderobotice::Component("BGFGlab") {}

    virtual void start() {
      mainThread = new MainThread( context() );
      mainThread->start();
    }

    virtual void stop() {
      context().tracer().debug( "stopping main thread");
      gbxiceutilacfr::stopAndJoin( mainThread );
      context().tracer().debug( "stopped main thread");
      
      mainThread = 0;
    }
  private:
    gbxiceutilacfr::ThreadPtr mainThread;
  };
}


int main(int argc, char **argv){
  bgfglab::Component component;

  jderobotice::Application app( component );
  return app.jderobotMain(argc, argv);
}
