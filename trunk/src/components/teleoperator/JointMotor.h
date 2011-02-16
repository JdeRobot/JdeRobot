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

#ifndef ____JointMotor_h__
#define ____JointMotor_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

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

namespace IceProxy
{

namespace RoboCompJointMotor
{

class JointMotor;

}

}

namespace RoboCompJointMotor
{

class JointMotor;
bool operator==(const JointMotor&, const JointMotor&);
bool operator<(const JointMotor&, const JointMotor&);

}

namespace IceInternal
{

::Ice::Object* upCast(::RoboCompJointMotor::JointMotor*);
::IceProxy::Ice::Object* upCast(::IceProxy::RoboCompJointMotor::JointMotor*);

}

namespace RoboCompJointMotor
{

typedef ::IceInternal::Handle< ::RoboCompJointMotor::JointMotor> JointMotorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::RoboCompJointMotor::JointMotor> JointMotorPrx;

void __read(::IceInternal::BasicStream*, JointMotorPrx&);
void __patch__JointMotorPtr(void*, ::Ice::ObjectPtr&);

}

namespace RoboCompJointMotor
{

class HardwareFailedException : public ::Ice::UserException
{
public:

    HardwareFailedException() {}
    explicit HardwareFailedException(const ::std::string&);
    virtual ~HardwareFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string what;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static HardwareFailedException __HardwareFailedException_init;

class OutOfRangeException : public ::Ice::UserException
{
public:

    OutOfRangeException() {}
    explicit OutOfRangeException(const ::std::string&);
    virtual ~OutOfRangeException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string what;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class UnknownMotorException : public ::Ice::UserException
{
public:

    UnknownMotorException() {}
    explicit UnknownMotorException(const ::std::string&);
    virtual ~UnknownMotorException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string what;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

struct MotorState
{
    ::Ice::Float pos;
    ::Ice::Float vel;
    ::Ice::Float power;
    ::std::string timeStamp;
    ::Ice::Int p;
    ::Ice::Int v;
    bool isMoving;

    bool operator==(const MotorState&) const;
    bool operator<(const MotorState&) const;
    bool operator!=(const MotorState& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const MotorState& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorState& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorState& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::RoboCompJointMotor::MotorState> MotorStateMap;
void __writeMotorStateMap(::IceInternal::BasicStream*, const MotorStateMap&);
void __readMotorStateMap(::IceInternal::BasicStream*, MotorStateMap&);

struct MotorParams
{
    ::std::string name;
    ::Ice::Byte busId;
    ::Ice::Float minPos;
    ::Ice::Float maxPos;
    ::Ice::Float maxVelocity;
    ::Ice::Float zeroPos;
    bool invertedSign;

    bool operator==(const MotorParams&) const;
    bool operator<(const MotorParams&) const;
    bool operator!=(const MotorParams& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const MotorParams& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorParams& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorParams& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::RoboCompJointMotor::MotorParams> MotorParamsList;
void __writeMotorParamsList(::IceInternal::BasicStream*, const ::RoboCompJointMotor::MotorParams*, const ::RoboCompJointMotor::MotorParams*);
void __readMotorParamsList(::IceInternal::BasicStream*, MotorParamsList&);

struct BusParams
{
    ::std::string handler;
    ::std::string device;
    ::Ice::Int numMotors;
    ::Ice::Int baudRate;
    ::Ice::Int basicPeriod;

    bool operator==(const BusParams&) const;
    bool operator<(const BusParams&) const;
    bool operator!=(const BusParams& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const BusParams& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const BusParams& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const BusParams& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct MotorGoalPosition
{
    ::std::string name;
    ::Ice::Float position;
    ::Ice::Float maxSpeed;

    bool operator==(const MotorGoalPosition&) const;
    bool operator<(const MotorGoalPosition&) const;
    bool operator!=(const MotorGoalPosition& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const MotorGoalPosition& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorGoalPosition& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorGoalPosition& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::RoboCompJointMotor::MotorGoalPosition> MotorGoalPositionList;
void __writeMotorGoalPositionList(::IceInternal::BasicStream*, const ::RoboCompJointMotor::MotorGoalPosition*, const ::RoboCompJointMotor::MotorGoalPosition*);
void __readMotorGoalPositionList(::IceInternal::BasicStream*, MotorGoalPositionList&);

struct MotorGoalVelocity
{
    ::std::string name;
    ::Ice::Float velocity;
    ::Ice::Float maxAcc;

    bool operator==(const MotorGoalVelocity&) const;
    bool operator<(const MotorGoalVelocity&) const;
    bool operator!=(const MotorGoalVelocity& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const MotorGoalVelocity& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const MotorGoalVelocity& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const MotorGoalVelocity& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::std::string> MotorList;

}

namespace IceProxy
{

namespace RoboCompJointMotor
{

class JointMotor : virtual public ::IceProxy::Ice::Object
{
public:

    void setPosition(const ::RoboCompJointMotor::MotorGoalPosition& goal)
    {
        setPosition(goal, 0);
    }
    void setPosition(const ::RoboCompJointMotor::MotorGoalPosition& goal, const ::Ice::Context& __ctx)
    {
        setPosition(goal, &__ctx);
    }
    
private:

    void setPosition(const ::RoboCompJointMotor::MotorGoalPosition&, const ::Ice::Context*);
    
public:

    void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity& goal)
    {
        setVelocity(goal, 0);
    }
    void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity& goal, const ::Ice::Context& __ctx)
    {
        setVelocity(goal, &__ctx);
    }
    
private:

    void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity&, const ::Ice::Context*);
    
public:

    void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals)
    {
        setSyncPosition(listGoals, 0);
    }
    void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList& listGoals, const ::Ice::Context& __ctx)
    {
        setSyncPosition(listGoals, &__ctx);
    }
    
private:

    void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList&, const ::Ice::Context*);
    
public:

    ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string& motor)
    {
        return getMotorParams(motor, 0);
    }
    ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string& motor, const ::Ice::Context& __ctx)
    {
        return getMotorParams(motor, &__ctx);
    }
    
private:

    ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string&, const ::Ice::Context*);
    
public:

    ::RoboCompJointMotor::MotorState getMotorState(const ::std::string& motor)
    {
        return getMotorState(motor, 0);
    }
    ::RoboCompJointMotor::MotorState getMotorState(const ::std::string& motor, const ::Ice::Context& __ctx)
    {
        return getMotorState(motor, &__ctx);
    }
    
private:

    ::RoboCompJointMotor::MotorState getMotorState(const ::std::string&, const ::Ice::Context*);
    
public:

    ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList& mList)
    {
        return getMotorStateMap(mList, 0);
    }
    ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList& mList, const ::Ice::Context& __ctx)
    {
        return getMotorStateMap(mList, &__ctx);
    }
    
private:

    ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList&, const ::Ice::Context*);
    
public:

    void getAllMotorState(::RoboCompJointMotor::MotorStateMap& mstateMap)
    {
        getAllMotorState(mstateMap, 0);
    }
    void getAllMotorState(::RoboCompJointMotor::MotorStateMap& mstateMap, const ::Ice::Context& __ctx)
    {
        getAllMotorState(mstateMap, &__ctx);
    }
    
private:

    void getAllMotorState(::RoboCompJointMotor::MotorStateMap&, const ::Ice::Context*);
    
public:

    ::RoboCompJointMotor::MotorParamsList getAllMotorParams()
    {
        return getAllMotorParams(0);
    }
    ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Context& __ctx)
    {
        return getAllMotorParams(&__ctx);
    }
    
private:

    ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Context*);
    
public:

