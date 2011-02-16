// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `JointMotor.ice'

#include "JointMotor.h"
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

static const ::std::string __RoboCompJointMotor__JointMotor__setPosition_name = "setPosition";

static const ::std::string __RoboCompJointMotor__JointMotor__setVelocity_name = "setVelocity";

static const ::std::string __RoboCompJointMotor__JointMotor__setSyncPosition_name = "setSyncPosition";

static const ::std::string __RoboCompJointMotor__JointMotor__getMotorParams_name = "getMotorParams";

static const ::std::string __RoboCompJointMotor__JointMotor__getMotorState_name = "getMotorState";

static const ::std::string __RoboCompJointMotor__JointMotor__getMotorStateMap_name = "getMotorStateMap";

static const ::std::string __RoboCompJointMotor__JointMotor__getAllMotorState_name = "getAllMotorState";

static const ::std::string __RoboCompJointMotor__JointMotor__getAllMotorParams_name = "getAllMotorParams";

static const ::std::string __RoboCompJointMotor__JointMotor__getBusParams_name = "getBusParams";

::Ice::Object* IceInternal::upCast(::RoboCompJointMotor::JointMotor* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::RoboCompJointMotor::JointMotor* p) { return p; }

void
RoboCompJointMotor::__read(::IceInternal::BasicStream* __is, ::RoboCompJointMotor::JointMotorPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::RoboCompJointMotor::JointMotor;
        v->__copyFrom(proxy);
    }
}

RoboCompJointMotor::HardwareFailedException::HardwareFailedException(const ::std::string& __ice_what) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    what(__ice_what)
{
}

RoboCompJointMotor::HardwareFailedException::~HardwareFailedException() throw()
{
}

static const char* __RoboCompJointMotor__HardwareFailedException_name = "RoboCompJointMotor::HardwareFailedException";

::std::string
RoboCompJointMotor::HardwareFailedException::ice_name() const
{
    return __RoboCompJointMotor__HardwareFailedException_name;
}

::Ice::Exception*
RoboCompJointMotor::HardwareFailedException::ice_clone() const
{
    return new HardwareFailedException(*this);
}

void
RoboCompJointMotor::HardwareFailedException::ice_throw() const
{
    throw *this;
}

void
RoboCompJointMotor::HardwareFailedException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::RoboCompJointMotor::HardwareFailedException"), false);
    __os->startWriteSlice();
    __os->write(what);
    __os->endWriteSlice();
}

void
RoboCompJointMotor::HardwareFailedException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(what);
    __is->endReadSlice();
}

void
RoboCompJointMotor::HardwareFailedException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::HardwareFailedException was not generated with stream support";
    throw ex;
}

void
RoboCompJointMotor::HardwareFailedException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::HardwareFailedException was not generated with stream support";
    throw ex;
}

struct __F__RoboCompJointMotor__HardwareFailedException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::RoboCompJointMotor::HardwareFailedException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__RoboCompJointMotor__HardwareFailedException__Ptr = new __F__RoboCompJointMotor__HardwareFailedException;

const ::IceInternal::UserExceptionFactoryPtr&
RoboCompJointMotor::HardwareFailedException::ice_factory()
{
    return __F__RoboCompJointMotor__HardwareFailedException__Ptr;
}

class __F__RoboCompJointMotor__HardwareFailedException__Init
{
public:

    __F__RoboCompJointMotor__HardwareFailedException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::RoboCompJointMotor::HardwareFailedException", ::RoboCompJointMotor::HardwareFailedException::ice_factory());
    }

    ~__F__RoboCompJointMotor__HardwareFailedException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::RoboCompJointMotor::HardwareFailedException");
    }
};

static __F__RoboCompJointMotor__HardwareFailedException__Init __F__RoboCompJointMotor__HardwareFailedException__i;

#ifdef __APPLE__
extern "C" { void __F__RoboCompJointMotor__HardwareFailedException__initializer() {} }
#endif

RoboCompJointMotor::OutOfRangeException::OutOfRangeException(const ::std::string& __ice_what) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    what(__ice_what)
{
}

RoboCompJointMotor::OutOfRangeException::~OutOfRangeException() throw()
{
}

