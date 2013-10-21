// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `image.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_image_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_image_h__

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
#include <jderobot/common.h>
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

class ImageDescription;

class ImageData;

class HSVFilter;

class CalibrationParams;

class ImageProvider;

class ImageSelector;

class CalibrationProvider;

}

}

namespace jderobot
{

class ImageDescription;
bool operator==(const ImageDescription&, const ImageDescription&);
bool operator<(const ImageDescription&, const ImageDescription&);

class ImageData;
bool operator==(const ImageData&, const ImageData&);
bool operator<(const ImageData&, const ImageData&);

class HSVFilter;
bool operator==(const HSVFilter&, const HSVFilter&);
bool operator<(const HSVFilter&, const HSVFilter&);

class CalibrationParams;
bool operator==(const CalibrationParams&, const CalibrationParams&);
bool operator<(const CalibrationParams&, const CalibrationParams&);

class ImageProvider;
bool operator==(const ImageProvider&, const ImageProvider&);
bool operator<(const ImageProvider&, const ImageProvider&);

class ImageSelector;
bool operator==(const ImageSelector&, const ImageSelector&);
bool operator<(const ImageSelector&, const ImageSelector&);

class CalibrationProvider;
bool operator==(const CalibrationProvider&, const CalibrationProvider&);
bool operator<(const CalibrationProvider&, const CalibrationProvider&);

}

namespace IceInternal
{

::Ice::Object* upCast(::jderobot::ImageDescription*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::ImageDescription*);

::Ice::Object* upCast(::jderobot::ImageData*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::ImageData*);

::Ice::Object* upCast(::jderobot::HSVFilter*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::HSVFilter*);

::Ice::Object* upCast(::jderobot::CalibrationParams*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::CalibrationParams*);

::Ice::Object* upCast(::jderobot::ImageProvider*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::ImageProvider*);

::Ice::Object* upCast(::jderobot::ImageSelector*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::ImageSelector*);

::Ice::Object* upCast(::jderobot::CalibrationProvider*);
::IceProxy::Ice::Object* upCast(::IceProxy::jderobot::CalibrationProvider*);

}

namespace jderobot
{

typedef ::IceInternal::Handle< ::jderobot::ImageDescription> ImageDescriptionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::ImageDescription> ImageDescriptionPrx;

void __read(::IceInternal::BasicStream*, ImageDescriptionPrx&);
void __patch__ImageDescriptionPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::ImageData> ImageDataPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::ImageData> ImageDataPrx;

void __read(::IceInternal::BasicStream*, ImageDataPrx&);
void __patch__ImageDataPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::HSVFilter> HSVFilterPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::HSVFilter> HSVFilterPrx;

void __read(::IceInternal::BasicStream*, HSVFilterPrx&);
void __patch__HSVFilterPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::CalibrationParams> CalibrationParamsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::CalibrationParams> CalibrationParamsPrx;

void __read(::IceInternal::BasicStream*, CalibrationParamsPrx&);
void __patch__CalibrationParamsPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::ImageProvider> ImageProviderPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::ImageProvider> ImageProviderPrx;

void __read(::IceInternal::BasicStream*, ImageProviderPrx&);
void __patch__ImageProviderPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::ImageSelector> ImageSelectorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::ImageSelector> ImageSelectorPrx;

void __read(::IceInternal::BasicStream*, ImageSelectorPrx&);
void __patch__ImageSelectorPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::jderobot::CalibrationProvider> CalibrationProviderPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::jderobot::CalibrationProvider> CalibrationProviderPrx;

void __read(::IceInternal::BasicStream*, CalibrationProviderPrx&);
void __patch__CalibrationProviderPtr(void*, ::Ice::ObjectPtr&);

}

namespace jderobot
{

enum FeaturesType
{
    Detected,
    Filtered,
    Ball,
    BlueNet,
    YellowNet,
    Field,
    Lines,
    Regions,
    Segments,
    HSV
};

void __write(::IceInternal::BasicStream*, FeaturesType);
void __read(::IceInternal::BasicStream*, FeaturesType&);

enum ObjectsType
{
    BallObj,
    BlueNetObj,
    YellowNetObj,
    FieldObj,
    LinesObj
};

void __write(::IceInternal::BasicStream*, ObjectsType);
void __read(::IceInternal::BasicStream*, ObjectsType&);

enum CameraType
{
    UPPERCAMERA,
    LOWERCAMERA
};

void __write(::IceInternal::BasicStream*, CameraType);
void __read(::IceInternal::BasicStream*, CameraType&);

}

namespace jderobot
{

class ImageDescription : virtual public ::Ice::Object
{
public:

    typedef ImageDescriptionPrx ProxyType;
    typedef ImageDescriptionPtr PointerType;
    
    ImageDescription() {}
    ImageDescription(::Ice::Int, ::Ice::Int, ::Ice::Int, const ::std::string&);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~ImageDescription() {}


public:

