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

#ifndef JDEROBOTICE_COMPONENT_CONTEXT_H
#define JDEROBOTICE_COMPONENT_CONTEXT_H

// We carefully select the minimum subset of Ice include file we commonly need.
// It would be easier to just include <Ice/Ice.h> at the expense of more includes
// and slower compilation. We are trying to be careful because this file is included
// by practically everything in Jderobot.
#include <Ice/Communicator.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/Properties.h>
#include <gbxutilacfr/tracer.h>
#include <gbxutilacfr/status.h>
#include <gbxutilacfr/stoppable.h>
#include <jderobot/jcm.h>
#include <string>
#include <tr1/memory>

namespace jderobotice
{
  //class Component;//forward declaration

  /**
   * \brief Component's naming and networking information.
   * 
   * Contains pointers to component's registered name plus its communicator,
   * adapter, etc. It makes it easy to pass all this information to the network and
   * hardware handlers from the class derived from Component.
   *
   * A note on thread safety. None of access functions are thread-safe. However, all
   * object which are referenced (with pointers and smart pointers) are themselves thread-safe. 
   *  
   * This means that once a copy of of Context is created, it is safe to use it 
   * from different threads.
   * Pass context by const reference and store a copy, e.g. a class definition 
   * would look like this:
   @verbatim
   class MyClass
   {
   public:
   MyClass( const jderobotice::Context & context )
   : context_(context) {};
   private:
   jderobotice::Context context_;
   }
   @endverbatim
  */
  class Context{
    friend class Component;
  public:
    /**
     * Context is built durign component initialization
     */
    Context(const std::string& tag,
	    std::tr1::shared_ptr<gbxutilacfr::Tracer> tracer,
	    std::tr1::shared_ptr<gbxutilacfr::Status> status);
	
	Context();

    /**
     * Destructor
     */
    ~Context();

    /**
     * Component's tag used in configuration files.
     */
    const std::string& tag() const;

    /** 
     * Component's fully-qualified name given to the Registry.
     */
    const jderobot::FQComponentName& name() const;

    /**
     * @brief Is this component executed inside and application or a service?
     * Returns TRUE if this instance of the Component is used in a stand-alone application.
     * Otherwise, returns FALSE, meaning that it's used inside an IceBox service.
     */
    bool isApplication() const;

    /**
     * Returns smart pointer to the component's communicator.
     */
    Ice::CommunicatorPtr communicator() const;

    /**
     * Returns smart pointer to the component's adapter.
     */
    Ice::ObjectAdapterPtr adapter() const;

    /**
     * Convenience function which returns smart pointer to the component's properties.
     * Same as calling communicator()->getProperties()
     */
    Ice::PropertiesPtr properties() const;

    /**
     * Returns tracer
     */
    gbxutilacfr::Tracer& tracer() const;

    /**
     * Returns status
     */
    gbxutilacfr::Status& status() const;

    /**
     * Activates server funcionality of the component.
     * It's safe to call it multiple times
     */
    void activate();

    /**
     * Tries to activate the adapter (by calling Context::activate()). If fails, sleeps for
     * \c retryIntervalSec [s] seconds. Will repeat until successful, the number of retries 
     * is exceeded (default -1, i.e. infinite), or the \c activity is stopped.
     * Threads are a commonly used activity which implement Stoppable interface. 
     *
     * Nothing is done if retryNumber=0.
     *
     * If a non-empty subsystem name is supplied, sends a Status heartbeat after every 
     * attempt (\see gbxutilacfr::Status). Status warnings are NOT issued because the Status
     * interface is not accessible until the adapter is activated.
     *
     * If a non-empty subsystem name is supplied, sends a Status heartbeat at every iteration 
     * (\see gbxutilacfr::Status).
     **/
    void activate( gbxutilacfr::Stoppable* activity, const std::string& subsysName="", 
		   int retryIntervalSec=2, int retryNumber=-1 );