static const char* __RoboCompJointMotor__OutOfRangeException_name = "RoboCompJointMotor::OutOfRangeException";

::std::string
RoboCompJointMotor::OutOfRangeException::ice_name() const
{
    return __RoboCompJointMotor__OutOfRangeException_name;
}

::Ice::Exception*
RoboCompJointMotor::OutOfRangeException::ice_clone() const
{
    return new OutOfRangeException(*this);
}

void
RoboCompJointMotor::OutOfRangeException::ice_throw() const
{
    throw *this;
}

void
RoboCompJointMotor::OutOfRangeException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::RoboCompJointMotor::OutOfRangeException"), false);
    __os->startWriteSlice();
    __os->write(what);
    __os->endWriteSlice();
}

void
RoboCompJointMotor::OutOfRangeException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(what);
    __is->endReadSlice();
}

void
RoboCompJointMotor::OutOfRangeException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::OutOfRangeException was not generated with stream support";
    throw ex;
}

void
RoboCompJointMotor::OutOfRangeException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::OutOfRangeException was not generated with stream support";
    throw ex;
}

struct __F__RoboCompJointMotor__OutOfRangeException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::RoboCompJointMotor::OutOfRangeException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__RoboCompJointMotor__OutOfRangeException__Ptr = new __F__RoboCompJointMotor__OutOfRangeException;

const ::IceInternal::UserExceptionFactoryPtr&
RoboCompJointMotor::OutOfRangeException::ice_factory()
{
    return __F__RoboCompJointMotor__OutOfRangeException__Ptr;
}

class __F__RoboCompJointMotor__OutOfRangeException__Init
{
public:

    __F__RoboCompJointMotor__OutOfRangeException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::RoboCompJointMotor::OutOfRangeException", ::RoboCompJointMotor::OutOfRangeException::ice_factory());
    }

    ~__F__RoboCompJointMotor__OutOfRangeException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::RoboCompJointMotor::OutOfRangeException");
    }
};

static __F__RoboCompJointMotor__OutOfRangeException__Init __F__RoboCompJointMotor__OutOfRangeException__i;

#ifdef __APPLE__
extern "C" { void __F__RoboCompJointMotor__OutOfRangeException__initializer() {} }
#endif

RoboCompJointMotor::UnknownMotorException::UnknownMotorException(const ::std::string& __ice_what) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    what(__ice_what)
{
}

RoboCompJointMotor::UnknownMotorException::~UnknownMotorException() throw()
{
}

static const char* __RoboCompJointMotor__UnknownMotorException_name = "RoboCompJointMotor::UnknownMotorException";

::std::string
RoboCompJointMotor::UnknownMotorException::ice_name() const
{
    return __RoboCompJointMotor__UnknownMotorException_name;
}

::Ice::Exception*
RoboCompJointMotor::UnknownMotorException::ice_clone() const
{
    return new UnknownMotorException(*this);
}

void
RoboCompJointMotor::UnknownMotorException::ice_throw() const
{
    throw *this;
}

void
RoboCompJointMotor::UnknownMotorException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::RoboCompJointMotor::UnknownMotorException"), false);
    __os->startWriteSlice();
    __os->write(what);
    __os->endWriteSlice();
}

void
RoboCompJointMotor::UnknownMotorException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(what);
    __is->endReadSlice();
}

void
RoboCompJointMotor::UnknownMotorException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::UnknownMotorException was not generated with stream support";
    throw ex;
}

void
RoboCompJointMotor::UnknownMotorException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception RoboCompJointMotor::UnknownMotorException was not generated with stream support";
    throw ex;
}

struct __F__RoboCompJointMotor__UnknownMotorException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::RoboCompJointMotor::UnknownMotorException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__RoboCompJointMotor__UnknownMotorException__Ptr = new __F__RoboCompJointMotor__UnknownMotorException;

const ::IceInternal::UserExceptionFactoryPtr&
RoboCompJointMotor::UnknownMotorException::ice_factory()
{
    return __F__RoboCompJointMotor__UnknownMotorException__Ptr;
}

class __F__RoboCompJointMotor__UnknownMotorException__Init
{
public:

