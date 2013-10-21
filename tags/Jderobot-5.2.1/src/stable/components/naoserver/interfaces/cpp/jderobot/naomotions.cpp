// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `naomotions.ice'

#include <naomotions.h>
#include <IceE/LocalException.h>
#include <IceE/ObjectFactory.h>
#include <IceE/BasicStream.h>
#include <IceE/Iterator.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

static const ::std::string __jderobot__NaoMotions__setMotion_name = "setMotion";

::Ice::Object* IceInternal::upCast(::jderobot::NaoMotions* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::NaoMotions* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::NaoMotionsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::NaoMotions;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::MotionType v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 7);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::MotionType& v)
{
    ::Ice::Byte val;
    __is->read(val, 7);
    v = static_cast< ::jderobot::MotionType>(val);
}

static const ::std::string __jderobot__NaoMotions_ids[2] =
{
    "::Ice::Object",
    "::jderobot::NaoMotions"
};

bool
jderobot::NaoMotions::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__NaoMotions_ids, __jderobot__NaoMotions_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::NaoMotions::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__NaoMotions_ids[0], &__jderobot__NaoMotions_ids[2]);
}

const ::std::string&
jderobot::NaoMotions::ice_id(const ::Ice::Current&) const
{
    return __jderobot__NaoMotions_ids[1];
}

const ::std::string&
jderobot::NaoMotions::ice_staticId()
{
    return __jderobot__NaoMotions_ids[1];
}

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::NaoMotions::___setMotion(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::jderobot::MotionType motion;
    ::jderobot::__read(__is, motion);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = setMotion(motion, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
static ::std::string __jderobot__NaoMotions_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setMotion"
};

::Ice::DispatchStatus
jderobot::NaoMotions::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__NaoMotions_all, __jderobot__NaoMotions_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__NaoMotions_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___setMotion(in, current);
        }
    }

    assert(false);
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::NaoMotions::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
}

void
jderobot::NaoMotions::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::Ice::Object::__read(__is, true);
}


bool
jderobot::operator==(const ::jderobot::NaoMotions& l, const ::jderobot::NaoMotions& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::NaoMotions& l, const ::jderobot::NaoMotions& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

void 
jderobot::__patch__NaoMotionsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::NaoMotionsPtr* p = static_cast< ::jderobot::NaoMotionsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::NaoMotionsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::NaoMotions::ice_staticId(), v->ice_id());
    }
}

::Ice::Int
IceProxy::jderobot::NaoMotions::setMotion(::jderobot::MotionType motion, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__NaoMotions__setMotion_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__NaoMotions__setMotion_name, ::Ice::Normal, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.os();
                ::jderobot::__write(__os, motion);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::Ice::Int __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(__ret);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__handler, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__handler, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

const ::std::string&
IceProxy::jderobot::NaoMotions::ice_staticId()
{
    return __jderobot__NaoMotions_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::NaoMotions::__newInstance() const
{
    return new NaoMotions;
}
