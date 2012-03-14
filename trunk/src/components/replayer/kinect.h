// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `kinect.ice'

#ifndef __kinect_h__
#define __kinect_h__

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
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
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

class PuntosPCLData;

class PuntosPCLInterface;

}

}

namespace jderobot
{

class PuntosPCLData;
bool operator==(const PuntosPCLData&, const PuntosPCLData&);
bool operator<(const PuntosPCLData&, const PuntosPCLData&);

class PuntosPCLInterface;
bool operator==(const PuntosPCLInterface&, const PuntosPCLInterface&);
bool operator<(const PuntosPCLInterface&, const PuntosPCLInterface&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::PuntosPCLData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::PuntosPCLData*);

::Ice::Object* upCast(::jderobot::PuntosPCLInterface*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::PuntosPCLInterface*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::PuntosPCLData> PuntosPCLDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::PuntosPCLData> PuntosPCLDataPrx;

void __read(::IceInternal::BasicStream*, PuntosPCLDataPrx&);
void __patch__PuntosPCLDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::PuntosPCLInterface> PuntosPCLInterfacePtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::PuntosPCLInterface> PuntosPCLInterfacePrx;

void __read(::IceInternal::BasicStream*, PuntosPCLInterfacePrx&);
void __patch__PuntosPCLInterfacePtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

struct Puntos
{
    ::Ice::Float x;
    ::Ice::Float y;
    ::Ice::Float z;
    ::Ice::Float r;
    ::Ice::Float g;
    ::Ice::Float b;

    bool operator==(const Puntos&) const;
    bool operator<(const Puntos&) const;
    bool operator!=(const Puntos& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const Puntos& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Puntos& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Puntos& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::jderobot::Puntos> PuntosPCL;
void __writePuntosPCL(::IceInternal::BasicStream*, const ::jderobot::Puntos*, const ::jderobot::Puntos*);
void __readPuntosPCL(::IceInternal::BasicStream*, PuntosPCL&);

}

namespace IceProxy
{

namespace jderobot
{

class PuntosPCLData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<PuntosPCLData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class PuntosPCLInterface : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::PuntosPCLDataPtr getKinectData()
    {
        return getKinectData(0);
    }
    ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Context& __ctx)
    {
        return getKinectData(&__ctx);
    }
    
private:

    ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<PuntosPCLInterface> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<PuntosPCLInterface*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<PuntosPCLInterface*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class PuntosPCLData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class PuntosPCLInterface : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class PuntosPCLData : virtual public ::IceDelegate::jderobot::PuntosPCLData,
                      virtual public ::IceDelegateM::Ice::Object
{
public:
};

class PuntosPCLInterface : virtual public ::IceDelegate::jderobot::PuntosPCLInterface,
                           virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class PuntosPCLData : virtual public ::IceDelegate::jderobot::PuntosPCLData,
                      virtual public ::IceDelegateD::Ice::Object
{
public:
};

class PuntosPCLInterface : virtual public ::IceDelegate::jderobot::PuntosPCLInterface,
                           virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class PuntosPCLData : virtual public ::Ice::Object
{
public:

    typedef PuntosPCLDataPrx ProxyType;
    typedef PuntosPCLDataPtr PointerType;
    
    PuntosPCLData() {}
    explicit PuntosPCLData(const ::jderobot::PuntosPCL&);
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

    virtual ~PuntosPCLData() {}

    friend class PuntosPCLData__staticInit;

public:

    ::jderobot::PuntosPCL p;
};

class PuntosPCLData__staticInit
{
public:

    ::jderobot::PuntosPCLData _init;
};

static PuntosPCLData__staticInit _PuntosPCLData_init;

class PuntosPCLInterface : virtual public ::Ice::Object
{
public:

    typedef PuntosPCLInterfacePrx ProxyType;
    typedef PuntosPCLInterfacePtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::PuntosPCLDataPtr getKinectData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getKinectData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