    ::RoboCompJointMotor::BusParams getBusParams()
    {
        return getBusParams(0);
    }
    ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Context& __ctx)
    {
        return getBusParams(&__ctx);
    }
    
private:

    ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<JointMotor> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<JointMotor> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<JointMotor*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<JointMotor*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace RoboCompJointMotor
{

class JointMotor : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void setPosition(const ::RoboCompJointMotor::MotorGoalPosition&, const ::Ice::Context*) = 0;

    virtual void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity&, const ::Ice::Context*) = 0;

    virtual void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList&, const ::Ice::Context*) = 0;

    virtual ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RoboCompJointMotor::MotorState getMotorState(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList&, const ::Ice::Context*) = 0;

    virtual void getAllMotorState(::RoboCompJointMotor::MotorStateMap&, const ::Ice::Context*) = 0;

    virtual ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Context*) = 0;

    virtual ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace RoboCompJointMotor
{

class JointMotor : virtual public ::IceDelegate::RoboCompJointMotor::JointMotor,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void setPosition(const ::RoboCompJointMotor::MotorGoalPosition&, const ::Ice::Context*);

    virtual void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity&, const ::Ice::Context*);

    virtual void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorState getMotorState(const ::std::string&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList&, const ::Ice::Context*);

    virtual void getAllMotorState(::RoboCompJointMotor::MotorStateMap&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Context*);

    virtual ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace RoboCompJointMotor
{

class JointMotor : virtual public ::IceDelegate::RoboCompJointMotor::JointMotor,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void setPosition(const ::RoboCompJointMotor::MotorGoalPosition&, const ::Ice::Context*);

    virtual void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity&, const ::Ice::Context*);

    virtual void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorState getMotorState(const ::std::string&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList&, const ::Ice::Context*);

    virtual void getAllMotorState(::RoboCompJointMotor::MotorStateMap&, const ::Ice::Context*);

    virtual ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Context*);

    virtual ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Context*);
};

}

}

namespace RoboCompJointMotor
{

class JointMotor : virtual public ::Ice::Object
{
public:

    typedef JointMotorPrx ProxyType;
    typedef JointMotorPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void setPosition(const ::RoboCompJointMotor::MotorGoalPosition&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setVelocity(const ::RoboCompJointMotor::MotorGoalVelocity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setVelocity(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setSyncPosition(const ::RoboCompJointMotor::MotorGoalPositionList&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setSyncPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RoboCompJointMotor::MotorParams getMotorParams(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getMotorParams(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RoboCompJointMotor::MotorState getMotorState(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getMotorState(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RoboCompJointMotor::MotorStateMap getMotorStateMap(const ::RoboCompJointMotor::MotorList&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getMotorStateMap(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void getAllMotorState(::RoboCompJointMotor::MotorStateMap&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getAllMotorState(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RoboCompJointMotor::MotorParamsList getAllMotorParams(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getAllMotorParams(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RoboCompJointMotor::BusParams getBusParams(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getBusParams(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
