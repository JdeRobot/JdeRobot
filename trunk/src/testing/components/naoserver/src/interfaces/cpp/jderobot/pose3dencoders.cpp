// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `pose3dencoders.ice'

#include <pose3dencoders.h>
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
    ::Ice::Object::__write(__os);
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
    ::Ice::Object::__read(__is, true);
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

#ifndef ICEE_PURE_CLIENT
::Ice::DispatchStatus
jderobot::Pose3DEncoders::___getPose3DEncodersData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::Pose3DEncodersDataPtr __ret = getPose3DEncodersData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}
#endif // ICEE_PURE_CLIENT

#ifndef ICEE_PURE_CLIENT
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
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
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
    throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}
#endif // ICEE_PURE_CLIENT

void
jderobot::Pose3DEncoders::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::Ice::Object::__write(__os);
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
    ::Ice::Object::__read(__is, true);
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

const ::std::string&
IceProxy::jderobot::Pose3DEncodersData::ice_staticId()
{
    return __jderobot__Pose3DEncodersData_ids[1];
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
        ::IceInternal::RequestHandlerPtr __handler;
        try
        {
            __checkTwowayOnly(__jderobot__Pose3DEncoders__getPose3DEncodersData_name);
            __handler = __getRequestHandler();
            ::IceInternal::Outgoing __outS(__handler.get(), _reference.get(), __jderobot__Pose3DEncoders__getPose3DEncodersData_name, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                if(!__ok)
                {
                    __outS.is()->throwUnknownUserException();
                }
                ::jderobot::Pose3DEncodersDataPtr __ret;
                ::IceInternal::BasicStream* __is = __outS.is();
                __is->read(::jderobot::__patch__Pose3DEncodersDataPtr, &__ret);
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
IceProxy::jderobot::Pose3DEncoders::ice_staticId()
{
    return __jderobot__Pose3DEncoders_ids[1];
}

::IceProxy::Ice::Object*
IceProxy::jderobot::Pose3DEncoders::__newInstance() const
{
    return new Pose3DEncoders;
}
