// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `encoders.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_encoders_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_encoders_h__

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

class EncodersData;

class Encoders;

}

}

namespace jderobot
{

class EncodersData;
bool operator==(const EncodersData&, const EncodersData&);
bool operator<(const EncodersData&, const EncodersData&);

class Encoders;
bool operator==(const Encoders&, const Encoders&);
bool operator<(const Encoders&, const Encoders&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::EncodersData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::EncodersData*);

::Ice::Object* upCast(::jderobot::Encoders*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Encoders*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::EncodersData> EncodersDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::EncodersData> EncodersDataPrx;

void __read(::IceInternal::BasicStream*, EncodersDataPrx&);
void __patch__EncodersDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Encoders> EncodersPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Encoders> EncodersPrx;

void __read(::IceInternal::BasicStream*, EncodersPrx&);
void __patch__EncodersPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

}

namespace jderobot
{

class EncodersData : virtual public ::Ice::Object
{
public:

    typedef EncodersDataPrx ProxyType;
    typedef EncodersDataPtr PointerType;
    
    EncodersData() {}
    EncodersData(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~EncodersData() {}


public:

    ::Ice::Float robotx;
    ::Ice::Float roboty;
    ::Ice::Float robottheta;
    ::Ice::Float robotcos;
    ::Ice::Float robotsin;
};

class Encoders : virtual public ::Ice::Object
{
public:

    typedef EncodersPrx ProxyType;
    typedef EncodersPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);
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

class EncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<EncodersData> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_secure(bool __secure) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<EncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<EncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<EncodersData> ice_twoway() const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_oneway() const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_batchOneway() const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_datagram() const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_batchDatagram() const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_timeout(int __timeout) const
    {
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Encoders : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::EncodersDataPtr getEncodersData()
    {
        return getEncodersData(0);
    }
    ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Context& __ctx)
    {
        return getEncodersData(&__ctx);
    }
    
private:

    ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Encoders> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_secure(bool __secure) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Encoders> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Encoders> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Encoders> ice_twoway() const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_oneway() const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_batchOneway() const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_datagram() const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_batchDatagram() const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
