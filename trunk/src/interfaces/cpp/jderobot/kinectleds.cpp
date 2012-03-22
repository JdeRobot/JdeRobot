// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `kinectleds.ice'

#include <kinectleds.h>
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

static const ::std::string __jderobot__KinectLeds__setLedActive_name = "setLedActive";

::Ice::Object* IceInternal::upCast(::jderobot::KinectLeds* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::KinectLeds* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::KinectLedsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::KinectLeds;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::KinectLedsAvailable v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 6);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::KinectLedsAvailable& v)
{
    ::Ice::Byte val;
    __is->read(val, 6);
    v = static_cast< ::jderobot::KinectLedsAvailable>(val);
}

void
IceProxy::jderobot::KinectLeds::setLedActive(::jderobot::KinectLedsAvailable led, const ::Ice::Context* __ctx)
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
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::KinectLeds* __del = dynamic_cast< ::IceDelegate::jderobot::KinectLeds*>(__delBase.get());
            __del->setLedActive(led, __ctx);
            return;
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
IceProxy::jderobot::KinectLeds::ice_staticId()
{
    return ::jderobot::KinectLeds::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::KinectLeds::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::KinectLeds);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::KinectLeds::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::KinectLeds);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::KinectLeds::__newInstance() const
{
    return new KinectLeds;
}

void
IceDelegateM::jderobot::KinectLeds::setLedActive(::jderobot::KinectLedsAvailable led, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__KinectLeds__setLedActive_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::jderobot::__write(__os, led);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
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
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateD::jderobot::KinectLeds::setLedActive(::jderobot::KinectLedsAvailable led, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::KinectLedsAvailable led, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_led(led)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::KinectLeds* servant = dynamic_cast< ::jderobot::KinectLeds*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setLedActive(_m_led, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::KinectLedsAvailable _m_led;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__KinectLeds__setLedActive_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(led, __current);
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
}

::Ice::ObjectPtr
jderobot::KinectLeds::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__KinectLeds_ids[2] =
{
    "::Ice::Object",
    "::jderobot::KinectLeds"
};

bool
jderobot::KinectLeds::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__KinectLeds_ids, __jderobot__KinectLeds_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::KinectLeds::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__KinectLeds_ids[0], &__jderobot__KinectLeds_ids[2]);
}

const ::std::string&
jderobot::KinectLeds::ice_id(const ::Ice::Current&) const
{
    return __jderobot__KinectLeds_ids[1];
}

const ::std::string&
jderobot::KinectLeds::ice_staticId()
{
    return __jderobot__KinectLeds_ids[1];
}

::Ice::DispatchStatus
jderobot::KinectLeds::___setLedActive(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::KinectLedsAvailable led;
    ::jderobot::__read(__is, led);
    __is->endReadEncaps();
    setLedActive(led, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__KinectLeds_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setLedActive"
};

::Ice::DispatchStatus
jderobot::KinectLeds::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__KinectLeds_all, __jderobot__KinectLeds_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__KinectLeds_all)
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
            return ___setLedActive(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::KinectLeds::__write(::IceInternal::BasicStream* __os) const
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
jderobot::KinectLeds::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::KinectLeds::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::KinectLeds was not generated with stream support";
    throw ex;
}

void
jderobot::KinectLeds::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::KinectLeds was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__KinectLedsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::KinectLedsPtr* p = static_cast< ::jderobot::KinectLedsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::KinectLedsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::KinectLeds::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::KinectLeds& l, const ::jderobot::KinectLeds& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::KinectLeds& l, const ::jderobot::KinectLeds& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
