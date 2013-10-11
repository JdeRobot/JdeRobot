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

#include <camera.h>
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

static const ::std::string __jderobot__Camera__getCameraDescription_name = "getCameraDescription";

static const ::std::string __jderobot__Camera__setCameraDescription_name = "setCameraDescription";

static const ::std::string __jderobot__Camera__startCameraStreaming_name = "startCameraStreaming";

static const ::std::string __jderobot__Camera__stopCameraStreaming_name = "stopCameraStreaming";

::Ice::Object* IceInternal::upCast(::jderobot::CameraDescription* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::CameraDescription* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Camera* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Camera* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CameraDescriptionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::CameraDescription;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CameraPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Camera;
        v->__copyFrom(proxy);
    }
}

jderobot::CameraDescription::CameraDescription(const ::std::string& __ice_name, const ::std::string& __ice_shortDescription, const ::std::string& __ice_streamingUri, ::Ice::Float __ice_fdistx, ::Ice::Float __ice_fdisty, ::Ice::Float __ice_u0, ::Ice::Float __ice_v0, ::Ice::Float __ice_skew, ::Ice::Float __ice_posx, ::Ice::Float __ice_posy, ::Ice::Float __ice_posz, ::Ice::Float __ice_foax, ::Ice::Float __ice_foay, ::Ice::Float __ice_foaz, ::Ice::Float __ice_roll) :
    name(__ice_name),
    shortDescription(__ice_shortDescription),
    streamingUri(__ice_streamingUri),
    fdistx(__ice_fdistx),
    fdisty(__ice_fdisty),
    u0(__ice_u0),
    v0(__ice_v0),
    skew(__ice_skew),
    posx(__ice_posx),
    posy(__ice_posy),
    posz(__ice_posz),
    foax(__ice_foax),
    foay(__ice_foay),
    foaz(__ice_foaz),
    roll(__ice_roll)
{
}

static const ::std::string __jderobot__CameraDescription_ids[2] =
{
    "::Ice::Object",
    "::jderobot::CameraDescription"
};

bool
jderobot::CameraDescription::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__CameraDescription_ids, __jderobot__CameraDescription_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::CameraDescription::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__CameraDescription_ids[0], &__jderobot__CameraDescription_ids[2]);
}

const ::std::string&
jderobot::CameraDescription::ice_id(const ::Ice::Current&) const
{
    return __jderobot__CameraDescription_ids[1];
}

const ::std::string&
jderobot::CameraDescription::ice_staticId()
{
    return __jderobot__CameraDescription_ids[1];
}

void
jderobot::CameraDescription::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(name);
    __os->write(shortDescription);
    __os->write(streamingUri);
    __os->write(fdistx);
    __os->write(fdisty);
    __os->write(u0);
    __os->write(v0);
    __os->write(skew);
    __os->write(posx);
    __os->write(posy);
    __os->write(posz);
    __os->write(foax);
    __os->write(foay);
    __os->write(foaz);
    __os->write(roll);
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::CameraDescription::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(name);
    __is->read(shortDescription);
    __is->read(streamingUri);
    __is->read(fdistx);
    __is->read(fdisty);
    __is->read(u0);
    __is->read(v0);
    __is->read(skew);
    __is->read(posx);
    __is->read(posy);
    __is->read(posz);
    __is->read(foax);
    __is->read(foay);
    __is->read(foaz);
    __is->read(roll);
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}

class __F__jderobot__CameraDescription : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::CameraDescription::ice_staticId());
        return new ::jderobot::CameraDescription;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__CameraDescription_Ptr = new __F__jderobot__CameraDescription;

const ::Ice::ObjectFactoryPtr&
jderobot::CameraDescription::ice_factory()
{
    return __F__jderobot__CameraDescription_Ptr;
}

class __F__jderobot__CameraDescription__Init
{
public:

    __F__jderobot__CameraDescription__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::CameraDescription::ice_staticId(), ::jderobot::CameraDescription::ice_factory());
    }

    ~__F__jderobot__CameraDescription__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::CameraDescription::ice_staticId());
    }
};

static __F__jderobot__CameraDescription__Init __F__jderobot__CameraDescription__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__CameraDescription__initializer() {} }
#endif


