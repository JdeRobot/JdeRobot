// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodymotors.ice'

#include <bodymotors.h>
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

static const ::std::string __jderobot__BodyMotors__getBodyMotorsParam_name = "getBodyMotorsParam";

static const ::std::string __jderobot__BodyMotors__setBodyMotorsData_name = "setBodyMotorsData";

::Ice::Object* IceInternal::upCast(::jderobot::BodyMotorsParam* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::BodyMotorsParam* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::BodyMotors* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::BodyMotors* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodyMotorsParamPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::BodyMotorsParam;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodyMotorsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::BodyMotors;
        v->__copyFrom(proxy);
    }
}

const ::std::string&
IceProxy::jderobot::BodyMotorsParam::ice_staticId()
{
    return ::jderobot::BodyMotorsParam::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::BodyMotorsParam::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::BodyMotorsParam);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::BodyMotorsParam::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::BodyMotorsParam);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::BodyMotorsParam::__newInstance() const
{
    return new BodyMotorsParam;
}

::jderobot::BodyMotorsParamPtr
IceProxy::jderobot::BodyMotors::getBodyMotorsParam(::jderobot::MotorsName name, ::jderobot::BodySide side, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyMotors__getBodyMotorsParam_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyMotors* __del = dynamic_cast< ::IceDelegate::jderobot::BodyMotors*>(__delBase.get());
            return __del->getBodyMotorsParam(name, side, __ctx);
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

::Ice::Int
IceProxy::jderobot::BodyMotors::setBodyMotorsData(::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyMotors__setBodyMotorsData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyMotors* __del = dynamic_cast< ::IceDelegate::jderobot::BodyMotors*>(__delBase.get());
            return __del->setBodyMotorsData(name, side, angle, speed, __ctx);
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
IceProxy::jderobot::BodyMotors::ice_staticId()
{
    return ::jderobot::BodyMotors::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::BodyMotors::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::BodyMotors);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::BodyMotors::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::BodyMotors);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::BodyMotors::__newInstance() const
{
    return new BodyMotors;
}

::jderobot::BodyMotorsParamPtr
IceDelegateM::jderobot::BodyMotors::getBodyMotorsParam(::jderobot::MotorsName name, ::jderobot::BodySide side, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyMotors__getBodyMotorsParam_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, name);
        ::jderobot::__write(__os, side);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::jderobot::BodyMotorsParamPtr __ret;
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
        __is->read(::jderobot::__patch__BodyMotorsParamPtr, &__ret);
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
IceDelegateM::jderobot::BodyMotors::setBodyMotorsData(::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyMotors__setBodyMotorsData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, name);
        ::jderobot::__write(__os, side);
        __os->write(angle);
        __os->write(speed);
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

::jderobot::BodyMotorsParamPtr
IceDelegateD::jderobot::BodyMotors::getBodyMotorsParam(::jderobot::MotorsName name, ::jderobot::BodySide side, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::BodyMotorsParamPtr& __result, ::jderobot::MotorsName name, ::jderobot::BodySide side, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_name(name),
            _m_side(side)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyMotors* servant = dynamic_cast< ::jderobot::BodyMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getBodyMotorsParam(_m_name, _m_side, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::BodyMotorsParamPtr& _result;
        ::jderobot::MotorsName _m_name;
        ::jderobot::BodySide _m_side;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyMotors__getBodyMotorsParam_name, ::Ice::Idempotent, __context);
    ::jderobot::BodyMotorsParamPtr __result;
    try
    {
        _DirectI __direct(__result, name, side, __current);
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

::Ice::Int
IceDelegateD::jderobot::BodyMotors::setBodyMotorsData(::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, ::jderobot::MotorsName name, ::jderobot::BodySide side, ::Ice::Float angle, ::Ice::Float speed, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_name(name),
            _m_side(side),
            _m_angle(angle),
            _m_speed(speed)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyMotors* servant = dynamic_cast< ::jderobot::BodyMotors*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->setBodyMotorsData(_m_name, _m_side, _m_angle, _m_speed, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        ::jderobot::MotorsName _m_name;
        ::jderobot::BodySide _m_side;
        ::Ice::Float _m_angle;
        ::Ice::Float _m_speed;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyMotors__setBodyMotorsData_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, name, side, angle, speed, __current);
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

jderobot::BodyMotorsParam::BodyMotorsParam(::Ice::Float __ice_minAngle, ::Ice::Float __ice_maxAngle, ::Ice::Float __ice_maxSpeed) :
    minAngle(__ice_minAngle),
    maxAngle(__ice_maxAngle),
    maxSpeed(__ice_maxSpeed)
{
}

::Ice::ObjectPtr
jderobot::BodyMotorsParam::ice_clone() const
{
    ::jderobot::BodyMotorsParamPtr __p = new ::jderobot::BodyMotorsParam(*this);
    return __p;
}

static const ::std::string __jderobot__BodyMotorsParam_ids[2] =
{
    "::Ice::Object",
    "::jderobot::BodyMotorsParam"
};

bool
jderobot::BodyMotorsParam::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__BodyMotorsParam_ids, __jderobot__BodyMotorsParam_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::BodyMotorsParam::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__BodyMotorsParam_ids[0], &__jderobot__BodyMotorsParam_ids[2]);
}

const ::std::string&
jderobot::BodyMotorsParam::ice_id(const ::Ice::Current&) const
{
    return __jderobot__BodyMotorsParam_ids[1];
}

const ::std::string&
jderobot::BodyMotorsParam::ice_staticId()
{
    return __jderobot__BodyMotorsParam_ids[1];
}

void
jderobot::BodyMotorsParam::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(minAngle);
    __os->write(maxAngle);
    __os->write(maxSpeed);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::BodyMotorsParam::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(minAngle);
    __is->read(maxAngle);
    __is->read(maxSpeed);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::BodyMotorsParam::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMotorsParam was not generated with stream support";
    throw ex;
}

void
jderobot::BodyMotorsParam::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMotorsParam was not generated with stream support";
    throw ex;
}

