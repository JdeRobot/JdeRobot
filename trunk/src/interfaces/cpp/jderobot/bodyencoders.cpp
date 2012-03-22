// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodyencoders.ice'

#include <bodyencoders.h>
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

static const ::std::string __jderobot__BodyEncoders__getArmEncodersData_name = "getArmEncodersData";

static const ::std::string __jderobot__BodyEncoders__getLegEncodersData_name = "getLegEncodersData";

static const ::std::string __jderobot__BodyEncoders__getOdometryData_name = "getOdometryData";

::Ice::Object* IceInternal::upCast(::jderobot::ArmEncodersData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::ArmEncodersData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::LegEncodersData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::LegEncodersData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::OdometryData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::OdometryData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::BodyEncoders* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::BodyEncoders* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::ArmEncodersDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::ArmEncodersData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::LegEncodersDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::LegEncodersData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::OdometryDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::OdometryData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodyEncodersPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::BodyEncoders;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::ArmEncodersData::ice_staticId()
{
    return ::jderobot::ArmEncodersData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::ArmEncodersData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::ArmEncodersData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::ArmEncodersData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::ArmEncodersData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::ArmEncodersData::__newInstance() const
{
    return new ArmEncodersData;
}

const ::std::string&
IceProxy::jderobot::LegEncodersData::ice_staticId()
{
    return ::jderobot::LegEncodersData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::LegEncodersData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::LegEncodersData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::LegEncodersData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::LegEncodersData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::LegEncodersData::__newInstance() const
{
    return new LegEncodersData;
}

const ::std::string&
IceProxy::jderobot::OdometryData::ice_staticId()
{
    return ::jderobot::OdometryData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::OdometryData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::OdometryData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::OdometryData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::OdometryData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::OdometryData::__newInstance() const
{
    return new OdometryData;
}

::jderobot::ArmEncodersDataPtr
IceProxy::jderobot::BodyEncoders::getArmEncodersData(::jderobot::BodySide side, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyEncoders__getArmEncodersData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyEncoders* __del = dynamic_cast< ::IceDelegate::jderobot::BodyEncoders*>(__delBase.get());
            return __del->getArmEncodersData(side, __ctx);
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

::jderobot::LegEncodersDataPtr
IceProxy::jderobot::BodyEncoders::getLegEncodersData(::jderobot::BodySide side, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyEncoders__getLegEncodersData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyEncoders* __del = dynamic_cast< ::IceDelegate::jderobot::BodyEncoders*>(__delBase.get());
            return __del->getLegEncodersData(side, __ctx);
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

::jderobot::OdometryDataPtr
IceProxy::jderobot::BodyEncoders::getOdometryData(::jderobot::CameraBody camera, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyEncoders__getOdometryData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyEncoders* __del = dynamic_cast< ::IceDelegate::jderobot::BodyEncoders*>(__delBase.get());
            return __del->getOdometryData(camera, __ctx);
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
IceProxy::jderobot::BodyEncoders::ice_staticId()
{
    return ::jderobot::BodyEncoders::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::BodyEncoders::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::BodyEncoders);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::BodyEncoders::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::BodyEncoders);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::BodyEncoders::__newInstance() const
{
    return new BodyEncoders;
}

::jderobot::ArmEncodersDataPtr
IceDelegateM::jderobot::BodyEncoders::getArmEncodersData(::jderobot::BodySide side, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyEncoders__getArmEncodersData_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, side);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::jderobot::ArmEncodersDataPtr __ret;
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
        __is->read(::jderobot::__patch__ArmEncodersDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::LegEncodersDataPtr
IceDelegateM::jderobot::BodyEncoders::getLegEncodersData(::jderobot::BodySide side, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyEncoders__getLegEncodersData_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, side);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::jderobot::LegEncodersDataPtr __ret;
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
        __is->read(::jderobot::__patch__LegEncodersDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::OdometryDataPtr
IceDelegateM::jderobot::BodyEncoders::getOdometryData(::jderobot::CameraBody camera, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyEncoders__getOdometryData_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, camera);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::jderobot::OdometryDataPtr __ret;
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
        __is->read(::jderobot::__patch__OdometryDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::ArmEncodersDataPtr
IceDelegateD::jderobot::BodyEncoders::getArmEncodersData(::jderobot::BodySide side, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::ArmEncodersDataPtr& __result, ::jderobot::BodySide side, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_side(side)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyEncoders* servant = dynamic_cast< ::jderobot::BodyEncoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getArmEncodersData(_m_side, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::ArmEncodersDataPtr& _result;
        ::jderobot::BodySide _m_side;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyEncoders__getArmEncodersData_name, ::Ice::Idempotent, __context);
    ::jderobot::ArmEncodersDataPtr __result;
    try
    {
        _DirectI __direct(__result, side, __current);
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

::jderobot::LegEncodersDataPtr
IceDelegateD::jderobot::BodyEncoders::getLegEncodersData(::jderobot::BodySide side, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::LegEncodersDataPtr& __result, ::jderobot::BodySide side, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_side(side)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyEncoders* servant = dynamic_cast< ::jderobot::BodyEncoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getLegEncodersData(_m_side, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::LegEncodersDataPtr& _result;
        ::jderobot::BodySide _m_side;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyEncoders__getLegEncodersData_name, ::Ice::Idempotent, __context);
    ::jderobot::LegEncodersDataPtr __result;
    try
    {
        _DirectI __direct(__result, side, __current);
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

::jderobot::OdometryDataPtr
IceDelegateD::jderobot::BodyEncoders::getOdometryData(::jderobot::CameraBody camera, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::OdometryDataPtr& __result, ::jderobot::CameraBody camera, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_camera(camera)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyEncoders* servant = dynamic_cast< ::jderobot::BodyEncoders*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getOdometryData(_m_camera, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::OdometryDataPtr& _result;
        ::jderobot::CameraBody _m_camera;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyEncoders__getOdometryData_name, ::Ice::Idempotent, __context);
    ::jderobot::OdometryDataPtr __result;
    try
    {
        _DirectI __direct(__result, camera, __current);
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

jderobot::ArmEncodersData::ArmEncodersData(const ::jderobot::BodyMotor& __ice_shoulder, const ::jderobot::BodyMotor& __ice_elbow, ::Ice::Int __ice_clock) :
    shoulder(__ice_shoulder),
    elbow(__ice_elbow),
    clock(__ice_clock)
{
}

::Ice::ObjectPtr
jderobot::ArmEncodersData::ice_clone() const
{
    ::jderobot::ArmEncodersDataPtr __p = new ::jderobot::ArmEncodersData(*this);
    return __p;
}

static const ::std::string __jderobot__ArmEncodersData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::ArmEncodersData"
};

bool
jderobot::ArmEncodersData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__ArmEncodersData_ids, __jderobot__ArmEncodersData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::ArmEncodersData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__ArmEncodersData_ids[0], &__jderobot__ArmEncodersData_ids[2]);
}

const ::std::string&
jderobot::ArmEncodersData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__ArmEncodersData_ids[1];
}

const ::std::string&
jderobot::ArmEncodersData::ice_staticId()
{
    return __jderobot__ArmEncodersData_ids[1];
}

void
jderobot::ArmEncodersData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    shoulder.__write(__os);
    elbow.__write(__os);
    __os->write(clock);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::ArmEncodersData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    shoulder.__read(__is);
    elbow.__read(__is);
    __is->read(clock);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::ArmEncodersData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ArmEncodersData was not generated with stream support";
    throw ex;
}

void
jderobot::ArmEncodersData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::ArmEncodersData was not generated with stream support";
    throw ex;
}

class __F__jderobot__ArmEncodersData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::ArmEncodersData::ice_staticId());
        return new ::jderobot::ArmEncodersData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__ArmEncodersData_Ptr = new __F__jderobot__ArmEncodersData;

const ::Ice::ObjectFactoryPtr&
jderobot::ArmEncodersData::ice_factory()
{
    return __F__jderobot__ArmEncodersData_Ptr;
}

class __F__jderobot__ArmEncodersData__Init
{
public:

    __F__jderobot__ArmEncodersData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::ArmEncodersData::ice_staticId(), ::jderobot::ArmEncodersData::ice_factory());
    }

    ~__F__jderobot__ArmEncodersData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::ArmEncodersData::ice_staticId());
    }
};

static __F__jderobot__ArmEncodersData__Init __F__jderobot__ArmEncodersData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__ArmEncodersData__initializer() {} }
#endif

void 
jderobot::__patch__ArmEncodersDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::ArmEncodersDataPtr* p = static_cast< ::jderobot::ArmEncodersDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::ArmEncodersDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::ArmEncodersData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::ArmEncodersData& l, const ::jderobot::ArmEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::ArmEncodersData& l, const ::jderobot::ArmEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

jderobot::LegEncodersData::LegEncodersData(const ::jderobot::BodyMotor& __ice_hip, const ::jderobot::BodyMotor& __ice_knee, const ::jderobot::BodyMotor& __ice_ankle, ::Ice::Int __ice_clock) :
    hip(__ice_hip),
    knee(__ice_knee),
    ankle(__ice_ankle),
    clock(__ice_clock)
{
}

::Ice::ObjectPtr
jderobot::LegEncodersData::ice_clone() const
{
    ::jderobot::LegEncodersDataPtr __p = new ::jderobot::LegEncodersData(*this);
    return __p;
}

static const ::std::string __jderobot__LegEncodersData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::LegEncodersData"
};

bool
jderobot::LegEncodersData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__LegEncodersData_ids, __jderobot__LegEncodersData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::LegEncodersData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__LegEncodersData_ids[0], &__jderobot__LegEncodersData_ids[2]);
}