    __F__RoboCompJointMotor__UnknownMotorException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::RoboCompJointMotor::UnknownMotorException", ::RoboCompJointMotor::UnknownMotorException::ice_factory());
    }

    ~__F__RoboCompJointMotor__UnknownMotorException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::RoboCompJointMotor::UnknownMotorException");
    }
};

static __F__RoboCompJointMotor__UnknownMotorException__Init __F__RoboCompJointMotor__UnknownMotorException__i;

#ifdef __APPLE__
extern "C" { void __F__RoboCompJointMotor__UnknownMotorException__initializer() {} }
#endif

bool
RoboCompJointMotor::MotorState::operator==(const MotorState& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(pos != __rhs.pos)
    {
        return false;
    }
    if(vel != __rhs.vel)
    {
        return false;
    }
    if(power != __rhs.power)
    {
        return false;
    }
    if(timeStamp != __rhs.timeStamp)
    {
        return false;
    }
    if(p != __rhs.p)
    {
        return false;
    }
    if(v != __rhs.v)
    {
        return false;
    }
    if(isMoving != __rhs.isMoving)
    {
        return false;
    }
    return true;
}

bool
RoboCompJointMotor::MotorState::operator<(const MotorState& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(pos < __rhs.pos)
    {
        return true;
    }
    else if(__rhs.pos < pos)
    {
        return false;
    }
    if(vel < __rhs.vel)
    {
        return true;
    }
    else if(__rhs.vel < vel)
    {
        return false;
    }
    if(power < __rhs.power)
    {
        return true;
    }
    else if(__rhs.power < power)
    {
        return false;
    }
    if(timeStamp < __rhs.timeStamp)
    {
        return true;
    }
    else if(__rhs.timeStamp < timeStamp)
    {
        return false;
    }
    if(p < __rhs.p)
    {
        return true;
    }
    else if(__rhs.p < p)
    {
        return false;
    }
    if(v < __rhs.v)
    {
        return true;
    }
    else if(__rhs.v < v)
    {
        return false;
    }
    if(isMoving < __rhs.isMoving)
    {
        return true;
    }
    else if(__rhs.isMoving < isMoving)
    {
        return false;
    }
    return false;
}

void
RoboCompJointMotor::MotorState::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(pos);
    __os->write(vel);
    __os->write(power);
    __os->write(timeStamp);
    __os->write(p);
    __os->write(v);
    __os->write(isMoving);
}

void
RoboCompJointMotor::MotorState::__read(::IceInternal::BasicStream* __is)
{
    __is->read(pos);
    __is->read(vel);
    __is->read(power);
    __is->read(timeStamp);
    __is->read(p);
    __is->read(v);
    __is->read(isMoving);
}

void
RoboCompJointMotor::__writeMotorStateMap(::IceInternal::BasicStream* __os, const ::RoboCompJointMotor::MotorStateMap& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::RoboCompJointMotor::MotorStateMap::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        p->second.__write(__os);
    }
}

void
RoboCompJointMotor::__readMotorStateMap(::IceInternal::BasicStream* __is, ::RoboCompJointMotor::MotorStateMap& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::RoboCompJointMotor::MotorState> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::RoboCompJointMotor::MotorStateMap::iterator __i = v.insert(v.end(), pair);
        __i->second.__read(__is);
    }
}

bool
RoboCompJointMotor::MotorParams::operator==(const MotorParams& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(busId != __rhs.busId)
    {
        return false;
    }
    if(minPos != __rhs.minPos)
    {
        return false;
    }
    if(maxPos != __rhs.maxPos)
    {
        return false;
    }
    if(maxVelocity != __rhs.maxVelocity)
    {
        return false;
    }
    if(zeroPos != __rhs.zeroPos)
    {
        return false;
    }
    if(invertedSign != __rhs.invertedSign)
    {
        return false;
    }
    return true;
}

