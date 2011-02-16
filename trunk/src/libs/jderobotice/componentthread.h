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

#ifndef JDEROBOTICE_COMPONENT_THREAD_H
#define JDEROBOTICE_COMPONENT_THREAD_H

#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include "context.h"
#include "component.h" // for ComponentAdapterActivationPolicy

namespace jderobotice {
  class ComponentThread : public gbxiceutilacfr::SafeThread{
  public: 
    ComponentThread( ComponentAdapterActivationPolicy adapterPolicy,
		     const jderobotice::Context& context );
  private: 
    virtual void walk();
    
    ComponentAdapterActivationPolicy adapterPolicy;
    jderobotice::Context context;
  };
}

#endif