    /**
     * Adds the \p object to the component adapter and gives it the \p name.
     * Note that \p name is just the interface name, not its fully-qualified name.
     * (In Ice terms this will become the object identity.)
     */
    void createInterfaceWithString( Ice::ObjectPtr      & object,
				    const std::string   & name) const;
    
    /**
     * Tries to setup the specified interface until is successful,
     * the number of retries is exceeded (default -1, i.e. infinite), or the \c activity 
     * is stopped. Threads are a commonly used activity which implement Stoppable interface.
     *
     * Nothing is done if retryNumber=0.
     * 
     * Status warnings are set between retries if subsystem name is specified 
     * (\see gbxutilacfr::Status), otherwise warnings are simply traced.
     *
     * We catch gbxutilacfr::Exception, sleep for \c retryIntervalSec [s] and try again.
     * 
     * Example:
     *@verbatim
     *Ice::ObjectPtr obj = new MyObjectI;
     *orcaice::createInterfaceWithString( context_, obj, "coolname", 
     *                                    (gbxutilacfr::Stoppable*)this );
     *@endverbatim
     */
    void createInterfaceWithString( Ice::ObjectPtr      & object,
				    const std::string   & name,
				    gbxutilacfr::Stoppable* activity, 
				    const std::string& subsysName="", 
				    int retryIntervalSec=2, int retryNumber=-1 ) const;

    /**
     * Behaves like @ref createInterfaceWithString but the proxy information
     * is looked up in the @p context properties based on the @p interfaceTag.
     * Throws ConfigFileException if the interface name cannot be read from the 
     * config file for some reason.
     */
//     void createInterfaceWithTag( Ice::ObjectPtr      & object,
// 				 const std::string   & interfaceTag);

    /**
     * Tries to setup the specified interface until successful, the number of 
     * retries is exceeded (default -1, i.e. infinite), or the \c activity is stopped.
     * Threads are a commonly used activity which implement Stoppable interface.
     *
     * Nothing is done if retryNumber=0.  
     *
     * Status warnings are set between retries if subsystem name is specified 
     * (\see gbxutilacfr::Status), otherwise warnings are simply traced.
     *
     * We catch gbxutilacfr::Exception, sleep for \c retryIntervalSec [s] and try again.
     * Gives up after \c retryNumber of attempts (-1 stands for infinite number of retries).
     *
     * We do NOT catch a possible orcaice::ConfigFileException exception.
     *
     *@verbatim
     *Ice::ObjectPtr obj = new MyObjectI;
     *try
     *{
     *orcaice::createInterfaceWithTag( context_, obj, "InterfaceTag", 
     *                                 (gbxutilacfr::Stoppable*)this );
     *}
     *catch ( const orcaice::ConfigFileException& e ) {
     *....
     *}
     *@endverbatim
     */
    // void createInterfaceWithTag( Ice::ObjectPtr      & object,
// 				 const std::string   & interfaceTag,
// 				 gbxutilacfr::Stoppable* activity, 
// 				 const std::string& subsysName="", 
// 				 int retryIntervalSec=2, int retryNumber=-1 );


    /**
     * Returns true if component is in the process of deactivating itself
     */
    bool isDeactivating();

    /**
     * Triggers component shutdown
     */
    void shutdown() const;

    /**
     * Returns debugging string describing all contents.
     */
    std::string toString() const;

  private:
    void setTag( const std::string& t );

    /**
     * Initialize context
     * this function should only be called by Component (it can because it's a friend)
     */
    void init( const jderobot::FQComponentName& name,
               const bool isApp,
               const Ice::ObjectAdapterPtr& adapter );


    std::string tag_;
    std::tr1::shared_ptr<gbxutilacfr::Tracer> tracer_;
    std::tr1::shared_ptr<gbxutilacfr::Status> status_;
    jderobot::FQComponentName name_;
    bool isApplication_;
    Ice::CommunicatorPtr communicator_;
    Ice::ObjectAdapterPtr adapter_;
  };

} // end namespace

#endif
