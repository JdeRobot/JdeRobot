// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `sonars.ice'

#include <sonars.h>
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

static const ::std::string __jderobot__Sonars__getSonarsData_name = "getSonarsData";

::Ice::Object* IceInternal::upCast(::jderobot::SonarsData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::SonarsData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Sonars* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Sonars* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::SonarsDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::SonarsData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::SonarsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Sonars;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::SonarsData::ice_staticId()
{
    return ::jderobot::SonarsData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::SonarsData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::SonarsData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::SonarsData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::SonarsData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::SonarsData::__newInstance() const
{
    return new SonarsData;
}

::jderobot::SonarsDataPtr
IceProxy::jderobot::Sonars::getSonarsData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Sonars__getSonarsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Sonars* __del = dynamic_cast< ::IceDelegate::jderobot::Sonars*>(__delBase.get());
            return __del->getSonarsData(__ctx);
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
IceProxy::jderobot::Sonars::ice_staticId()
{
    return ::jderobot::Sonars::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Sonars::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Sonars);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Sonars::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Sonars);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Sonars::__newInstance() const
{
    return new Sonars;
}

::jderobot::SonarsDataPtr
IceDelegateM::jderobot::Sonars::getSonarsData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Sonars__getSonarsData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::SonarsDataPtr __ret;
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
        __is->read(::jderobot::__patch__SonarsDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::SonarsDataPtr
IceDelegateD::jderobot::Sonars::getSonarsData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::SonarsDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Sonars* servant = dynamic_cast< ::jderobot::Sonars*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getSonarsData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::SonarsDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Sonars__getSonarsData_name, ::Ice::Idempotent, __context);
    ::jderobot::SonarsDataPtr __result;
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

jderobot::SonarsData::SonarsData(const ::jderobot::IntSeq& __ice_us, ::Ice::Int __ice_numSonars) :
    us(__ice_us),
    numSonars(__ice_numSonars)
{
}

::Ice::ObjectPtr
jderobot::SonarsData::ice_clone() const
{
    ::jderobot::SonarsDataPtr __p = new ::jderobot::SonarsData(*this);
    return __p;
}

static const ::std::string __jderobot__SonarsData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::SonarsData"
};

bool
jderobot::SonarsData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__SonarsData_ids, __jderobot__SonarsData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::SonarsData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__SonarsData_ids[0], &__jderobot__SonarsData_ids[2]);
}

const ::std::string&
jderobot::SonarsData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__SonarsData_ids[1];
}

const ::std::string&
jderobot::SonarsData::ice_staticId()
{
    return __jderobot__SonarsData_ids[1];
}

void
jderobot::SonarsData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(us.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&us[0], &us[0] + us.size());
    }
    __os->write(numSonars);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::SonarsData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(us);
    __is->read(numSonars);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::SonarsData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::SonarsData was not generated with stream support";
    throw ex;
}

void
jderobot::SonarsData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::SonarsData was not generated with stream support";
    throw ex;
}

class __F__jderobot__SonarsData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::SonarsData::ice_staticId());
        return new ::jderobot::SonarsData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__SonarsData_Ptr = new __F__jderobot__SonarsData;

const ::Ice::ObjectFactoryPtr&
jderobot::SonarsData::ice_factory()
{
    return __F__jderobot__SonarsData_Ptr;
}

class __F__jderobot__SonarsData__Init
{
public:

    __F__jderobot__SonarsData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::SonarsData::ice_staticId(), ::jderobot::SonarsData::ice_factory());
    }

    ~__F__jderobot__SonarsData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::SonarsData::ice_staticId());
    }
};

static __F__jderobot__SonarsData__Init __F__jderobot__SonarsData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__SonarsData__initializer() {} }
#endif

void 
jderobot::__patch__SonarsDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::SonarsDataPtr* p = static_cast< ::jderobot::SonarsDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::SonarsDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::SonarsData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::SonarsData& l, const ::jderobot::SonarsData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::SonarsData& l, const ::jderobot::SonarsData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::Sonars::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__Sonars_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Sonars"
};

bool
jderobot::Sonars::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Sonars_ids, __jderobot__Sonars_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Sonars::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Sonars_ids[0], &__jderobot__Sonars_ids[2]);
}

const ::std::string&
jderobot::Sonars::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Sonars_ids[1];
}

const ::std::string&
jderobot::Sonars::ice_staticId()
{
    return __jderobot__Sonars_ids[1];
}

::Ice::DispatchStatus
jderobot::Sonars::___getSonarsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::SonarsDataPtr __ret = getSonarsData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__Sonars_all[] =
{
    "getSonarsData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::Sonars::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Sonars_all, __jderobot__Sonars_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Sonars_all)
    {
        case 0:
        {
            return ___getSonarsData(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::Sonars::__write(::IceInternal::BasicStream* __os) const
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
jderobot::Sonars::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::Sonars::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Sonars was not generated with stream support";
    throw ex;
}

void
jderobot::Sonars::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Sonars was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__SonarsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::SonarsPtr* p = static_cast< ::jderobot::SonarsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::SonarsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Sonars::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Sonars& l, const ::jderobot::Sonars& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Sonars& l, const ::jderobot::Sonars& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
