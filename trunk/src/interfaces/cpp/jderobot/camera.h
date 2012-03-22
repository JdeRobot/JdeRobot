// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `camera.ice'

#ifndef __camera_h__
#define __camera_h__

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

class CameraDescription;

class Camera;

}

}

namespace jderobot
{

class CameraDescription;
bool operator==(const CameraDescription&, const CameraDescription&);
bool operator<(const CameraDescription&, const CameraDescription&);

class Camera;
bool operator==(const Camera&, const Camera&);
bool operator<(const Camera&, const Camera&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::CameraDescription*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::CameraDescription*);

::Ice::Object* upCast(::jderobot::Camera*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::Camera*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::CameraDescription> CameraDescriptionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::CameraDescription> CameraDescriptionPrx;

void __read(::IceInternal::BasicStream*, CameraDescriptionPrx&);
void __patch__CameraDescriptionPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::Camera> CameraPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::Camera> CameraPrx;

void __read(::IceInternal::BasicStream*, CameraPrx&);
void __patch__CameraPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceAsync
{

}

namespace IceProxy
{

namespace jderobot
{

class CameraDescription : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CameraDescription*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class Camera : virtual public ::IceProxy::jderobot::ImageProvider
{
public:

    ::jderobot::CameraDescriptionPtr getCameraDescription()
    {
        return getCameraDescription(0);
    }
    ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Context& __ctx)
    {
        return getCameraDescription(&__ctx);
    }
    
private:

    ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Context*);
    
public:

    ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr& description)
    {
        return setCameraDescription(description, 0);
    }
    ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Context& __ctx)
    {
        return setCameraDescription(description, &__ctx);
    }
    
private:

    ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Context*);
    
public:

    ::std::string startCameraStreaming()
    {
        return startCameraStreaming(0);
    }
    ::std::string startCameraStreaming(const ::Ice::Context& __ctx)
    {
        return startCameraStreaming(&__ctx);
    }
    
private:

    ::std::string startCameraStreaming(const ::Ice::Context*);
    
public:

    void stopCameraStreaming()
    {
        stopCameraStreaming(0);
    }
    void stopCameraStreaming(const ::Ice::Context& __ctx)
    {
        stopCameraStreaming(&__ctx);
    }
    
private:

    void stopCameraStreaming(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<Camera> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<Camera*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class CameraDescription : virtual public ::IceDelegate::Ice::Object
{
public:
};

class Camera : virtual public ::IceDelegate::jderobot::ImageProvider
{
public:

    virtual ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Context*) = 0;

    virtual ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Context*) = 0;

    virtual ::std::string startCameraStreaming(const ::Ice::Context*) = 0;

    virtual void stopCameraStreaming(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace jderobot
{

class CameraDescription : virtual public ::IceDelegate::jderobot::CameraDescription,
                          virtual public ::IceDelegateM::Ice::Object
{
public:
};

class Camera : virtual public ::IceDelegate::jderobot::Camera,
               virtual public ::IceDelegateM::jderobot::ImageProvider
{
public:

    virtual ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Context*);

    virtual ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Context*);

    virtual ::std::string startCameraStreaming(const ::Ice::Context*);

    virtual void stopCameraStreaming(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace jderobot
{

class CameraDescription : virtual public ::IceDelegate::jderobot::CameraDescription,
                          virtual public ::IceDelegateD::Ice::Object
{
public:
};

class Camera : virtual public ::IceDelegate::jderobot::Camera,
               virtual public ::IceDelegateD::jderobot::ImageProvider
{
public:

    virtual ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Context*);

    virtual ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Context*);

    virtual ::std::string startCameraStreaming(const ::Ice::Context*);

    virtual void stopCameraStreaming(const ::Ice::Context*);
};

}

}

namespace jderobot
{

class CameraDescription : virtual public ::Ice::Object
{
public:

    typedef CameraDescriptionPrx ProxyType;
    typedef CameraDescriptionPtr PointerType;
    
    CameraDescription() {}
    CameraDescription(const ::std::string&, const ::std::string&, const ::std::string&, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);
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

    virtual ~CameraDescription() {}

    friend class CameraDescription__staticInit;

public:

    ::std::string name;

    ::std::string shortDescription;

    ::std::string streamingUri;

    ::Ice::Float fdistx;

    ::Ice::Float fdisty;

    ::Ice::Float u0;

    ::Ice::Float v0;

    ::Ice::Float skew;

    ::Ice::Float posx;

    ::Ice::Float posy;

    ::Ice::Float posz;

    ::Ice::Float foax;

    ::Ice::Float foay;

    ::Ice::Float foaz;

    ::Ice::Float roll;
};

class CameraDescription__staticInit
{
public:

    ::jderobot::CameraDescription _init;
};

static CameraDescription__staticInit _CameraDescription_init;

class Camera : virtual public ::jderobot::ImageProvider
{
public:

    typedef CameraPrx ProxyType;
    typedef CameraPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getCameraDescription(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setCameraDescription(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string startCameraStreaming(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___startCameraStreaming(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopCameraStreaming(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___stopCameraStreaming(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