    ::Ice::Int width;
    ::Ice::Int height;
    ::Ice::Int size;
    ::std::string format;
};

class ImageData : virtual public ::Ice::Object
{
public:

    typedef ImageDataPrx ProxyType;
    typedef ImageDataPtr PointerType;
    
    ImageData() {}
    ImageData(const ::jderobot::Time&, const ::jderobot::ImageDescriptionPtr&, const ::jderobot::ByteSeq&);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~ImageData() {}


public:

    ::jderobot::Time timeStamp;
    ::jderobot::ImageDescriptionPtr description;
    ::jderobot::ByteSeq pixelData;
};

class HSVFilter : virtual public ::Ice::Object
{
public:

    typedef HSVFilterPrx ProxyType;
    typedef HSVFilterPtr PointerType;
    
    HSVFilter() {}
    HSVFilter(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~HSVFilter() {}


public:

    ::Ice::Float hmin;
    ::Ice::Float hmax;
    ::Ice::Float smin;
    ::Ice::Float smax;
    ::Ice::Float vmin;
    ::Ice::Float vmax;
};

class CalibrationParams : virtual public ::Ice::Object
{
public:

    typedef CalibrationParamsPrx ProxyType;
    typedef CalibrationParamsPtr PointerType;
    
    CalibrationParams() {}
    CalibrationParams(::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float, ::Ice::Float);

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~CalibrationParams() {}


public:

    ::Ice::Float robotx;
    ::Ice::Float roboty;
    ::Ice::Float robott;
    ::Ice::Float u0;
    ::Ice::Float v0;
    ::Ice::Float fdistx;
    ::Ice::Float fdisty;
};

class ImageProvider : virtual public ::Ice::Object
{
public:

