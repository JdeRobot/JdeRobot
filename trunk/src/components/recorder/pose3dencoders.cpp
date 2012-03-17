// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `pose3dencoders.ice'

#include "pose3dencoders.h"
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

static const ::std::string __jderobot__Pose3DEncoders__getPose3DEncodersData_name = "getPose3DEncodersData";

::Ice::Object* IceInternal::upCast(::jderobot::Pose3DEncodersData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Pose3DEncodersData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Pose3DEncoders* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Pose3DEncoders* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::Pose3DEncodersDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Pose3DEncodersData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::Pose3DEncodersPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Pose3DEncoders;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::Pose3DEncodersData::ice_staticId()
{
    return ::jderobot::Pose3DEncodersData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Pose3DEncodersData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Pose3DEncodersData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Pose3DEncodersData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Pose3DEncodersData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DEncodersData::__newInstance() const
{
    return new Pose3DEncodersData;
}

::jderobot::Pose3DEncodersDataPtr
IceProxy::jderobot::Pose3DEncoders::getPose3DEncodersData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Pose3DEncoders__getPose3DEncodersData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Pose3DEncoders* __del = dynamic_cast< ::IceDelegate::jderobot::Pose3DEncoders*>(__delBase.get());
            return __del->getPose3DEncodersData(__ctx);
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
IceProxy::jderobot::Pose3DEncoders::ice_staticId()
{
    return ::jderobot::Pose3DEncoders::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Pose3DEncoders::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Pose3DEncoders);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Pose3DEncoders::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Pose3DEncoders);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DEncoders::__newInstance() const
{
    return new Pose3DEncoders;
}

::jderobot::Pose3DEncodersDataPtr
IceDelegateM::jderobot::Pose3DEncoders::getPose3DEncodersData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Pose3DEncoders__getPose3DEncodersData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::Pose3DEncodersDataPtr __ret;
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
        __is->read(::jderobot::__patch__Pose3DEncodersDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::Pose3DEncodersDataPtr
IceDelegateD::jderobot::Pose3DEncoders::getPose3DEncodersData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::Pose3DEncodersDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Pose3DEncoders* servant = dynamic_cast< ::jderobot::Pose3DEncoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPose3DEncodersData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::Pose3DEncodersDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Pose3DEncoders__getPose3DEncodersData_name, ::Ice::Idempotent, __context);
    ::jderobot::Pose3DEncodersDataPtr __result;
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

jderobot::Pose3DEncodersData::Pose3DEncodersData(::Ice::Float __ice_x, ::Ice::Float __ice_y, ::Ice::Float __ice_z, ::Ice::Float __ice_pan, ::Ice::Float __ice_tilt, ::Ice::Float __ice_roll, ::Ice::Int __ice_clock) :
    x(__ice_x),
    y(__ice_y),
    z(__ice_z),
    pan(__ice_pan),
    tilt(__ice_tilt),
    roll(__ice_roll),
    clock(__ice_clock)
{
}

::Ice::ObjectPtr
jderobot::Pose3DEncodersData::ice_clone() const
{
    ::jderobot::Pose3DEncodersDataPtr __p = new ::jderobot::Pose3DEncodersData(*this);
    return __p;
}

static const ::std::string __jderobot__Pose3DEncodersData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Pose3DEncodersData"
};

bool
jderobot::Pose3DEncodersData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Pose3DEncodersData_ids, __jderobot__Pose3DEncodersData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Pose3DEncodersData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Pose3DEncodersData_ids[0], &__jderobot__Pose3DEncodersData_ids[2]);
}

const ::std::string&
jderobot::Pose3DEncodersData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Pose3DEncodersData_ids[1];
}

const ::std::string&
jderobot::Pose3DEncodersData::ice_staticId()
{
    return __jderobot__Pose3DEncodersData_ids[1];
}

void
jderobot::Pose3DEncodersData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(x);
    __os->write(y);
    __os->write(z);
    __os->write(pan);
    __os->write(tilt);
    __os->write(roll);
    __os->write(clock);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::Pose3DEncodersData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(x);
    __is->read(y);
    __is->read(z);
    __is->read(pan);
    __is->read(tilt);
    __is->read(roll);
    __is->read(clock);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::Pose3DEncodersData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DEncodersData was not generated with stream support";
    throw ex;
}

void
jderobot::Pose3DEncodersData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DEncodersData was not generated with stream support";
    throw ex;
}

class __F__jderobot__Pose3DEncodersData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::Pose3DEncodersData::ice_staticId());
        return new ::jderobot::Pose3DEncodersData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__Pose3DEncodersData_Ptr = new __F__jderobot__Pose3DEncodersData;

const ::Ice::ObjectFactoryPtr&
jderobot::Pose3DEncodersData::ice_factory()
{
    return __F__jderobot__Pose3DEncodersData_Ptr;
}

class __F__jderobot__Pose3DEncodersData__Init
{
public:

    __F__jderobot__Pose3DEncodersData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::Pose3DEncodersData::ice_staticId(), ::jderobot::Pose3DEncodersData::ice_factory());
    }

    ~__F__jderobot__Pose3DEncodersData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::Pose3DEncodersData::ice_staticId());
    }
};

static __F__jderobot__Pose3DEncodersData__Init __F__jderobot__Pose3DEncodersData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__Pose3DEncodersData__initializer() {} }
#endif

void 
jderobot::__patch__Pose3DEncodersDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::Pose3DEncodersDataPtr* p = static_cast< ::jderobot::Pose3DEncodersDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::Pose3DEncodersDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Pose3DEncodersData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Pose3DEncodersData& l, const ::jderobot::Pose3DEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Pose3DEncodersData& l, const ::jderobot::Pose3DEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::Pose3DEncoders::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__Pose3DEncoders_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Pose3DEncoders"
};

bool
jderobot::Pose3DEncoders::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Pose3DEncoders_ids, __jderobot__Pose3DEncoders_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Pose3DEncoders::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Pose3DEncoders_ids[0], &__jderobot__Pose3DEncoders_ids[2]);
}

const ::std::string&
jderobot::Pose3DEncoders::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Pose3DEncoders_ids[1];
}

const ::std::string&
jderobot::Pose3DEncoders::ice_staticId()
{
    return __jderobot__Pose3DEncoders_ids[1];
}

::Ice::DispatchStatus
jderobot::Pose3DEncoders::___getPose3DEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::Pose3DEncodersDataPtr __ret = getPose3DEncodersData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__Pose3DEncoders_all[] =
{
    "getPose3DEncodersData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::Pose3DEncoders::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Pose3DEncoders_all, __jderobot__Pose3DEncoders_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Pose3DEncoders_all)
    {
        case 0:
        {
            return ___getPose3DEncodersData(in, current);
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
jderobot::Pose3DEncoders::__write(::IceInternal::BasicStream* __os) const
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
jderobot::Pose3DEncoders::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::Pose3DEncoders::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DEncoders was not generated with stream support";
    throw ex;
}

void
jderobot::Pose3DEncoders::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DEncoders was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__Pose3DEncodersPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::Pose3DEncodersPtr* p = static_cast< ::jderobot::Pose3DEncodersPtr*>(__addr);
    assert(p);
    *p = ::jderobot::Pose3DEncodersPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Pose3DEncoders::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Pose3DEncoders& l, const ::jderobot::Pose3DEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Pose3DEncoders& l, const ::jderobot::Pose3DEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
