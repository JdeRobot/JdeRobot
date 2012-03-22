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

#ifndef __body_h__
#define __body_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <jderobot/common.h>
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

namespace jderobot
{

enum BodySide
{
    Left,
    Right
};

void __write(::IceInternal::BasicStream*, BodySide);
void __read(::IceInternal::BasicStream*, BodySide&);

enum CameraBody
{
    Top,
    Bottom
};

void __write(::IceInternal::BasicStream*, CameraBody);
void __read(::IceInternal::BasicStream*, CameraBody&);

enum MotorsName
{
    HipYawPitch,
    HipPitch,
    HipRoll,
    KneePitch,
    AnklePitch,
    AnkleRoll,
    ShoulderPitch,
    ShoulderRoll,
    ElbowYaw,
    ElbowRoll
};

void __write(::IceInternal::BasicStream*, MotorsName);
void __read(::IceInternal::BasicStream*, MotorsName&);

struct BodyMotor
{
    ::Ice::Float pitch;
    ::Ice::Float yaw;
    ::Ice::Float roll;

    bool operator==(const BodyMotor&) const;
    bool operator<(const BodyMotor&) const;
    bool operator!=(const BodyMotor& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const BodyMotor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const BodyMotor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const BodyMotor& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

}

#endif
