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

#ifndef JDEROBOT_SUBSYSTEM_THREAD_H
#define JDEROBOT_SUBSYSTEM_THREAD_H

#include <gbxsickacfr/gbxiceutilacfr/thread.h>
#include <gbxutilacfr/subhealth.h>
#include <gbxutilacfr/status.h>
#include <gbxutilacfr/tracer.h>

namespace jderobotice {

  /*!
    @brief A class implementing the common subsystem state machine, also catches all 
    @brief possible exceptions and integrates some Status operations.

    If an exception is caught when the thread is not stopping, a status fault is issued.
    Then the thread will wait for someone to call stop().

    The state machine is defined by gbxutilacfr::Status.
  
    Re-implementation all three functions is optional.

    @verbatim
    void MyThread::initialize()
    {
    while ( !isStopping() )
    {
    // initialize
    }
    }

    void MyThread::work()
    {
    // main loop
    while ( !isStopping() )
    {
    // do something
    }
    }
    @endverbatim
  */
  class SubsystemThread : public gbxiceutilacfr::Thread
  {
  public: 
    //! Constructor.
    SubsystemThread( gbxutilacfr::Tracer& tracer, 
                     gbxutilacfr::Status& status, 
                     const std::string& subsysName="SubsystemThread",
                     double maxHeartbeatIntervalSec=-1.0 );

    ~SubsystemThread();
   
    //! Convinient access to the component-wide tracer.
    gbxutilacfr::Tracer& tracer() { return tracer_; };

    //! Passes this information to the system Status.
    void setMaxHeartbeatInterval( double interval );

    //! Passes this information to the system Status.
    void setSubsystemType( gbxutilacfr::SubsystemType type );

    //! Returns subsystem name assigned to this thread.
    std::string subsysName() const;

    //! Convinient for giving somebody else the right to set health of this subsystem.
    gbxutilacfr::SubHealth& health() { return health_; };

    // from IceUtil::Thread
    // This implementation calls protectedRun(), catches all possible exceptions, prints out 
    // errors and waits for someone to call stop().
    virtual void run();

  private:

    // implementation note: FSM actions are private so that the derived class
    // can re-implement them but cannot call them.

    //! Action performed when in Intialising state.
    //! Default imlementation does nothing.
    virtual void initialize() {};
    
    //! Action performed when in Working state.
    //! Default imlementation does nothing.
    virtual void work() {};

    //! Action performed when in Finalising state.
    //! Default imlementation does nothing.
    virtual void finalize() {};

    // this private function is not virtual!
    // it's part of internal implementation.
    // implements the state machine.
    void protectedRun();

    gbxutilacfr::Tracer& tracer_;
    gbxutilacfr::Status& status_;
    gbxutilacfr::SubHealth health_;
  };
  //! A smart pointer to the SubsystemThread class.
  typedef IceUtil::Handle<SubsystemThread> SubsystemThreadPtr;

}

#endif
