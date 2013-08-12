// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `exceptions.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_exceptions_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_exceptions_h__

#include <IceE/ProxyF.h>
#include <IceE/ObjectF.h>
#include <IceE/Exception.h>
#include <IceE/ScopedArray.h>
#include <IceE/UserExceptionFactory.h>
#include <IceE/FactoryTable.h>
#include <IceE/UndefSysMacros.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

namespace jderobot
{

class JderobotException : public ::Ice::UserException
{
public:

    JderobotException() {}
    explicit JderobotException(const ::std::string&);
    virtual ~JderobotException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string what;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

static JderobotException __JderobotException_init;

class ConfigurationNotExistException : public ::jderobot::JderobotException
{
public:

    ConfigurationNotExistException() {}
    explicit ConfigurationNotExistException(const ::std::string&);
    virtual ~ConfigurationNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class DataNotExistException : public ::jderobot::JderobotException
{
public:

    DataNotExistException() {}
    explicit DataNotExistException(const ::std::string&);
    virtual ~DataNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class HardwareFailedException : public ::jderobot::JderobotException
{
public:

    HardwareFailedException() {}
    explicit HardwareFailedException(const ::std::string&);
    virtual ~HardwareFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class NoTopicException : public ::jderobot::JderobotException
{
public:

    NoTopicException() {}
    explicit NoTopicException(const ::std::string&);
    virtual ~NoTopicException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class SubscriptionFailedException : public ::jderobot::JderobotException
{
public:

    SubscriptionFailedException() {}
    explicit SubscriptionFailedException(const ::std::string&);
    virtual ~SubscriptionFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class SubscriptionPushFailedException : public ::jderobot::JderobotException
{
public:

    SubscriptionPushFailedException() {}
    explicit SubscriptionPushFailedException(const ::std::string&);
    virtual ~SubscriptionPushFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

}

#endif
