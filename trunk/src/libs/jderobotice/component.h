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

#ifndef JDEROBOTICE_COMPONENT_H
#define JDEROBOTICE_COMPONENT_H

#include <Ice/ObjectAdapter.h>
#include <IceUtil/Config.h> /*noncopyable*/
#include <gbxutilacfr/stoppable.h>
#include "context.h"

namespace jderobotice
{
  /** 
   * This enum type is used to describe the policy of activating the component's adapter.
   */
  enum ComponentAdapterActivationPolicy{
    AdapterManualActivation=0,/**< Component adapter will be activated in 
				 the code of the component derived from jderobotice::Component.*/
    AdapterAutoActivation/**< Component adapter will be activated by the internal 
			    infrastructure thead launched automatically by jderobotice::Component.*/
  };


  /**
   * Base class for all componets using libJderobotIce
   * Deriving from this makes it easy to use code as either a
   * stand-alone application (using jderobotice::Application)
   * or as a service in an IceBox (using jderobotice::Service). Two functions
   * must be implemented: start and stop.
   *
   * Most of the state of the component is summarized in its
   * Component::context. The information is read-only because it is set 
   * prior to the component's initialization.
   *
   * @par Component Initialisation
   * 
   *   Component initialisation code can be put in two places:
   *
   *   1. Component Constructor
   *
   *      - In the constructor, none of the jderobotice machinery is initialized.  This
   *        means that no remote calls can be made (the communicator isn't initialized yet).
   *      - Any code that should be executed before any remote calls are made belongs in 
   *        the Component constructor.
   *
   *   2. Component::start()
   *
   *      - start() is called after all resources in Context are initialized.
   *      - This is the place to launch threads from and get out.
   *        - Don't do anything that might loop forever in here, otherwise it won't be possible
   *          to bring the component down cleanly. 
   *
   * @par Component Destruction
   *
   *   Clean-up code belongs in one of two places:
   *
   *   1. Component::stop()
   *
   *     - The context and communicator are still available at this point
   *
   *   2. Component Destructor
   *
   *     - The context and communicator are no longer available
   *
   * @sa Application, Service, Context
   */
  class Component : private IceUtil::noncopyable
  {
    // these are declared friends so they can call init(), tag(), finalize()
    friend class Application;
    friend class Service;
  
  public:
    /**
     * Constructor
     * \param tag with which to identify it in the config files.
     * \param adapterPolicy \sa ComponentAdapterActivationPolicy
     * \param tracerConfig \sa gbxutilacfr::TrivialTracer and gbxutilacfr::Tracer::Config
     */
    Component( const std::string& tag,
	       const int* tracerConfig = 0,
	       ComponentAdapterActivationPolicy adapterPolicy=AdapterAutoActivation);

    /**
     * Destructor
     */
    virtual ~Component();

    /**
     * This function is called by the component's container (Application or Service).
     * It should return immediately, possibly after launching a thread. GUI components
     * are an exception to this rule, they may run in the calling thread provided that
     * the Ctrl-C handler was not installed (see Application, Service).
     */
    virtual void start()=0;

    /**
     * This function is called by the component's container (Application or Service)
     * when the component is ordered to stop execution.
     * Default implementation does nothing.
     */
    virtual void stop() {};

    /**
     * This function is called by Application when the executable is called with \c --help
     * command line option.
     */
    virtual const std::string help( const std::string& executable ) const;

    /**
     * This function is called by Application on startup (including when the executable is 
     * called with \c --version command line option). Standard Jderobot components return an
     * empty string. Component from external project may reimplement this function to supply
     * the project version, which is probably different from the Jderobot version. 
     * Example:
     * \verbatim
     *virtual const std::string version() const { return std::string(PROJECT_VERSION); };
     * \endverbatim
     */
    virtual const std::string version() const { return std::string(""); };

  protected:
    
    /**
     * Activates the component's adapter.
     * Activation makes provided interfaces accessible from the outside world.
     * It also tries to register the adapter with the IceGrid Registry. 
     * If the registry is unreachable, the adapter is not fully activated.
     *
     * A Jderobot configuration property Jderobot.Component.RequireRegistry determines 
     * what happens if the Registry is not available. If RequireRegistry=1 (default) 
     * and the registry is unreachable, an jderobotice::NetworkException is thrown.
     * In this case it is safe to call activate again later, hoping that the regisry 
     * will become reachable. If RequireRegistry=0 no exception is thrown.
     */
     void activate();

    /**
     * Component's "context", which contains component's naming and networking information.
     * It can be used directly or passed to threads and classes. For example:
     * \verbatim
     *context().tracer().info("Everything is OK");
     *MainLoop myloop( context() );
     * \endverbatim
     */
    const Context& context() const;

  private:

    /**
     * One of the container classes (Application or Service) will
     * call this function before calling start().
     * This is the reason for them to be friends.
     */
    void init( const jderobot::FQComponentName& name,
	       const bool isApp,
	       const Ice::ObjectAdapterPtr& adapter );

    /**
     * Cleans up internal resources after stop() was called
     * but before complete shutdown.
     */
    void finalize();

    /**
     * Only Service should need to use this when the IceBox tells it
     * what the actual tag is.
     */
    void setTag( const std::string& t );

    class PImpl;
    std::auto_ptr<PImpl> pImpl;
  };

} // end namespace

#endif
