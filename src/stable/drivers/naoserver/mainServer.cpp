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
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef _WIN32
# include <signal.h>
#endif

#include "alcore/altypes.h"
#include "altools/alxplatform.h"
#include "alcore/alptr.h"
#include "alcommon/albroker.h"
#include "alcommon/almodule.h"
#include "alcommon/albrokermanager.h"
#include "alcommon/altoolsmain.h"

using namespace std;
using namespace AL;

//<EXE_INCLUDE> don't remove this comment
#include "naoserver.h"

//</EXE_INCLUDE> don't remove this comment

//<ODECLAREINSTANCE> don't remove this comment

//</ODECLAREINSTANCE> don't remove this comment

#ifdef HELLOWORLD_IS_REMOTE
# define ALCALL
#else
// when not remote, we're in a dll, so export the entry point
# ifdef _WIN32
#  define ALCALL __declspec(dllexport)
# else
#  define ALCALL
# endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    ALCALL int _createModule ( ALPtr<ALBroker> broker ) {
        // init broker with the main broker instance
        // from the parent executable
        AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
        AL::ALBrokerManager::getInstance()->addBroker(broker);
        // create module instances
    //    AL::ALModule::createModule<NaoServer>(broker, "NaoServer");
        // create modules instance
        //<OGETINSTANCE> don't remove this comment
	    ALModule::createModule<NaoServer>(broker,"NaoServer");

	    //</OGETINSTANCE> don't remove this comment
        
        return 0;
    }

    ALCALL int _closeModule () {
        // Delete module instance
	    //<OKILLINSTANCE> don't remove this comment

	    //</OKILLINSTANCE> don't remove this comment
        
        return 0;
    }
  
# ifdef __cplusplus
} // extern "C"
# endif

int main(int argc, char* argv[])
{
    // pointer to createModule
    TMainType sig;
    sig = &_createModule;
    // call main
    return ALTools::mainFunction("NaoServer", argc, argv, sig);
}
