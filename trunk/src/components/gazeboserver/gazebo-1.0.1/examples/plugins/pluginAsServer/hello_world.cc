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

class PrinterI : public Printer {
public:
    virtual void printString(const string& s, const Ice::Current&);
};

void 
PrinterI::
printString(const string& s, const Ice::Current&)
{
    cout << s << endl;
}



void *showGui(void*) {

    Ice::CommunicatorPtr ic;
    int dummy = 0;

    try {
        
        ic = Ice::initialize(dummy, 0);
        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -p 10000");
        Ice::ObjectPtr object = new PrinterI;
        adapter->add(object, ic->stringToIdentity("SimplePrinter"));
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& e) {
        cerr << e << endl;
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            cerr << e << endl;
        }
    }
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
