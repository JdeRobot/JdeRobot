/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include "gazebo.hh"
#include "common/common.h"
#include <stdio.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <Printer.h>

using namespace std;
using namespace Demo;
// using namespace Ice;





void *showGui(void*) {

    Ice::CommunicatorPtr ic;
    int dummy = 0;

    try {
        
        ic = Ice::initialize(dummy, 0);
        Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter:default -p 10000");
        PrinterPrx printer = PrinterPrx::checkedCast(base);
        if (!printer)
            throw "Invalid proxy";

        printer->printString("Hello World!");
    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    if (ic)
        ic->destroy();
};


namespace gazebo
{  
  class HelloWorld : public WorldPlugin
  {
    public: HelloWorld() : WorldPlugin() 
            {
	      int count = 0;
	      
	      if (count == 0){
		pthread_t thr_gui;
		pthread_create(&thr_gui, NULL, &showGui, NULL);
	      }
	      

	      
            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {

	      
	    }

  };
  GZ_REGISTER_WORLD_PLUGIN(HelloWorld)
}
