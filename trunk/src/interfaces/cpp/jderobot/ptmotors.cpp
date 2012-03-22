// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ptmotors.ice'

#include <ptmotors.h>
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

static const ::std::string __jderobot__PTMotors__setPTMotorsData_name = "setPTMotorsData";

static const ::std::string __jderobot__PTMotors__getPTMotorsData_name = "getPTMotorsData";

static const ::std::string __jderobot__PTMotors__getPTMotorsParams_name = "getPTMotorsParams";

::Ice::Object* IceInternal::upCast(::jderobot::PTMotorsData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PTMotorsData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::PTMotorsParams* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PTMotorsParams* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::PTMotors* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PTMotors* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PTMotorsDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PTMotorsData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PTMotorsParamsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PTMotorsParams;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PTMotorsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PTMotors;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::PTMotorsData::ice_staticId()
{
    return ::jderobot::PTMotorsData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PTMotorsData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PTMotorsData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PTMotorsData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PTMotorsData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PTMotorsData::__newInstance() const
{
    return new PTMotorsData;
}

const ::std::string&
IceProxy::jderobot::PTMotorsParams::ice_staticId()
{
    return ::jderobot::PTMotorsParams::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PTMotorsParams::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PTMotorsParams);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PTMotorsParams::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PTMotorsParams);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PTMotorsParams::__newInstance() const
{
    return new PTMotorsParams;
}

::Ice::Int
IceProxy::jderobot::PTMotors::setPTMotorsData(const ::jderobot::PTMotorsDataPtr& data, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__PTMotors__setPTMotorsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::PTMotors* __del = dynamic_cast< ::IceDelegate::jderobot::PTMotors*>(__delBase.get());
            return __del->setPTMotorsData(data, __ctx);
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

::jderobot::PTMotorsDataPtr
IceProxy::jderobot::PTMotors::getPTMotorsData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__PTMotors__getPTMotorsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::PTMotors* __del = dynamic_cast< ::IceDelegate::jderobot::PTMotors*>(__delBase.get());
            return __del->getPTMotorsData(__ctx);
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

::jderobot::PTMotorsParamsPtr
IceProxy::jderobot::PTMotors::getPTMotorsParams(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__PTMotors__getPTMotorsParams_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::PTMotors* __del = dynamic_cast< ::IceDelegate::jderobot::PTMotors*>(__delBase.get());
            return __del->getPTMotorsParams(__ctx);
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
IceProxy::jderobot::PTMotors::ice_staticId()
{
    return ::jderobot::PTMotors::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PTMotors::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PTMotors);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PTMotors::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PTMotors);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PTMotors::__newInstance() const
{
    return new PTMotors;
}

::Ice::Int
IceDelegateM::jderobot::PTMotors::setPTMotorsData(const ::jderobot::PTMotorsDataPtr& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__PTMotors__setPTMotorsData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(data.get())));
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

::jderobot::PTMotorsDataPtr
IceDelegateM::jderobot::PTMotors::getPTMotorsData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__PTMotors__getPTMotorsData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::PTMotorsDataPtr __ret;
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
        __is->read(::jderobot::__patch__PTMotorsDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::PTMotorsParamsPtr
IceDelegateM::jderobot::PTMotors::getPTMotorsParams(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__PTMotors__getPTMotorsParams_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::PTMotorsParamsPtr __ret;
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
        __is->read(::jderobot::__patch__PTMotorsParamsPtr, &__ret);
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
IceDelegateD::jderobot::PTMotors::setPTMotorsData(const ::jderobot::PTMotorsDataPtr& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::jderobot::PTMotorsDataPtr& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::PTMotors* servant = dynamic_cast< ::jderobot::PTMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->setPTMotorsData(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::jderobot::PTMotorsDataPtr& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__PTMotors__setPTMotorsData_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, data, __current);
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

::jderobot::PTMotorsDataPtr
IceDelegateD::jderobot::PTMotors::getPTMotorsData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::PTMotorsDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::PTMotors* servant = dynamic_cast< ::jderobot::PTMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPTMotorsData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::PTMotorsDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__PTMotors__getPTMotorsData_name, ::Ice::Idempotent, __context);
    ::jderobot::PTMotorsDataPtr __result;
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

::jderobot::PTMotorsParamsPtr
IceDelegateD::jderobot::PTMotors::getPTMotorsParams(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::PTMotorsParamsPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::PTMotors* servant = dynamic_cast< ::jderobot::PTMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPTMotorsParams(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::PTMotorsParamsPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__PTMotors__getPTMotorsParams_name, ::Ice::Idempotent, __context);
    ::jderobot::PTMotorsParamsPtr __result;
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

jderobot::PTMotorsData::PTMotorsData(::Ice::Float __ice_latitude, ::Ice::Float __ice_latitudeSpeed, ::Ice::Float __ice_longitude, ::Ice::Float __ice_longitudeSpeed) :
    latitude(__ice_latitude),
    latitudeSpeed(__ice_latitudeSpeed),
    longitude(__ice_longitude),
    longitudeSpeed(__ice_longitudeSpeed)
{
}

::Ice::ObjectPtr
jderobot::PTMotorsData::ice_clone() const
{
    ::jderobot::PTMotorsDataPtr __p = new ::jderobot::PTMotorsData(*this);
    return __p;
}

static const ::std::string __jderobot__PTMotorsData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PTMotorsData"
};

bool
jderobot::PTMotorsData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PTMotorsData_ids, __jderobot__PTMotorsData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PTMotorsData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PTMotorsData_ids[0], &__jderobot__PTMotorsData_ids[2]);
}

const ::std::string&
jderobot::PTMotorsData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PTMotorsData_ids[1];
}

