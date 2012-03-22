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

#include <camera.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

const ::std::string&
IceProxy::jderobot::CameraDescription::ice_staticId()
{
    return ::jderobot::CameraDescription::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::CameraDescription::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::CameraDescription);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::CameraDescription::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::CameraDescription);
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
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__jderobot__Camera__getCameraDescription_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Camera* __del = dynamic_cast< ::IceDelegate::jderobot::Camera*>(__delBase.get());
            return __del->getCameraDescription(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Int
IceProxy::jderobot::Camera::setCameraDescription(const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__jderobot__Camera__setCameraDescription_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Camera* __del = dynamic_cast< ::IceDelegate::jderobot::Camera*>(__delBase.get());
            return __del->setCameraDescription(description, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::jderobot::Camera::startCameraStreaming(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__jderobot__Camera__startCameraStreaming_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Camera* __del = dynamic_cast< ::IceDelegate::jderobot::Camera*>(__delBase.get());
            return __del->startCameraStreaming(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::jderobot::Camera::stopCameraStreaming(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Camera* __del = dynamic_cast< ::IceDelegate::jderobot::Camera*>(__delBase.get());
            __del->stopCameraStreaming(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::jderobot::Camera::ice_staticId()
{
    return ::jderobot::Camera::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Camera::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Camera);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Camera::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Camera);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Camera::__newInstance() const
{
    return new Camera;
}

::jderobot::CameraDescriptionPtr
IceDelegateM::jderobot::Camera::getCameraDescription(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Camera__getCameraDescription_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::CameraDescriptionPtr __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(::jderobot::__patch__CameraDescriptionPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::jderobot::Camera::setCameraDescription(const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Camera__setCameraDescription_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(description.get())));
        __os->writePendingObjects();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::jderobot::Camera::startCameraStreaming(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Camera__startCameraStreaming_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::std::string __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::jderobot::Camera::stopCameraStreaming(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Camera__stopCameraStreaming_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::jderobot::CameraDescriptionPtr
IceDelegateD::jderobot::Camera::getCameraDescription(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::CameraDescriptionPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Camera* servant = dynamic_cast< ::jderobot::Camera*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getCameraDescription(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::CameraDescriptionPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Camera__getCameraDescription_name, ::Ice::Idempotent, __context);
    ::jderobot::CameraDescriptionPtr __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Int
IceDelegateD::jderobot::Camera::setCameraDescription(const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::jderobot::CameraDescriptionPtr& description, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_description(description)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Camera* servant = dynamic_cast< ::jderobot::Camera*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->setCameraDescription(_m_description, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::jderobot::CameraDescriptionPtr& _m_description;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Camera__setCameraDescription_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, description, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::jderobot::Camera::startCameraStreaming(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Camera* servant = dynamic_cast< ::jderobot::Camera*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->startCameraStreaming(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Camera__startCameraStreaming_name, ::Ice::Normal, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::jderobot::Camera::stopCameraStreaming(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Camera* servant = dynamic_cast< ::jderobot::Camera*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->stopCameraStreaming(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Camera__stopCameraStreaming_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
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

::Ice::ObjectPtr
jderobot::CameraDescription::ice_clone() const
{
    ::jderobot::CameraDescriptionPtr __p = new ::jderobot::CameraDescription(*this);
    return __p;
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::CameraDescription::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::CameraDescription was not generated with stream support";
    throw ex;
}

void
jderobot::CameraDescription::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::CameraDescription was not generated with stream support";
    throw ex;
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

::Ice::ObjectPtr
jderobot::Camera::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
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

::Ice::DispatchStatus
jderobot::Camera::___getCameraDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::CameraDescriptionPtr __ret = getCameraDescription(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::Camera::___setCameraDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::CameraDescriptionPtr description;
    __is->read(::jderobot::__patch__CameraDescriptionPtr, &description);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setCameraDescription(description, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::Camera::___startCameraStreaming(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = startCameraStreaming(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::Camera::___stopCameraStreaming(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    stopCameraStreaming(__current);
    return ::Ice::DispatchOK;
}

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
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
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
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::Camera::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::Camera::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Camera was not generated with stream support";
    throw ex;
}

void
jderobot::Camera::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Camera was not generated with stream support";
    throw ex;
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
