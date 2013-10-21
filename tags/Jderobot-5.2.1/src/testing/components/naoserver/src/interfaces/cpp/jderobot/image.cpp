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

#include <image.h>
#include <IceE/LocalException.h>
#include <IceE/ObjectFactory.h>
#include <IceE/BasicStream.h>
#include <IceE/LocalException.h>
#include <IceE/Iterator.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

static const ::std::string __jderobot__ImageProvider__getImageDescription_name = "getImageDescription";

static const ::std::string __jderobot__ImageProvider__getImageData_name = "getImageData";

static const ::std::string __jderobot__ImageSelector__getImageDataWithFeatures_name = "getImageDataWithFeatures";

static const ::std::string __jderobot__ImageSelector__getHSVFilter_name = "getHSVFilter";

static const ::std::string __jderobot__ImageSelector__setHSVFilter_name = "setHSVFilter";

static const ::std::string __jderobot__ImageSelector__setCam_name = "setCam";

static const ::std::string __jderobot__CalibrationProvider__getCalibrationImg_name = "getCalibrationImg";

static const ::std::string __jderobot__CalibrationProvider__getCalibrationParams_name = "getCalibrationParams";

static const ::std::string __jderobot__CalibrationProvider__setCalibrationParams_name = "setCalibrationParams";

::Ice::Object* IceInternal::upCast(::jderobot::ImageDescription* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageDescription* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::HSVFilter* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::HSVFilter* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::CalibrationParams* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::CalibrationParams* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageProvider* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageProvider* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageSelector* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageSelector* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::CalibrationProvider* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::CalibrationProvider* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ImageDescriptionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ImageDescription;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ImageDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ImageData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::HSVFilterPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::HSVFilter;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CalibrationParamsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::CalibrationParams;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ImageProviderPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ImageProvider;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ImageSelectorPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ImageSelector;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CalibrationProviderPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::CalibrationProvider;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::FeaturesType v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 10);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::FeaturesType& v)
{
    ::Ice::Byte val;
    __is->read(val, 10);
    v = static_cast< ::jderobot::FeaturesType>(val);
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::ObjectsType v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 5);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ObjectsType& v)
{
    ::Ice::Byte val;
    __is->read(val, 5);
    v = static_cast< ::jderobot::ObjectsType>(val);
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::CameraType v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CameraType& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::jderobot::CameraType>(val);
}

jderobot::ImageDescription::ImageDescription(::Ice::Int __ice_width, ::Ice::Int __ice_height, ::Ice::Int __ice_size, const ::std::string& __ice_format) :
    width(__ice_width),
    height(__ice_height),
    size(__ice_size),
    format(__ice_format)
{
}

static const ::std::string __jderobot__ImageDescription_ids[2] =
{
    "::Ice::Object",
    "::jderobot::ImageDescription"
};

bool
jderobot::ImageDescription::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ImageDescription_ids, __jderobot__ImageDescription_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::ImageDescription::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ImageDescription_ids[0], &__jderobot__ImageDescription_ids[2]);
}

const ::std::string&
jderobot::ImageDescription::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ImageDescription_ids[1];
}

const ::std::string&
jderobot::ImageDescription::ice_staticId()
{
    return __jderobot__ImageDescription_ids[1];
}

void
jderobot::ImageDescription::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(width);
    __os->write(height);
    __os->write(size);
    __os->write(format);
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::ImageDescription::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(width);
    __is->read(height);
    __is->read(size);
    __is->read(format);
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}

class __F__jderobot__ImageDescription : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::ImageDescription::ice_staticId());
        return new ::jderobot::ImageDescription;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__ImageDescription_Ptr = new __F__jderobot__ImageDescription;

const ::Ice::ObjectFactoryPtr&
jderobot::ImageDescription::ice_factory()
{
    return __F__jderobot__ImageDescription_Ptr;
}

class __F__jderobot__ImageDescription__Init
{
public:

    __F__jderobot__ImageDescription__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::ImageDescription::ice_staticId(), ::jderobot::ImageDescription::ice_factory());
    }

    ~__F__jderobot__ImageDescription__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::ImageDescription::ice_staticId());
    }
};

