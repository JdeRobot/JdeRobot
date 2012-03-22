// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `image.ice'

#include <image.h>
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

static const ::std::string __jderobot__ImageConsumer__report_name = "report";

static const ::std::string __jderobot__ImageProvider__getImageDescription_name = "getImageDescription";

static const ::std::string __jderobot__ImageProvider__getImageData_name = "getImageData";

::Ice::Object* IceInternal::upCast(::jderobot::ImageDescription* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageDescription* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageConsumer* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageConsumer* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::ImageProvider* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ImageProvider* p) { return p; }

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
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ImageConsumerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ImageConsumer;
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

IceAsync::jderobot::AMD_ImageProvider_getImageData::AMD_ImageProvider_getImageData(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::jderobot::AMD_ImageProvider_getImageData::ice_response(const ::jderobot::ImageDataPtr& __ret)
{
    if(__validateResponse(true))
    {
        try
        {
            ::IceInternal::BasicStream* __os = this->__os();
            __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
            __os->writePendingObjects();
        }
        catch(const ::Ice::Exception& __ex)
        {
            __exception(__ex);
            return;
        }
        __response(true);
    }
}

void
IceAsync::jderobot::AMD_ImageProvider_getImageData::ice_exception(const ::std::exception& ex)
{
    if(const ::jderobot::DataNotExistException* __ex = dynamic_cast<const ::jderobot::DataNotExistException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else if(const ::jderobot::HardwareFailedException* __ex = dynamic_cast<const ::jderobot::HardwareFailedException*>(&ex))
    {
        if(__validateResponse(false))
        {
            __os()->write(*__ex);
            __response(false);
        }
    }
    else
    {
        if(__validateException(ex))
        {
            __exception(ex);
        }
    }
}

void
IceAsync::jderobot::AMD_ImageProvider_getImageData::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

const ::std::string&
IceProxy::jderobot::ImageDescription::ice_staticId()
{
    return ::jderobot::ImageDescription::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::ImageDescription::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::ImageDescription);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::ImageDescription::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::ImageDescription);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageDescription::__newInstance() const
{
    return new ImageDescription;
}

const ::std::string&
IceProxy::jderobot::ImageData::ice_staticId()
{
    return ::jderobot::ImageData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::ImageData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::ImageData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::ImageData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::ImageData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageData::__newInstance() const
{
    return new ImageData;
}

void
IceProxy::jderobot::ImageConsumer::report(const ::jderobot::ImageDataPtr& obj, const ::Ice::Context* __ctx)
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
            ::IceDelegate::jderobot::ImageConsumer* __del = dynamic_cast< ::IceDelegate::jderobot::ImageConsumer*>(__delBase.get());
            __del->report(obj, __ctx);
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
IceProxy::jderobot::ImageConsumer::ice_staticId()
{
    return ::jderobot::ImageConsumer::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::ImageConsumer::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::ImageConsumer);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::ImageConsumer::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::ImageConsumer);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageConsumer::__newInstance() const
{
    return new ImageConsumer;
}

::jderobot::ImageDescriptionPtr
IceProxy::jderobot::ImageProvider::getImageDescription(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__ImageProvider__getImageDescription_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::ImageProvider* __del = dynamic_cast< ::IceDelegate::jderobot::ImageProvider*>(__delBase.get());
            return __del->getImageDescription(__ctx);
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

::jderobot::ImageDataPtr
IceProxy::jderobot::ImageProvider::getImageData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__ImageProvider__getImageData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::ImageProvider* __del = dynamic_cast< ::IceDelegate::jderobot::ImageProvider*>(__delBase.get());
            return __del->getImageData(__ctx);
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

const ::std::string&
IceProxy::jderobot::ImageProvider::ice_staticId()
{
    return ::jderobot::ImageProvider::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::ImageProvider::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::ImageProvider);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::ImageProvider::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::ImageProvider);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ImageProvider::__newInstance() const
{
    return new ImageProvider;
}

void
IceDelegateM::jderobot::ImageConsumer::report(const ::jderobot::ImageDataPtr& obj, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__ImageConsumer__report_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(obj.get())));
        __os->writePendingObjects();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
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

