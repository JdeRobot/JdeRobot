// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `encoders.ice'

#include <encoders.h>
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

static const ::std::string __jderobot__Encoders__getEncodersData_name = "getEncodersData";

::Ice::Object* IceInternal::upCast(::jderobot::EncodersData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::EncodersData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Encoders* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Encoders* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::EncodersDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::EncodersData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::EncodersPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Encoders;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::EncodersData::ice_staticId()
{
    return ::jderobot::EncodersData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::EncodersData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::EncodersData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::EncodersData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::EncodersData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::EncodersData::__newInstance() const
{
    return new EncodersData;
}

::jderobot::EncodersDataPtr
IceProxy::jderobot::Encoders::getEncodersData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Encoders__getEncodersData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Encoders* __del = dynamic_cast< ::IceDelegate::jderobot::Encoders*>(__delBase.get());
            return __del->getEncodersData(__ctx);
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
IceProxy::jderobot::Encoders::ice_staticId()
{
    return ::jderobot::Encoders::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Encoders::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Encoders);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Encoders::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Encoders);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Encoders::__newInstance() const
{
    return new Encoders;
}

::jderobot::EncodersDataPtr
IceDelegateM::jderobot::Encoders::getEncodersData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Encoders__getEncodersData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::EncodersDataPtr __ret;
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
        __is->read(::jderobot::__patch__EncodersDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::EncodersDataPtr
IceDelegateD::jderobot::Encoders::getEncodersData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::EncodersDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Encoders* servant = dynamic_cast< ::jderobot::Encoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getEncodersData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::EncodersDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Encoders__getEncodersData_name, ::Ice::Idempotent, __context);
    ::jderobot::EncodersDataPtr __result;
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

jderobot::EncodersData::EncodersData(::Ice::Float __ice_robotx, ::Ice::Float __ice_roboty, ::Ice::Float __ice_robottheta, ::Ice::Float __ice_robotcos, ::Ice::Float __ice_robotsin) :
    robotx(__ice_robotx),
    roboty(__ice_roboty),
    robottheta(__ice_robottheta),
    robotcos(__ice_robotcos),
    robotsin(__ice_robotsin)
{
}

::Ice::ObjectPtr
jderobot::EncodersData::ice_clone() const
{
    ::jderobot::EncodersDataPtr __p = new ::jderobot::EncodersData(*this);
    return __p;
}

static const ::std::string __jderobot__EncodersData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::EncodersData"
};

bool
jderobot::EncodersData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__EncodersData_ids, __jderobot__EncodersData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::EncodersData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__EncodersData_ids[0], &__jderobot__EncodersData_ids[2]);
}

const ::std::string&
jderobot::EncodersData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__EncodersData_ids[1];
}

const ::std::string&
jderobot::EncodersData::ice_staticId()
{
    return __jderobot__EncodersData_ids[1];
}

void
jderobot::EncodersData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(robotx);
    __os->write(roboty);
    __os->write(robottheta);
    __os->write(robotcos);
    __os->write(robotsin);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::EncodersData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(robotx);
    __is->read(roboty);
    __is->read(robottheta);
    __is->read(robotcos);
    __is->read(robotsin);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::EncodersData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::EncodersData was not generated with stream support";
    throw ex;
}

void
jderobot::EncodersData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::EncodersData was not generated with stream support";
    throw ex;
}

class __F__jderobot__EncodersData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::EncodersData::ice_staticId());
        return new ::jderobot::EncodersData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__EncodersData_Ptr = new __F__jderobot__EncodersData;

const ::Ice::ObjectFactoryPtr&
jderobot::EncodersData::ice_factory()
{
    return __F__jderobot__EncodersData_Ptr;
}

class __F__jderobot__EncodersData__Init
{
public:

    __F__jderobot__EncodersData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::EncodersData::ice_staticId(), ::jderobot::EncodersData::ice_factory());
    }

    ~__F__jderobot__EncodersData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::EncodersData::ice_staticId());
    }
};

static __F__jderobot__EncodersData__Init __F__jderobot__EncodersData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__EncodersData__initializer() {} }
#endif

void 
jderobot::__patch__EncodersDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::EncodersDataPtr* p = static_cast< ::jderobot::EncodersDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::EncodersDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::EncodersData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::EncodersData& l, const ::jderobot::EncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::EncodersData& l, const ::jderobot::EncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::Encoders::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__Encoders_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Encoders"
};

bool
jderobot::Encoders::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Encoders_ids, __jderobot__Encoders_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Encoders::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Encoders_ids[0], &__jderobot__Encoders_ids[2]);
}

const ::std::string&
jderobot::Encoders::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Encoders_ids[1];
}

const ::std::string&
jderobot::Encoders::ice_staticId()
{
    return __jderobot__Encoders_ids[1];
}

::Ice::DispatchStatus
jderobot::Encoders::___getEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::EncodersDataPtr __ret = getEncodersData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__Encoders_all[] =
{
    "getEncodersData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::Encoders::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Encoders_all, __jderobot__Encoders_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Encoders_all)
    {
        case 0:
        {
            return ___getEncodersData(in, current);
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
jderobot::Encoders::__write(::IceInternal::BasicStream* __os) const
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
jderobot::Encoders::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::Encoders::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Encoders was not generated with stream support";
    throw ex;
}

void
jderobot::Encoders::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Encoders was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__EncodersPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::EncodersPtr* p = static_cast< ::jderobot::EncodersPtr*>(__addr);
    assert(p);
    *p = ::jderobot::EncodersPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Encoders::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Encoders& l, const ::jderobot::Encoders& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Encoders& l, const ::jderobot::Encoders& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
