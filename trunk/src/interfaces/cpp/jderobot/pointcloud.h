// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `pointcloud.ice'

#ifndef __pointcloud_h__
#define __pointcloud_h__

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

class pointCloudData;

class pointCloud;

}

}

namespace jderobot
{

class pointCloudData;
bool operator==(const pointCloudData&, const pointCloudData&);
bool operator<(const pointCloudData&, const pointCloudData&);

class pointCloud;
bool operator==(const pointCloud&, const pointCloud&);
bool operator<(const pointCloud&, const pointCloud&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::pointCloudData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::pointCloudData*);

::Ice::Object* upCast(::jderobot::pointCloud*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::pointCloud*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::pointCloudData> pointCloudDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::pointCloudData> pointCloudDataPrx;

void __read(::IceInternal::BasicStream*, pointCloudDataPrx&);
void __patch__pointCloudDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::pointCloud> pointCloudPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::pointCloud> pointCloudPrx;

void __read(::IceInternal::BasicStream*, pointCloudPrx&);
void __patch__pointCloudPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

struct RGBPoint
{
    ::Ice::Float x;
    ::Ice::Float y;
    ::Ice::Float z;
    ::Ice::Float r;
    ::Ice::Float g;
    ::Ice::Float b;

    bool operator==(const RGBPoint&) const;
    bool operator<(const RGBPoint&) const;
    bool operator!=(const RGBPoint& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const RGBPoint& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const RGBPoint& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const RGBPoint& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::jderobot::RGBPoint> RGBPointsPCL;
void __writeRGBPointsPCL(::IceInternal::BasicStream*, const ::jderobot::RGBPoint*, const ::jderobot::RGBPoint*);
void __readRGBPointsPCL(::IceInternal::BasicStream*, RGBPointsPCL&);

}

namespace IceProxy
{

namespace jderobot
{

class pointCloudData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloudData> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloudData*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<pointCloudData*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class pointCloud : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::pointCloudDataPtr getCloudData()
    {
        return getCloudData(0);
    }
    ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Context& __ctx)
    {
        return getCloudData(&__ctx);
    }
    
private:

    ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<pointCloud> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<pointCloud> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<pointCloud*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<pointCloud*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class pointCloudData : virtual public ::IceDelegate::Ice::Object
{
public:
};

class pointCloud : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class pointCloudData : virtual public ::IceDelegate::jderobot::pointCloudData,
                       virtual public ::IceDelegateM::Ice::Object
{
public:
};

class pointCloud : virtual public ::IceDelegate::jderobot::pointCloud,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class pointCloudData : virtual public ::IceDelegate::jderobot::pointCloudData,
                       virtual public ::IceDelegateD::Ice::Object
{
public:
};

class pointCloud : virtual public ::IceDelegate::jderobot::pointCloud,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class pointCloudData : virtual public ::Ice::Object
{
public:

    typedef pointCloudDataPrx ProxyType;
    typedef pointCloudDataPtr PointerType;
    
    pointCloudData() {}
    explicit pointCloudData(const ::jderobot::RGBPointsPCL&);
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

    virtual ~pointCloudData() {}

    friend class pointCloudData__staticInit;

public:

    ::jderobot::RGBPointsPCL p;
};

class pointCloudData__staticInit
{
public:

    ::jderobot::pointCloudData _init;
};

static pointCloudData__staticInit _pointCloudData_init;

class pointCloud : virtual public ::Ice::Object
{
public:

    typedef pointCloudPrx ProxyType;
    typedef pointCloudPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::pointCloudDataPtr getCloudData(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getCloudData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
