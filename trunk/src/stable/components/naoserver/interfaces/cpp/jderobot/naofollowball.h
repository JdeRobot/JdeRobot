// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `naofollowball.ice'

#ifndef __naofollowball_h__
#define __naofollowball_h__

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

class NaoFollowBall;

}

}

namespace jderobot
{

class NaoFollowBall;
bool operator==(const NaoFollowBall&, const NaoFollowBall&);
bool operator<(const NaoFollowBall&, const NaoFollowBall&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::NaoFollowBall*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::NaoFollowBall*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::NaoFollowBall> NaoFollowBallPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::NaoFollowBall> NaoFollowBallPrx;

void __read(::IceInternal::BasicStream*, NaoFollowBallPrx&);
void __patch__NaoFollowBallPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

}

namespace jderobot
{

class NaoFollowBall : virtual public ::Ice::Object
{
public:

    typedef NaoFollowBallPrx ProxyType;
    typedef NaoFollowBallPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void setParams(::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setParams(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void setMinParams(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setMinParams(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void setMaxParams(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setMaxParams(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void start(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___start(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void stop(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___stop(::IceInternal::Incoming&, const ::Ice::Current&);
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

class NaoFollowBall : virtual public ::IceProxy::Ice::Object
{
public:

    void setParams(::Ice::Int rMin, ::Ice::Int rMax, ::Ice::Int gMin, ::Ice::Int gMax, ::Ice::Int bMin, ::Ice::Int bMax)
    {
        setParams(rMin, rMax, gMin, gMax, bMin, bMax, 0);
    }
    void setParams(::Ice::Int rMin, ::Ice::Int rMax, ::Ice::Int gMin, ::Ice::Int gMax, ::Ice::Int bMin, ::Ice::Int bMax, const ::Ice::Context& __ctx)
    {
        setParams(rMin, rMax, gMin, gMax, bMin, bMax, &__ctx);
    }
    
private:

    void setParams(::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
    
public:

    void setMinParams(::Ice::Int rMin, ::Ice::Int gMin, ::Ice::Int bMin)
    {
        setMinParams(rMin, gMin, bMin, 0);
    }
    void setMinParams(::Ice::Int rMin, ::Ice::Int gMin, ::Ice::Int bMin, const ::Ice::Context& __ctx)
    {
        setMinParams(rMin, gMin, bMin, &__ctx);
    }
    
private:

    void setMinParams(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
    
public:

    void setMaxParams(::Ice::Int rMax, ::Ice::Int gMax, ::Ice::Int bMax)
    {
        setMaxParams(rMax, gMax, bMax, 0);
    }
    void setMaxParams(::Ice::Int rMax, ::Ice::Int gMax, ::Ice::Int bMax, const ::Ice::Context& __ctx)
    {
        setMaxParams(rMax, gMax, bMax, &__ctx);
    }
    
private:

    void setMaxParams(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
    
public:

    void start()
    {
        start(0);
    }
    void start(const ::Ice::Context& __ctx)
    {
        start(&__ctx);
    }
    
private:

    void start(const ::Ice::Context*);
    
public:

    void stop()
    {
        stop(0);
    }
    void stop(const ::Ice::Context& __ctx)
    {
        stop(&__ctx);
    }
    
private:

    void stop(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_secure(bool __secure) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_twoway() const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_oneway() const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_batchOneway() const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_datagram() const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_batchDatagram() const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<NaoFollowBall> ice_timeout(int __timeout) const
    {
        return dynamic_cast<NaoFollowBall*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
