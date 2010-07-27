/*
 * Orca-Robotics Project: Components for robotics 
 *               http://orca-robotics.sf.net/
 * Copyright (c) 2004-2009 Alex Brooks, Alexei Makarenko, Tobias Kaupp
 *
 * This copy of Orca is licensed to you under the terms described in
 * the LICENSE.orca file included in this distribution.
 *
 * 2010
 * This code is part of orcaice library from Orca-Robotics Project.
 * It has been modified by David Lobato <dav.lobato@gmail.com> and 
 * included in jderobot project.
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/exceptions.h>
#include "catchutils.h"
#include "exceptions.h"

// #include <iostream>
#include <sstream>
using namespace std;

namespace jderobotice
{

std::string catchExceptions( gbxutilacfr::Tracer& tracer, const std::string& activity )
{
    stringstream exceptionSS;
    if ( !activity.empty() )
        exceptionSS << "(while " + activity + ") ";

    try {
        // re-throw 
        throw;
    }
    catch ( Ice::CommunicatorDestroyedException & ) {
        // This is OK: it means that the communicator shut down (eg via Ctrl-C)
        // somewhere in mainLoop. Eventually, component will tell us to stop.
        return string();
    }
    catch ( const jderobotice::ComponentDeactivatingException & e ) {
        // this is OK.
        return string();
    }
    catch ( const jderobot::JderobotException &e )
    {
        exceptionSS << "caught unexpected exception: " << e << ": " << e.what;
    }
    catch ( const std::exception &e ) {
        exceptionSS << "caught unexpected exception: " << e.what();
    }
    catch ( const std::string &e ) {
        exceptionSS << "caught unexpected string: " << e;
    }
    catch ( const char *e ) {
        exceptionSS << "caught unexpected char *: " << e;
    }
    catch ( ... ) {
        exceptionSS << "caught unexpected unknown exception.";
    }

    // empty string indicates that the problem can be ignored status-wise.
    if ( !exceptionSS.str().empty() )
        tracer.warning( exceptionSS.str() );

    return exceptionSS.str();
}

std::string catchExceptionsWithSleep( gbxutilacfr::Tracer& tracer, const std::string& activity, 
            int sleepIntervalMSec )
{    
    string problem = catchExceptions( tracer, activity );

    if ( sleepIntervalMSec>0 ) {
        stringstream ss; ss << "Will try again after " << sleepIntervalMSec << " ms";
        tracer.info( ss.str() );
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(sleepIntervalMSec));
    }

    return problem;
}

std::string catchExceptionsWithStatus( const std::string& activity, 
            gbxutilacfr::SubHealth& subHealth, 
            gbxutilacfr::SubsystemHealth newHealth )
{
    stringstream exceptionSS;

    string fullActivity;
    if ( !activity.empty() )
        fullActivity = "(while " + activity + ") ";

    try {
        // re-throw 
        throw;
    }
    catch ( Ice::CommunicatorDestroyedException & ) {
        // This is OK: it means that the communicator shut down (eg via Ctrl-C)
        // somewhere in mainLoop. Eventually, component will tell us to stop.
        return string();
    }
    catch ( const jderobotice::ComponentDeactivatingException & e ) {
        // this is OK.
        return string();
    }
    catch ( const jderobot::JderobotException &e )
    {
        exceptionSS << "caught unexpected exception: " << e << ": " << e.what;
    }
    catch ( const std::exception &e ) {
        exceptionSS << "caught unexpected exception: " << e.what();
    }
    catch ( const std::string &e ) {
        exceptionSS << "caught unexpected string: " << e;
    }
    catch ( const char *e ) {
        exceptionSS << "caught unexpected char *: " << e;
    }
    catch ( ... ) {
        exceptionSS << "caught unexpected unknown exception.";
    }

    switch ( newHealth )
    {
    case gbxutilacfr::SubsystemFault :
        subHealth.fault( fullActivity + exceptionSS.str() );
        break;
    case gbxutilacfr::SubsystemWarning :
        subHealth.warning( fullActivity + exceptionSS.str() );
        break;
    case gbxutilacfr::SubsystemOk :
        subHealth.ok( fullActivity + exceptionSS.str() );
        break;
    default :
        assert( !"Unknown subsystem health type" );
    }

    return ( fullActivity + exceptionSS.str() );
}

std::string catchExceptionsWithStatusAndSleep( const std::string& activity, 
            gbxutilacfr::SubHealth& subHealth, 
            gbxutilacfr::SubsystemHealth newHealth, 
            int sleepIntervalMSec )
{    
    string problem = catchExceptionsWithStatus( activity, subHealth, newHealth );

    // Slow things down in case of persistent error
    if ( sleepIntervalMSec>0 ) {
        // should we get the tracer in here?
//         stringstream ss; ss << "Will try again after " << sleepIntervalMSec << " ms";
//         tracer.info( ss.str() );
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(sleepIntervalMSec));
    }

    return problem;
}

void catchMainLoopExceptions( gbxutilacfr::SubHealth& subHealth )
{
    catchExceptionsWithStatusAndSleep( string("running in main loop"), subHealth, gbxutilacfr::SubsystemFault );
}

} // namespace