const ::std::string&
jderobot::LegEncodersData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__LegEncodersData_ids[1];
}

const ::std::string&
jderobot::LegEncodersData::ice_staticId()
{
    return __jderobot__LegEncodersData_ids[1];
}

void
jderobot::LegEncodersData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    hip.__write(__os);
    knee.__write(__os);
    ankle.__write(__os);
    __os->write(clock);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::LegEncodersData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    hip.__read(__is);
    knee.__read(__is);
    ankle.__read(__is);
    __is->read(clock);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::LegEncodersData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::LegEncodersData was not generated with stream support";
    throw ex;
}

void
jderobot::LegEncodersData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::LegEncodersData was not generated with stream support";
    throw ex;
}

class __F__jderobot__LegEncodersData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::LegEncodersData::ice_staticId());
        return new ::jderobot::LegEncodersData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__LegEncodersData_Ptr = new __F__jderobot__LegEncodersData;

const ::Ice::ObjectFactoryPtr&
jderobot::LegEncodersData::ice_factory()
{
    return __F__jderobot__LegEncodersData_Ptr;
}

class __F__jderobot__LegEncodersData__Init
{
public:

    __F__jderobot__LegEncodersData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::LegEncodersData::ice_staticId(), ::jderobot::LegEncodersData::ice_factory());
    }

    ~__F__jderobot__LegEncodersData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::LegEncodersData::ice_staticId());
    }
};

