// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `sonars.ice'

#ifndef __sonars_h__
#define __sonars_h__

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

class SonarsData;

class Sonars;

}

}

namespace jderobot
{

class SonarsData;
bool operator==(const SonarsData&, const SonarsData&);
bool operator<(const SonarsData&, const SonarsData&);

class Sonars;
bool operator==(const Sonars&, const Sonars&);
bool operator<(const Sonars&, const Sonars&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::SonarsData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::SonarsData*);

::Ice::Object* upCast(::jderobot::Sonars*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Sonars*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::SonarsData> SonarsDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::SonarsData> SonarsDataPrx;

void __read(::IceInternal::BasicStream*, SonarsDataPrx&);
void __patch__SonarsDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Sonars> SonarsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Sonars> SonarsPrx;

void __read(::IceInternal::BasicStream*, SonarsPrx&);
void __patch__SonarsPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace jderobot
{

class SonarsData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<SonarsData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<SonarsData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<SonarsData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<SonarsData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Sonars : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::SonarsDataPtr getSonarsData()
    {
        return getSonarsData(0);
    }
    ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Context& __ctx)
    {
        return getSonarsData(&__ctx);
    }
    
private:

    ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Sonars> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Sonars> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Sonars*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Sonars*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class SonarsData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Sonars : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class SonarsData : virtual public ::IceDelegate::jderobot::SonarsData,
                   virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Sonars : virtual public ::IceDelegate::jderobot::Sonars,
               virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class SonarsData : virtual public ::IceDelegate::jderobot::SonarsData,
                   virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Sonars : virtual public ::IceDelegate::jderobot::Sonars,
               virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class SonarsData : virtual public ::Ice::Object
{
public:

    typedef SonarsDataPrx ProxyType;
    typedef SonarsDataPtr PointerType;
    
    SonarsData() {}
    SonarsData(const ::jderobot::IntSeq&, ::Ice::Int);
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

    virtual ~SonarsData() {}

    friend class SonarsData__staticInit;

public:

    ::jderobot::IntSeq us;

    ::Ice::Int numSonars;
};

class SonarsData__staticInit
{
public:

    ::jderobot::SonarsData _init;
};

static SonarsData__staticInit _SonarsData_init;

class Sonars : virtual public ::Ice::Object
{
public:

    typedef SonarsPrx ProxyType;
    typedef SonarsPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::SonarsDataPtr getSonarsData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getSonarsData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
