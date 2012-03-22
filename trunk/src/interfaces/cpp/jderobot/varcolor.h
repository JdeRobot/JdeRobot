// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `varcolor.ice'

#ifndef __varcolor_h__
#define __varcolor_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/IncomingAsync.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <jderobot/image.h>
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

class VarColor;

}

}

namespace jderobot
{

class VarColor;
bool operator==(const VarColor&, const VarColor&);
bool operator<(const VarColor&, const VarColor&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::VarColor*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::VarColor*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::VarColor> VarColorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::VarColor> VarColorPrx;

void __read(::IceInternal::BasicStream*, VarColorPrx&);
void __patch__VarColorPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

class AMD_VarColor_getData : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::jderobot::ImageDataPtr&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::jderobot::AMD_VarColor_getData> AMD_VarColor_getDataPtr;

}

namespace IceAsync
{

namespace jderobot
{

class AMD_VarColor_getData : public ::jderobot::AMD_VarColor_getData, public ::IceInternal::IncomingAsync
{
public:

    AMD_VarColor_getData(::IceInternal::Incoming&);

    virtual void ice_response(const ::jderobot::ImageDataPtr&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

}

}

namespace IceProxy
{

namespace jderobot
{

class VarColor : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::ImageDescriptionPtr getDescription()
    {
        return getDescription(0);
    }
    ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Context& __ctx)
    {
        return getDescription(&__ctx);
    }
    
private:

    ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Context*);
    
public:

    ::jderobot::ImageDataPtr getData()
    {
        return getData(0);
    }
    ::jderobot::ImageDataPtr getData(const ::Ice::Context& __ctx)
    {
        return getData(&__ctx);
    }
    
private:

    ::jderobot::ImageDataPtr getData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<VarColor> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<VarColor> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<VarColor*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<VarColor*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class VarColor : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Context*) = 0;

    virtual ::jderobot::ImageDataPtr getData(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class VarColor : virtual public ::IceDelegate::jderobot::VarColor,
                 virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Context*);

    virtual ::jderobot::ImageDataPtr getData(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class VarColor : virtual public ::IceDelegate::jderobot::VarColor,
                 virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Context*);

    virtual ::jderobot::ImageDataPtr getData(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class VarColor : virtual public ::Ice::Object
{
public:

    typedef VarColorPrx ProxyType;
    typedef VarColorPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::ImageDescriptionPtr getDescription(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getDescription(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void getData_async(const ::jderobot::AMD_VarColor_getDataPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
