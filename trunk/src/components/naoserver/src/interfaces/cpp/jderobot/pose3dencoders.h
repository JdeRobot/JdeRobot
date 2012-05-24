// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `pose3dencoders.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_pose3dencoders_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_pose3dencoders_h__

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

class Pose3DEncodersData;

class Pose3DEncoders;

}

}

namespace jderobot
{

class Pose3DEncodersData;
bool operator==(const Pose3DEncodersData&, const Pose3DEncodersData&);
bool operator<(const Pose3DEncodersData&, const Pose3DEncodersData&);

class Pose3DEncoders;
bool operator==(const Pose3DEncoders&, const Pose3DEncoders&);
bool operator<(const Pose3DEncoders&, const Pose3DEncoders&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::Pose3DEncodersData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Pose3DEncodersData*);

::Ice::Object* upCast(::jderobot::Pose3DEncoders*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Pose3DEncoders*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::Pose3DEncodersData> Pose3DEncodersDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Pose3DEncodersData> Pose3DEncodersDataPrx;

void __read(::IceInternal::BasicStream*, Pose3DEncodersDataPrx&);
void __patch__Pose3DEncodersDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Pose3DEncoders> Pose3DEncodersPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Pose3DEncoders> Pose3DEncodersPrx;

void __read(::IceInternal::BasicStream*, Pose3DEncodersPrx&);
void __patch__Pose3DEncodersPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

}

namespace jderobot
{

class Pose3DEncodersData : virtual public ::Ice::Object
{
public:

    typedef Pose3DEncodersDataPrx ProxyType;
    typedef Pose3DEncodersDataPtr PointerType;
    
    Pose3DEncodersData() {}
    Pose3DEncodersData(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Int);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~Pose3DEncodersData() {}


public:

    ::Ice::Float x;
    ::Ice::Float y;
    ::Ice::Float z;
    ::Ice::Float pan;
    ::Ice::Float tilt;
    ::Ice::Float roll;
    ::Ice::Int clock;
};

class Pose3DEncoders : virtual public ::Ice::Object
{
public:

    typedef Pose3DEncodersPrx ProxyType;
    typedef Pose3DEncodersPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getPose3DEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);
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

class Pose3DEncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_secure(bool __secure) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_twoway() const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_oneway() const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_batchOneway() const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_datagram() const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_batchDatagram() const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Pose3DEncoders : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData()
    {
        return getPose3DEncodersData(0);
    }
    ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Context& __ctx)
    {
        return getPose3DEncodersData(&__ctx);
    }
    
private:

    ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_secure(bool __secure) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_twoway() const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_oneway() const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_batchOneway() const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_datagram() const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_batchDatagram() const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
