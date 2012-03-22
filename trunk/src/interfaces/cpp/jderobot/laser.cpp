// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `laser.ice'

#include <laser.h>
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

static const ::std::string __jderobot__Laser__getLaserData_name = "getLaserData";

::Ice::Object* IceInternal::upCast(::jderobot::LaserData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::LaserData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Laser* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Laser* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::LaserDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::LaserData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::LaserPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Laser;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::LaserData::ice_staticId()
{
    return ::jderobot::LaserData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::LaserData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::LaserData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::LaserData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::LaserData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::LaserData::__newInstance() const
{
    return new LaserData;
}

::jderobot::LaserDataPtr
IceProxy::jderobot::Laser::getLaserData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Laser__getLaserData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Laser* __del = dynamic_cast< ::IceDelegate::jderobot::Laser*>(__delBase.get());
            return __del->getLaserData(__ctx);
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
IceProxy::jderobot::Laser::ice_staticId()
{
    return ::jderobot::Laser::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Laser::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Laser);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Laser::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Laser);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Laser::__newInstance() const
{
    return new Laser;
}

::jderobot::LaserDataPtr
IceDelegateM::jderobot::Laser::getLaserData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Laser__getLaserData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::LaserDataPtr __ret;
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
        __is->read(::jderobot::__patch__LaserDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::LaserDataPtr
IceDelegateD::jderobot::Laser::getLaserData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::LaserDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Laser* servant = dynamic_cast< ::jderobot::Laser*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getLaserData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::LaserDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Laser__getLaserData_name, ::Ice::Idempotent, __context);
    ::jderobot::LaserDataPtr __result;
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

jderobot::LaserData::LaserData(const ::jderobot::IntSeq& __ice_distanceData, ::Ice::Int __ice_numLaser) :
    distanceData(__ice_distanceData),
    numLaser(__ice_numLaser)
{
}

::Ice::ObjectPtr
jderobot::LaserData::ice_clone() const
{
    ::jderobot::LaserDataPtr __p = new ::jderobot::LaserData(*this);
    return __p;
}

static const ::std::string __jderobot__LaserData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::LaserData"
};

bool
jderobot::LaserData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__LaserData_ids, __jderobot__LaserData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::LaserData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__LaserData_ids[0], &__jderobot__LaserData_ids[2]);
}

const ::std::string&
jderobot::LaserData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__LaserData_ids[1];
}

const ::std::string&
jderobot::LaserData::ice_staticId()
{
    return __jderobot__LaserData_ids[1];
}

void
jderobot::LaserData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(distanceData.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&distanceData[0], &distanceData[0] + distanceData.size());
    }
    __os->write(numLaser);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::LaserData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(distanceData);
    __is->read(numLaser);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::LaserData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::LaserData was not generated with stream support";
    throw ex;
}

void
jderobot::LaserData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::LaserData was not generated with stream support";
    throw ex;
}

class __F__jderobot__LaserData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::LaserData::ice_staticId());
        return new ::jderobot::LaserData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__LaserData_Ptr = new __F__jderobot__LaserData;

const ::Ice::ObjectFactoryPtr&
jderobot::LaserData::ice_factory()
{
    return __F__jderobot__LaserData_Ptr;
}

class __F__jderobot__LaserData__Init
{
public:

    __F__jderobot__LaserData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::LaserData::ice_staticId(), ::jderobot::LaserData::ice_factory());
    }

    ~__F__jderobot__LaserData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::LaserData::ice_staticId());
    }
};

static __F__jderobot__LaserData__Init __F__jderobot__LaserData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__LaserData__initializer() {} }
#endif

void 
jderobot::__patch__LaserDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::LaserDataPtr* p = static_cast< ::jderobot::LaserDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::LaserDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::LaserData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::LaserData& l, const ::jderobot::LaserData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::LaserData& l, const ::jderobot::LaserData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::Laser::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__Laser_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Laser"
};

bool
jderobot::Laser::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Laser_ids, __jderobot__Laser_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Laser::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Laser_ids[0], &__jderobot__Laser_ids[2]);
}

const ::std::string&
jderobot::Laser::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Laser_ids[1];
}

const ::std::string&
jderobot::Laser::ice_staticId()
{
    return __jderobot__Laser_ids[1];
}

::Ice::DispatchStatus
jderobot::Laser::___getLaserData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::LaserDataPtr __ret = getLaserData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__Laser_all[] =
{
    "getLaserData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::Laser::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Laser_all, __jderobot__Laser_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Laser_all)
    {
        case 0:
        {
            return ___getLaserData(in, current);
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
jderobot::Laser::__write(::IceInternal::BasicStream* __os) const
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
jderobot::Laser::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::Laser::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Laser was not generated with stream support";
    throw ex;
}

void
jderobot::Laser::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Laser was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__LaserPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::LaserPtr* p = static_cast< ::jderobot::LaserPtr*>(__addr);
    assert(p);
    *p = ::jderobot::LaserPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Laser::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Laser& l, const ::jderobot::Laser& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Laser& l, const ::jderobot::Laser& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
