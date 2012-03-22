// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `encoders.ice'

#ifndef __encoders_h__
#define __encoders_h__

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

namespace IceProxy
{

namespace jderobot
{

class EncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<EncodersData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<EncodersData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<EncodersData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<EncodersData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
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
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Encoders> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Encoders*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Encoders*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class EncodersData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Encoders : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class EncodersData : virtual public ::IceDelegate::jderobot::EncodersData,
                     virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Encoders : virtual public ::IceDelegate::jderobot::Encoders,
                 virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class EncodersData : virtual public ::IceDelegate::jderobot::EncodersData,
                     virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Encoders : virtual public ::IceDelegate::jderobot::Encoders,
                 virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Context*);
};

}

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

    virtual ~EncodersData() {}

    friend class EncodersData__staticInit;

public:

    ::Ice::Float robotx;

    ::Ice::Float roboty;

    ::Ice::Float robottheta;

    ::Ice::Float robotcos;

    ::Ice::Float robotsin;
};

class EncodersData__staticInit
{
public:

    ::jderobot::EncodersData _init;
};

static EncodersData__staticInit _EncodersData_init;

class Encoders : virtual public ::Ice::Object
{
public:

    typedef EncodersPrx ProxyType;
    typedef EncodersPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::EncodersDataPtr getEncodersData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