static __F__jderobot__ImageDescription__Init __F__jderobot__ImageDescription__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__ImageDescription__initializer() {} }
#endif


bool
jderobot::operator==(const ::jderobot::ImageDescription& l, const ::jderobot::ImageDescription& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ImageDescription& l, const ::jderobot::ImageDescription& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__ImageDescriptionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ImageDescriptionPtr* p = static_cast< ::jderobot::ImageDescriptionPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ImageDescriptionPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ImageDescription::ice_staticId(), v->ice_id());
    }
}

jderobot::ImageData::ImageData(const ::jderobot::Time& __ice_timeStamp, const ::jderobot::ImageDescriptionPtr& __ice_description, const ::jderobot::ByteSeq& __ice_pixelData) :
    timeStamp(__ice_timeStamp),
    description(__ice_description),
    pixelData(__ice_pixelData)
{
}

static const ::std::string __jderobot__ImageData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::ImageData"
};

bool
jderobot::ImageData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ImageData_ids, __jderobot__ImageData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::ImageData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ImageData_ids[0], &__jderobot__ImageData_ids[2]);
}

const ::std::string&
jderobot::ImageData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ImageData_ids[1];
}

const ::std::string&
jderobot::ImageData::ice_staticId()
{
    return __jderobot__ImageData_ids[1];
}

void
jderobot::ImageData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    timeStamp.__write(__os);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(description.get())));
    if(pixelData.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&pixelData[0], &pixelData[0] + pixelData.size());
    }
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::ImageData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    timeStamp.__read(__is);
    __is->read(::jderobot::__patch__ImageDescriptionPtr, &description);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___pixelData;
    __is->read(___pixelData);
    ::std::vector< ::Ice::Byte>(___pixelData.first, ___pixelData.second).swap(pixelData);
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}

class __F__jderobot__ImageData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::ImageData::ice_staticId());
        return new ::jderobot::ImageData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__ImageData_Ptr = new __F__jderobot__ImageData;

const ::Ice::ObjectFactoryPtr&
jderobot::ImageData::ice_factory()
{
    return __F__jderobot__ImageData_Ptr;
}

class __F__jderobot__ImageData__Init
{
public:

    __F__jderobot__ImageData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::ImageData::ice_staticId(), ::jderobot::ImageData::ice_factory());
    }

    ~__F__jderobot__ImageData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::ImageData::ice_staticId());
    }
};

static __F__jderobot__ImageData__Init __F__jderobot__ImageData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__ImageData__initializer() {} }
#endif


bool
jderobot::operator==(const ::jderobot::ImageData& l, const ::jderobot::ImageData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ImageData& l, const ::jderobot::ImageData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__ImageDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ImageDataPtr* p = static_cast< ::jderobot::ImageDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ImageDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ImageData::ice_staticId(), v->ice_id());
    }
}

jderobot::HSVFilter::HSVFilter(::Ice::Float __ice_hmin, ::Ice::Float __ice_hmax, ::Ice::Float __ice_smin, ::Ice::Float __ice_smax, ::Ice::Float __ice_vmin, ::Ice::Float __ice_vmax) :
    hmin(__ice_hmin),
    hmax(__ice_hmax),
    smin(__ice_smin),
    smax(__ice_smax),
    vmin(__ice_vmin),
    vmax(__ice_vmax)
{
}

static const ::std::string __jderobot__HSVFilter_ids[2] =
{
    "::Ice::Object",
    "::jderobot::HSVFilter"
};

bool
jderobot::HSVFilter::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__HSVFilter_ids, __jderobot__HSVFilter_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::HSVFilter::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__HSVFilter_ids[0], &__jderobot__HSVFilter_ids[2]);
}

const ::std::string&
jderobot::HSVFilter::ice_id(const ::Ice::Current&) const
{
    return __jderobot__HSVFilter_ids[1];
}

const ::std::string&
jderobot::HSVFilter::ice_staticId()
{
    return __jderobot__HSVFilter_ids[1];
}

void
jderobot::HSVFilter::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(hmin);
    __os->write(hmax);
    __os->write(smin);
    __os->write(smax);
    __os->write(vmin);
    __os->write(vmax);
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::HSVFilter::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(hmin);
    __is->read(hmax);
    __is->read(smin);
    __is->read(smax);
    __is->read(vmin);
    __is->read(vmax);
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}

