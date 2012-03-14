// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `pose3dmotors.ice'

#include "pose3dmotors.h"
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

static const ::std::string __jderobot__Pose3DMotors__setPose3DMotorsData_name = "setPose3DMotorsData";

static const ::std::string __jderobot__Pose3DMotors__getPose3DMotorsData_name = "getPose3DMotorsData";

static const ::std::string __jderobot__Pose3DMotors__getPose3DMotorsParams_name = "getPose3DMotorsParams";

::Ice::Object* IceInternal::upCast(::jderobot::Pose3DMotorsData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Pose3DMotorsData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Pose3DMotorsParams* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Pose3DMotorsParams* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::Pose3DMotors* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::Pose3DMotors* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::Pose3DMotorsDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Pose3DMotorsData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::Pose3DMotorsParamsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Pose3DMotorsParams;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::Pose3DMotorsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::Pose3DMotors;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::Pose3DMotorsData::ice_staticId()
{
    return ::jderobot::Pose3DMotorsData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Pose3DMotorsData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Pose3DMotorsData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Pose3DMotorsData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Pose3DMotorsData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DMotorsData::__newInstance() const
{
    return new Pose3DMotorsData;
}

const ::std::string&
IceProxy::jderobot::Pose3DMotorsParams::ice_staticId()
{
    return ::jderobot::Pose3DMotorsParams::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Pose3DMotorsParams::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Pose3DMotorsParams);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Pose3DMotorsParams::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Pose3DMotorsParams);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DMotorsParams::__newInstance() const
{
    return new Pose3DMotorsParams;
}

::Ice::Int
IceProxy::jderobot::Pose3DMotors::setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr& data, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Pose3DMotors__setPose3DMotorsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Pose3DMotors* __del = dynamic_cast< ::IceDelegate::jderobot::Pose3DMotors*>(__delBase.get());
            return __del->setPose3DMotorsData(data, __ctx);
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

::jderobot::Pose3DMotorsDataPtr
IceProxy::jderobot::Pose3DMotors::getPose3DMotorsData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Pose3DMotors__getPose3DMotorsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Pose3DMotors* __del = dynamic_cast< ::IceDelegate::jderobot::Pose3DMotors*>(__delBase.get());
            return __del->getPose3DMotorsData(__ctx);
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

::jderobot::Pose3DMotorsParamsPtr
IceProxy::jderobot::Pose3DMotors::getPose3DMotorsParams(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__Pose3DMotors__getPose3DMotorsParams_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::Pose3DMotors* __del = dynamic_cast< ::IceDelegate::jderobot::Pose3DMotors*>(__delBase.get());
            return __del->getPose3DMotorsParams(__ctx);
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
IceProxy::jderobot::Pose3DMotors::ice_staticId()
{
    return ::jderobot::Pose3DMotors::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::Pose3DMotors::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::Pose3DMotors);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::Pose3DMotors::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::Pose3DMotors);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DMotors::__newInstance() const
{
    return new Pose3DMotors;
}

::Ice::Int
IceDelegateM::jderobot::Pose3DMotors::setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Pose3DMotors__setPose3DMotorsData_name, ::Ice::Normal, __context);
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

::jderobot::Pose3DMotorsDataPtr
IceDelegateM::jderobot::Pose3DMotors::getPose3DMotorsData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Pose3DMotors__getPose3DMotorsData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::Pose3DMotorsDataPtr __ret;
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
        __is->read(::jderobot::__patch__Pose3DMotorsDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::Pose3DMotorsParamsPtr
IceDelegateM::jderobot::Pose3DMotors::getPose3DMotorsParams(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__Pose3DMotors__getPose3DMotorsParams_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::Pose3DMotorsParamsPtr __ret;
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
        __is->read(::jderobot::__patch__Pose3DMotorsParamsPtr, &__ret);
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
IceDelegateD::jderobot::Pose3DMotors::setPose3DMotorsData(const ::jderobot::Pose3DMotorsDataPtr& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::jderobot::Pose3DMotorsDataPtr& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Pose3DMotors* servant = dynamic_cast< ::jderobot::Pose3DMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->setPose3DMotorsData(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::jderobot::Pose3DMotorsDataPtr& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Pose3DMotors__setPose3DMotorsData_name, ::Ice::Normal, __context);
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

::jderobot::Pose3DMotorsDataPtr
IceDelegateD::jderobot::Pose3DMotors::getPose3DMotorsData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::Pose3DMotorsDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Pose3DMotors* servant = dynamic_cast< ::jderobot::Pose3DMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPose3DMotorsData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::Pose3DMotorsDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Pose3DMotors__getPose3DMotorsData_name, ::Ice::Idempotent, __context);
    ::jderobot::Pose3DMotorsDataPtr __result;
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

::jderobot::Pose3DMotorsParamsPtr
IceDelegateD::jderobot::Pose3DMotors::getPose3DMotorsParams(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::Pose3DMotorsParamsPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::Pose3DMotors* servant = dynamic_cast< ::jderobot::Pose3DMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPose3DMotorsParams(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::Pose3DMotorsParamsPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__Pose3DMotors__getPose3DMotorsParams_name, ::Ice::Idempotent, __context);
    ::jderobot::Pose3DMotorsParamsPtr __result;
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

jderobot::Pose3DMotorsData::Pose3DMotorsData(::Ice::Float __ice_x, ::Ice::Float __ice_y, ::Ice::Float __ice_z, ::Ice::Float __ice_pan, ::Ice::Float __ice_tilt, ::Ice::Float __ice_roll, ::Ice::Float __ice_panSpeed, ::Ice::Float __ice_tiltSpeed) :
    x(__ice_x),
    y(__ice_y),
    z(__ice_z),
    pan(__ice_pan),
    tilt(__ice_tilt),
    roll(__ice_roll),
    panSpeed(__ice_panSpeed),
    tiltSpeed(__ice_tiltSpeed)
{
}

::Ice::ObjectPtr
jderobot::Pose3DMotorsData::ice_clone() const
{
    ::jderobot::Pose3DMotorsDataPtr __p = new ::jderobot::Pose3DMotorsData(*this);
    return __p;
}

static const ::std::string __jderobot__Pose3DMotorsData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Pose3DMotorsData"
};

bool
jderobot::Pose3DMotorsData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Pose3DMotorsData_ids, __jderobot__Pose3DMotorsData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Pose3DMotorsData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Pose3DMotorsData_ids[0], &__jderobot__Pose3DMotorsData_ids[2]);
}