class __F__jderobot__BodyMotorsParam : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::BodyMotorsParam::ice_staticId());
        return new ::jderobot::BodyMotorsParam;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__BodyMotorsParam_Ptr = new __F__jderobot__BodyMotorsParam;

const ::Ice::ObjectFactoryPtr&
jderobot::BodyMotorsParam::ice_factory()
{
    return __F__jderobot__BodyMotorsParam_Ptr;
}

class __F__jderobot__BodyMotorsParam__Init
{
public:

    __F__jderobot__BodyMotorsParam__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::BodyMotorsParam::ice_staticId(), ::jderobot::BodyMotorsParam::ice_factory());
    }

    ~__F__jderobot__BodyMotorsParam__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::BodyMotorsParam::ice_staticId());
    }
};

static __F__jderobot__BodyMotorsParam__Init __F__jderobot__BodyMotorsParam__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__BodyMotorsParam__initializer() {} }
#endif

void 
jderobot::__patch__BodyMotorsParamPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::BodyMotorsParamPtr* p = static_cast< ::jderobot::BodyMotorsParamPtr*>(__addr);
    assert(p);
    *p = ::jderobot::BodyMotorsParamPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::BodyMotorsParam::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::BodyMotorsParam& l, const ::jderobot::BodyMotorsParam& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::BodyMotorsParam& l, const ::jderobot::BodyMotorsParam& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::BodyMotors::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__BodyMotors_ids[2] =
{
    "::Ice::Object",
    "::jderobot::BodyMotors"
};

bool
jderobot::BodyMotors::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__BodyMotors_ids, __jderobot__BodyMotors_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::BodyMotors::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__BodyMotors_ids[0], &__jderobot__BodyMotors_ids[2]);
}

const ::std::string&
jderobot::BodyMotors::ice_id(const ::Ice::Current&) const
{
    return __jderobot__BodyMotors_ids[1];
}

const ::std::string&
jderobot::BodyMotors::ice_staticId()
{
    return __jderobot__BodyMotors_ids[1];
}

::Ice::DispatchStatus
jderobot::BodyMotors::___getBodyMotorsParam(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::MotorsName name;
    ::jderobot::BodySide side;
    ::jderobot::__read(__is, name);
    ::jderobot::__read(__is, side);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::BodyMotorsParamPtr __ret = getBodyMotorsParam(name, side, __current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::BodyMotors::___setBodyMotorsData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::MotorsName name;
    ::jderobot::BodySide side;
    ::Ice::Float angle;
    ::Ice::Float speed;
    ::jderobot::__read(__is, name);
    ::jderobot::__read(__is, side);
    __is->read(angle);
    __is->read(speed);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setBodyMotorsData(name, side, angle, speed, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__BodyMotors_all[] =
{
    "getBodyMotorsParam",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setBodyMotorsData"
};

::Ice::DispatchStatus
jderobot::BodyMotors::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__BodyMotors_all, __jderobot__BodyMotors_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__BodyMotors_all)
    {
        case 0:
        {
            return ___getBodyMotorsParam(in, current);
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
        case 5:
        {
            return ___setBodyMotorsData(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::BodyMotors::__write(::IceInternal::BasicStream* __os) const
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
jderobot::BodyMotors::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::BodyMotors::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMotors was not generated with stream support";
    throw ex;
}

void
jderobot::BodyMotors::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMotors was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__BodyMotorsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::BodyMotorsPtr* p = static_cast< ::jderobot::BodyMotorsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::BodyMotorsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::BodyMotors::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::BodyMotors& l, const ::jderobot::BodyMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::BodyMotors& l, const ::jderobot::BodyMotors& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