bool
RoboCompJointMotor::MotorParams::operator<(const MotorParams& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(name < __rhs.name)
    {
        return true;
    }
    else if(__rhs.name < name)
    {
        return false;
    }
    if(busId < __rhs.busId)
    {
        return true;
    }
    else if(__rhs.busId < busId)
    {
        return false;
    }
    if(minPos < __rhs.minPos)
    {
        return true;
    }
    else if(__rhs.minPos < minPos)
    {
        return false;
    }
    if(maxPos < __rhs.maxPos)
    {
        return true;
    }
    else if(__rhs.maxPos < maxPos)
    {
        return false;
    }
    if(maxVelocity < __rhs.maxVelocity)
    {
        return true;
    }
    else if(__rhs.maxVelocity < maxVelocity)
    {
        return false;
    }
    if(zeroPos < __rhs.zeroPos)
    {
        return true;
    }
    else if(__rhs.zeroPos < zeroPos)
    {
        return false;
    }
    if(invertedSign < __rhs.invertedSign)
    {
        return true;
    }
    else if(__rhs.invertedSign < invertedSign)
    {
        return false;
    }
    return false;
}

void
RoboCompJointMotor::MotorParams::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(name);
    __os->write(busId);
    __os->write(minPos);
    __os->write(maxPos);
    __os->write(maxVelocity);
    __os->write(zeroPos);
    __os->write(invertedSign);
}

void
RoboCompJointMotor::MotorParams::__read(::IceInternal::BasicStream* __is)
{
    __is->read(name);
    __is->read(busId);
    __is->read(minPos);
    __is->read(maxPos);
    __is->read(maxVelocity);
    __is->read(zeroPos);
    __is->read(invertedSign);
}

void
RoboCompJointMotor::__writeMotorParamsList(::IceInternal::BasicStream* __os, const ::RoboCompJointMotor::MotorParams* begin, const ::RoboCompJointMotor::MotorParams* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
RoboCompJointMotor::__readMotorParamsList(::IceInternal::BasicStream* __is, ::RoboCompJointMotor::MotorParamsList& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 19);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

bool
RoboCompJointMotor::BusParams::operator==(const BusParams& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(handler != __rhs.handler)
    {
        return false;
    }
    if(device != __rhs.device)
    {
        return false;
    }
    if(numMotors != __rhs.numMotors)
    {
        return false;
    }
    if(baudRate != __rhs.baudRate)
    {
        return false;
    }
    if(basicPeriod != __rhs.basicPeriod)
    {
        return false;
    }
    return true;
}

bool
RoboCompJointMotor::BusParams::operator<(const BusParams& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(handler < __rhs.handler)
    {
        return true;
    }
    else if(__rhs.handler < handler)
    {
        return false;
    }
    if(device < __rhs.device)
    {
        return true;
    }
    else if(__rhs.device < device)
    {
        return false;
    }
    if(numMotors < __rhs.numMotors)
    {
        return true;
    }
    else if(__rhs.numMotors < numMotors)
    {
        return false;
    }
    if(baudRate < __rhs.baudRate)
    {
        return true;
    }
    else if(__rhs.baudRate < baudRate)
    {
        return false;
    }
    if(basicPeriod < __rhs.basicPeriod)
    {
        return true;
    }
    else if(__rhs.basicPeriod < basicPeriod)
    {
        return false;
    }
    return false;
}

void
RoboCompJointMotor::BusParams::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(handler);
    __os->write(device);
    __os->write(numMotors);
    __os->write(baudRate);
    __os->write(basicPeriod);
}

void
RoboCompJointMotor::BusParams::__read(::IceInternal::BasicStream* __is)
{
    __is->read(handler);
    __is->read(device);
    __is->read(numMotors);
    __is->read(baudRate);
    __is->read(basicPeriod);
}

bool
RoboCompJointMotor::MotorGoalPosition::operator==(const MotorGoalPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(position != __rhs.position)
    {
        return false;
    }
    if(maxSpeed != __rhs.maxSpeed)
    {
        return false;
    }
    return true;
}

bool
RoboCompJointMotor::MotorGoalPosition::operator<(const MotorGoalPosition& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(name < __rhs.name)
    {
        return true;
    }
    else if(__rhs.name < name)
    {
        return false;
    }
    if(position < __rhs.position)
    {
        return true;
    }
    else if(__rhs.position < position)
    {
        return false;
    }
    if(maxSpeed < __rhs.maxSpeed)
    {
        return true;
    }
    else if(__rhs.maxSpeed < maxSpeed)
    {
        return false;
    }
    return false;
}