const ::std::string&
jderobot::Pose3DMotorsData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Pose3DMotorsData_ids[1];
}

const ::std::string&
jderobot::Pose3DMotorsData::ice_staticId()
{
    return __jderobot__Pose3DMotorsData_ids[1];
}

void
jderobot::Pose3DMotorsData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(x);
    __os->write(y);
    __os->write(z);
    __os->write(pan);
    __os->write(tilt);
    __os->write(roll);
    __os->write(panSpeed);
    __os->write(tiltSpeed);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::Pose3DMotorsData::__read(::IceInternal::BasicStream* __is, bool __rid)
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
    __is->read(panSpeed);
    __is->read(tiltSpeed);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::Pose3DMotorsData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotorsData was not generated with stream support";
    throw ex;
}

void
jderobot::Pose3DMotorsData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotorsData was not generated with stream support";
    throw ex;
}

class __F__jderobot__Pose3DMotorsData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::Pose3DMotorsData::ice_staticId());
        return new ::jderobot::Pose3DMotorsData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__Pose3DMotorsData_Ptr = new __F__jderobot__Pose3DMotorsData;

const ::Ice::ObjectFactoryPtr&
jderobot::Pose3DMotorsData::ice_factory()
{
    return __F__jderobot__Pose3DMotorsData_Ptr;
}

class __F__jderobot__Pose3DMotorsData__Init
{
public:

    __F__jderobot__Pose3DMotorsData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::Pose3DMotorsData::ice_staticId(), ::jderobot::Pose3DMotorsData::ice_factory());
    }

    ~__F__jderobot__Pose3DMotorsData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::Pose3DMotorsData::ice_staticId());
    }
};

static __F__jderobot__Pose3DMotorsData__Init __F__jderobot__Pose3DMotorsData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__Pose3DMotorsData__initializer() {} }
#endif

void 
jderobot::__patch__Pose3DMotorsDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::Pose3DMotorsDataPtr* p = static_cast< ::jderobot::Pose3DMotorsDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::Pose3DMotorsDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Pose3DMotorsData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Pose3DMotorsData& l, const ::jderobot::Pose3DMotorsData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Pose3DMotorsData& l, const ::jderobot::Pose3DMotorsData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

jderobot::Pose3DMotorsParams::Pose3DMotorsParams(::Ice::Float __ice_maxPan, ::Ice::Float __ice_minPan, ::Ice::Float __ice_maxTilt, ::Ice::Float __ice_minTilt, ::Ice::Float __ice_maxPanSpeed, ::Ice::Float __ice_maxTiltSpeed) :
    maxPan(__ice_maxPan),
    minPan(__ice_minPan),
    maxTilt(__ice_maxTilt),
    minTilt(__ice_minTilt),
    maxPanSpeed(__ice_maxPanSpeed),
    maxTiltSpeed(__ice_maxTiltSpeed)
{
}

::Ice::ObjectPtr
jderobot::Pose3DMotorsParams::ice_clone() const
{
    ::jderobot::Pose3DMotorsParamsPtr __p = new ::jderobot::Pose3DMotorsParams(*this);
    return __p;
}

static const ::std::string __jderobot__Pose3DMotorsParams_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Pose3DMotorsParams"
};