static __F__jderobot__LegEncodersData__Init __F__jderobot__LegEncodersData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__LegEncodersData__initializer() {} }
#endif

void 
jderobot::__patch__LegEncodersDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::LegEncodersDataPtr* p = static_cast< ::jderobot::LegEncodersDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::LegEncodersDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::LegEncodersData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::LegEncodersData& l, const ::jderobot::LegEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::LegEncodersData& l, const ::jderobot::LegEncodersData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

jderobot::OdometryData::OdometryData(const ::jderobot::seqFloat& __ice_odometry) :
    odometry(__ice_odometry)
{
}

::Ice::ObjectPtr
jderobot::OdometryData::ice_clone() const
{
    ::jderobot::OdometryDataPtr __p = new ::jderobot::OdometryData(*this);
    return __p;
}

static const ::std::string __jderobot__OdometryData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::OdometryData"
};

bool
jderobot::OdometryData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__OdometryData_ids, __jderobot__OdometryData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::OdometryData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__OdometryData_ids[0], &__jderobot__OdometryData_ids[2]);
}

const ::std::string&
jderobot::OdometryData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__OdometryData_ids[1];
}

const ::std::string&
jderobot::OdometryData::ice_staticId()
{
    return __jderobot__OdometryData_ids[1];
}

void
jderobot::OdometryData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(odometry.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&odometry[0], &odometry[0] + odometry.size());
    }
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::OdometryData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(odometry);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::OdometryData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::OdometryData was not generated with stream support";
    throw ex;
}

