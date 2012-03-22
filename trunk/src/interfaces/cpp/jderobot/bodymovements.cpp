// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `bodymovements.ice'

#include <bodymovements.h>
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

static const ::std::string __jderobot__BodyMovements__doMovement_name = "doMovement";

static const ::std::string __jderobot__BodyMovements__getMovement_name = "getMovement";

::Ice::Object* IceInternal::upCast(::jderobot::BodyMovementsData* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::BodyMovementsData* p) { return p; }

::Ice::Object* IceInternal::upCast(::jderobot::BodyMovements* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::jderobot::BodyMovements* p) { return p; }

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodyMovementsDataPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::BodyMovementsData;
        v->__copyFrom(proxy);
    }
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodyMovementsPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::jderobot::BodyMovements;
        v->__copyFrom(proxy);
    }
}

bool
jderobot::ArmPosition::operator==(const ArmPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(shoulder != __rhs.shoulder)
    {
        return false;
    }
    if(elbow != __rhs.elbow)
    {
        return false;
    }
    return true;
}

bool
jderobot::ArmPosition::operator<(const ArmPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(shoulder < __rhs.shoulder)
    {
        return true;
    }
    else if(__rhs.shoulder < shoulder)
    {
        return false;
    }
    if(elbow < __rhs.elbow)
    {
        return true;
    }
    else if(__rhs.elbow < elbow)
    {
        return false;
    }
    return false;
}

void
jderobot::ArmPosition::__write(::IceInternal::BasicStream* __os) const
{
    shoulder.__write(__os);
    elbow.__write(__os);
}

void
jderobot::ArmPosition::__read(::IceInternal::BasicStream* __is)
{
    shoulder.__read(__is);
    elbow.__read(__is);
}

bool
jderobot::LegPosition::operator==(const LegPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(hip != __rhs.hip)
    {
        return false;
    }
    if(knee != __rhs.knee)
    {
        return false;
    }
    if(ankle != __rhs.ankle)
    {
        return false;
    }
    return true;
}

bool
jderobot::LegPosition::operator<(const LegPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(hip < __rhs.hip)
    {
        return true;
    }
    else if(__rhs.hip < hip)
    {
        return false;
    }
    if(knee < __rhs.knee)
    {
        return true;
    }
    else if(__rhs.knee < knee)
    {
        return false;
    }
    if(ankle < __rhs.ankle)
    {
        return true;
    }
    else if(__rhs.ankle < ankle)
    {
        return false;
    }
    return false;
}

void
jderobot::LegPosition::__write(::IceInternal::BasicStream* __os) const
{
    hip.__write(__os);
    knee.__write(__os);
    ankle.__write(__os);
}

void
jderobot::LegPosition::__read(::IceInternal::BasicStream* __is)
{
    hip.__read(__is);
    knee.__read(__is);
    ankle.__read(__is);
}

bool
jderobot::BodyPosition::operator==(const BodyPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(lArm != __rhs.lArm)
    {
        return false;
    }
    if(rArm != __rhs.rArm)
    {
        return false;
    }
    if(rLeg != __rhs.rLeg)
    {
        return false;
    }
    if(lLeg != __rhs.lLeg)
    {
        return false;
    }
    if(head != __rhs.head)
    {
        return false;
    }
    if(time != __rhs.time)
    {
        return false;
    }
    return true;
}

bool
jderobot::BodyPosition::operator<(const BodyPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(lArm < __rhs.lArm)
    {
        return true;
    }
    else if(__rhs.lArm < lArm)
    {
        return false;
    }
    if(rArm < __rhs.rArm)
    {
        return true;
    }
    else if(__rhs.rArm < rArm)
    {
        return false;
    }
    if(rLeg < __rhs.rLeg)
    {
        return true;
    }
    else if(__rhs.rLeg < rLeg)
    {
        return false;
    }
    if(lLeg < __rhs.lLeg)
    {
        return true;
    }
    else if(__rhs.lLeg < lLeg)
    {
        return false;
    }
    if(head < __rhs.head)
    {
        return true;
    }
    else if(__rhs.head < head)
    {
        return false;
    }
    if(time < __rhs.time)
    {
        return true;
    }
    else if(__rhs.time < time)
    {
        return false;
    }
    return false;
}

void
jderobot::BodyPosition::__write(::IceInternal::BasicStream* __os) const
{
    lArm.__write(__os);
    rArm.__write(__os);
    rLeg.__write(__os);
    lLeg.__write(__os);
    head.__write(__os);
    __os->write(time);
}

