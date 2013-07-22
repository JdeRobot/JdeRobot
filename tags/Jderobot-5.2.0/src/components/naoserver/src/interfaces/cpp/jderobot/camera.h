// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `camera.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_camera_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_camera_h__

#include <IceE/ProxyF.h>
#include <IceE/ObjectF.h>
#include <IceE/Exception.h>
#include <IceE/ScopedArray.h>
#include <IceE/Proxy.h>
#include <IceE/Object.h>
#ifndef ICEE_PURE_CLIENT
#  include <IceE/Incoming.h>
#endif
#include <IceE/Outgoing.h>
#include <IceE/UserExceptionFactory.h>
#include <IceE/FactoryTable.h>
#include <jderobot/image.h>
#include <IceE/UndefSysMacros.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
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

namespace jderobot
{

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

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~CameraDescription() {}


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

class Camera : virtual public ::jderobot::ImageProvider
{
public:

    typedef CameraPrx ProxyType;
    typedef CameraPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::CameraDescriptionPtr getCameraDescription(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getCameraDescription(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::Ice::Int setCameraDescription(const ::jderobot::CameraDescriptionPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setCameraDescription(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::std::string startCameraStreaming(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___startCameraStreaming(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void stopCameraStreaming(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___stopCameraStreaming(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

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
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_secure(bool __secure) const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<CameraDescription> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<CameraDescription> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_twoway() const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_oneway() const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_batchOneway() const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_datagram() const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_batchDatagram() const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<CameraDescription> ice_timeout(int __timeout) const
    {
        return dynamic_cast<CameraDescription*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
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
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_secure(bool __secure) const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<Camera> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<Camera> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<Camera> ice_twoway() const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_oneway() const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_batchOneway() const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_datagram() const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_batchDatagram() const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<Camera> ice_timeout(int __timeout) const
    {
        return dynamic_cast<Camera*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
