// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `pose3dmotors.ice'

#ifndef __pose3dmotors_h__
#define __pose3dmotors_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <jderobot/common.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
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

namespace IceProxy
{

namespace jderobot
{

class Pose3DMotorsData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Pose3DMotorsData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Pose3DMotorsParams : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotorsParams> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotorsParams*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Pose3DMotorsParams*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
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
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DMotors> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DMotors*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Pose3DMotors*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace jderobot
{

class Pose3DMotorsData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Pose3DMotorsParams : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Pose3DMotors : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Context*) = 0;

    virtual ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Context*) = 0;

    virtual ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class Pose3DMotorsData : virtual public ::IceDelegate::jderobot::Pose3DMotorsData,
                         virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Pose3DMotorsParams : virtual public ::IceDelegate::jderobot::Pose3DMotorsParams,
                           virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Pose3DMotors : virtual public ::IceDelegate::jderobot::Pose3DMotors,
                     virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Context*);

    virtual ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class Pose3DMotorsData : virtual public ::IceDelegate::jderobot::Pose3DMotorsData,
                         virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Pose3DMotorsParams : virtual public ::IceDelegate::jderobot::Pose3DMotorsParams,
                           virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Pose3DMotors : virtual public ::IceDelegate::jderobot::Pose3DMotors,
                     virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Context*);

    virtual ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Context*);
};

}

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
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~Pose3DMotorsData() {}

    friend class Pose3DMotorsData__staticInit;

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

class Pose3DMotorsData__staticInit
{
public:

    ::jderobot::Pose3DMotorsData _init;
};

static Pose3DMotorsData__staticInit _Pose3DMotorsData_init;

class Pose3DMotorsParams : virtual public ::Ice::Object
{
public:

    typedef Pose3DMotorsParamsPrx ProxyType;
    typedef Pose3DMotorsParamsPtr PointerType;
    
    Pose3DMotorsParams() {}
    Pose3DMotorsParams(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

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
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setPose3DMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::Pose3DMotorsDataPtr getPose3DMotorsData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPose3DMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::Pose3DMotorsParamsPtr getPose3DMotorsParams(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPose3DMotorsParams(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