void
jderobot::BodyPosition::__read(::IceInternal::BasicStream* __is)
{
    lArm.__read(__is);
    rArm.__read(__is);
    rLeg.__read(__is);
    lLeg.__read(__is);
    head.__read(__is);
    __is->read(time);
}

void
jderobot::__writeBodyMov(::IceInternal::BasicStream* __os, const ::jderobot::BodyPosition* begin, const ::jderobot::BodyPosition* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
jderobot::__readBodyMov(::IceInternal::BasicStream* __is, ::jderobot::BodyMov& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->checkFixedSeq(sz, 136);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
    }
}

const ::std::string&
IceProxy::jderobot::BodyMovementsData::ice_staticId()
{
    return ::jderobot::BodyMovementsData::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::BodyMovementsData::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::BodyMovementsData);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::BodyMovementsData::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::BodyMovementsData);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::BodyMovementsData::__newInstance() const
{
    return new BodyMovementsData;
}

::Ice::Int
IceProxy::jderobot::BodyMovements::doMovement(const ::jderobot::BodyMovementsDataPtr& data, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyMovements__doMovement_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyMovements* __del = dynamic_cast< ::IceDelegate::jderobot::BodyMovements*>(__delBase.get());
            return __del->doMovement(data, __ctx);
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

::jderobot::BodyMovementsDataPtr
IceProxy::jderobot::BodyMovements::getMovement(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__jderobot__BodyMovements__getMovement_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::jderobot::BodyMovements* __del = dynamic_cast< ::IceDelegate::jderobot::BodyMovements*>(__delBase.get());
            return __del->getMovement(__ctx);
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
IceProxy::jderobot::BodyMovements::ice_staticId()
{
    return ::jderobot::BodyMovements::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::jderobot::BodyMovements::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::jderobot::BodyMovements);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::jderobot::BodyMovements::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::jderobot::BodyMovements);
}

::IceProxy::Ice::Object*
IceProxy::jderobot::BodyMovements::__newInstance() const
{
    return new BodyMovements;
}

::Ice::Int
IceDelegateM::jderobot::BodyMovements::doMovement(const ::jderobot::BodyMovementsDataPtr& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyMovements__doMovement_name, ::Ice::Normal, __context);
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

::jderobot::BodyMovementsDataPtr
IceDelegateM::jderobot::BodyMovements::getMovement(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __jderobot__BodyMovements__getMovement_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::jderobot::BodyMovementsDataPtr __ret;
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
        __is->read(::jderobot::__patch__BodyMovementsDataPtr, &__ret);
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
IceDelegateD::jderobot::BodyMovements::doMovement(const ::jderobot::BodyMovementsDataPtr& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::jderobot::BodyMovementsDataPtr& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyMovements* servant = dynamic_cast< ::jderobot::BodyMovements*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->doMovement(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::jderobot::BodyMovementsDataPtr& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyMovements__doMovement_name, ::Ice::Normal, __context);
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

::jderobot::BodyMovementsDataPtr
IceDelegateD::jderobot::BodyMovements::getMovement(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::jderobot::BodyMovementsDataPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::jderobot::BodyMovements* servant = dynamic_cast< ::jderobot::BodyMovements*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getMovement(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::jderobot::BodyMovementsDataPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __jderobot__BodyMovements__getMovement_name, ::Ice::Idempotent, __context);
    ::jderobot::BodyMovementsDataPtr __result;
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

jderobot::BodyMovementsData::BodyMovementsData(const ::jderobot::BodyMov& __ice_mov) :
    mov(__ice_mov)
{
}

::Ice::ObjectPtr
jderobot::BodyMovementsData::ice_clone() const
{
    ::jderobot::BodyMovementsDataPtr __p = new ::jderobot::BodyMovementsData(*this);
    return __p;
}

static const ::std::string __jderobot__BodyMovementsData_ids[2] =
{
    "::Ice::Object",
    "::jderobot::BodyMovementsData"
};

bool
jderobot::BodyMovementsData::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__BodyMovementsData_ids, __jderobot__BodyMovementsData_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::BodyMovementsData::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__BodyMovementsData_ids[0], &__jderobot__BodyMovementsData_ids[2]);
}

const ::std::string&
jderobot::BodyMovementsData::ice_id(const ::Ice::Current&) const
{
    return __jderobot__BodyMovementsData_ids[1];
}

const ::std::string&
jderobot::BodyMovementsData::ice_staticId()
{
    return __jderobot__BodyMovementsData_ids[1];
}

void
jderobot::BodyMovementsData::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(mov.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::jderobot::__writeBodyMov(__os, &mov[0], &mov[0] + mov.size());
    }
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
jderobot::BodyMovementsData::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    ::jderobot::__readBodyMov(__is, mov);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
jderobot::BodyMovementsData::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMovementsData was not generated with stream support";
    throw ex;
}

