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

#include <exceptions.h>
#include <IceE/BasicStream.h>
#include <IceE/Object.h>
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

jderobot::JderobotException::JderobotException(const ::std::string& __ice_what) :
    ::Ice::UserException(),
    what(__ice_what)
{
}

jderobot::JderobotException::~JderobotException() throw()
{
}

static const char* __jderobot__JderobotException_name = "jderobot::JderobotException";

::std::string
jderobot::JderobotException::ice_name() const
{
    return __jderobot__JderobotException_name;
}

::Ice::Exception*
jderobot::JderobotException::ice_clone() const
{
    return new JderobotException(*this);
}

void
jderobot::JderobotException::ice_throw() const
{
    throw *this;
}

void
jderobot::JderobotException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::JderobotException"), false);
    __os->startWriteSlice();
    __os->write(what);
    __os->endWriteSlice();
}

void
jderobot::JderobotException::__read(::IceInternal::BasicStream* __is, bool __rid)
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

struct __F__jderobot__JderobotException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::JderobotException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__JderobotException__Ptr = new __F__jderobot__JderobotException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::JderobotException::ice_factory()
{
    return __F__jderobot__JderobotException__Ptr;
}

class __F__jderobot__JderobotException__Init
{
public:

    __F__jderobot__JderobotException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::JderobotException", ::jderobot::JderobotException::ice_factory());
    }

    ~__F__jderobot__JderobotException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::JderobotException");
    }
};

static __F__jderobot__JderobotException__Init __F__jderobot__JderobotException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__JderobotException__initializer() {} }
#endif

jderobot::ConfigurationNotExistException::ConfigurationNotExistException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::ConfigurationNotExistException::~ConfigurationNotExistException() throw()
{
}

static const char* __jderobot__ConfigurationNotExistException_name = "jderobot::ConfigurationNotExistException";

::std::string
jderobot::ConfigurationNotExistException::ice_name() const
{
    return __jderobot__ConfigurationNotExistException_name;
}

::Ice::Exception*
jderobot::ConfigurationNotExistException::ice_clone() const
{
    return new ConfigurationNotExistException(*this);
}

void
jderobot::ConfigurationNotExistException::ice_throw() const
{
    throw *this;
}

void
jderobot::ConfigurationNotExistException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::ConfigurationNotExistException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::ConfigurationNotExistException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__ConfigurationNotExistException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::ConfigurationNotExistException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__ConfigurationNotExistException__Ptr = new __F__jderobot__ConfigurationNotExistException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::ConfigurationNotExistException::ice_factory()
{
    return __F__jderobot__ConfigurationNotExistException__Ptr;
}

class __F__jderobot__ConfigurationNotExistException__Init
{
public:

    __F__jderobot__ConfigurationNotExistException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::ConfigurationNotExistException", ::jderobot::ConfigurationNotExistException::ice_factory());
    }

    ~__F__jderobot__ConfigurationNotExistException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::ConfigurationNotExistException");
    }
};

static __F__jderobot__ConfigurationNotExistException__Init __F__jderobot__ConfigurationNotExistException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__ConfigurationNotExistException__initializer() {} }
#endif

jderobot::DataNotExistException::DataNotExistException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::DataNotExistException::~DataNotExistException() throw()
{
}

static const char* __jderobot__DataNotExistException_name = "jderobot::DataNotExistException";

::std::string
jderobot::DataNotExistException::ice_name() const
{
    return __jderobot__DataNotExistException_name;
}

::Ice::Exception*
jderobot::DataNotExistException::ice_clone() const
{
    return new DataNotExistException(*this);
}

void
jderobot::DataNotExistException::ice_throw() const
{
    throw *this;
}

void
jderobot::DataNotExistException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::DataNotExistException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::DataNotExistException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__DataNotExistException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::DataNotExistException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__DataNotExistException__Ptr = new __F__jderobot__DataNotExistException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::DataNotExistException::ice_factory()
{
    return __F__jderobot__DataNotExistException__Ptr;
}

class __F__jderobot__DataNotExistException__Init
{
public:

    __F__jderobot__DataNotExistException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::DataNotExistException", ::jderobot::DataNotExistException::ice_factory());
    }

    ~__F__jderobot__DataNotExistException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::DataNotExistException");
    }
};

static __F__jderobot__DataNotExistException__Init __F__jderobot__DataNotExistException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__DataNotExistException__initializer() {} }
#endif

jderobot::HardwareFailedException::HardwareFailedException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::HardwareFailedException::~HardwareFailedException() throw()
{
}

static const char* __jderobot__HardwareFailedException_name = "jderobot::HardwareFailedException";

::std::string
jderobot::HardwareFailedException::ice_name() const
{
    return __jderobot__HardwareFailedException_name;
}

::Ice::Exception*
jderobot::HardwareFailedException::ice_clone() const
{
    return new HardwareFailedException(*this);
}

void
jderobot::HardwareFailedException::ice_throw() const
{
    throw *this;
}

void
jderobot::HardwareFailedException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::HardwareFailedException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::HardwareFailedException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__HardwareFailedException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::HardwareFailedException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__HardwareFailedException__Ptr = new __F__jderobot__HardwareFailedException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::HardwareFailedException::ice_factory()
{
    return __F__jderobot__HardwareFailedException__Ptr;
}

