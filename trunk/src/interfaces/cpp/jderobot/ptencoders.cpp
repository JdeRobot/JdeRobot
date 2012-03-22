// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ptencoders.ice'

#include <ptencoders.h>
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

static const ::std::string __jderobot__PTEncoders__getPTEncodersData_name = "getPTEncodersData";

::Ice::Object* IceInternal::upCast(::jderobot::PTEncodersData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PTEncodersData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::PTEncoders* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PTEncoders* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PTEncodersDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PTEncodersData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PTEncodersPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PTEncoders;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::PTEncodersData::ice_staticId()
{
    return ::jderobot::PTEncodersData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PTEncodersData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PTEncodersData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PTEncodersData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PTEncodersData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PTEncodersData::__newInstance() const
{
    return new PTEncodersData;
}

::jderobot::PTEncodersDataPtr
IceProxy::jderobot::PTEncoders::getPTEncodersData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__PTEncoders__getPTEncodersData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::PTEncoders* __del = dynamic_cast< ::IceDelegate::jderobot::PTEncoders*>(__delBase.get());
            return __del->getPTEncodersData(__ctx);
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
IceProxy::jderobot::PTEncoders::ice_staticId()
{
    return ::jderobot::PTEncoders::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PTEncoders::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PTEncoders);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PTEncoders::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PTEncoders);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PTEncoders::__newInstance() const
{
    return new PTEncoders;
}

::jderobot::PTEncodersDataPtr
IceDelegateM::jderobot::PTEncoders::getPTEncodersData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__PTEncoders__getPTEncodersData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::PTEncodersDataPtr __ret;
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
        __is->read(::jderobot::__patch__PTEncodersDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::PTEncodersDataPtr
IceDelegateD::jderobot::PTEncoders::getPTEncodersData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::PTEncodersDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::PTEncoders* servant = dynamic_cast< ::jderobot::PTEncoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPTEncodersData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::PTEncodersDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__PTEncoders__getPTEncodersData_name, ::Ice::Idempotent, __context);
    ::jderobot::PTEncodersDataPtr __result;
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

jderobot::PTEncodersData::PTEncodersData(::Ice::Float __ice_panAngle, ::Ice::Float __ice_tiltAngle, ::Ice::Int __ice_clock) :
    panAngle(__ice_panAngle),
    tiltAngle(__ice_tiltAngle),
    clock(__ice_clock)
{
}

::Ice::ObjectPtr
jderobot::PTEncodersData::ice_clone() const
{
    ::jderobot::PTEncodersDataPtr __p = new ::jderobot::PTEncodersData(*this);
    return __p;
}

static const ::std::string __jderobot__PTEncodersData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PTEncodersData"
};

bool
jderobot::PTEncodersData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PTEncodersData_ids, __jderobot__PTEncodersData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PTEncodersData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PTEncodersData_ids[0], &__jderobot__PTEncodersData_ids[2]);
}

const ::std::string&
jderobot::PTEncodersData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PTEncodersData_ids[1];
}

const ::std::string&
jderobot::PTEncodersData::ice_staticId()
{
    return __jderobot__PTEncodersData_ids[1];
}

void
jderobot::PTEncodersData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(panAngle);
    __os->write(tiltAngle);
    __os->write(clock);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::PTEncodersData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(panAngle);
    __is->read(tiltAngle);
    __is->read(clock);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::PTEncodersData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTEncodersData was not generated with stream support";
    throw ex;
}

void
jderobot::PTEncodersData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTEncodersData was not generated with stream support";
    throw ex;
}

class __F__jderobot__PTEncodersData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::PTEncodersData::ice_staticId());
        return new ::jderobot::PTEncodersData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__PTEncodersData_Ptr = new __F__jderobot__PTEncodersData;

const ::Ice::ObjectFactoryPtr&
jderobot::PTEncodersData::ice_factory()
{
    return __F__jderobot__PTEncodersData_Ptr;
}

class __F__jderobot__PTEncodersData__Init
{
public:

    __F__jderobot__PTEncodersData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::PTEncodersData::ice_staticId(), ::jderobot::PTEncodersData::ice_factory());
    }

    ~__F__jderobot__PTEncodersData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::PTEncodersData::ice_staticId());
    }
};

static __F__jderobot__PTEncodersData__Init __F__jderobot__PTEncodersData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__PTEncodersData__initializer() {} }
#endif

void 
jderobot::__patch__PTEncodersDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PTEncodersDataPtr* p = static_cast< ::jderobot::PTEncodersDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PTEncodersDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PTEncodersData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PTEncodersData& l, const ::jderobot::PTEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PTEncodersData& l, const ::jderobot::PTEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::PTEncoders::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__PTEncoders_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PTEncoders"
};

bool
jderobot::PTEncoders::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PTEncoders_ids, __jderobot__PTEncoders_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PTEncoders::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PTEncoders_ids[0], &__jderobot__PTEncoders_ids[2]);
}

const ::std::string&
jderobot::PTEncoders::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PTEncoders_ids[1];
}

const ::std::string&
jderobot::PTEncoders::ice_staticId()
{
    return __jderobot__PTEncoders_ids[1];
}

::Ice::DispatchStatus
jderobot::PTEncoders::___getPTEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::PTEncodersDataPtr __ret = getPTEncodersData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__PTEncoders_all[] =
{
    "getPTEncodersData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::PTEncoders::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__PTEncoders_all, __jderobot__PTEncoders_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__PTEncoders_all)
    {
        case 0:
        {
            return ___getPTEncodersData(in, current);
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
jderobot::PTEncoders::__write(::IceInternal::BasicStream* __os) const
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
jderobot::PTEncoders::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::PTEncoders::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTEncoders was not generated with stream support";
    throw ex;
}

void
jderobot::PTEncoders::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTEncoders was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__PTEncodersPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PTEncodersPtr* p = static_cast< ::jderobot::PTEncodersPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PTEncodersPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PTEncoders::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PTEncoders& l, const ::jderobot::PTEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PTEncoders& l, const ::jderobot::PTEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