bool
jderobot::Pose3DMotorsParams::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Pose3DMotorsParams_ids, __jderobot__Pose3DMotorsParams_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Pose3DMotorsParams::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Pose3DMotorsParams_ids[0], &__jderobot__Pose3DMotorsParams_ids[2]);
}

const ::std::string&
jderobot::Pose3DMotorsParams::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Pose3DMotorsParams_ids[1];
}

const ::std::string&
jderobot::Pose3DMotorsParams::ice_staticId()
{
    return __jderobot__Pose3DMotorsParams_ids[1];
}

void
jderobot::Pose3DMotorsParams::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(maxPan);
    __os->write(minPan);
    __os->write(maxTilt);
    __os->write(minTilt);
    __os->write(maxPanSpeed);
    __os->write(maxTiltSpeed);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::Pose3DMotorsParams::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(maxPan);
    __is->read(minPan);
    __is->read(maxTilt);
    __is->read(minTilt);
    __is->read(maxPanSpeed);
    __is->read(maxTiltSpeed);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::Pose3DMotorsParams::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotorsParams was not generated with stream support";
    throw ex;
}

void
jderobot::Pose3DMotorsParams::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotorsParams was not generated with stream support";
    throw ex;
}

class __F__jderobot__Pose3DMotorsParams : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::Pose3DMotorsParams::ice_staticId());
        return new ::jderobot::Pose3DMotorsParams;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__Pose3DMotorsParams_Ptr = new __F__jderobot__Pose3DMotorsParams;

const ::Ice::ObjectFactoryPtr&
jderobot::Pose3DMotorsParams::ice_factory()
{
    return __F__jderobot__Pose3DMotorsParams_Ptr;
}

class __F__jderobot__Pose3DMotorsParams__Init
{
public:

    __F__jderobot__Pose3DMotorsParams__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::Pose3DMotorsParams::ice_staticId(), ::jderobot::Pose3DMotorsParams::ice_factory());
    }

    ~__F__jderobot__Pose3DMotorsParams__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::Pose3DMotorsParams::ice_staticId());
    }
};

static __F__jderobot__Pose3DMotorsParams__Init __F__jderobot__Pose3DMotorsParams__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__Pose3DMotorsParams__initializer() {} }
#endif

void 
jderobot::__patch__Pose3DMotorsParamsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::Pose3DMotorsParamsPtr* p = static_cast< ::jderobot::Pose3DMotorsParamsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::Pose3DMotorsParamsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Pose3DMotorsParams::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Pose3DMotorsParams& l, const ::jderobot::Pose3DMotorsParams& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Pose3DMotorsParams& l, const ::jderobot::Pose3DMotorsParams& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::Pose3DMotors::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__Pose3DMotors_ids[2] =
{
    "::Ice::Object",
    "::jderobot::Pose3DMotors"
};

bool
jderobot::Pose3DMotors::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__Pose3DMotors_ids, __jderobot__Pose3DMotors_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::Pose3DMotors::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__Pose3DMotors_ids[0], &__jderobot__Pose3DMotors_ids[2]);
}

const ::std::string&
jderobot::Pose3DMotors::ice_id(const ::Ice::Current&) const
{
    return __jderobot__Pose3DMotors_ids[1];
}

const ::std::string&
jderobot::Pose3DMotors::ice_staticId()
{
    return __jderobot__Pose3DMotors_ids[1];
}

::Ice::DispatchStatus
jderobot::Pose3DMotors::___setPose3DMotorsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::Pose3DMotorsDataPtr data;
    __is->read(::jderobot::__patch__Pose3DMotorsDataPtr, &data);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setPose3DMotorsData(data, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::Pose3DMotors::___getPose3DMotorsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::Pose3DMotorsDataPtr __ret = getPose3DMotorsData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::Pose3DMotors::___getPose3DMotorsParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::Pose3DMotorsParamsPtr __ret = getPose3DMotorsParams(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__Pose3DMotors_all[] =
{
    "getPose3DMotorsData",
    "getPose3DMotorsParams",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setPose3DMotorsData"
};

::Ice::DispatchStatus
jderobot::Pose3DMotors::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__Pose3DMotors_all, __jderobot__Pose3DMotors_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__Pose3DMotors_all)
    {
        case 0:
        {
            return ___getPose3DMotorsData(in, current);
        }
        case 1:
        {
            return ___getPose3DMotorsParams(in, current);
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
            return ___setPose3DMotorsData(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::Pose3DMotors::__write(::IceInternal::BasicStream* __os) const
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
jderobot::Pose3DMotors::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::Pose3DMotors::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotors was not generated with stream support";
    throw ex;
}

void
jderobot::Pose3DMotors::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::Pose3DMotors was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__Pose3DMotorsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::Pose3DMotorsPtr* p = static_cast< ::jderobot::Pose3DMotorsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::Pose3DMotorsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::Pose3DMotors::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::Pose3DMotors& l, const ::jderobot::Pose3DMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::Pose3DMotors& l, const ::jderobot::Pose3DMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
