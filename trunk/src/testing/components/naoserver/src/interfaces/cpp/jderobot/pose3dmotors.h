// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `pose3dmotors.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_pose3dmotors_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_pose3dmotors_h__

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

class Pose3DMotorsData;

class Pose3DMotorsParams;

class Pose3DMotors;

}

}

namespace jderobot
{

class Pose3DMotorsData;
bool operator==(const Pose3DMotorsData&, const Pose3DMotorsData&);
bool operator<(const Pose3DMotorsData&, const Pose3DMotorsData&);

class Pose3DMotorsParams;
bool operator==(const Pose3DMotorsParams&, const Pose3DMotorsParams&);
bool operator<(const Pose3DMotorsParams&, const Pose3DMotorsParams&);

class Pose3DMotors;
bool operator==(const Pose3DMotors&, const Pose3DMotors&);
bool operator<(const Pose3DMotors&, const Pose3DMotors&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::Pose3DMotorsData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Pose3DMotorsData*);

::Ice::Object* upCast(::jderobot::Pose3DMotorsParams*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Pose3DMotorsParams*);

::Ice::Object* upCast(::jderobot::Pose3DMotors*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Pose3DMotors*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::Pose3DMotorsData> Pose3DMotorsDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Pose3DMotorsData> Pose3DMotorsDataPrx;

void __read(::IceInternal::BasicStream*, Pose3DMotorsDataPrx&);
void __patch__Pose3DMotorsDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Pose3DMotorsParams> Pose3DMotorsParamsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Pose3DMotorsParams> Pose3DMotorsParamsPrx;

void __read(::IceInternal::BasicStream*, Pose3DMotorsParamsPrx&);
void __patch__Pose3DMotorsParamsPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Pose3DMotors> Pose3DMotorsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Pose3DMotors> Pose3DMotorsPrx;

void __read(::IceInternal::BasicStream*, Pose3DMotorsPrx&);
void __patch__Pose3DMotorsPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

}

namespace jderobot
{

class Pose3DMotorsData : virtual public ::Ice::Object
{
public:

    typedef Pose3DMotorsDataPrx ProxyType;
    typedef Pose3DMotorsDataPtr PointerType;
    
    Pose3DMotorsData() {}
    Pose3DMotorsData(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~Pose3DMotorsData() {}


public:

    ::Ice::Float x;
    ::Ice::Float y;
    ::Ice::Float z;
    ::Ice::Float pan;
    ::Ice::Float tilt;
    ::Ice::Float roll;
    ::Ice::Float panSpeed;
    ::Ice::Float tiltSpeed;
};

class Pose3DMotorsParams : virtual public ::Ice::Object
{
public:

    typedef Pose3DMotorsParamsPrx ProxyType;
    typedef Pose3DMotorsParamsPtr PointerType;
    
    Pose3DMotorsParams() {}
    Pose3DMotorsParams(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~Pose3DMotorsParams() {}


public:

    ::Ice::Float maxPan;
    ::Ice::Float minPan;
    ::Ice::Float maxTilt;
    ::Ice::Float minTilt;
    ::Ice::Float maxPanSpeed;
    ::Ice::Float maxTiltSpeed;
};

class Pose3DMotors : virtual public ::Ice::Object
{
public:

    typedef Pose3DMotorsPrx ProxyType;
    typedef Pose3DMotorsPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setPose3DMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getPose3DMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getPose3DMotorsParams(::IceInternal::Incoming&, const ::Ice::Current&);
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

class Pose3DMotorsData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_secure(bool __secure) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_twoway() const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_oneway() const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_batchOneway() const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_datagram() const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_batchDatagram() const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Pose3DMotorsParams : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_secure(bool __secure) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_twoway() const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_oneway() const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_batchOneway() const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_datagram() const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_batchDatagram() const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Pose3DMotors : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr& data)
    {
        return setPose3DMotorsData(data, 0);
    }
    ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr& data, const ::Ice::Context& __ctx)
    {
        return setPose3DMotorsData(data, &__ctx);
    }
    
private:

    ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Context*);
    
public:

    ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData()
    {
        return getPose3DMotorsData(0);
    }
    ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Context& __ctx)
    {
        return getPose3DMotorsData(&__ctx);
    }
    
private:

    ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Context*);
    
public:

    ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams()
    {
        return getPose3DMotorsParams(0);
    }
    ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Context& __ctx)
    {
        return getPose3DMotorsParams(&__ctx);
    }
    
private:

    ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_secure(bool __secure) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_twoway() const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_oneway() const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_batchOneway() const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_datagram() const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_batchDatagram() const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