void
RoboCompJointMotor::MotorGoalPosition::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(name);
    __os->write(position);
    __os->write(maxSpeed);
}

void
RoboCompJointMotor::MotorGoalPosition::__read(::IceInternal::BasicStream* __is)
{
    __is->read(name);
    __is->read(position);
    __is->read(maxSpeed);
}

void
RoboCompJointMotor::__writeMotorGoalPositionList(::IceInternal::BasicStream* __os, const ::RoboCompJointMotor::MotorGoalPosition* begin, const ::RoboCompJointMotor::MotorGoalPosition* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
RoboCompJointMotor::__readMotorGoalPositionList(::IceInternal::BasicStream* __is, ::RoboCompJointMotor::MotorGoalPositionList& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 9);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

bool
RoboCompJointMotor::MotorGoalVelocity::operator==(const MotorGoalVelocity& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(velocity != __rhs.velocity)
    {
        return false;
    }
    if(maxAcc != __rhs.maxAcc)
    {
        return false;
    }
    return true;
}

bool
RoboCompJointMotor::MotorGoalVelocity::operator<(const MotorGoalVelocity& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(name < __rhs.name)
    {
        return true;
    }
    else if(__rhs.name < name)
    {
        return false;
    }
    if(velocity < __rhs.velocity)
    {
        return true;
    }
    else if(__rhs.velocity < velocity)
    {
        return false;
    }
    if(maxAcc < __rhs.maxAcc)
    {
        return true;
    }
    else if(__rhs.maxAcc < maxAcc)
    {
        return false;
    }
    return false;
}

void
RoboCompJointMotor::MotorGoalVelocity::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(name);
    __os->write(velocity);
    __os->write(maxAcc);
}

void
RoboCompJointMotor::MotorGoalVelocity::__read(::IceInternal::BasicStream* __is)
{
    __is->read(name);
    __is->read(velocity);
    __is->read(maxAcc);
}

