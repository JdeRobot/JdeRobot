// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `body.ice'

#include <body.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <Ice/LocalException.h>
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

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::BodySide v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::BodySide& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::jderobot::BodySide>(val);
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::CameraBody v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::CameraBody& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::jderobot::CameraBody>(val);
}

void
jderobot::__write(::IceInternal::BasicStream* __os, ::jderobot::MotorsName v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 10);
}

void
jderobot::__read(::IceInternal::BasicStream* __is, ::jderobot::MotorsName& v)
{
    ::Ice::Byte val;
    __is->read(val, 10);
    v = static_cast< ::jderobot::MotorsName>(val);
}

bool
jderobot::BodyMotor::operator==(const BodyMotor& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(pitch != __rhs.pitch)
    {
        return false;
    }
    if(yaw != __rhs.yaw)
    {
        return false;
    }
    if(roll != __rhs.roll)
    {
        return false;
    }
    return true;
}

bool
jderobot::BodyMotor::operator<(const BodyMotor& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(pitch < __rhs.pitch)
    {
        return true;
    }
    else if(__rhs.pitch < pitch)
    {
        return false;
    }
    if(yaw < __rhs.yaw)
    {
        return true;
    }
    else if(__rhs.yaw < yaw)
    {
        return false;
    }
    if(roll < __rhs.roll)
    {
        return true;
    }
    else if(__rhs.roll < roll)
    {
        return false;
    }
    return false;
}

void
jderobot::BodyMotor::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(pitch);
    __os->write(yaw);
    __os->write(roll);
}

void
jderobot::BodyMotor::__read(::IceInternal::BasicStream* __is)
{
    __is->read(pitch);
    __is->read(yaw);
    __is->read(roll);
}