bool
jderobot::operator==(const ::jderobot::CameraDescription& l, const ::jderobot::CameraDescription& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::CameraDescription& l, const ::jderobot::CameraDescription& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__CameraDescriptionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::CameraDescriptionPtr* p = static_cast< ::jderobot::CameraDescriptionPtr*>(__addr);
    assert(p);
    *p = ::jderobot::CameraDescriptionPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::CameraDescription::ice_staticId(), v->ice_id());
    }
}

static const ::std::string __jderobot__Camera_ids[3] =
{
    "::Ice::Object",
    "::jderobot::Camera",
    "::jderobot::ImageProvider"
};

bool
jderobot::Camera::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Camera_ids, __jderobot__Camera_ids + 3, _s);
}

::std::vector< ::std::string>
jderobot::Camera::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Camera_ids[0], &__jderobot__Camera_ids[3]);
}

const ::std::string&
jderobot::Camera::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Camera_ids[1];
}

const ::std::string&
jderobot::Camera::ice_staticId()
{
    return __jderobot__Camera_ids[1];
}

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Camera::___getCameraDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::CameraDescriptionPtr __ret = getCameraDescription(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Camera::___setCameraDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::CameraDescriptionPtr description;
    __is->read(::jderobot::__patch__CameraDescriptionPtr, &description);
    __is->readPendingObjects();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setCameraDescription(description, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Camera::___startCameraStreaming(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = startCameraStreaming(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Camera::___stopCameraStreaming(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    stopCameraStreaming(__current);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
static ::std::string __jderobot__Camera_all[] =
{
    "getCameraDescription",
    "getImageData",
    "getImageDescription",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setCameraDescription",
    "startCameraStreaming",
    "stopCameraStreaming"
};

::Ice::DispatchStatus
jderobot::Camera::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Camera_all, __jderobot__Camera_all + 10, current.operation);
    if(r.first == r.second)
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Camera_all)
    {
        case 0:
        {
            return ___getCameraDescription(in, current);
        }
        case 1:
        {
            return ___getImageData(in, current);
        }
        case 2:
        {
            return ___getImageDescription(in, current);
        }
        case 3:
        {
            return ___ice_id(in, current);
        }
        case 4:
        {
            return ___ice_ids(in, current);
        }
        case 5:
        {
            return ___ice_isA(in, current);
        }
        case 6:
        {
            return ___ice_ping(in, current);
        }
        case 7:
        {
            return ___setCameraDescription(in, current);
        }
        case 8:
        {
            return ___startCameraStreaming(in, current);
        }
        case 9:
        {
            return ___stopCameraStreaming(in, current);
        }
    }

    assert(false);
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::Camera::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::Camera::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::operator==(const ::jderobot::Camera& l, const ::jderobot::Camera& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Camera& l, const ::jderobot::Camera& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__CameraPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::CameraPtr* p = static_cast< ::jderobot::CameraPtr*>(__addr);
    assert(p);
    *p = ::jderobot::CameraPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Camera::ice_staticId(), v->ice_id());
    }
}

const ::std::string&
IceProxy::jderobot::CameraDescription::ice_staticId()
{
    return __jderobot__CameraDescription_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::CameraDescription::__newInstance() const
{
    return new CameraDescription;
}

::jderobot::CameraDescriptionPtr
IceProxy::jderobot::Camera::getCameraDescription(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__Camera__getCameraDescription_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Camera__getCameraDescription_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::CameraDescriptionPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__CameraDescriptionPtr, &__ret);
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

::Ice::Int
IceProxy::jderobot::Camera::setCameraDescription(const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__Camera__setCameraDescription_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Camera__setCameraDescription_name, ::Ice::Normal, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                __os->write(::Ice::ObjectPtr(::IceInternal::upCast(description.get())));
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
                ::Ice::Int __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(__ret);
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
            __handleExceptionWrapper(__handler, __ex);
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

::std::string
IceProxy::jderobot::Camera::startCameraStreaming(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__Camera__startCameraStreaming_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Camera__startCameraStreaming_name, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::std::string __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(__ret);
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
            __handleExceptionWrapper(__handler, __ex);
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
IceProxy::jderobot::Camera::stopCameraStreaming(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Camera__stopCameraStreaming_name, ::Ice::Normal, __ctx);
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
            __handleExceptionWrapper(__handler, __ex);
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
IceProxy::jderobot::Camera::ice_staticId()
{
    return __jderobot__Camera_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Camera::__newInstance() const
{
    return new Camera;
}
