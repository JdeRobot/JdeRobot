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

#include <jderobotutil/jderobotutil.h>
#include <iostream>
#include "application.h"
#include "component.h"
#include "jderobotice.h"


using namespace std;

namespace jderobotice {

  // namespace {

  // /*
  //     Order priority for different sources of configuration parameters, from lowest to highest:
  //         1. jderobot factory defaults
  //         2. global config file
  //         3. component config file
  //         4. command line arguments
  // */
  //void setProperties( Ice::PropertiesPtr   &properties,
  //		      const Ice::StringSeq &commandLineArgs,
  //		      const std::string    &componentTag ){
  //     // pre-parse Jderobot-specific command line arguments
  //     // (nothing here right now)
  //     // jderobotice::parseJderobotCommandLineOptions( args );
    
    // Level 4. Highest priority, apply properties from the command line arguments
    // read in all command line options starting with '--", but not "-"
    // note that something like --bullshit will be parsed to --bullshit=1
    // Note that this is a standard Ice function.
    //properties->parseCommandLineOptions( "", commandLineArgs );
    //initTracerInfo( componentTag+": Loaded command line properties" );

  //     // Level 3. Now, apply properties from this component's config file (do not force!)
  //     detail::addPropertiesFromApplicationConfigFile( properties, commandLineArgs, componentTag );

  //     // Level 2. Now, apply properties from the global Jderobot config file
  //     detail::addPropertiesFromGlobalConfigFile( properties, componentTag );

  //     // Level 1. apply Jderobot factory defaults
  //     jderobotice::detail::setFactoryProperties( properties, componentTag );
  //     initTracerInfo( componentTag+": Loaded factory default properties" );

  //     // Level 0. sort out platform and component names, apply defaults, set adapter names.
  //     jderobotice::detail::postProcessComponentProperties( properties, componentTag );
  //}

  // }

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  Application::Application( jderobotice::Component &component ) :
    Ice::Application(),
    component_(component),
    isComponentStopped_(false)
  {
  }

  int
  Application::jderobotMain(int argc, char* argv[])
  {
    // convert to string sequence for convenience
    Ice::StringSeq args = Ice::argsToStringSeq( argc, argv );

    // parse command line arguments for flags which require immediate action
    for ( unsigned int i=0; i<args.size(); ++i ) {
      if ( !args[i].compare( 0,2, "-h" ) ||
	   !args[i].compare( 0,6, "--help" ) )
        {
	  std::cout << "jderobot: " << component_.help( args[0] );
	  // nothing to clean up yet
	  exit(0);
        }
      // else if ( !args[i].compare( 0,2, "-v" ) ||
      //                   !args[i].compare( 0,9, "--version" ) )
      //         {
      //             jderobotice::detail::printAllVersions( component_ );
      //             // nothing to clean up yet
      //             exit(0);
      //         }
    }

    //     // print version information on the first line
    //     jderobotice::detail::printAllVersions( component_ );

    //Ice::InitializationData initData;
    // Note that we don't use the version which takes arguments so that the config file which may be
    // specified by mistake with --Ice.Config is not loaded.
    //initData.properties = Ice::createProperties();

    // Set the component's properties based on the various sources from which properties can be read
    //setProperties( initData.properties, args, component_.context().tag() );

    //args = initData.properties->parseCommandLineOptions( "", args );

    // now pass the startup options to Ice which will start the Communicator
    return Ice::Application::main(args);//,initData);//, argv );
  }

