// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `kinect.ice'

#include "kinect.h"
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

static const ::std::string __jderobot__PuntosPCLInterface__getKinectData_name = "getKinectData";

::Ice::Object* IceInternal::upCast(::jderobot::PuntosPCLData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PuntosPCLData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::PuntosPCLInterface* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::PuntosPCLInterface* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PuntosPCLDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PuntosPCLData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::PuntosPCLInterfacePrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::PuntosPCLInterface;
        v->__copyFrom(proxy);
    }
}

bool
jderobot::Puntos::operator==(const Puntos& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(x != __rhs.x)
    {
        return false;
    }
    if(y != __rhs.y)
    {
        return false;
    }
    if(z != __rhs.z)
    {
        return false;
    }
    if(r != __rhs.r)
    {
        return false;
    }
    if(g != __rhs.g)
    {
        return false;
    }
    if(b != __rhs.b)
    {
        return false;
    }
    return true;
}

bool
jderobot::Puntos::operator<(const Puntos& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(x < __rhs.x)
    {
        return true;
    }
    else if(__rhs.x < x)
    {
        return false;
    }
    if(y < __rhs.y)
    {
        return true;
    }
    else if(__rhs.y < y)
    {
        return false;
    }
    if(z < __rhs.z)
    {
        return true;
    }
    else if(__rhs.z < z)
    {
        return false;
    }
    if(r < __rhs.r)
    {
        return true;
    }
    else if(__rhs.r < r)
    {
        return false;
    }
    if(g < __rhs.g)
    {
        return true;
    }
    else if(__rhs.g < g)
    {
        return false;
    }
    if(b < __rhs.b)
    {
        return true;
    }
    else if(__rhs.b < b)
    {
        return false;
    }
    return false;
}

void
jderobot::Puntos::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(x);
    __os->write(y);
    __os->write(z);
    __os->write(r);
    __os->write(g);
    __os->write(b);
}

void
jderobot::Puntos::__read(::IceInternal::BasicStream* __is)
{
    __is->read(x);
    __is->read(y);
    __is->read(z);
    __is->read(r);
    __is->read(g);
    __is->read(b);
}

void
jderobot::__writePuntosPCL(::IceInternal::BasicStream* __os, const ::jderobot::Puntos* begin, const ::jderobot::Puntos* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
jderobot::__readPuntosPCL(::IceInternal::BasicStream* __is, ::jderobot::PuntosPCL& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->checkFixedSeq(sz, 24);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
    }
}

const ::std::string&
IceProxy::jderobot::PuntosPCLData::ice_staticId()
{
    return ::jderobot::PuntosPCLData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PuntosPCLData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PuntosPCLData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PuntosPCLData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PuntosPCLData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PuntosPCLData::__newInstance() const
{
    return new PuntosPCLData;
}

::jderobot::PuntosPCLDataPtr
IceProxy::jderobot::PuntosPCLInterface::getKinectData(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__PuntosPCLInterface__getKinectData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::PuntosPCLInterface* __del = dynamic_cast< ::IceDelegate::jderobot::PuntosPCLInterface*>(__delBase.get());
            return __del->getKinectData(__ctx);
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
IceProxy::jderobot::PuntosPCLInterface::ice_staticId()
{
    return ::jderobot::PuntosPCLInterface::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::PuntosPCLInterface::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::PuntosPCLInterface);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::PuntosPCLInterface::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::PuntosPCLInterface);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::PuntosPCLInterface::__newInstance() const
{
    return new PuntosPCLInterface;
}

::jderobot::PuntosPCLDataPtr
IceDelegateM::jderobot::PuntosPCLInterface::getKinectData(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__PuntosPCLInterface__getKinectData_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::PuntosPCLDataPtr __ret;
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
        __is->read(::jderobot::__patch__PuntosPCLDataPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::jderobot::PuntosPCLDataPtr
IceDelegateD::jderobot::PuntosPCLInterface::getKinectData(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::PuntosPCLDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::PuntosPCLInterface* servant = dynamic_cast< ::jderobot::PuntosPCLInterface*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getKinectData(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::PuntosPCLDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__PuntosPCLInterface__getKinectData_name, ::Ice::Idempotent, __context);
    ::jderobot::PuntosPCLDataPtr __result;
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

jderobot::PuntosPCLData::PuntosPCLData(const ::jderobot::PuntosPCL& __ice_p) :
    p(__ice_p)
{
}

::Ice::ObjectPtr
jderobot::PuntosPCLData::ice_clone() const
{
    ::jderobot::PuntosPCLDataPtr __p = new ::jderobot::PuntosPCLData(*this);
    return __p;
}

static const ::std::string __jderobot__PuntosPCLData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PuntosPCLData"
};

bool
jderobot::PuntosPCLData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PuntosPCLData_ids, __jderobot__PuntosPCLData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PuntosPCLData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PuntosPCLData_ids[0], &__jderobot__PuntosPCLData_ids[2]);
}

const ::std::string&
jderobot::PuntosPCLData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PuntosPCLData_ids[1];
}

