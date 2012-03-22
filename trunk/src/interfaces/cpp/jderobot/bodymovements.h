// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodymovements.ice'

#ifndef __bodymovements_h__
#define __bodymovements_h__

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

class BodyMovementsData;

class BodyMovements;

}

}

namespace jderobot
{

class BodyMovementsData;
bool operator==(const BodyMovementsData&, const BodyMovementsData&);
bool operator<(const BodyMovementsData&, const BodyMovementsData&);

class BodyMovements;
bool operator==(const BodyMovements&, const BodyMovements&);
bool operator<(const BodyMovements&, const BodyMovements&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::BodyMovementsData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::BodyMovementsData*);

::Ice::Object* upCast(::jderobot::BodyMovements*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::BodyMovements*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::BodyMovementsData> BodyMovementsDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::BodyMovementsData> BodyMovementsDataPrx;

void __read(::IceInternal::BasicStream*, BodyMovementsDataPrx&);
void __patch__BodyMovementsDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::BodyMovements> BodyMovementsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::BodyMovements> BodyMovementsPrx;

void __read(::IceInternal::BasicStream*, BodyMovementsPrx&);
void __patch__BodyMovementsPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

struct ArmPosition
{
    ::jderobot::BodyMotor shoulder;
    ::jderobot::BodyMotor elbow;

    bool operator==(const ArmPosition&) const;
    bool operator<(const ArmPosition&) const;
    bool operator!=(const ArmPosition& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ArmPosition& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ArmPosition& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ArmPosition& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct LegPosition
{
    ::jderobot::BodyMotor hip;
    ::jderobot::BodyMotor knee;
    ::jderobot::BodyMotor ankle;

    bool operator==(const LegPosition&) const;
    bool operator<(const LegPosition&) const;
    bool operator!=(const LegPosition& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const LegPosition& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const LegPosition& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const LegPosition& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct BodyPosition
{
    ::jderobot::ArmPosition lArm;
    ::jderobot::ArmPosition rArm;
    ::jderobot::LegPosition rLeg;
    ::jderobot::LegPosition lLeg;
    ::jderobot::BodyMotor head;
    ::Ice::Float time;

    bool operator==(const BodyPosition&) const;
    bool operator<(const BodyPosition&) const;
    bool operator!=(const BodyPosition& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const BodyPosition& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const BodyPosition& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const BodyPosition& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::jderobot::BodyPosition> BodyMov;
void __writeBodyMov(::IceInternal::BasicStream*, const ::jderobot::BodyPosition*, const ::jderobot::BodyPosition*);
void __readBodyMov(::IceInternal::BasicStream*, BodyMov&);

}

namespace IceProxy
{

namespace jderobot
{

class BodyMovementsData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovementsData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovementsData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<BodyMovementsData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class BodyMovements : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr& data)
    {
        return doMovement(data, 0);
    }
    ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr& data, const ::Ice::Context& __ctx)
    {
        return doMovement(data, &__ctx);
    }
    
private:

    ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr&, const ::Ice::Context*);
    
public:

    ::jderobot::BodyMovementsDataPtr getMovement()
    {
        return getMovement(0);
    }
    ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Context& __ctx)
    {
        return getMovement(&__ctx);
    }
    
private:

    ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMovements> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMovements*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<BodyMovements*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class BodyMovementsData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class BodyMovements : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr&, const ::Ice::Context*) = 0;

    virtual ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class BodyMovementsData : virtual public ::IceDelegate::jderobot::BodyMovementsData,
                          virtual public ::IceDelegateM::Ice::Object
{
public:
};

class BodyMovements : virtual public ::IceDelegate::jderobot::BodyMovements,
                      virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class BodyMovementsData : virtual public ::IceDelegate::jderobot::BodyMovementsData,
                          virtual public ::IceDelegateD::Ice::Object
{
public:
};

class BodyMovements : virtual public ::IceDelegate::jderobot::BodyMovements,
                      virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr&, const ::Ice::Context*);

    virtual ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class BodyMovementsData : virtual public ::Ice::Object
{
public:

    typedef BodyMovementsDataPrx ProxyType;
    typedef BodyMovementsDataPtr PointerType;
    
    BodyMovementsData() {}
    explicit BodyMovementsData(const ::jderobot::BodyMov&);
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

    virtual ~BodyMovementsData() {}

    friend class BodyMovementsData__staticInit;

public:

    ::jderobot::BodyMov mov;
};

class BodyMovementsData__staticInit
{
public:

    ::jderobot::BodyMovementsData _init;
};

static BodyMovementsData__staticInit _BodyMovementsData_init;

class BodyMovements : virtual public ::Ice::Object
{
public:

    typedef BodyMovementsPrx ProxyType;
    typedef BodyMovementsPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int doMovement(const ::jderobot::BodyMovementsDataPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___doMovement(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::jderobot::BodyMovementsDataPtr getMovement(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getMovement(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
