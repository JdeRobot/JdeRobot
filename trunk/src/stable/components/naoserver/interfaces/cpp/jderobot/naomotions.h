// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `naomotions.ice'

#ifndef __naomotions_h__
#define __naomotions_h__

#include <IceE/ProxyF.h>
#include <IceE/ObjectF.h>
#include <IceE/Exception.h>
#include <IceE/ScopedArray.h>
#include <IceE/Proxy.h>
#include <IceE/Object.h>
#ifndef ICEE_PURE_CLIENT
#  include <IceE/Incoming.h>
#endif
#include <IceE/Outgoing.h>
#include <IceE/UndefSysMacros.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace jderobot
{

class NaoMotions;

}

}

namespace jderobot
{

class NaoMotions;
bool operator==(const NaoMotions&, const NaoMotions&);
bool operator<(const NaoMotions&, const NaoMotions&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::NaoMotions*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::NaoMotions*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::NaoMotions> NaoMotionsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::NaoMotions> NaoMotionsPrx;

void __read(::IceInternal::BasicStream*, NaoMotionsPrx&);
void __patch__NaoMotionsPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

enum MotionType
{
    RigthKick,
    LeftKick,
    StandupBack,
    StandupFront,
    Intercept,
    ChangeCamera,
    ResetNaoqi
};

void __write(::IceInternal::BasicStream*, MotionType);
void __read(::IceInternal::BasicStream*, MotionType&);

}

namespace jderobot
{

class NaoMotions : virtual public ::Ice::Object
{
public:

    typedef NaoMotionsPrx ProxyType;
    typedef NaoMotionsPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int setMotion(::jderobot::MotionType, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setMotion(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

}

namespace IceProxy
{

namespace jderobot
{

class NaoMotions : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int setMotion(::jderobot::MotionType motion)
    {
        return setMotion(motion, 0);
    }
    ::Ice::Int setMotion(::jderobot::MotionType motion, const ::Ice::Context& __ctx)
    {
        return setMotion(motion, &__ctx);
    }
    
private:

    ::Ice::Int setMotion(::jderobot::MotionType, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_secure(bool __secure) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<NaoMotions> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<NaoMotions> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_twoway() const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_oneway() const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_batchOneway() const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_datagram() const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_batchDatagram() const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<NaoMotions> ice_timeout(int __timeout) const
    {
        return dynamic_cast<NaoMotions*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
