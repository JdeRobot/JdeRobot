// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `datetime.ice'

#include <datetime.h>
#include <IceE/BasicStream.h>
#include <IceE/Object.h>
#include <IceE/Iterator.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 103
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

bool
jderobot::Time::operator==(const Time& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(seconds != __rhs.seconds)
    {
        return false;
    }
    if(useconds != __rhs.useconds)
    {
        return false;
    }
    return true;
}

bool
jderobot::Time::operator<(const Time& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(seconds < __rhs.seconds)
    {
        return true;
    }
    else if(__rhs.seconds < seconds)
    {
        return false;
    }
    if(useconds < __rhs.useconds)
    {
        return true;
    }
    else if(__rhs.useconds < useconds)
    {
        return false;
    }
    return false;
}

void
jderobot::Time::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(seconds);
    __os->write(useconds);
}

void
jderobot::Time::__read(::IceInternal::BasicStream* __is)
{
    __is->read(seconds);
    __is->read(useconds);
}