const ::std::string&
jderobot::PTMotorsData::ice_staticId()
{
    return __jderobot__PTMotorsData_ids[1];
}

void
jderobot::PTMotorsData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(latitude);
    __os->write(latitudeSpeed);
    __os->write(longitude);
    __os->write(longitudeSpeed);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::PTMotorsData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(latitude);
    __is->read(latitudeSpeed);
    __is->read(longitude);
    __is->read(longitudeSpeed);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::PTMotorsData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotorsData was not generated with stream support";
    throw ex;
}

void
jderobot::PTMotorsData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotorsData was not generated with stream support";
    throw ex;
}

class __F__jderobot__PTMotorsData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::PTMotorsData::ice_staticId());
        return new ::jderobot::PTMotorsData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__PTMotorsData_Ptr = new __F__jderobot__PTMotorsData;

const ::Ice::ObjectFactoryPtr&
jderobot::PTMotorsData::ice_factory()
{
    return __F__jderobot__PTMotorsData_Ptr;
}

class __F__jderobot__PTMotorsData__Init
{
public:

    __F__jderobot__PTMotorsData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::PTMotorsData::ice_staticId(), ::jderobot::PTMotorsData::ice_factory());
    }

    ~__F__jderobot__PTMotorsData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::PTMotorsData::ice_staticId());
    }
};

static __F__jderobot__PTMotorsData__Init __F__jderobot__PTMotorsData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__PTMotorsData__initializer() {} }
#endif

void 
jderobot::__patch__PTMotorsDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PTMotorsDataPtr* p = static_cast< ::jderobot::PTMotorsDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PTMotorsDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PTMotorsData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PTMotorsData& l, const ::jderobot::PTMotorsData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PTMotorsData& l, const ::jderobot::PTMotorsData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

jderobot::PTMotorsParams::PTMotorsParams(::Ice::Float __ice_maxLongitude, ::Ice::Float __ice_minLongitude, ::Ice::Float __ice_maxLatitude, ::Ice::Float __ice_minLatitude, ::Ice::Float __ice_maxLongitudeSpeed, ::Ice::Float __ice_maxLatitudeSpeed) :
    maxLongitude(__ice_maxLongitude),
    minLongitude(__ice_minLongitude),
    maxLatitude(__ice_maxLatitude),
    minLatitude(__ice_minLatitude),
    maxLongitudeSpeed(__ice_maxLongitudeSpeed),
    maxLatitudeSpeed(__ice_maxLatitudeSpeed)
{
}

::Ice::ObjectPtr
jderobot::PTMotorsParams::ice_clone() const
{
    ::jderobot::PTMotorsParamsPtr __p = new ::jderobot::PTMotorsParams(*this);
    return __p;
}

static const ::std::string __jderobot__PTMotorsParams_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PTMotorsParams"
};

