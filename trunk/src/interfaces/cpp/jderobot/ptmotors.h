// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ptmotors.ice'

#ifndef __ptmotors_h__
#define __ptmotors_h__

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

class PTMotorsData;

class PTMotorsParams;

class PTMotors;

}

}

namespace jderobot
{

class PTMotorsData;
bool operator==(const PTMotorsData&, const PTMotorsData&);
bool operator<(const PTMotorsData&, const PTMotorsData&);

class PTMotorsParams;
bool operator==(const PTMotorsParams&, const PTMotorsParams&);
bool operator<(const PTMotorsParams&, const PTMotorsParams&);

class PTMotors;
bool operator==(const PTMotors&, const PTMotors&);
bool operator<(const PTMotors&, const PTMotors&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::PTMotorsData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::PTMotorsData*);

::Ice::Object* upCast(::jderobot::PTMotorsParams*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::PTMotorsParams*);

::Ice::Object* upCast(::jderobot::PTMotors*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::PTMotors*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::PTMotorsData> PTMotorsDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::PTMotorsData> PTMotorsDataPrx;

void __read(::IceInternal::BasicStream*, PTMotorsDataPrx&);
void __patch__PTMotorsDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::PTMotorsParams> PTMotorsParamsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::PTMotorsParams> PTMotorsParamsPrx;

void __read(::IceInternal::BasicStream*, PTMotorsParamsPrx&);
void __patch__PTMotorsParamsPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::PTMotors> PTMotorsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::PTMotors> PTMotorsPrx;

void __read(::IceInternal::BasicStream*, PTMotorsPrx&);
void __patch__PTMotorsPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace jderobot
{

class PTMotorsData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<PTMotorsData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class PTMotorsParams : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotorsParams> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotorsParams*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<PTMotorsParams*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class PTMotors : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr& data)
    {
        return setPTMotorsData(data, 0);
    }
    ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr& data, const ::Ice::Context& __ctx)
    {
        return setPTMotorsData(data, &__ctx);
    }
    
private:

    ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr&, const ::Ice::Context*);
    
public:

    ::jderobot::PTMotorsDataPtr getPTMotorsData()
    {
        return getPTMotorsData(0);
    }
    ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Context& __ctx)
    {
        return getPTMotorsData(&__ctx);
    }
    
private:

    ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Context*);
    
public:

    ::jderobot::PTMotorsParamsPtr getPTMotorsParams()
    {
        return getPTMotorsParams(0);
    }
    ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Context& __ctx)
    {
        return getPTMotorsParams(&__ctx);
    }
    
private:

    ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<PTMotors> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PTMotors> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PTMotors*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<PTMotors*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class PTMotorsData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class PTMotorsParams : virtual public ::IceDelegate::Ice::Object
{
public:
};

class PTMotors : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr&, const ::Ice::Context*) = 0;

    virtual ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Context*) = 0;

    virtual ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class PTMotorsData : virtual public ::IceDelegate::jderobot::PTMotorsData,
                     virtual public ::IceDelegateM::Ice::Object
{
public:
};

class PTMotorsParams : virtual public ::IceDelegate::jderobot::PTMotorsParams,
                       virtual public ::IceDelegateM::Ice::Object
{
public:
};

class PTMotors : virtual public ::IceDelegate::jderobot::PTMotors,
                 virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Context*);

    virtual ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class PTMotorsData : virtual public ::IceDelegate::jderobot::PTMotorsData,
                     virtual public ::IceDelegateD::Ice::Object
{
public:
};

class PTMotorsParams : virtual public ::IceDelegate::jderobot::PTMotorsParams,
                       virtual public ::IceDelegateD::Ice::Object
{
public:
};

class PTMotors : virtual public ::IceDelegate::jderobot::PTMotors,
                 virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Context*);

    virtual ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class PTMotorsData : virtual public ::Ice::Object
{
public:

    typedef PTMotorsDataPrx ProxyType;
    typedef PTMotorsDataPtr PointerType;
    
    PTMotorsData() {}
    PTMotorsData(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);
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

    virtual ~PTMotorsData() {}

    friend class PTMotorsData__staticInit;

public:

    ::Ice::Float latitude;

    ::Ice::Float latitudeSpeed;

    ::Ice::Float longitude;

    ::Ice::Float longitudeSpeed;
};

class PTMotorsData__staticInit
{
public:

    ::jderobot::PTMotorsData _init;
};

static PTMotorsData__staticInit _PTMotorsData_init;

class PTMotorsParams : virtual public ::Ice::Object
{
public:

    typedef PTMotorsParamsPrx ProxyType;
    typedef PTMotorsParamsPtr PointerType;
    
    PTMotorsParams() {}
    PTMotorsParams(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);
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

    virtual ~PTMotorsParams() {}

public:

    ::Ice::Float maxLongitude;

    ::Ice::Float minLongitude;

    ::Ice::Float maxLatitude;

    ::Ice::Float minLatitude;

    ::Ice::Float maxLongitudeSpeed;

    ::Ice::Float maxLatitudeSpeed;
};

class PTMotors : virtual public ::Ice::Object
{
public:

    typedef PTMotorsPrx ProxyType;
    typedef PTMotorsPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int setPTMotorsData(const ::jderobot::PTMotorsDataPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setPTMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::PTMotorsDataPtr getPTMotorsData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPTMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::PTMotorsParamsPtr getPTMotorsParams(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPTMotorsParams(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