class __F__jderobot__HSVFilter : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::HSVFilter::ice_staticId());
        return new ::jderobot::HSVFilter;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__HSVFilter_Ptr = new __F__jderobot__HSVFilter;

const ::Ice::ObjectFactoryPtr&
jderobot::HSVFilter::ice_factory()
{
    return __F__jderobot__HSVFilter_Ptr;
}

class __F__jderobot__HSVFilter__Init
{
public:

    __F__jderobot__HSVFilter__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::HSVFilter::ice_staticId(), ::jderobot::HSVFilter::ice_factory());
    }

    ~__F__jderobot__HSVFilter__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::HSVFilter::ice_staticId());
    }
};

static __F__jderobot__HSVFilter__Init __F__jderobot__HSVFilter__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__HSVFilter__initializer() {} }
#endif


bool
jderobot::operator==(const ::jderobot::HSVFilter& l, const ::jderobot::HSVFilter& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::HSVFilter& l, const ::jderobot::HSVFilter& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__HSVFilterPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::HSVFilterPtr* p = static_cast< ::jderobot::HSVFilterPtr*>(__addr);
    assert(p);
    *p = ::jderobot::HSVFilterPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::HSVFilter::ice_staticId(), v->ice_id());
    }
}

jderobot::CalibrationParams::CalibrationParams(::Ice::Float __ice_robotx, ::Ice::Float __ice_roboty, ::Ice::Float __ice_robott, ::Ice::Float __ice_u0, ::Ice::Float __ice_v0, ::Ice::Float __ice_fdistx, ::Ice::Float __ice_fdisty) :
    robotx(__ice_robotx),
    roboty(__ice_roboty),
    robott(__ice_robott),
    u0(__ice_u0),
    v0(__ice_v0),
    fdistx(__ice_fdistx),
    fdisty(__ice_fdisty)
{
}

static const ::std::string __jderobot__CalibrationParams_ids[2] =
{
    "::Ice::Object",
    "::jderobot::CalibrationParams"
};

bool
jderobot::CalibrationParams::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__CalibrationParams_ids, __jderobot__CalibrationParams_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::CalibrationParams::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__CalibrationParams_ids[0], &__jderobot__CalibrationParams_ids[2]);
}

const ::std::string&
jderobot::CalibrationParams::ice_id(const ::Ice::Current&) const
{
    return __jderobot__CalibrationParams_ids[1];
}

const ::std::string&
jderobot::CalibrationParams::ice_staticId()
{
    return __jderobot__CalibrationParams_ids[1];
}

void
jderobot::CalibrationParams::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(robotx);
    __os->write(roboty);
    __os->write(robott);
    __os->write(u0);
    __os->write(v0);
    __os->write(fdistx);
    __os->write(fdisty);
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::CalibrationParams::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(robotx);
    __is->read(roboty);
    __is->read(robott);
    __is->read(u0);
    __is->read(v0);
    __is->read(fdistx);
    __is->read(fdisty);
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}

class __F__jderobot__CalibrationParams : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::CalibrationParams::ice_staticId());
        return new ::jderobot::CalibrationParams;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__CalibrationParams_Ptr = new __F__jderobot__CalibrationParams;

const ::Ice::ObjectFactoryPtr&
jderobot::CalibrationParams::ice_factory()
{
    return __F__jderobot__CalibrationParams_Ptr;
}

class __F__jderobot__CalibrationParams__Init
{
public:

    __F__jderobot__CalibrationParams__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::CalibrationParams::ice_staticId(), ::jderobot::CalibrationParams::ice_factory());
    }

    ~__F__jderobot__CalibrationParams__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::CalibrationParams::ice_staticId());
    }
};

static __F__jderobot__CalibrationParams__Init __F__jderobot__CalibrationParams__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__CalibrationParams__initializer() {} }
#endif


bool
jderobot::operator==(const ::jderobot::CalibrationParams& l, const ::jderobot::CalibrationParams& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::CalibrationParams& l, const ::jderobot::CalibrationParams& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__CalibrationParamsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::CalibrationParamsPtr* p = static_cast< ::jderobot::CalibrationParamsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::CalibrationParamsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::CalibrationParams::ice_staticId(), v->ice_id());
    }
}

