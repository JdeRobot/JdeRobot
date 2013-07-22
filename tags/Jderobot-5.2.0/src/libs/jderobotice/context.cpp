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
#include <Ice/LocalException.h>
#include <gbxsickacfr/gbxiceutilacfr/threadutils.h>
#include <sstream>
#include "exceptions.h"
#include "context.h"

namespace jderobotice {
  Context::Context(const std::string& tag,
		   std::tr1::shared_ptr<gbxutilacfr::Tracer> tracer,
		   std::tr1::shared_ptr<gbxutilacfr::Status> status)
    : tag_(tag),tracer_(tracer),status_(status) {}

  /*destructor defined where PImpl is complete, so auto_ptr doesn't 
    bother with incomplete type warnings*/
  Context::~Context() {};
  
  Context::Context() {};

  void Context::setTag( const std::string& t ) { tag_ = t; };

  const std::string& Context::tag() const { return tag_; };

  const jderobot::FQComponentName& Context::name() const { return name_; };

  bool Context::isApplication() const { return isApplication_; };

  Ice::CommunicatorPtr Context::communicator() const { return communicator_; };

  Ice::ObjectAdapterPtr Context::adapter() const { return adapter_; };
  
  Ice::PropertiesPtr Context::properties() const { return communicator_->getProperties(); };

  gbxutilacfr::Tracer& Context::tracer() const { return *(tracer_); };

  gbxutilacfr::Status& Context::status() const { return *(status_); };

  void 
  Context::init( const jderobot::FQComponentName &name,
		 const bool isApp,
		 const Ice::ObjectAdapterPtr &adapter ){
    name_ = name;
    isApplication_ = isApp;
    adapter_ = adapter;
    communicator_ = adapter->getCommunicator();
  };

  void
  Context::activate(){
    tracer_->print( "Activating adapter..." );
    try{
      // This next line was to work around an Ice3.2 bug.
      // See: http://www.zeroc.com/forums/help-center/3266-icegrid-activationtimedout.html#post14380
      // communicator_->setDefaultLocator(Ice::LocatorPrx::uncheckedCast(communicator_->getDefaultLocator()->ice_collocationOptimized(false)));
      
      adapter_->activate();
      tracer_->print( "Adapter activated" );
    }
    catch ( Ice::DNSException& e ){
      std::stringstream ss;
      ss << "(while activating jderobotice::Component) \n"<<e<<"\nCheck network.";
      throw jderobotice::NetworkException( ERROR_INFO, ss.str() );
    }
    catch ( Ice::ConnectionRefusedException& e ){
      bool requireRegistry = 
	properties()->getPropertyAsInt( "Jderobot.Component.RequireRegistry" );
      if ( requireRegistry ) {
	std::stringstream ss; 
	ss<<"(while activating jderobotice::Component) failed: \n"
	  <<e<<"\nCheck IceGrid Registry.";
	ss<<"\nYou may allow to continue by setting Jderobot.Component.RequireRegistry=0.";
	throw jderobotice::NetworkException( ERROR_INFO, ss.str() );
      }else{
	std::stringstream ss; 
	ss<<"(while activating orcaice::Component) failed:\n"<<e;
	tracer_->print( ss.str() );
	tracer_->print("Continuing, but only direct outgoing connections will be possible." );
	tracer_->print("You may enforce registration by setting Jderobot.Component.RequireRegistry=1." );
      }
    }
    catch ( Ice::ConnectFailedException& e ){
      bool requireRegistry = 
	properties()->getPropertyAsInt( "Jderobot.Component.RequireRegistry" );
      if ( requireRegistry ) {
	std::stringstream ss; 
	ss<<"(while activating jderobotice::Component) failed: \n"
	  <<e<<"\nCheck IceGrid Registry.";
	ss<<"\nYou may allow to continue by setting Jderobot.Component.RequireRegistry=0.";
	throw jderobotice::NetworkException( ERROR_INFO, ss.str() );
      }else{
	std::stringstream ss; 
	ss<<"(while activating orcaice::Component) failed:\n"<<e;
	tracer_->print( ss.str() );
	tracer_->print("Continuing, but only direct outgoing connections will be possible." );
	tracer_->print("You may enforce registration by setting Jderobot.Component.RequireRegistry=1." );
      }
    }
    catch( const Ice::ObjectAdapterDeactivatedException& e ){
      std::stringstream ss;
      ss << "(while activating jderobotice::Component) failed: component is deactivating: "
	 << e;
      throw jderobotice::NetworkException( ERROR_INFO, ss.str() );
    }
    catch( const Ice::Exception& e ){
      std::stringstream ss; 
      ss<<"jderobotice::Component: Failed to activate component: "
	<<e<<"\nCheck IceGrid Registry.";
      throw jderobotice::NetworkException( ERROR_INFO, ss.str() );
    }
  }

  void Context::createInterfaceWithString( Ice::ObjectPtr& object,
					   const std::string& proxyString) const{
    try{
      // register object with the adapter
      adapter()->add( object, communicator()->stringToIdentity(proxyString) );
    }
    catch ( const Ice::AlreadyRegisteredException &e ){
      std::stringstream ss;
      ss<<"Failed to create interface "<<proxyString<<": "<<e;
      throw gbxutilacfr::Exception( ERROR_INFO, ss.str() );
    }
    catch( const Ice::ObjectAdapterDeactivatedException &e ){
      std::stringstream ss;
      ss << "jderobotice::Component: Failed to create interface because the adapter is destroyed : " << e;
      tracer().warning( ss.str() );
      throw jderobotice::ComponentDeactivatingException( ERROR_INFO, ss.str() );
    }

    // locally register this interface with Home interface 
    //orca::ProvidedInterface iface;
    //iface.name = proxyString;
    // this is a local call
    //iface.id   = object->ice_id();
    //home().addProvidedInterface( iface );
  }

