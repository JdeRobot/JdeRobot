// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodymotors.ice'

#ifndef __bodymotors_h__
#define __bodymotors_h__

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

class BodyMotorsParam;

class BodyMotors;

}

}

namespace jderobot
{

class BodyMotorsParam;
bool operator==(const BodyMotorsParam&, const BodyMotorsParam&);
bool operator<(const BodyMotorsParam&, const BodyMotorsParam&);

class BodyMotors;
bool operator==(const BodyMotors&, const BodyMotors&);
bool operator<(const BodyMotors&, const BodyMotors&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::BodyMotorsParam*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::BodyMotorsParam*);

::Ice::Object* upCast(::jderobot::BodyMotors*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::BodyMotors*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::BodyMotorsParam> BodyMotorsParamPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::BodyMotorsParam> BodyMotorsParamPrx;

void __read(::IceInternal::BasicStream*, BodyMotorsParamPrx&);
void __patch__BodyMotorsParamPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::BodyMotors> BodyMotorsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::BodyMotors> BodyMotorsPrx;

void __read(::IceInternal::BasicStream*, BodyMotorsPrx&);
void __patch__BodyMotorsPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace jderobot
{

class BodyMotorsParam : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotorsParam> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotorsParam*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<BodyMotorsParam*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class BodyMotors : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName name, ::jderobot::BodySide side)
    {
        return getBodyMotorsParam(name, side, 0);
    }
    ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName name, ::jderobot::BodySide side, const ::Ice::Context& __ctx)
    {
        return getBodyMotorsParam(name, side, &__ctx);
    }
    
private:

    ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName, ::jderobot::BodySide, const ::Ice::Context*);
    
public:

    ::Ice::Int setBodyMotorsData(::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed)
    {
        return setBodyMotorsData(name, side, angle, speed, 0);
    }
    ::Ice::Int setBodyMotorsData(::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed, const ::Ice::Context& __ctx)
    {
        return setBodyMotorsData(name, side, angle, speed, &__ctx);
    }
    
private:

    ::Ice::Int setBodyMotorsData(::jderobot::MotorsName, ::jderobot::BodySide, ::Ice::Float, ::Ice::Float, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<BodyMotors> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<BodyMotors*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<BodyMotors*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class BodyMotorsParam : virtual public ::IceDelegate::Ice::Object
{
public:
};

class BodyMotors : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName, ::jderobot::BodySide, const ::Ice::Context*) = 0;

    virtual ::Ice::Int setBodyMotorsData(::jderobot::MotorsName, ::jderobot::BodySide, ::Ice::Float, ::Ice::Float, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class BodyMotorsParam : virtual public ::IceDelegate::jderobot::BodyMotorsParam,
                        virtual public ::IceDelegateM::Ice::Object
{
public:
};

class BodyMotors : virtual public ::IceDelegate::jderobot::BodyMotors,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName, ::jderobot::BodySide, const ::Ice::Context*);

    virtual ::Ice::Int setBodyMotorsData(::jderobot::MotorsName, ::jderobot::BodySide, ::Ice::Float, ::Ice::Float, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class BodyMotorsParam : virtual public ::IceDelegate::jderobot::BodyMotorsParam,
                        virtual public ::IceDelegateD::Ice::Object
{
public:
};

class BodyMotors : virtual public ::IceDelegate::jderobot::BodyMotors,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName, ::jderobot::BodySide, const ::Ice::Context*);

    virtual ::Ice::Int setBodyMotorsData(::jderobot::MotorsName, ::jderobot::BodySide, ::Ice::Float, ::Ice::Float, const ::Ice::Context*);
};

}

}

namespace jderobot
{

class BodyMotorsParam : virtual public ::Ice::Object
{
public:

    typedef BodyMotorsParamPrx ProxyType;
    typedef BodyMotorsParamPtr PointerType;
    
    BodyMotorsParam() {}
    BodyMotorsParam(::Ice::Float, ::Ice::Float, ::Ice::Float);
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

    virtual ~BodyMotorsParam() {}

    friend class BodyMotorsParam__staticInit;

public:

    ::Ice::Float minAngle;

    ::Ice::Float maxAngle;

    ::Ice::Float maxSpeed;
};

class BodyMotorsParam__staticInit
{
public:

    ::jderobot::BodyMotorsParam _init;
};

static BodyMotorsParam__staticInit _BodyMotorsParam_init;

class BodyMotors : virtual public ::Ice::Object
{
public:

    typedef BodyMotorsPrx ProxyType;
    typedef BodyMotorsPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::BodyMotorsParamPtr getBodyMotorsParam(::jderobot::MotorsName, ::jderobot::BodySide, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getBodyMotorsParam(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int setBodyMotorsData(::jderobot::MotorsName, ::jderobot::BodySide, ::Ice::Float, ::Ice::Float, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setBodyMotorsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