class __F__jderobot__HardwareFailedException__Init
{
public:

    __F__jderobot__HardwareFailedException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::HardwareFailedException", ::jderobot::HardwareFailedException::ice_factory());
    }

    ~__F__jderobot__HardwareFailedException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::HardwareFailedException");
    }
};

static __F__jderobot__HardwareFailedException__Init __F__jderobot__HardwareFailedException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__HardwareFailedException__initializer() {} }
#endif

jderobot::NoTopicException::NoTopicException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::NoTopicException::~NoTopicException() throw()
{
}

static const char* __jderobot__NoTopicException_name = "jderobot::NoTopicException";

::std::string
jderobot::NoTopicException::ice_name() const
{
    return __jderobot__NoTopicException_name;
}

::Ice::Exception*
jderobot::NoTopicException::ice_clone() const
{
    return new NoTopicException(*this);
}

void
jderobot::NoTopicException::ice_throw() const
{
    throw *this;
}

void
jderobot::NoTopicException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::NoTopicException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::NoTopicException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__NoTopicException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::NoTopicException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__NoTopicException__Ptr = new __F__jderobot__NoTopicException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::NoTopicException::ice_factory()
{
    return __F__jderobot__NoTopicException__Ptr;
}

class __F__jderobot__NoTopicException__Init
{
public:

    __F__jderobot__NoTopicException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::NoTopicException", ::jderobot::NoTopicException::ice_factory());
    }

    ~__F__jderobot__NoTopicException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::NoTopicException");
    }
};

static __F__jderobot__NoTopicException__Init __F__jderobot__NoTopicException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__NoTopicException__initializer() {} }
#endif

jderobot::SubscriptionFailedException::SubscriptionFailedException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::SubscriptionFailedException::~SubscriptionFailedException() throw()
{
}

static const char* __jderobot__SubscriptionFailedException_name = "jderobot::SubscriptionFailedException";

::std::string
jderobot::SubscriptionFailedException::ice_name() const
{
    return __jderobot__SubscriptionFailedException_name;
}

::Ice::Exception*
jderobot::SubscriptionFailedException::ice_clone() const
{
    return new SubscriptionFailedException(*this);
}

void
jderobot::SubscriptionFailedException::ice_throw() const
{
    throw *this;
}

void
jderobot::SubscriptionFailedException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::SubscriptionFailedException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::SubscriptionFailedException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__SubscriptionFailedException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::SubscriptionFailedException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__SubscriptionFailedException__Ptr = new __F__jderobot__SubscriptionFailedException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::SubscriptionFailedException::ice_factory()
{
    return __F__jderobot__SubscriptionFailedException__Ptr;
}

class __F__jderobot__SubscriptionFailedException__Init
{
public:

    __F__jderobot__SubscriptionFailedException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::SubscriptionFailedException", ::jderobot::SubscriptionFailedException::ice_factory());
    }

    ~__F__jderobot__SubscriptionFailedException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::SubscriptionFailedException");
    }
};

static __F__jderobot__SubscriptionFailedException__Init __F__jderobot__SubscriptionFailedException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__SubscriptionFailedException__initializer() {} }
#endif

jderobot::SubscriptionPushFailedException::SubscriptionPushFailedException(const ::std::string& __ice_what) :
    ::jderobot::JderobotException(__ice_what)
{
}

jderobot::SubscriptionPushFailedException::~SubscriptionPushFailedException() throw()
{
}

static const char* __jderobot__SubscriptionPushFailedException_name = "jderobot::SubscriptionPushFailedException";

::std::string
jderobot::SubscriptionPushFailedException::ice_name() const
{
    return __jderobot__SubscriptionPushFailedException_name;
}

::Ice::Exception*
jderobot::SubscriptionPushFailedException::ice_clone() const
{
    return new SubscriptionPushFailedException(*this);
}

void
jderobot::SubscriptionPushFailedException::ice_throw() const
{
    throw *this;
}

void
jderobot::SubscriptionPushFailedException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::jderobot::SubscriptionPushFailedException"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
    ::jderobot::JderobotException::__write(__os);
}

void
jderobot::SubscriptionPushFailedException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
    ::jderobot::JderobotException::__read(__is, true);
}

struct __F__jderobot__SubscriptionPushFailedException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::jderobot::SubscriptionPushFailedException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__jderobot__SubscriptionPushFailedException__Ptr = new __F__jderobot__SubscriptionPushFailedException;

const ::IceInternal::UserExceptionFactoryPtr&
jderobot::SubscriptionPushFailedException::ice_factory()
{
    return __F__jderobot__SubscriptionPushFailedException__Ptr;
}

class __F__jderobot__SubscriptionPushFailedException__Init
{
public:

    __F__jderobot__SubscriptionPushFailedException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::jderobot::SubscriptionPushFailedException", ::jderobot::SubscriptionPushFailedException::ice_factory());
    }

    ~__F__jderobot__SubscriptionPushFailedException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::jderobot::SubscriptionPushFailedException");
    }
};

static __F__jderobot__SubscriptionPushFailedException__Init __F__jderobot__SubscriptionPushFailedException__i;

#ifdef __APPLE__
extern "C" { void __F__jderobot__SubscriptionPushFailedException__initializer() {} }
#endif
