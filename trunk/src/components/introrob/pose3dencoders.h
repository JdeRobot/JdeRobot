// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `pose3dencoders.ice'

#ifndef __pose3dencoders_h__
#define __pose3dencoders_h__

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

namespace IceProxy
{

namespace jderobot
{

class Pose3DEncodersData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncodersData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncodersData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Pose3DEncodersData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
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
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Pose3DEncoders> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Pose3DEncoders*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Pose3DEncoders*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class Pose3DEncodersData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Pose3DEncoders : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class Pose3DEncodersData : virtual public ::IceDelegate::jderobot::Pose3DEncodersData,
                           virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Pose3DEncoders : virtual public ::IceDelegate::jderobot::Pose3DEncoders,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class Pose3DEncodersData : virtual public ::IceDelegate::jderobot::Pose3DEncodersData,
                           virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Pose3DEncoders : virtual public ::IceDelegate::jderobot::Pose3DEncoders,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Context*);
};

}

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

    virtual ~Pose3DEncodersData() {}

    friend class Pose3DEncodersData__staticInit;

public:

    ::Ice::Float x;

    ::Ice::Float y;

    ::Ice::Float z;

    ::Ice::Float pan;

    ::Ice::Float tilt;

    ::Ice::Float roll;

    ::Ice::Int clock;
};

class Pose3DEncodersData__staticInit
{
public:

    ::jderobot::Pose3DEncodersData _init;
};

static Pose3DEncodersData__staticInit _Pose3DEncodersData_init;

class Pose3DEncoders : virtual public ::Ice::Object
{
public:

    typedef Pose3DEncodersPrx ProxyType;
    typedef Pose3DEncodersPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::Pose3DEncodersDataPtr getPose3DEncodersData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPose3DEncodersData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
