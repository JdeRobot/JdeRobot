// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `motors.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_motors_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_motors_h__

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
#include <IceE/UserExceptionFactory.h>
#include <IceE/FactoryTable.h>
#include <jderobot/common.h>
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

class Motors;

}

}

namespace jderobot
{

class Motors;
bool operator==(const Motors&, const Motors&);
bool operator<(const Motors&, const Motors&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::Motors*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Motors*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::Motors> MotorsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Motors> MotorsPrx;

void __read(::IceInternal::BasicStream*, MotorsPrx&);
void __patch__MotorsPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

}

namespace jderobot
{

class Motors : virtual public ::Ice::Object
{
public:

    typedef MotorsPrx ProxyType;
    typedef MotorsPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Float getV(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getV(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Int setV(::Ice::Float, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setV(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Float getW(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getW(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Int setW(::Ice::Float, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setW(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Float getL(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getL(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Int setL(::Ice::Float, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setL(::IceInternal::Incoming&, const ::Ice::Current&);
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

class Motors : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Float getV()
    {
        return getV(0);
    }
    ::Ice::Float getV(const ::Ice::Context& __ctx)
    {
        return getV(&__ctx);
    }
    
private:

    ::Ice::Float getV(const ::Ice::Context*);
    
public:

    ::Ice::Int setV(::Ice::Float v)
    {
        return setV(v, 0);
    }
    ::Ice::Int setV(::Ice::Float v, const ::Ice::Context& __ctx)
    {
        return setV(v, &__ctx);
    }
    
private:

    ::Ice::Int setV(::Ice::Float, const ::Ice::Context*);
    
public:

    ::Ice::Float getW()
    {
        return getW(0);
    }
    ::Ice::Float getW(const ::Ice::Context& __ctx)
    {
        return getW(&__ctx);
    }
    
private:

    ::Ice::Float getW(const ::Ice::Context*);
    
public:

    ::Ice::Int setW(::Ice::Float w)
    {
        return setW(w, 0);
    }
    ::Ice::Int setW(::Ice::Float w, const ::Ice::Context& __ctx)
    {
        return setW(w, &__ctx);
    }
    
private:

    ::Ice::Int setW(::Ice::Float, const ::Ice::Context*);
    
public:

    ::Ice::Float getL()
    {
        return getL(0);
    }
    ::Ice::Float getL(const ::Ice::Context& __ctx)
    {
        return getL(&__ctx);
    }
    
private:

    ::Ice::Float getL(const ::Ice::Context*);
    
public:

    ::Ice::Int setL(::Ice::Float l)
    {
        return setL(l, 0);
    }
    ::Ice::Int setL(::Ice::Float l, const ::Ice::Context& __ctx)
    {
        return setL(l, &__ctx);
    }
    
private:

    ::Ice::Int setL(::Ice::Float, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Motors> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_secure(bool __secure) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Motors> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Motors> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Motors> ice_twoway() const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_oneway() const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_batchOneway() const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_datagram() const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_batchDatagram() const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Motors> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Motors*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
