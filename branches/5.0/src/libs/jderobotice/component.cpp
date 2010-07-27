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

#include <string>
#include <jderobot/common.h>
#include <jderobotutil/jderobotutil.h>

#include "jderobotice.h"
#include "component.h"
#include "componentthread.h"
#include "tracerImpl.h"
#include "statusImpl.h"
#include "catchutils.h"
// #include "detail/componentInfoImpl.h"
// #include "detail/statusImpl.h"

// debug only
#include <iostream>
#include <tr1/memory>
using namespace std;

namespace jderobotice {

  class Component::PImpl{
  public:
    std::tr1::shared_ptr<TracerI> tracer;
    std::tr1::shared_ptr<StatusI> status;
    ComponentAdapterActivationPolicy adapterPolicy;
    std::tr1::shared_ptr<Context> context;
    gbxiceutilacfr::ThreadPtr componentThread;
  };

  Component::Component( const std::string& tag,
			ComponentAdapterActivationPolicy adapterPolicy )
    : pImpl(new PImpl()) {
    
    pImpl->tracer.reset(new TracerI());
    pImpl->status.reset(new StatusI(*pImpl->tracer));

    pImpl->adapterPolicy = adapterPolicy;

    /*context build*/
    pImpl->context.reset(new Context(tag,pImpl->tracer,pImpl->status));
  }

  //
  // IMPORTANT! This destructor must be here (in the .cpp file)
  // Otherwise, the destructors of the forward-declared types will not be called.
  //
  Component::~Component() {
  }

  void Component::activate(){ pImpl->context->activate(); }

  const Context& Component::context() const { return *(pImpl->context); }

  void
  Component::init( const jderobot::FQComponentName& name,
		   const bool isApp,
		   const Ice::ObjectAdapterPtr& adapter ) {
    // set context with component info
    // this is the only storage of this info
    pImpl->context->init( name, isApp, adapter);

    
    //create infrastructure thead
    pImpl->componentThread = new ComponentThread( pImpl->adapterPolicy, context() );
    try {
      pImpl->componentThread->start();
    }
    catch ( ... ){
      jderobotice::catchExceptions( pImpl->context->tracer(), "starting component utility thread" );
      pImpl->context->shutdown();
    }
    pImpl->context->tracer().print( pImpl->context->tag() + ": Component infrastructure thread created." );
  };

  void
  Component::finalize(){
    if ( pImpl->componentThread ){
      pImpl->context->tracer().debug( "jderobotice::Component: stopping ComponentThread....", 2 );
      gbxiceutilacfr::stopAndJoin( pImpl->componentThread );
      pImpl->context->tracer().debug( "jderobotice::Component: ComponentThread stopped.", 2 );
    }
  }

  void Component::setTag( const std::string& t ) { pImpl->context->setTag(t); };

  const std::string
  Component::help( const std::string& executable ) const
  {
    // strip possible extension
    // std::string exec = hydroutil::basename( executable, true );
    std::string s;
    // s += "Standard usage:\n";
    //     s += "  " + executable + " --help           Prints this help and exists.\n";
    //     s += "  " + executable + " --version        Prints version info and exists.\n";
    //     s += "  " + executable + " configfile.cfg   Same as "+executable+" --Jderobot.Config=configfile.cfg\n";
    //     s += "  " + executable + "                  Same as "+executable+" --Jderobot.Config="+exec+".cfg\n";
    return s;
  }
} // namespace
