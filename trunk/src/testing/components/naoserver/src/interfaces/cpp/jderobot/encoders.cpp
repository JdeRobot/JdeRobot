// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `encoders.ice'

#include <encoders.h>
#include <IceE/LocalException.h>
#include <IceE/ObjectFactory.h>
#include <IceE/BasicStream.h>
#include <IceE/LocalException.h>
#include <IceE/Iterator.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
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

jderobot::EncodersData::EncodersData(::Ice::Float __ice_robotx, ::Ice::Float __ice_roboty, ::Ice::Float __ice_robottheta, ::Ice::Float __ice_robotcos, ::Ice::Float __ice_robotsin) :
    robotx(__ice_robotx),
    roboty(__ice_roboty),
    robottheta(__ice_robottheta),
    robotcos(__ice_robotcos),
    robotsin(__ice_robotsin)
{
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
    ::Ice::Object::__write(__os);
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
    ::Ice::Object::__read(__is, true);
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

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Encoders::___getEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::EncodersDataPtr __ret = getEncodersData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
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
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
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
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::Encoders::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
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
    ::Ice::Object::__read(__is, true);
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

const ::std::string&
IceProxy::jderobot::EncodersData::ice_staticId()
{
    return __jderobot__EncodersData_ids[1];
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
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__Encoders__getEncodersData_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Encoders__getEncodersData_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::EncodersDataPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__EncodersDataPtr, &__ret);
                __is->readPendingObjects();
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
            __handleExceptionWrapperRelaxed(__handler, __ex, __cnt);
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
IceProxy::jderobot::Encoders::ice_staticId()
{
    return __jderobot__Encoders_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Encoders::__newInstance() const
{
    return new Encoders;
}