::jderobot::ImageDescriptionPtr
IceDelegateM::jderobot::ImageProvider::getImageDescription(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__ImageProvider__getImageDescription_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::ImageDescriptionPtr __ret;
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
        __is->read(::jderobot::__patch__ImageDescriptionPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::ImageDataPtr
IceDelegateM::jderobot::ImageProvider::getImageData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__ImageProvider__getImageData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::ImageDataPtr __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
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
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(::jderobot::__patch__ImageDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::jderobot::ImageConsumer::report(const ::jderobot::ImageDataPtr& obj, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::jderobot::ImageDataPtr& obj, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_obj(obj)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::ImageConsumer* servant = dynamic_cast< ::jderobot::ImageConsumer*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->report(_m_obj, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::jderobot::ImageDataPtr& _m_obj;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__ImageConsumer__report_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(obj, __current);
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

::jderobot::ImageDescriptionPtr
IceDelegateD::jderobot::ImageProvider::getImageDescription(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::ImageDescriptionPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::ImageProvider* servant = dynamic_cast< ::jderobot::ImageProvider*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getImageDescription(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::ImageDescriptionPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__ImageProvider__getImageDescription_name, ::Ice::Idempotent, __context);
    ::jderobot::ImageDescriptionPtr __result;
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

::jderobot::ImageDataPtr
IceDelegateD::jderobot::ImageProvider::getImageData(const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
    return ::jderobot::ImageDataPtr(); // to avoid a warning with some compilers;
}

jderobot::ImageDescription::ImageDescription(::Ice::Int __ice_width, ::Ice::Int __ice_height, ::Ice::Int __ice_size, const ::std::string& __ice_format) :
    width(__ice_width),
    height(__ice_height),
    size(__ice_size),
    format(__ice_format)
{
}

::Ice::ObjectPtr
jderobot::ImageDescription::ice_clone() const
{
    ::jderobot::ImageDescriptionPtr __p = new ::jderobot::ImageDescription(*this);
    return __p;
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::ImageDescription::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageDescription was not generated with stream support";
    throw ex;
}

void
jderobot::ImageDescription::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageDescription was not generated with stream support";
    throw ex;
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

jderobot::ImageData::ImageData(const ::jderobot::Time& __ice_timeStamp, const ::jderobot::ImageDescriptionPtr& __ice_description, const ::jderobot::ByteSeq& __ice_pixelData) :
    timeStamp(__ice_timeStamp),
    description(__ice_description),
    pixelData(__ice_pixelData)
{
}

::Ice::ObjectPtr
jderobot::ImageData::ice_clone() const
{
    ::jderobot::ImageDataPtr __p = new ::jderobot::ImageData(*this);
    return __p;
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
jderobot::ImageData::__incRef()
{
    __gcIncRef();
}

void
jderobot::ImageData::__decRef()
{
    __gcDecRef();
}

void
jderobot::ImageData::__addObject(::IceInternal::GCCountMap& _c)
{
    ::IceInternal::GCCountMap::iterator pos = _c.find(this);
    if(pos == _c.end())
    {
        _c[this] = 1;
    }
    else
    {
        ++pos->second;
    }
}

bool
jderobot::ImageData::__usesClasses()
{
    return true;
}

void
jderobot::ImageData::__gcReachable(::IceInternal::GCCountMap& _c) const
{
    if(description)
    {
        ::IceInternal::upCast(description.get())->__addObject(_c);
    }
}

void
jderobot::ImageData::__gcClear()
{
    if(description)
    {
        if(::IceInternal::upCast(description.get())->__usesClasses())
        {
            ::IceInternal::upCast(description.get())->__decRefUnsafe();
            description.__clearHandleUnsafe();
        }
        else
        {
            description = 0;
        }
    }
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
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
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::ImageData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageData was not generated with stream support";
    throw ex;
}

void
jderobot::ImageData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageData was not generated with stream support";
    throw ex;
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

::Ice::ObjectPtr
jderobot::ImageConsumer::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__ImageConsumer_ids[2] =
{
    "::Ice::Object",
    "::jderobot::ImageConsumer"
};

bool
jderobot::ImageConsumer::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ImageConsumer_ids, __jderobot__ImageConsumer_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::ImageConsumer::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ImageConsumer_ids[0], &__jderobot__ImageConsumer_ids[2]);
}

const ::std::string&
jderobot::ImageConsumer::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ImageConsumer_ids[1];
}

const ::std::string&
jderobot::ImageConsumer::ice_staticId()
{
    return __jderobot__ImageConsumer_ids[1];
}

::Ice::DispatchStatus
jderobot::ImageConsumer::___report(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::ImageDataPtr obj;
    __is->read(::jderobot::__patch__ImageDataPtr, &obj);
    __is->readPendingObjects();
    __is->endReadEncaps();
    report(obj, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__ImageConsumer_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "report"
};

::Ice::DispatchStatus
jderobot::ImageConsumer::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__ImageConsumer_all, __jderobot__ImageConsumer_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__ImageConsumer_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___report(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::ImageConsumer::__write(::IceInternal::BasicStream* __os) const
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
jderobot::ImageConsumer::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::ImageConsumer::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageConsumer was not generated with stream support";
    throw ex;
}

void
jderobot::ImageConsumer::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageConsumer was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__ImageConsumerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ImageConsumerPtr* p = static_cast< ::jderobot::ImageConsumerPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ImageConsumerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ImageConsumer::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::ImageConsumer& l, const ::jderobot::ImageConsumer& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ImageConsumer& l, const ::jderobot::ImageConsumer& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::ImageProvider::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
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

::Ice::DispatchStatus
jderobot::ImageProvider::___getImageDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::ImageDescriptionPtr __ret = getImageDescription(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::ImageProvider::___getImageData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::jderobot::AMD_ImageProvider_getImageDataPtr __cb = new IceAsync::jderobot::AMD_ImageProvider_getImageData(__inS);
    try
    {
        getImageData_async(__cb, __current);
    }
    catch(const ::std::exception& __ex)
    {
        __cb->ice_exception(__ex);
    }
    catch(...)
    {
        __cb->ice_exception();
    }
    return ::Ice::DispatchAsync;
}

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
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
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
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::ImageProvider::__write(::IceInternal::BasicStream* __os) const
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
jderobot::ImageProvider::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::ImageProvider::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageProvider was not generated with stream support";
    throw ex;
}

void
jderobot::ImageProvider::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ImageProvider was not generated with stream support";
    throw ex;
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