void
jderobot::OdometryData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::OdometryData was not generated with stream support";
    throw ex;
}

class __F__jderobot__OdometryData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::OdometryData::ice_staticId());
        return new ::jderobot::OdometryData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__OdometryData_Ptr = new __F__jderobot__OdometryData;

const ::Ice::ObjectFactoryPtr&
jderobot::OdometryData::ice_factory()
{
    return __F__jderobot__OdometryData_Ptr;
}

class __F__jderobot__OdometryData__Init
{
public:

    __F__jderobot__OdometryData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::OdometryData::ice_staticId(), ::jderobot::OdometryData::ice_factory());
    }

    ~__F__jderobot__OdometryData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::OdometryData::ice_staticId());
    }
};

static __F__jderobot__OdometryData__Init __F__jderobot__OdometryData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__OdometryData__initializer() {} }
#endif

void 
jderobot::__patch__OdometryDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::OdometryDataPtr* p = static_cast< ::jderobot::OdometryDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::OdometryDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::OdometryData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::OdometryData& l, const ::jderobot::OdometryData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::OdometryData& l, const ::jderobot::OdometryData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::BodyEncoders::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__BodyEncoders_ids[2] =
{
    "::Ice::Object",
    "::jderobot::BodyEncoders"
};

bool
jderobot::BodyEncoders::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__BodyEncoders_ids, __jderobot__BodyEncoders_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::BodyEncoders::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__BodyEncoders_ids[0], &__jderobot__BodyEncoders_ids[2]);
}

const ::std::string&
jderobot::BodyEncoders::ice_id(const ::Ice::Current&) const
{
    return __jderobot__BodyEncoders_ids[1];
}

const ::std::string&
jderobot::BodyEncoders::ice_staticId()
{
    return __jderobot__BodyEncoders_ids[1];
}

::Ice::DispatchStatus
jderobot::BodyEncoders::___getArmEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::BodySide side;
    ::jderobot::__read(__is, side);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::ArmEncodersDataPtr __ret = getArmEncodersData(side, __current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::BodyEncoders::___getLegEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::BodySide side;
    ::jderobot::__read(__is, side);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::LegEncodersDataPtr __ret = getLegEncodersData(side, __current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::BodyEncoders::___getOdometryData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::CameraBody camera;
    ::jderobot::__read(__is, camera);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::OdometryDataPtr __ret = getOdometryData(camera, __current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__BodyEncoders_all[] =
{
    "getArmEncodersData",
    "getLegEncodersData",
    "getOdometryData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::BodyEncoders::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__BodyEncoders_all, __jderobot__BodyEncoders_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__BodyEncoders_all)
    {
        case 0:
        {
            return ___getArmEncodersData(in, current);
        }
        case 1:
        {
            return ___getLegEncodersData(in, current);
        }
        case 2:
        {
            return ___getOdometryData(in, current);
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
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::BodyEncoders::__write(::IceInternal::BasicStream* __os) const
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
jderobot::BodyEncoders::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::BodyEncoders::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyEncoders was not generated with stream support";
    throw ex;
}

void
jderobot::BodyEncoders::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyEncoders was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__BodyEncodersPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::BodyEncodersPtr* p = static_cast< ::jderobot::BodyEncodersPtr*>(__addr);
    assert(p);
    *p = ::jderobot::BodyEncodersPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::BodyEncoders::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::BodyEncoders& l, const ::jderobot::BodyEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::BodyEncoders& l, const ::jderobot::BodyEncoders& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
