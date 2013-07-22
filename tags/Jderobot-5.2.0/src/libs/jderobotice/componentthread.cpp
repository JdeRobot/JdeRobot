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

#include "componentthread.h"
#include "catchutils.h"
#include "exceptions.h"
#include <iostream>
#include <IceGrid/Registry.h>

using namespace std;

namespace jderobotice {
  ComponentThread::ComponentThread( ComponentAdapterActivationPolicy adapterPolicy,
				    const jderobotice::Context& context ) :
    SafeThread(context.tracer()),
    adapterPolicy(adapterPolicy),
    context(context) {}

  void
  ComponentThread::walk()
  {    
    context.status().infrastructureInitialising();

    Ice::PropertiesPtr props = context.properties();

    const int sleepIntervalMs = 1000;

    context.status().infrastructureWorking();

    //
    // activate component's adapter
    //
    if ( adapterPolicy == AdapterAutoActivation ) {
      // not supplying subsystem name because we are in a special Infrastructure subsystem.
      context.activate( this );
      context.tracer().info( "Component infrastructure: adapter activated." );
    }

    try {
      while ( !isStopping() ){
	context.tracer().info( "Component infrastructure: nothing left to do, quitting" );
	return;
	//IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(sleepIntervalMs));
      }
    }
    catch ( ... ){
      jderobotice::catchExceptions( context.tracer(), "running component utility thread" );
    }
    
    context.status().infrastructureFinalising();
  }
}