  int
  Application::run( int argc, char* argv[] )
  {
    // communicator is already initialized by Ice::Application
    // all defaults are already applied
    Ice::PropertiesPtr props = communicator()->getProperties();
    Ice::StringSeq args = Ice::argsToStringSeq( argc, argv );
    args = props->parseCommandLineOptions( "", args );

    // what to do when a signal is received (e.g. Ctrl-C)
    if ( props->getPropertyAsInt("Jderobot.Application.CallbackOnInterrupt") ) {
      // normally, we want to get a change to shut down our component before
      //shutting down the communicator
      callbackOnInterrupt();
    } else {
      shutdownOnInterrupt();
    }


    // create the one-and-only component adapter
    try{
      adapter_ = communicator()->createObjectAdapter(component_.context().tag());
    }
    catch( const Ice::InitializationException& e ){
      stringstream ss;
      ss << "(while creating component adapter) : " << e.what();
      component_.context().tracer().error(ss.str());
      component_.context().tracer().info("Application quitting. Jderobot out.");
      return 1;
    }
    component_.context().tracer().info(component_.context().tag()+": Created object adapter.");

    //
    // Give the component all the stuff it needs to initialize
    //
    jderobot::FQComponentName fqCompName;
    fqCompName.platform = props->getProperty( component_.context().tag() + ".Platform" );
    fqCompName.component = props->getProperty( component_.context().tag() + ".Component" );

    bool isApp = true;
    component_.init( fqCompName, isApp, adapter_ );
    component_.context().tracer().info(component_.context().tag() + ": Application initialized.");

    
    //
    // Start the component, catching all exceptions
    //
    stringstream exceptionSS;
    try{
      component_.start();
    }
    catch ( const std::exception &e ) {
      exceptionSS << "(while starting component) : " << e.what();
    }
    catch ( const std::string &e ) {
      exceptionSS << "(while starting component) : " << e;
    }
    catch ( const char *e ) {
      exceptionSS << "(while starting component) : " << e;
    }
    catch ( ... ) {
      exceptionSS << "(while starting component) .";
    }

    if ( !exceptionSS.str().empty() ) {
      component_.context().tracer().error( component_.context().tag() + ": " + exceptionSS.str());
      component_.context().tracer().info(component_.context().tag() + 
			     ": Application quitting. Jderobot out.");
      return 1;
    }

    // component started without a problem. now will wait for the communicator to shutdown
    // this will typically happen after a signal is recieved from Ctrl-C or from IceGrid.
    communicator()->waitForShutdown();
    component_.context().tracer().info(component_.context().tag() + ": Communicator has shut down.");

    stopComponent();

    adapter_->waitForDeactivate();
    component_.context().tracer().info(component_.context().tag() + ": Adapter deactivated.");
    component_.context().tracer().info(component_.context().tag() + ": Jderobot out.");

    // communicator will be destroyed by Ice::Application
    return 0;
  }

  void
  Application::stopComponent(){
    IceUtil::Mutex::Lock lock(mutex_);
  
    if ( isComponentStopped_ )
      return;
  
    // first tell component to shutdown
    component_.context().tracer().info(component_.context().tag() + ": Stopping component...");
    component_.stop();
    component_.finalize();
    component_.context().tracer().info(component_.context().tag() + ": Component stopped.");
    isComponentStopped_ = true;
  }

  // this function is only called if we choose to do callbackOnInterrupt()
  void
  Application::interruptCallback( int signal ){
    component_.context().tracer().info(component_.context().tag() + ": Received interrupt signal.");
    stopComponent();
  
    // now we can just destroy the communicator
    component_.context().tracer().info(component_.context().tag() + ": Shutting down communicator.");
    // (being extra careful here, copy exception handling from Ice::Application::destroyOnInterruptCallback() )
    stringstream exceptionSS;
    try{
      assert(communicator() != 0);
      // we just shutdown the communicator, Ice::Application will destroy it later.
      //
      communicator()->shutdown();
    }
    catch(const std::exception& ex) {
      exceptionSS << " (while shutting down in response to signal " << signal << "): " << ex.what();
    }
    catch(const std::string& msg) {
      exceptionSS << " (while shutting down in response to signal " << signal << "): " << msg;
    }
    catch(const char* msg) {
      exceptionSS << " (while shutting down in response to signal " << signal << "): " << msg;
    }
    catch(...) {
      exceptionSS << " (while shutting down in response to signal " << signal << "): unknown exception";
    }
  
    if ( !exceptionSS.str().empty() )
      component_.context().tracer().error(component_.context().tag() + ": " + exceptionSS.str());
  }

} // namespace
