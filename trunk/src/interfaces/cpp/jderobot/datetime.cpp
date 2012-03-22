// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `datetime.ice'

#include <datetime.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
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
