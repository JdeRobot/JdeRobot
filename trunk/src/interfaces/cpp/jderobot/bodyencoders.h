// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodyencoders.ice'

#ifndef __bodyencoders_h__
#define __bodyencoders_h__

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
#include <jderobot/body.h>
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

class ArmEncodersData;

class LegEncodersData;

class OdometryData;

class BodyEncoders;

}

}

namespace jderobot
{

class ArmEncodersData;
bool operator==(const ArmEncodersData&, const ArmEncodersData&);
bool operator<(const ArmEncodersData&, const ArmEncodersData&);

class LegEncodersData;
bool operator==(const LegEncodersData&, const LegEncodersData&);
bool operator<(const LegEncodersData&, const LegEncodersData&);

class OdometryData;
bool operator==(const OdometryData&, const OdometryData&);
bool operator<(const OdometryData&, const OdometryData&);

class BodyEncoders;
bool operator==(const BodyEncoders&, const BodyEncoders&);
bool operator<(const BodyEncoders&, const BodyEncoders&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::ArmEncodersData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::ArmEncodersData*);

::Ice::Object* upCast(::jderobot::LegEncodersData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::LegEncodersData*);

::Ice::Object* upCast(::jderobot::OdometryData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::OdometryData*);

::Ice::Object* upCast(::jderobot::BodyEncoders*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::BodyEncoders*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::ArmEncodersData> ArmEncodersDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::ArmEncodersData> ArmEncodersDataPrx;

void __read(::IceInternal::BasicStream*, ArmEncodersDataPrx&);
void __patch__ArmEncodersDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::LegEncodersData> LegEncodersDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::LegEncodersData> LegEncodersDataPrx;

void __read(::IceInternal::BasicStream*, LegEncodersDataPrx&);
void __patch__LegEncodersDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::OdometryData> OdometryDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::OdometryData> OdometryDataPrx;

void __read(::IceInternal::BasicStream*, OdometryDataPrx&);
void __patch__OdometryDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::BodyEncoders> BodyEncodersPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::BodyEncoders> BodyEncodersPrx;

void __read(::IceInternal::BasicStream*, BodyEncodersPrx&);
void __patch__BodyEncodersPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace jderobot
{

class ArmEncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ArmEncodersData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ArmEncodersData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ArmEncodersData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class LegEncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LegEncodersData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LegEncodersData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<LegEncodersData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class OdometryData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<OdometryData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<OdometryData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<OdometryData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<OdometryData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class BodyEncoders : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide side)
    {
        return getArmEncodersData(side, 0);
    }
    ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide side, const ::Ice::Context& __ctx)
    {
        return getArmEncodersData(side, &__ctx);
    }
    
private:

    ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide, const ::Ice::Context*);
    
public:

    ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide side)
    {
        return getLegEncodersData(side, 0);
    }
    ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide side, const ::Ice::Context& __ctx)
    {
        return getLegEncodersData(side, &__ctx);
    }
    
private:

    ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide, const ::Ice::Context*);
    
public:

    ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody camera)
    {
        return getOdometryData(camera, 0);
    }
    ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody camera, const ::Ice::Context& __ctx)
    {
        return getOdometryData(camera, &__ctx);
    }
    
private:

    ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyEncoders> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyEncoders*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<BodyEncoders*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class ArmEncodersData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class LegEncodersData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class OdometryData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class BodyEncoders : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide, const ::Ice::Context*) = 0;

    virtual ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide, const ::Ice::Context*) = 0;

    virtual ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class ArmEncodersData : virtual public ::IceDelegate::jderobot::ArmEncodersData,
                        virtual public ::IceDelegateM::Ice::Object
{
public:
};

class LegEncodersData : virtual public ::IceDelegate::jderobot::LegEncodersData,
                        virtual public ::IceDelegateM::Ice::Object
{
public:
};

class OdometryData : virtual public ::IceDelegate::jderobot::OdometryData,
                     virtual public ::IceDelegateM::Ice::Object
{
public:
};

class BodyEncoders : virtual public ::IceDelegate::jderobot::BodyEncoders,
                     virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide, const ::Ice::Context*);

    virtual ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide, const ::Ice::Context*);

    virtual ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class ArmEncodersData : virtual public ::IceDelegate::jderobot::ArmEncodersData,
                        virtual public ::IceDelegateD::Ice::Object
{
public:
};

class LegEncodersData : virtual public ::IceDelegate::jderobot::LegEncodersData,
                        virtual public ::IceDelegateD::Ice::Object
{
public:
};

class OdometryData : virtual public ::IceDelegate::jderobot::OdometryData,
                     virtual public ::IceDelegateD::Ice::Object
{
public:
};

class BodyEncoders : virtual public ::IceDelegate::jderobot::BodyEncoders,
                     virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide, const ::Ice::Context*);

    virtual ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide, const ::Ice::Context*);

    virtual ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody, const ::Ice::Context*);
};

}

}

namespace jderobot
{

class ArmEncodersData : virtual public ::Ice::Object
{
public:

    typedef ArmEncodersDataPrx ProxyType;
    typedef ArmEncodersDataPtr PointerType;
    
    ArmEncodersData() {}
    ArmEncodersData(const ::jderobot::BodyMotor&, const ::jderobot::BodyMotor&, ::Ice::Int);
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

    virtual ~ArmEncodersData() {}

    friend class ArmEncodersData__staticInit;

public:

    ::jderobot::BodyMotor shoulder;

    ::jderobot::BodyMotor elbow;

    ::Ice::Int clock;
};

class ArmEncodersData__staticInit
{
public:

    ::jderobot::ArmEncodersData _init;
};

static ArmEncodersData__staticInit _ArmEncodersData_init;

class LegEncodersData : virtual public ::Ice::Object
{
public:

    typedef LegEncodersDataPrx ProxyType;
    typedef LegEncodersDataPtr PointerType;
    
    LegEncodersData() {}
    LegEncodersData(const ::jderobot::BodyMotor&, const ::jderobot::BodyMotor&, const ::jderobot::BodyMotor&, ::Ice::Int);
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

    virtual ~LegEncodersData() {}

public:

    ::jderobot::BodyMotor hip;

    ::jderobot::BodyMotor knee;

    ::jderobot::BodyMotor ankle;

    ::Ice::Int clock;
};

class OdometryData : virtual public ::Ice::Object
{
public:

    typedef OdometryDataPrx ProxyType;
    typedef OdometryDataPtr PointerType;
    
    OdometryData() {}
    explicit OdometryData(const ::jderobot::seqFloat&);
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

    virtual ~OdometryData() {}

public:

    ::jderobot::seqFloat odometry;
};

class BodyEncoders : virtual public ::Ice::Object
{
public:

    typedef BodyEncodersPrx ProxyType;
    typedef BodyEncodersPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::ArmEncodersDataPtr getArmEncodersData(::jderobot::BodySide, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getArmEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::LegEncodersDataPtr getLegEncodersData(::jderobot::BodySide, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getLegEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::OdometryDataPtr getOdometryData(::jderobot::CameraBody, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getOdometryData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
