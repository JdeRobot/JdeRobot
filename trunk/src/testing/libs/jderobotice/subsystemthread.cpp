/*
 * Orca-Robotics Project: Components for robotics 
 *               http://orca-robotics.sf.net/
 * Copyright (c) 2004-2009 Alex Brooks, Alexei Makarenko, Tobias Kaupp
 *
 * This copy of Orca is licensed to you under the terms described in
 * the LICENSE file included in this distribution.
 *
 * 2010
 * This code is part of orcaice library from Orca-Robotics Project.
 * It has been modified by David Lobato <dav.lobato@gmail.com> and 
 * included in jderobot project.
 */

#include <iostream>
#include <sstream>

#include "subsystemthread.h"
#include "exceptions.h"
#include <Ice/Ice.h>

using namespace jderobotice;
using namespace std;

SubsystemThread::SubsystemThread( gbxutilacfr::Tracer& tracer, 
                                  gbxutilacfr::Status& status, 
                                  const std::string& subsysName,
                                  double maxHeartbeatIntervalSec ) : 
  tracer_(tracer),
  status_(status),
  health_( status, subsysName ) 
{
  status_.addSubsystem( health_.name(), maxHeartbeatIntervalSec );
}

SubsystemThread::~SubsystemThread()
{
  status_.removeSubsystem( health_.name() );
}

void 
SubsystemThread::setMaxHeartbeatInterval( double interval ) 
{ 
  status_.setMaxHeartbeatInterval( health_.name(), interval ); 
}

void 
SubsystemThread::setSubsystemType( gbxutilacfr::SubsystemType type ) 
{ 
  status_.setSubsystemType( health_.name(), type ); 
}

std::string 
SubsystemThread::subsysName() const 
{ 
  return health_.name(); 
}

void 
SubsystemThread::run()
{
  stringstream ss;
  try
    {
      protectedRun();
    }
  catch ( const Ice::CommunicatorDestroyedException & )
    {
      // This is OK: it means that the communicator shut down (eg via Ctrl-C)
    }
  catch ( const jderobotice::ComponentDeactivatingException& )
    {
      // This is OK: it means the component was told to shutdown
    }
  catch ( const std::exception &e )
    {
      ss << "SubstatusThread::run() "<<subsysName()<<": Caught unexpected exception: " << e.what();
    }
  catch ( const std::string &e )
    {
      ss << "SubstatusThread::run() "<<subsysName()<<": Caught unexpected string: " << e;
    }
  catch ( const char *e )
    {
      ss << "SubstatusThread::run() "<<subsysName()<<": Caught unexpected char *: " << e;
    }
  catch ( ... )
    {
      ss << "SubstatusThread::run() "<<subsysName()<<": Caught unexpected unknown exception.";
    }

  // report status fault if there was an exception and we are not stopping
  if ( !ss.str().empty() ) {
    if ( !isStopping() )
      health_.fault( ss.str() );
    else
      tracer_.warning( subsysName()+": (while stopping subsystem thread): "+ss.str() );
  }
  else {
    tracer_.info( subsysName()+": dropping out from run() " );
  }

  // wait for somebody to realize that we are quitting and tell us to stop.
  waitForStop();
}

void 
SubsystemThread::protectedRun()
{
  // init: Idle --> Initialising
  status_.initialising( health_.name() );
  initialize();
    
  if ( !isStopping() )
    {
      // finished: Initialising --> Working
      status_.working( health_.name() );
      work();

      // finished: Working --> Finalising
      status_.finalising( health_.name() );
      finalize();
    }
  else 
    {
      // finished: Initialising --> Finalising
      status_.finalising( health_.name() );
      finalize();
    }

  // finished: Finalising --> Shutdown
  status_.setSubsystemStatus( health_.name(), gbxutilacfr::SubsystemShutdown, gbxutilacfr::SubsystemOk );
}