void
IceProxy::RoboCompJointMotor::JointMotor::setPosition(const ::RoboCompJointMotor::MotorGoalPosition& goal, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__setPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            __del->setPosition(goal, __ctx);
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

void
IceProxy::RoboCompJointMotor::JointMotor::setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity& goal, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__setVelocity_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            __del->setVelocity(goal, __ctx);
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

void
IceProxy::RoboCompJointMotor::JointMotor::setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__setSyncPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            __del->setSyncPosition(listGoals, __ctx);
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

::RoboCompJointMotor::MotorParams
IceProxy::RoboCompJointMotor::JointMotor::getMotorParams(const ::std::string& motor, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getMotorParams_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            return __del->getMotorParams(motor, __ctx);
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

::RoboCompJointMotor::MotorState
IceProxy::RoboCompJointMotor::JointMotor::getMotorState(const ::std::string& motor, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getMotorState_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            return __del->getMotorState(motor, __ctx);
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

::RoboCompJointMotor::MotorStateMap
IceProxy::RoboCompJointMotor::JointMotor::getMotorStateMap(const ::RoboCompJointMotor::MotorList& mList, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getMotorStateMap_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            return __del->getMotorStateMap(mList, __ctx);
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

void
IceProxy::RoboCompJointMotor::JointMotor::getAllMotorState(::RoboCompJointMotor::MotorStateMap& mstateMap, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getAllMotorState_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            __del->getAllMotorState(mstateMap, __ctx);
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

::RoboCompJointMotor::MotorParamsList
IceProxy::RoboCompJointMotor::JointMotor::getAllMotorParams(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getAllMotorParams_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            return __del->getAllMotorParams(__ctx);
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

::RoboCompJointMotor::BusParams
IceProxy::RoboCompJointMotor::JointMotor::getBusParams(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__RoboCompJointMotor__JointMotor__getBusParams_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompJointMotor::JointMotor* __del = dynamic_cast< ::IceDelegate::RoboCompJointMotor::JointMotor*>(__delBase.get());
            return __del->getBusParams(__ctx);
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
IceProxy::RoboCompJointMotor::JointMotor::ice_staticId()
{
    return ::RoboCompJointMotor::JointMotor::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::RoboCompJointMotor::JointMotor::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::RoboCompJointMotor::JointMotor);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::RoboCompJointMotor::JointMotor::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::RoboCompJointMotor::JointMotor);
}

::IceProxy::Ice::Object*
IceProxy::RoboCompJointMotor::JointMotor::__newInstance() const
{
    return new JointMotor;
}

void
IceDelegateM::RoboCompJointMotor::JointMotor::setPosition(const ::RoboCompJointMotor::MotorGoalPosition& goal, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__setPosition_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        goal.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::HardwareFailedException&)
            {
                throw;
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
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

void
IceDelegateM::RoboCompJointMotor::JointMotor::setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity& goal, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__setVelocity_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        goal.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::HardwareFailedException&)
            {
                throw;
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
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

void
IceDelegateM::RoboCompJointMotor::JointMotor::setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__setSyncPosition_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(listGoals.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::RoboCompJointMotor::__writeMotorGoalPositionList(__os, &listGoals[0], &listGoals[0] + listGoals.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::HardwareFailedException&)
            {
                throw;
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
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

::RoboCompJointMotor::MotorParams
IceDelegateM::RoboCompJointMotor::JointMotor::getMotorParams(const ::std::string& motor, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getMotorParams_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(motor);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::RoboCompJointMotor::MotorParams __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RoboCompJointMotor::MotorState
IceDelegateM::RoboCompJointMotor::JointMotor::getMotorState(const ::std::string& motor, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getMotorState_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(motor);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::RoboCompJointMotor::MotorState __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RoboCompJointMotor::MotorStateMap
IceDelegateM::RoboCompJointMotor::JointMotor::getMotorStateMap(const ::RoboCompJointMotor::MotorList& mList, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getMotorStateMap_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(mList.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&mList[0], &mList[0] + mList.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::RoboCompJointMotor::MotorStateMap __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::RoboCompJointMotor::__readMotorStateMap(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RoboCompJointMotor::JointMotor::getAllMotorState(::RoboCompJointMotor::MotorStateMap& mstateMap, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getAllMotorState_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::RoboCompJointMotor::UnknownMotorException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::RoboCompJointMotor::__readMotorStateMap(__is, mstateMap);
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RoboCompJointMotor::MotorParamsList
IceDelegateM::RoboCompJointMotor::JointMotor::getAllMotorParams(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getAllMotorParams_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::RoboCompJointMotor::MotorParamsList __ret;
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
        ::RoboCompJointMotor::__readMotorParamsList(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RoboCompJointMotor::BusParams
IceDelegateM::RoboCompJointMotor::JointMotor::getBusParams(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompJointMotor__JointMotor__getBusParams_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::RoboCompJointMotor::BusParams __ret;
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
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::RoboCompJointMotor::JointMotor::setPosition(const ::RoboCompJointMotor::MotorGoalPosition& goal, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::RoboCompJointMotor::MotorGoalPosition& goal, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_goal(goal)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setPosition(_m_goal, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::RoboCompJointMotor::MotorGoalPosition& _m_goal;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__setPosition_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(goal, __current);
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
    catch(const ::RoboCompJointMotor::HardwareFailedException&)
    {
        throw;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

void
IceDelegateD::RoboCompJointMotor::JointMotor::setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity& goal, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::RoboCompJointMotor::MotorGoalVelocity& goal, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_goal(goal)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setVelocity(_m_goal, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::RoboCompJointMotor::MotorGoalVelocity& _m_goal;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__setVelocity_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(goal, __current);
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
    catch(const ::RoboCompJointMotor::HardwareFailedException&)
    {
        throw;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

void
IceDelegateD::RoboCompJointMotor::JointMotor::setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_listGoals(listGoals)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setSyncPosition(_m_listGoals, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::RoboCompJointMotor::MotorGoalPositionList& _m_listGoals;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__setSyncPosition_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(listGoals, __current);
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
    catch(const ::RoboCompJointMotor::HardwareFailedException&)
    {
        throw;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

::RoboCompJointMotor::MotorParams
IceDelegateD::RoboCompJointMotor::JointMotor::getMotorParams(const ::std::string& motor, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::MotorParams& __result, const ::std::string& motor, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_motor(motor)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getMotorParams(_m_motor, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::RoboCompJointMotor::MotorParams& _result;
        const ::std::string& _m_motor;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getMotorParams_name, ::Ice::Normal, __context);
    ::RoboCompJointMotor::MotorParams __result;
    try
    {
        _DirectI __direct(__result, motor, __current);
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
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

::RoboCompJointMotor::MotorState
IceDelegateD::RoboCompJointMotor::JointMotor::getMotorState(const ::std::string& motor, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::MotorState& __result, const ::std::string& motor, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_motor(motor)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getMotorState(_m_motor, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::RoboCompJointMotor::MotorState& _result;
        const ::std::string& _m_motor;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getMotorState_name, ::Ice::Normal, __context);
    ::RoboCompJointMotor::MotorState __result;
    try
    {
        _DirectI __direct(__result, motor, __current);
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
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

::RoboCompJointMotor::MotorStateMap
IceDelegateD::RoboCompJointMotor::JointMotor::getMotorStateMap(const ::RoboCompJointMotor::MotorList& mList, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::MotorStateMap& __result, const ::RoboCompJointMotor::MotorList& mList, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_mList(mList)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getMotorStateMap(_m_mList, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::RoboCompJointMotor::MotorStateMap& _result;
        const ::RoboCompJointMotor::MotorList& _m_mList;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getMotorStateMap_name, ::Ice::Normal, __context);
    ::RoboCompJointMotor::MotorStateMap __result;
    try
    {
        _DirectI __direct(__result, mList, __current);
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
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

void
IceDelegateD::RoboCompJointMotor::JointMotor::getAllMotorState(::RoboCompJointMotor::MotorStateMap& mstateMap, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::MotorStateMap& mstateMap, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_mstateMap(mstateMap)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->getAllMotorState(_m_mstateMap, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::RoboCompJointMotor::MotorStateMap& _m_mstateMap;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getAllMotorState_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(mstateMap, __current);
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
    catch(const ::RoboCompJointMotor::UnknownMotorException&)
    {
        throw;
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

::RoboCompJointMotor::MotorParamsList
IceDelegateD::RoboCompJointMotor::JointMotor::getAllMotorParams(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::MotorParamsList& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getAllMotorParams(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RoboCompJointMotor::MotorParamsList& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getAllMotorParams_name, ::Ice::Normal, __context);
    ::RoboCompJointMotor::MotorParamsList __result;
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

::RoboCompJointMotor::BusParams
IceDelegateD::RoboCompJointMotor::JointMotor::getBusParams(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompJointMotor::BusParams& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompJointMotor::JointMotor* servant = dynamic_cast< ::RoboCompJointMotor::JointMotor*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getBusParams(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RoboCompJointMotor::BusParams& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompJointMotor__JointMotor__getBusParams_name, ::Ice::Normal, __context);
    ::RoboCompJointMotor::BusParams __result;
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

::Ice::ObjectPtr
RoboCompJointMotor::JointMotor::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __RoboCompJointMotor__JointMotor_ids[2] =
{
    "::Ice::Object",
    "::RoboCompJointMotor::JointMotor"
};

bool
RoboCompJointMotor::JointMotor::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__RoboCompJointMotor__JointMotor_ids, __RoboCompJointMotor__JointMotor_ids + 2, _s);
}

::std::vector< ::std::string>
RoboCompJointMotor::JointMotor::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__RoboCompJointMotor__JointMotor_ids[0], &__RoboCompJointMotor__JointMotor_ids[2]);
}

const ::std::string&
RoboCompJointMotor::JointMotor::ice_id(const ::Ice::Current&) const
{
    return __RoboCompJointMotor__JointMotor_ids[1];
}

const ::std::string&
RoboCompJointMotor::JointMotor::ice_staticId()
{
    return __RoboCompJointMotor__JointMotor_ids[1];
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___setPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::RoboCompJointMotor::MotorGoalPosition goal;
    goal.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setPosition(goal, __current);
    }
    catch(const ::RoboCompJointMotor::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___setVelocity(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::RoboCompJointMotor::MotorGoalVelocity goal;
    goal.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setVelocity(goal, __current);
    }
    catch(const ::RoboCompJointMotor::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___setSyncPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::RoboCompJointMotor::MotorGoalPositionList listGoals;
    ::RoboCompJointMotor::__readMotorGoalPositionList(__is, listGoals);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setSyncPosition(listGoals, __current);
    }
    catch(const ::RoboCompJointMotor::HardwareFailedException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getMotorParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string motor;
    __is->read(motor);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::RoboCompJointMotor::MotorParams __ret = getMotorParams(motor, __current);
        __ret.__write(__os);
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getMotorState(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string motor;
    __is->read(motor);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::RoboCompJointMotor::MotorState __ret = getMotorState(motor, __current);
        __ret.__write(__os);
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getMotorStateMap(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::RoboCompJointMotor::MotorList mList;
    __is->read(mList);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::RoboCompJointMotor::MotorStateMap __ret = getMotorStateMap(mList, __current);
        ::RoboCompJointMotor::__writeMotorStateMap(__os, __ret);
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getAllMotorState(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RoboCompJointMotor::MotorStateMap mstateMap;
    try
    {
        getAllMotorState(mstateMap, __current);
        ::RoboCompJointMotor::__writeMotorStateMap(__os, mstateMap);
    }
    catch(const ::RoboCompJointMotor::UnknownMotorException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getAllMotorParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RoboCompJointMotor::MotorParamsList __ret = getAllMotorParams(__current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::RoboCompJointMotor::__writeMotorParamsList(__os, &__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::___getBusParams(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RoboCompJointMotor::BusParams __ret = getBusParams(__current);
    __ret.__write(__os);
    return ::Ice::DispatchOK;
}

static ::std::string __RoboCompJointMotor__JointMotor_all[] =
{
    "getAllMotorParams",
    "getAllMotorState",
    "getBusParams",
    "getMotorParams",
    "getMotorState",
    "getMotorStateMap",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "setPosition",
    "setSyncPosition",
    "setVelocity"
};

::Ice::DispatchStatus
RoboCompJointMotor::JointMotor::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__RoboCompJointMotor__JointMotor_all, __RoboCompJointMotor__JointMotor_all + 13, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __RoboCompJointMotor__JointMotor_all)
    {
        case 0:
        {
            return ___getAllMotorParams(in, current);
        }
        case 1:
        {
            return ___getAllMotorState(in, current);
        }
        case 2:
        {
            return ___getBusParams(in, current);
        }
        case 3:
        {
            return ___getMotorParams(in, current);
        }
        case 4:
        {
            return ___getMotorState(in, current);
        }
        case 5:
        {
            return ___getMotorStateMap(in, current);
        }
        case 6:
        {
            return ___ice_id(in, current);
        }
        case 7:
        {
            return ___ice_ids(in, current);
        }
        case 8:
        {
            return ___ice_isA(in, current);
        }
        case 9:
        {
            return ___ice_ping(in, current);
        }
        case 10:
        {
            return ___setPosition(in, current);
        }
        case 11:
        {
            return ___setSyncPosition(in, current);
        }
        case 12:
        {
            return ___setVelocity(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
RoboCompJointMotor::JointMotor::__write(::IceInternal::BasicStream* __os) const
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
RoboCompJointMotor::JointMotor::__read(::IceInternal::BasicStream* __is, bool __rid)
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
RoboCompJointMotor::JointMotor::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RoboCompJointMotor::JointMotor was not generated with stream support";
    throw ex;
}

void
RoboCompJointMotor::JointMotor::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RoboCompJointMotor::JointMotor was not generated with stream support";
    throw ex;
}

void 
RoboCompJointMotor::__patch__JointMotorPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::RoboCompJointMotor::JointMotorPtr* p = static_cast< ::RoboCompJointMotor::JointMotorPtr*>(__addr);
    assert(p);
    *p = ::RoboCompJointMotor::JointMotorPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::RoboCompJointMotor::JointMotor::ice_staticId(), v->ice_id());
    }
}

bool
RoboCompJointMotor::operator==(const ::RoboCompJointMotor::JointMotor& l, const ::RoboCompJointMotor::JointMotor& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
RoboCompJointMotor::operator<(const ::RoboCompJointMotor::JointMotor& l, const ::RoboCompJointMotor::JointMotor& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