static const ::std::string __jderobot__ImageProvider_ids[2] =
{
    "::Ice::Object",
    "::jderobot::ImageProvider"
};

bool
jderobot::ImageProvider::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ImageProvider_ids, __jderobot__ImageProvider_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::ImageProvider::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ImageProvider_ids[0], &__jderobot__ImageProvider_ids[2]);
}

const ::std::string&
jderobot::ImageProvider::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ImageProvider_ids[1];
}

const ::std::string&
jderobot::ImageProvider::ice_staticId()
{
    return __jderobot__ImageProvider_ids[1];
}

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageProvider::___getImageDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::ImageDescriptionPtr __ret = getImageDescription(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageProvider::___getImageData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::jderobot::ImageDataPtr __ret = getImageData(__current);
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
        __os->writePendingObjects();
    }
    catch(const ::jderobot::DataNotExistException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::jderobot::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
static ::std::string __jderobot__ImageProvider_all[] =
{
    "getImageData",
    "getImageDescription",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::ImageProvider::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__ImageProvider_all, __jderobot__ImageProvider_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__ImageProvider_all)
    {
        case 0:
        {
            return ___getImageData(in, current);
        }
        case 1:
        {
            return ___getImageDescription(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::ImageProvider::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::ImageProvider::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}


bool
jderobot::operator==(const ::jderobot::ImageProvider& l, const ::jderobot::ImageProvider& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ImageProvider& l, const ::jderobot::ImageProvider& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__ImageProviderPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ImageProviderPtr* p = static_cast< ::jderobot::ImageProviderPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ImageProviderPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ImageProvider::ice_staticId(), v->ice_id());
    }
}

static const ::std::string __jderobot__ImageSelector_ids[3] =
{
    "::Ice::Object",
    "::jderobot::ImageProvider",
    "::jderobot::ImageSelector"
};

bool
jderobot::ImageSelector::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ImageSelector_ids, __jderobot__ImageSelector_ids + 3, _s);
}

::std::vector< ::std::string>
jderobot::ImageSelector::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ImageSelector_ids[0], &__jderobot__ImageSelector_ids[3]);
}

const ::std::string&
jderobot::ImageSelector::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ImageSelector_ids[2];
}

const ::std::string&
jderobot::ImageSelector::ice_staticId()
{
    return __jderobot__ImageSelector_ids[2];
}

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageSelector::___getImageDataWithFeatures(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::FeaturesType type;
    ::jderobot::__read(__is, type);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::jderobot::ImageDataPtr __ret = getImageDataWithFeatures(type, __current);
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
        __os->writePendingObjects();
    }
    catch(const ::jderobot::DataNotExistException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::jderobot::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageSelector::___getHSVFilter(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::CameraType cam;
    ::jderobot::ObjectsType obj;
    ::jderobot::__read(__is, cam);
    ::jderobot::__read(__is, obj);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::HSVFilterPtr __ret = getHSVFilter(cam, obj, __current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageSelector::___setHSVFilter(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::CameraType cam;
    ::jderobot::ObjectsType obj;
    ::jderobot::HSVFilterPtr newFilter;
    ::jderobot::__read(__is, cam);
    ::jderobot::__read(__is, obj);
    __is->read(::jderobot::__patch__HSVFilterPtr, &newFilter);
    __is->readPendingObjects();
    setHSVFilter(cam, obj, newFilter, __current);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::ImageSelector::___setCam(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::CameraType cam;
    ::jderobot::__read(__is, cam);
    setCam(cam, __current);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
static ::std::string __jderobot__ImageSelector_all[] =
{
    "getHSVFilter",
    "getImageData",
    "getImageDataWithFeatures",
    "getImageDescription",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setCam",
    "setHSVFilter"
};

::Ice::DispatchStatus
jderobot::ImageSelector::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__ImageSelector_all, __jderobot__ImageSelector_all + 10, current.operation);
    if(r.first == r.second)
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__ImageSelector_all)
    {
        case 0:
        {
            return ___getHSVFilter(in, current);
        }
        case 1:
        {
            return ___getImageData(in, current);
        }
        case 2:
        {
            return ___getImageDataWithFeatures(in, current);
        }
        case 3:
        {
            return ___getImageDescription(in, current);
        }
        case 4:
        {
            return ___ice_id(in, current);
        }
        case 5:
        {
            return ___ice_ids(in, current);
        }
        case 6:
        {
            return ___ice_isA(in, current);
        }
        case 7:
        {
            return ___ice_ping(in, current);
        }
        case 8:
        {
            return ___setCam(in, current);
        }
        case 9:
        {
            return ___setHSVFilter(in, current);
        }
    }

    assert(false);
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::ImageSelector::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::ImageSelector::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}


bool
jderobot::operator==(const ::jderobot::ImageSelector& l, const ::jderobot::ImageSelector& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ImageSelector& l, const ::jderobot::ImageSelector& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__ImageSelectorPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ImageSelectorPtr* p = static_cast< ::jderobot::ImageSelectorPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ImageSelectorPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ImageSelector::ice_staticId(), v->ice_id());
    }
}