void
jderobot::BodyMovementsData::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMovementsData was not generated with stream support";
    throw ex;
}

class __F__jderobot__BodyMovementsData : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::jderobot::BodyMovementsData::ice_staticId());
        return new ::jderobot::BodyMovementsData;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__jderobot__BodyMovementsData_Ptr = new __F__jderobot__BodyMovementsData;

const ::Ice::ObjectFactoryPtr&
jderobot::BodyMovementsData::ice_factory()
{
    return __F__jderobot__BodyMovementsData_Ptr;
}

class __F__jderobot__BodyMovementsData__Init
{
public:

    __F__jderobot__BodyMovementsData__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::jderobot::BodyMovementsData::ice_staticId(), ::jderobot::BodyMovementsData::ice_factory());
    }

    ~__F__jderobot__BodyMovementsData__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::jderobot::BodyMovementsData::ice_staticId());
    }
};

static __F__jderobot__BodyMovementsData__Init __F__jderobot__BodyMovementsData__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__BodyMovementsData__initializer() {} }
#endif

void 
jderobot::__patch__BodyMovementsDataPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::BodyMovementsDataPtr* p = static_cast< ::jderobot::BodyMovementsDataPtr*>(__addr);
    assert(p);
    *p = ::jderobot::BodyMovementsDataPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::BodyMovementsData::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::BodyMovementsData& l, const ::jderobot::BodyMovementsData& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::BodyMovementsData& l, const ::jderobot::BodyMovementsData& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
jderobot::BodyMovements::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __jderobot__BodyMovements_ids[2] =
{
    "::Ice::Object",
    "::jderobot::BodyMovements"
};

bool
jderobot::BodyMovements::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__jderobot__BodyMovements_ids, __jderobot__BodyMovements_ids + 2, _s);
}

::std::vector< ::std::string>
jderobot::BodyMovements::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__jderobot__BodyMovements_ids[0], &__jderobot__BodyMovements_ids[2]);
}

const ::std::string&
jderobot::BodyMovements::ice_id(const ::Ice::Current&) const
{
    return __jderobot__BodyMovements_ids[1];
}

const ::std::string&
jderobot::BodyMovements::ice_staticId()
{
    return __jderobot__BodyMovements_ids[1];
}

::Ice::DispatchStatus
jderobot::BodyMovements::___doMovement(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::jderobot::BodyMovementsDataPtr data;
    __is->read(::jderobot::__patch__BodyMovementsDataPtr, &data);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = doMovement(data, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
jderobot::BodyMovements::___getMovement(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::jderobot::BodyMovementsDataPtr __ret = getMovement(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __jderobot__BodyMovements_all[] =
{
    "doMovement",
    "getMovement",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
jderobot::BodyMovements::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__jderobot__BodyMovements_all, __jderobot__BodyMovements_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __jderobot__BodyMovements_all)
    {
        case 0:
        {
            return ___doMovement(in, current);
        }
        case 1:
        {
            return ___getMovement(in, current);
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
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
jderobot::BodyMovements::__write(::IceInternal::BasicStream* __os) const
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
jderobot::BodyMovements::__read(::IceInternal::BasicStream* __is, bool __rid)
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
jderobot::BodyMovements::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMovements was not generated with stream support";
    throw ex;
}

void
jderobot::BodyMovements::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type jderobot::BodyMovements was not generated with stream support";
    throw ex;
}

void 
jderobot::__patch__BodyMovementsPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::jderobot::BodyMovementsPtr* p = static_cast< ::jderobot::BodyMovementsPtr*>(__addr);
    assert(p);
    *p = ::jderobot::BodyMovementsPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::jderobot::BodyMovements::ice_staticId(), v->ice_id());
    }
}

bool
jderobot::operator==(const ::jderobot::BodyMovements& l, const ::jderobot::BodyMovements& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
jderobot::operator<(const ::jderobot::BodyMovements& l, const ::jderobot::BodyMovements& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