  void Context::createInterfaceWithString( Ice::ObjectPtr& object,
					   const std::string& name,
					   gbxutilacfr::Stoppable* activity, 
					   const std::string& subsysName, 
					   int retryIntervalSec, int retryNumber ) const{
    assert( activity && "Null activity pointer" );

    int count = 0;
    while ( !activity->isStopping() && ( retryNumber<0 || count<retryNumber) ){
      try {
	createInterfaceWithString( object, name );
	if ( !subsysName.empty() )
	  status().ok( subsysName );
	break;
      }
      catch ( const std::exception& e ) {
	std::stringstream ss;
	ss << "Failed to create interface with string "<<name<<": " << e.what() << std::endl
	   <<"Will retry in "<<retryIntervalSec<<"s.\n";
	if ( !subsysName.empty() )
	  status().warning( subsysName, ss.str() );
	else
	  tracer().warning( subsysName, ss.str() );
      }
      catch ( ... ) {
	std::stringstream ss;
	ss << "Caught something while creating interface with string "<<name<<". " << std::endl
	   <<"Will retry in "<<retryIntervalSec<<"s.\n";
	if ( !subsysName.empty() )
	  status().warning( subsysName, ss.str() );
	else
	  tracer().warning( subsysName, ss.str() );
      }
      ++count;
      gbxiceutilacfr::checkedSleep( activity, retryIntervalSec*1000 );
    }
  }

  // void createInterfaceWithTag( Ice::ObjectPtr& object,
// 			       const std::string& interfaceTag){
//     // look up naming information in the properties
//     orca::FQInterfaceName fqIName = getProvidedInterface( context, ifaceTag );

//     createInterfaceWithString( context, object, fqIName.iface );
//   }

//   void createInterfaceWithTag( Ice::ObjectPtr& object,
// 			       const std::string& interfaceTag,
// 			       gbxutilacfr::Stoppable* activity, const std::string& subsysName, 
// 			       int retryIntervalSec, int retryNumber ){
//     assert( activity && "Null activity pointer" );

//     int count = 0;
//     while ( !activity->isStopping() && ( retryNumber<0 || count<retryNumber) ){
//       try {
// 	createInterfaceWithTag( context, object, interfaceTag );
// 	if ( !subsysName.empty() )
// 	  status().ok( subsysName );
// 	break;
//       }
//       catch ( const std::exception& e ) {
// 	std::stringstream ss;
// 	ss << "Failed to create interface with tag "<<interfaceTag<<" : " << e.what() << std::endl
// 	   <<"Will retry in "<<retryIntervalSec<<"s.";
// 	if ( !subsysName.empty() )
// 	  status().warning( subsysName, ss.str() );
// 	else
// 	  tracer().warning( subsysName, ss.str() );
//       }
//       catch ( ... ) {
// 	std::stringstream ss;
// 	ss << "Caught something while creating interface with tag "<<interfaceTag<<". "<<std::endl
// 	   <<"Will retry in "<<retryIntervalSec<<"s.\n";
// 	if ( !subsysName.empty() )
// 	  status().warning( subsysName, ss.str() );
// 	else
// 	  tracer().warning( subsysName, ss.str() );
//       }
//       ++count;
//       gbxiceutilacfr::checkedSleep( activity, retryIntervalSec*1000 );
//     }
//   }

  void Context::activate( gbxutilacfr::Stoppable* activity, const std::string& subsysName, 
			  int retryIntervalSec, int retryNumber ){
    assert( activity && "Null activity pointer" );
    
    int count = 0;
    while ( !activity->isStopping() && ( retryNumber<0 || count<retryNumber) ){
      try {
	activate();
	break;
      }
      catch ( Ice::CommunicatorDestroyedException ){
	// This means we're shutting down.
	break;
      }
      catch ( const std::exception& e ) {
	std::stringstream ss;
	ss << "Failed to activate component : "<<e.what()<<std::endl
	   <<"Will retry in "<<retryIntervalSec<<"s.\n";
	tracer().warning( subsysName, ss.str() );
      }
      catch ( ... ) {
	std::stringstream ss;
	ss << "Caught something while activating. " << std::endl
	   <<"Will retry in "<<retryIntervalSec<<"s.\n";
	tracer().warning( subsysName, ss.str() );
      }
      ++count;
      gbxiceutilacfr::checkedSleep( activity, retryIntervalSec*1000 );
      if ( !subsysName.empty() )
	status().heartbeat( subsysName );
    }
  }


  void
  Context::shutdown() const{
    if ( !communicator_ )
      throw gbxutilacfr::Exception( ERROR_INFO, "Trying to shutdown component before context initializition." );

    if ( isApplication() ) {
      tracer().print( "Triggering component shutdown ...");
      communicator()->shutdown();
    }
    else {
      tracer().print( "NOT triggering component shutdown because the component is running as an IceBox service ...");
    }
  }

  bool 
  Context::isDeactivating(){
    try {
      adapter()->getCommunicator();
      return false;
    } 
    catch (const Ice::ObjectAdapterDeactivatedException&) {
      return true;
    }
  }

  std::string 
  Context::toString() const{
    std::stringstream ss;
    ss<<"tag="<<tag();
    ss<<"\nplatform="<<name().platform;
    ss<<"\ncomponent="<<name().component;
    ss<<"\nisApplication="<<(int)isApplication();
    ss<<"\ncommunicator="<<(int)(communicator()!=0);
    ss<<"\nadapter="<<(int)(adapter()!=0);
    ss<<"\nproperties=("<<properties()->getCommandLineOptions().size()<<")";

    return ss.str();
  }   

} // namespace