    typedef ImageProviderPrx ProxyType;
    typedef ImageProviderPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::ImageDescriptionPtr getImageDescription(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getImageDescription(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::jderobot::ImageDataPtr getImageData(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getImageData(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class ImageSelector : virtual public ::jderobot::ImageProvider
{
public:

    typedef ImageSelectorPrx ProxyType;
    typedef ImageSelectorPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::ImageDataPtr getImageDataWithFeatures(::jderobot::FeaturesType, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getImageDataWithFeatures(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::jderobot::HSVFilterPtr getHSVFilter(::jderobot::CameraType, ::jderobot::ObjectsType, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getHSVFilter(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void setHSVFilter(::jderobot::CameraType, ::jderobot::ObjectsType, const ::jderobot::HSVFilterPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setHSVFilter(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void setCam(::jderobot::CameraType, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setCam(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class CalibrationProvider : virtual public ::Ice::Object
{
public:

    typedef CalibrationProviderPrx ProxyType;
    typedef CalibrationProviderPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::jderobot::ImageDataPtr getCalibrationImg(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getCalibrationImg(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual ::jderobot::CalibrationParamsPtr getCalibrationParams(const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___getCalibrationParams(::IceInternal::Incoming&, const ::Ice::Current&);
#endif // ICEE_PURE_CLIENT

    virtual void setCalibrationParams(const ::jderobot::CalibrationParamsPtr&, const ::Ice::Current& = ::Ice::Current()) = 0;
#ifndef ICEE_PURE_CLIENT
    ::Ice::DispatchStatus ___setCalibrationParams(::IceInternal::Incoming&, const ::Ice::Current&);
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

class ImageDescription : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_secure(bool __secure) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<ImageDescription> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<ImageDescription> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_twoway() const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_oneway() const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_batchOneway() const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_datagram() const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_batchDatagram() const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageDescription> ice_timeout(int __timeout) const
    {
        return dynamic_cast<ImageDescription*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class ImageData : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<ImageData> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_secure(bool __secure) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<ImageData> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<ImageData> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<ImageData> ice_twoway() const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_oneway() const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_batchOneway() const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_datagram() const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_batchDatagram() const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageData> ice_timeout(int __timeout) const
    {
        return dynamic_cast<ImageData*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class HSVFilter : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_secure(bool __secure) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<HSVFilter> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<HSVFilter> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_twoway() const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_oneway() const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_batchOneway() const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_datagram() const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_batchDatagram() const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<HSVFilter> ice_timeout(int __timeout) const
    {
        return dynamic_cast<HSVFilter*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class CalibrationParams : virtual public ::IceProxy::Ice::Object
{
public:
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_secure(bool __secure) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<CalibrationParams> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<CalibrationParams> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_twoway() const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_oneway() const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_batchOneway() const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_datagram() const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_batchDatagram() const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationParams> ice_timeout(int __timeout) const
    {
        return dynamic_cast<CalibrationParams*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class ImageProvider : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::ImageDescriptionPtr getImageDescription()
    {
        return getImageDescription(0);
    }
    ::jderobot::ImageDescriptionPtr getImageDescription(const ::Ice::Context& __ctx)
    {
        return getImageDescription(&__ctx);
    }
    
private:

    ::jderobot::ImageDescriptionPtr getImageDescription(const ::Ice::Context*);
    
public:

    ::jderobot::ImageDataPtr getImageData()
    {
        return getImageData(0);
    }
    ::jderobot::ImageDataPtr getImageData(const ::Ice::Context& __ctx)
    {
        return getImageData(&__ctx);
    }
    
private:

    ::jderobot::ImageDataPtr getImageData(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_secure(bool __secure) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<ImageProvider> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<ImageProvider> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_twoway() const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_oneway() const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_batchOneway() const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_datagram() const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_batchDatagram() const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageProvider> ice_timeout(int __timeout) const
    {
        return dynamic_cast<ImageProvider*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class ImageSelector : virtual public ::IceProxy::jderobot::ImageProvider
{
public:

    ::jderobot::ImageDataPtr getImageDataWithFeatures(::jderobot::FeaturesType type)
    {
        return getImageDataWithFeatures(type, 0);
    }
    ::jderobot::ImageDataPtr getImageDataWithFeatures(::jderobot::FeaturesType type, const ::Ice::Context& __ctx)
    {
        return getImageDataWithFeatures(type, &__ctx);
    }
    
private:

    ::jderobot::ImageDataPtr getImageDataWithFeatures(::jderobot::FeaturesType, const ::Ice::Context*);
    
public:

    ::jderobot::HSVFilterPtr getHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj)
    {
        return getHSVFilter(cam, obj, 0);
    }
    ::jderobot::HSVFilterPtr getHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj, const ::Ice::Context& __ctx)
    {
        return getHSVFilter(cam, obj, &__ctx);
    }
    
private:

    ::jderobot::HSVFilterPtr getHSVFilter(::jderobot::CameraType, ::jderobot::ObjectsType, const ::Ice::Context*);
    
public:

    void setHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj, const ::jderobot::HSVFilterPtr& newFilter)
    {
        setHSVFilter(cam, obj, newFilter, 0);
    }
    void setHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj, const ::jderobot::HSVFilterPtr& newFilter, const ::Ice::Context& __ctx)
    {
        setHSVFilter(cam, obj, newFilter, &__ctx);
    }
    
private:

    void setHSVFilter(::jderobot::CameraType, ::jderobot::ObjectsType, const ::jderobot::HSVFilterPtr&, const ::Ice::Context*);
    
public:

    void setCam(::jderobot::CameraType cam)
    {
        setCam(cam, 0);
    }
    void setCam(::jderobot::CameraType cam, const ::Ice::Context& __ctx)
    {
        setCam(cam, &__ctx);
    }
    
private:

    void setCam(::jderobot::CameraType, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_secure(bool __secure) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<ImageSelector> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<ImageSelector> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_twoway() const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_oneway() const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_batchOneway() const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_datagram() const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_batchDatagram() const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<ImageSelector> ice_timeout(int __timeout) const
    {
        return dynamic_cast<ImageSelector*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class CalibrationProvider : virtual public ::IceProxy::Ice::Object
{
public:

    ::jderobot::ImageDataPtr getCalibrationImg()
    {
        return getCalibrationImg(0);
    }
    ::jderobot::ImageDataPtr getCalibrationImg(const ::Ice::Context& __ctx)
    {
        return getCalibrationImg(&__ctx);
    }
    
private:

    ::jderobot::ImageDataPtr getCalibrationImg(const ::Ice::Context*);
    
public:

    ::jderobot::CalibrationParamsPtr getCalibrationParams()
    {
        return getCalibrationParams(0);
    }
    ::jderobot::CalibrationParamsPtr getCalibrationParams(const ::Ice::Context& __ctx)
    {
        return getCalibrationParams(&__ctx);
    }
    
private:

    ::jderobot::CalibrationParamsPtr getCalibrationParams(const ::Ice::Context*);
    
public:

    void setCalibrationParams(const ::jderobot::CalibrationParamsPtr& newParams)
    {
        setCalibrationParams(newParams, 0);
    }
    void setCalibrationParams(const ::jderobot::CalibrationParamsPtr& newParams, const ::Ice::Context& __ctx)
    {
        setCalibrationParams(newParams, &__ctx);
    }
    
private:

    void setCalibrationParams(const ::jderobot::CalibrationParamsPtr&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_secure(bool __secure) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
#ifdef ICEE_HAS_ROUTER
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
#endif // ICEE_HAS_ROUTER
    
#ifdef ICEE_HAS_LOCATOR
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_adapterId(const std::string& __id) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
#endif // ICEE_HAS_LOCATOR
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_twoway() const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_oneway() const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_batchOneway() const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_datagram() const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_batchDatagram() const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<CalibrationProvider> ice_timeout(int __timeout) const
    {
        return dynamic_cast<CalibrationProvider*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    static const ::std::string& ice_staticId();
    
private:
    
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

#endif