const ::std::string&
jderobot::PuntosPCLData::ice_staticId()
{
    return __jderobot__PuntosPCLData_ids[1];
}

void
jderobot::PuntosPCLData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(p.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::jderobot::__writePuntosPCL(__os, &p[0], &p[0] + p.size());
    }
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::PuntosPCLData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    ::jderobot::__readPuntosPCL(__is, p);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::PuntosPCLData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PuntosPCLData was not generated with stream support";
    throw ex;
}

void
jderobot::PuntosPCLData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PuntosPCLData was not generated with stream support";
    throw ex;
}

class __F__jderobot__PuntosPCLData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::PuntosPCLData::ice_staticId());
        return new ::jderobot::PuntosPCLData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__PuntosPCLData_Ptr = new __F__jderobot__PuntosPCLData;

const ::Ice::ObjectFactoryPtr&
jderobot::PuntosPCLData::ice_factory()
{
    return __F__jderobot__PuntosPCLData_Ptr;
}

class __F__jderobot__PuntosPCLData__Init
{
public:

    __F__jderobot__PuntosPCLData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::PuntosPCLData::ice_staticId(), ::jderobot::PuntosPCLData::ice_factory());
    }

    ~__F__jderobot__PuntosPCLData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::PuntosPCLData::ice_staticId());
    }
};

static __F__jderobot__PuntosPCLData__Init __F__jderobot__PuntosPCLData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__PuntosPCLData__initializer() {} }
#endif

void 
jderobot::__patch__PuntosPCLDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PuntosPCLDataPtr* p = static_cast< ::jderobot::PuntosPCLDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::PuntosPCLDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PuntosPCLData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PuntosPCLData& l, const ::jderobot::PuntosPCLData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PuntosPCLData& l, const ::jderobot::PuntosPCLData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::PuntosPCLInterface::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__PuntosPCLInterface_ids[2] =
{
    "::Ice::Object",
    "::jderobot::PuntosPCLInterface"
};

bool
jderobot::PuntosPCLInterface::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__PuntosPCLInterface_ids, __jderobot__PuntosPCLInterface_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::PuntosPCLInterface::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__PuntosPCLInterface_ids[0], &__jderobot__PuntosPCLInterface_ids[2]);
}

const ::std::string&
jderobot::PuntosPCLInterface::ice_id(const ::Ice::Current&) const
{
    return __jderobot__PuntosPCLInterface_ids[1];
}

const ::std::string&
jderobot::PuntosPCLInterface::ice_staticId()
{
    return __jderobot__PuntosPCLInterface_ids[1];
}

::Ice::DispatchStatus
jderobot::PuntosPCLInterface::___getKinectData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::PuntosPCLDataPtr __ret = getKinectData(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__PuntosPCLInterface_all[] =
{
    "getKinectData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::PuntosPCLInterface::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__PuntosPCLInterface_all, __jderobot__PuntosPCLInterface_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__PuntosPCLInterface_all)
    {
        case 0:
        {
            return ___getKinectData(in, current);
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
jderobot::PuntosPCLInterface::__write(::IceInternal::BasicStream* __os) const
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
jderobot::PuntosPCLInterface::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::PuntosPCLInterface::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PuntosPCLInterface was not generated with stream support";
    throw ex;
}

void
jderobot::PuntosPCLInterface::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::PuntosPCLInterface was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__PuntosPCLInterfacePtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::PuntosPCLInterfacePtr* p = static_cast< ::jderobot::PuntosPCLInterfacePtr*>(__addr);
    assert(p);
    *p = ::jderobot::PuntosPCLInterfacePtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::PuntosPCLInterface::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::PuntosPCLInterface& l, const ::jderobot::PuntosPCLInterface& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::PuntosPCLInterface& l, const ::jderobot::PuntosPCLInterface& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