static const ::std::string __jderobot__CalibrationProvider_ids[2] =
{
    "::Ice::Object",
    "::jderobot::CalibrationProvider"
};

bool
jderobot::CalibrationProvider::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__CalibrationProvider_ids, __jderobot__CalibrationProvider_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::CalibrationProvider::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__CalibrationProvider_ids[0], &__jderobot__CalibrationProvider_ids[2]);
}

const ::std::string&
jderobot::CalibrationProvider::ice_id(const ::Ice::Current&) const
{
    return __jderobot__CalibrationProvider_ids[1];
}

const ::std::string&
jderobot::CalibrationProvider::ice_staticId()
{
    return __jderobot__CalibrationProvider_ids[1];
}

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::CalibrationProvider::___getCalibrationImg(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::jderobot::ImageDataPtr __ret = getCalibrationImg(__current);
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
        __os->writePendingObjects();
    }
    catch(const ::jderobot::DataNotExistException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::jderobot::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::CalibrationProvider::___getCalibrationParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::CalibrationParamsPtr __ret = getCalibrationParams(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::CalibrationProvider::___setCalibrationParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::CalibrationParamsPtr newParams;
    __is->read(::jderobot::__patch__CalibrationParamsPtr, &newParams);
    __is->readPendingObjects();
    setCalibrationParams(newParams, __current);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
static ::std::string __jderobot__CalibrationProvider_all[] =
{
    "getCalibrationImg",
    "getCalibrationParams",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setCalibrationParams"
};

::Ice::DispatchStatus
jderobot::CalibrationProvider::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__CalibrationProvider_all, __jderobot__CalibrationProvider_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__CalibrationProvider_all)
    {
        case 0:
        {
            return ___getCalibrationImg(in, current);
        }
        case 1:
        {
            return ___getCalibrationParams(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
        case 6:
        {
            return ___setCalibrationParams(in, current);
        }
    }

    assert(false);
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::CalibrationProvider::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::CalibrationProvider::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}


bool
jderobot::operator==(const ::jderobot::CalibrationProvider& l, const ::jderobot::CalibrationProvider& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::CalibrationProvider& l, const ::jderobot::CalibrationProvider& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__CalibrationProviderPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::CalibrationProviderPtr* p = static_cast< ::jderobot::CalibrationProviderPtr*>(__addr);
    assert(p);
    *p = ::jderobot::CalibrationProviderPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::CalibrationProvider::ice_staticId(), v->ice_id());
    }
}

