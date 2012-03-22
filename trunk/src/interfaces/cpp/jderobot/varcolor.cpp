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

#include <varcolor.h>
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

static const ::std::string __jderobot__VarColor__getDescription_name = "getDescription";

static const ::std::string __jderobot__VarColor__getData_name = "getData";

::Ice::Object* IceInternal::upCast(::jderobot::VarColor* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::VarColor* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::VarColorPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::VarColor;
        v->__copyFrom(proxy);
    }
}

IceAsync::jderobot::AMD_VarColor_getData::AMD_VarColor_getData(::IceInternal::Incoming& in) :
    ::IceInternal::IncomingAsync(in)
{
}

void
IceAsync::jderobot::AMD_VarColor_getData::ice_response(const ::jderobot::ImageDataPtr& __ret)
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
IceAsync::jderobot::AMD_VarColor_getData::ice_exception(const ::std::exception& ex)
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
IceAsync::jderobot::AMD_VarColor_getData::ice_exception()
{
    if(__validateException())
    {
        __exception();
    }
}

::jderobot::ImageDescriptionPtr
IceProxy::jderobot::VarColor::getDescription(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__VarColor__getDescription_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::VarColor* __del = dynamic_cast< ::IceDelegate::jderobot::VarColor*>(__delBase.get());
            return __del->getDescription(__ctx);
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
IceProxy::jderobot::VarColor::getData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__VarColor__getData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::VarColor* __del = dynamic_cast< ::IceDelegate::jderobot::VarColor*>(__delBase.get());
            return __del->getData(__ctx);
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
IceProxy::jderobot::VarColor::ice_staticId()
{
    return ::jderobot::VarColor::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::VarColor::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::VarColor);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::VarColor::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::VarColor);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::VarColor::__newInstance() const
{
    return new VarColor;
}

::jderobot::ImageDescriptionPtr
IceDelegateM::jderobot::VarColor::getDescription(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__VarColor__getDescription_name, ::Ice::Idempotent, __context);
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
IceDelegateM::jderobot::VarColor::getData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__VarColor__getData_name, ::Ice::Idempotent, __context);
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

::jderobot::ImageDescriptionPtr
IceDelegateD::jderobot::VarColor::getDescription(const ::Ice::Context* __context)
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
            ::jderobot::VarColor* servant = dynamic_cast< ::jderobot::VarColor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getDescription(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::ImageDescriptionPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__VarColor__getDescription_name, ::Ice::Idempotent, __context);
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
IceDelegateD::jderobot::VarColor::getData(const ::Ice::Context*)
{
    throw ::Ice::CollocationOptimizationException(__FILE__, __LINE__);
    return ::jderobot::ImageDataPtr(); // to avoid a warning with some compilers;
}

::Ice::ObjectPtr
jderobot::VarColor::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__VarColor_ids[2] =
{
    "::Ice::Object",
    "::jderobot::VarColor"
};

bool
jderobot::VarColor::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__VarColor_ids, __jderobot__VarColor_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::VarColor::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__VarColor_ids[0], &__jderobot__VarColor_ids[2]);
}

const ::std::string&
jderobot::VarColor::ice_id(const ::Ice::Current&) const
{
    return __jderobot__VarColor_ids[1];
}

const ::std::string&
jderobot::VarColor::ice_staticId()
{
    return __jderobot__VarColor_ids[1];
}

::Ice::DispatchStatus
jderobot::VarColor::___getDescription(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::ImageDescriptionPtr __ret = getDescription(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::VarColor::___getData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::jderobot::AMD_VarColor_getDataPtr __cb = new IceAsync::jderobot::AMD_VarColor_getData(__inS);
    try
    {
        getData_async(__cb, __current);
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

static ::std::string __jderobot__VarColor_all[] =
{
    "getData",
    "getDescription",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::VarColor::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__VarColor_all, __jderobot__VarColor_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__VarColor_all)
    {
        case 0:
        {
            return ___getData(in, current);
        }
        case 1:
        {
            return ___getDescription(in, current);
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
jderobot::VarColor::__write(::IceInternal::BasicStream* __os) const
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
jderobot::VarColor::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::VarColor::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::VarColor was not generated with stream support";
    throw ex;
}

void
jderobot::VarColor::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::VarColor was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__VarColorPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::VarColorPtr* p = static_cast< ::jderobot::VarColorPtr*>(__addr);
    assert(p);
    *p = ::jderobot::VarColorPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::VarColor::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::VarColor& l, const ::jderobot::VarColor& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::VarColor& l, const ::jderobot::VarColor& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