bool
jderobot::PTMotorsParams::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PTMotorsParams_ids, __jderobot__PTMotorsParams_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PTMotorsParams::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PTMotorsParams_ids[0], &__jderobot__PTMotorsParams_ids[2]);
}

const ::std::string&
jderobot::PTMotorsParams::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PTMotorsParams_ids[1];
}

const ::std::string&
jderobot::PTMotorsParams::ice_staticId()
{
    return __jderobot__PTMotorsParams_ids[1];
}

void
jderobot::PTMotorsParams::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(maxLongitude);
    __os->write(minLongitude);
    __os->write(maxLatitude);
    __os->write(minLatitude);
    __os->write(maxLongitudeSpeed);
    __os->write(maxLatitudeSpeed);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::PTMotorsParams::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(maxLongitude);
    __is->read(minLongitude);
    __is->read(maxLatitude);
    __is->read(minLatitude);
    __is->read(maxLongitudeSpeed);
    __is->read(maxLatitudeSpeed);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::PTMotorsParams::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotorsParams was not generated with stream support";
    throw ex;
}

void
jderobot::PTMotorsParams::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotorsParams was not generated with stream support";
    throw ex;
}

class __F__jderobot__PTMotorsParams : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::PTMotorsParams::ice_staticId());
        return new ::jderobot::PTMotorsParams;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__PTMotorsParams_Ptr = new __F__jderobot__PTMotorsParams;

const ::Ice::ObjectFactoryPtr&
jderobot::PTMotorsParams::ice_factory()
{
    return __F__jderobot__PTMotorsParams_Ptr;
}

class __F__jderobot__PTMotorsParams__Init
{
public:

    __F__jderobot__PTMotorsParams__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::PTMotorsParams::ice_staticId(), ::jderobot::PTMotorsParams::ice_factory());
    }

    ~__F__jderobot__PTMotorsParams__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::PTMotorsParams::ice_staticId());
    }
};

static __F__jderobot__PTMotorsParams__Init __F__jderobot__PTMotorsParams__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__PTMotorsParams__initializer() {} }
#endif

void 
jderobot::__patch__PTMotorsParamsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PTMotorsParamsPtr* p = static_cast< ::jderobot::PTMotorsParamsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PTMotorsParamsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PTMotorsParams::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PTMotorsParams& l, const ::jderobot::PTMotorsParams& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PTMotorsParams& l, const ::jderobot::PTMotorsParams& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::PTMotors::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__PTMotors_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PTMotors"
};

bool
jderobot::PTMotors::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PTMotors_ids, __jderobot__PTMotors_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PTMotors::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PTMotors_ids[0], &__jderobot__PTMotors_ids[2]);
}

const ::std::string&
jderobot::PTMotors::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PTMotors_ids[1];
}

const ::std::string&
jderobot::PTMotors::ice_staticId()
{
    return __jderobot__PTMotors_ids[1];
}

::Ice::DispatchStatus
jderobot::PTMotors::___setPTMotorsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::PTMotorsDataPtr data;
    __is->read(::jderobot::__patch__PTMotorsDataPtr, &data);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setPTMotorsData(data, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::PTMotors::___getPTMotorsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::PTMotorsDataPtr __ret = getPTMotorsData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::PTMotors::___getPTMotorsParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::PTMotorsParamsPtr __ret = getPTMotorsParams(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__PTMotors_all[] =
{
    "getPTMotorsData",
    "getPTMotorsParams",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setPTMotorsData"
};

::Ice::DispatchStatus
jderobot::PTMotors::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__PTMotors_all, __jderobot__PTMotors_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__PTMotors_all)
    {
        case 0:
        {
            return ___getPTMotorsData(in, current);
        }
        case 1:
        {
            return ___getPTMotorsParams(in, current);
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
            return ___setPTMotorsData(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::PTMotors::__write(::IceInternal::BasicStream* __os) const
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
jderobot::PTMotors::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::PTMotors::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotors was not generated with stream support";
    throw ex;
}

void
jderobot::PTMotors::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PTMotors was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__PTMotorsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PTMotorsPtr* p = static_cast< ::jderobot::PTMotorsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PTMotorsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PTMotors::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PTMotors& l, const ::jderobot::PTMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PTMotors& l, const ::jderobot::PTMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