const ::std::string&
IceProxy::jderobot::ImageDescription::ice_staticId()
{
    return __jderobot__ImageDescription_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageDescription::__newInstance() const
{
    return new ImageDescription;
}

const ::std::string&
IceProxy::jderobot::ImageData::ice_staticId()
{
    return __jderobot__ImageData_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageData::__newInstance() const
{
    return new ImageData;
}

const ::std::string&
IceProxy::jderobot::HSVFilter::ice_staticId()
{
    return __jderobot__HSVFilter_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::HSVFilter::__newInstance() const
{
    return new HSVFilter;
}

const ::std::string&
IceProxy::jderobot::CalibrationParams::ice_staticId()
{
    return __jderobot__CalibrationParams_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::CalibrationParams::__newInstance() const
{
    return new CalibrationParams;
}

::jderobot::ImageDescriptionPtr
IceProxy::jderobot::ImageProvider::getImageDescription(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__ImageProvider__getImageDescription_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageProvider__getImageDescription_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::ImageDescriptionPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__ImageDescriptionPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::jderobot::ImageDataPtr
IceProxy::jderobot::ImageProvider::getImageData(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__ImageProvider__getImageData_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageProvider__getImageData_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    try
                    {
                        __outS.is()->throwException();
                    }
                    catch(const ::jderobot::DataNotExistException&)
                    {
                        throw;
                    }
                    catch(const ::jderobot::HardwareFailedException&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::jderobot::ImageDataPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__ImageDataPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

const ::std::string&
IceProxy::jderobot::ImageProvider::ice_staticId()
{
    return __jderobot__ImageProvider_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageProvider::__newInstance() const
{
    return new ImageProvider;
}

::jderobot::ImageDataPtr
IceProxy::jderobot::ImageSelector::getImageDataWithFeatures(::jderobot::FeaturesType type, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__ImageSelector__getImageDataWithFeatures_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageSelector__getImageDataWithFeatures_name, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                ::jderobot::__write(__os, type);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    try
                    {
                        __outS.is()->throwException();
                    }
                    catch(const ::jderobot::DataNotExistException&)
                    {
                        throw;
                    }
                    catch(const ::jderobot::HardwareFailedException&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::jderobot::ImageDataPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__ImageDataPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::jderobot::HSVFilterPtr
IceProxy::jderobot::ImageSelector::getHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__ImageSelector__getHSVFilter_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageSelector__getHSVFilter_name, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                ::jderobot::__write(__os, cam);
                ::jderobot::__write(__os, obj);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::HSVFilterPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__HSVFilterPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::jderobot::ImageSelector::setHSVFilter(::jderobot::CameraType cam, ::jderobot::ObjectsType obj, const ::jderobot::HSVFilterPtr& newFilter, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageSelector__setHSVFilter_name, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                ::jderobot::__write(__os, cam);
                ::jderobot::__write(__os, obj);
                __os->write(::Ice::ObjectPtr(::IceInternal::upCast(newFilter.get())));
                __os->writePendingObjects();
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::jderobot::ImageSelector::setCam(::jderobot::CameraType cam, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__ImageSelector__setCam_name, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                ::jderobot::__write(__os, cam);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

const ::std::string&
IceProxy::jderobot::ImageSelector::ice_staticId()
{
    return __jderobot__ImageSelector_ids[2];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageSelector::__newInstance() const
{
    return new ImageSelector;
}

::jderobot::ImageDataPtr
IceProxy::jderobot::CalibrationProvider::getCalibrationImg(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__CalibrationProvider__getCalibrationImg_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__CalibrationProvider__getCalibrationImg_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    try
                    {
                        __outS.is()->throwException();
                    }
                    catch(const ::jderobot::DataNotExistException&)
                    {
                        throw;
                    }
                    catch(const ::jderobot::HardwareFailedException&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::jderobot::ImageDataPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__ImageDataPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::jderobot::CalibrationParamsPtr
IceProxy::jderobot::CalibrationProvider::getCalibrationParams(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__CalibrationProvider__getCalibrationParams_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__CalibrationProvider__getCalibrationParams_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::CalibrationParamsPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__CalibrationParamsPtr, &__ret);
                __is->readPendingObjects();
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::jderobot::CalibrationProvider::setCalibrationParams(const ::jderobot::CalibrationParamsPtr& newParams, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__CalibrationProvider__setCalibrationParams_name, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                __os->write(::Ice::ObjectPtr(::IceInternal::upCast(newParams.get())));
                __os->writePendingObjects();
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

const ::std::string&
IceProxy::jderobot::CalibrationProvider::ice_staticId()
{
    return __jderobot__CalibrationProvider_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::CalibrationProvider::__newInstance() const
{
    return new CalibrationProvider;
}
