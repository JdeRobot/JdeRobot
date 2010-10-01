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

#include <jderobot/image.h>
#include <jderobotice/context.h>
#include <jderobotice/subsystemthread.h>
#include <colorspaces/colorspacesmm.h>
#include <memory>
#include "model.h"
#include "controller.h"

namespace bgfglab {
  class MainThread: public jderobotice::SubsystemThread {
  public:
    MainThread(const jderobotice::Context &context);
  private:
    virtual void initialize();
    virtual void work();
    virtual void finalize();
    
    //copy image to local variable
    void getImage();

    //image source: 0 remote, 1 local
    int imgSource;
    std::string localSourcePath;//local source path
    int localSourceSeqCounter;

    //imageprovider interface
    jderobot::ImageProviderPrx imagePrx;
    colorspaces::Image::FormatPtr fmt;

    //context within we are running
    jderobotice::Context context;

    std::auto_ptr<Model> model;//data container
    std::auto_ptr<Controller> controller;

    bool stopAfterDumpFinished;//controlls if thread is stoped after dumping finished
  };
}
