// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `jcm.ice'

#ifndef __jcm_h__
#define __jcm_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
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

struct FQExecutableName
{
    ::std::string executable;
    ::std::string host;

    bool operator==(const FQExecutableName&) const;
    bool operator<(const FQExecutableName&) const;
    bool operator!=(const FQExecutableName& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const FQExecutableName& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const FQExecutableName& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const FQExecutableName& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct FQComponentName
{
    ::std::string platform;
    ::std::string component;

    bool operator==(const FQComponentName&) const;
    bool operator<(const FQComponentName&) const;
    bool operator!=(const FQComponentName& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const FQComponentName& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const FQComponentName& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const FQComponentName& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct FQInterfaceName
{
    ::std::string platform;
    ::std::string component;
    ::std::string iface;

    bool operator==(const FQInterfaceName&) const;
    bool operator<(const FQInterfaceName&) const;
    bool operator!=(const FQInterfaceName& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const FQInterfaceName& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const FQInterfaceName& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const FQInterfaceName& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct FQTopicName
{
    ::std::string platform;
    ::std::string component;
    ::std::string iface;
    ::std::string topic;

    bool operator==(const FQTopicName&) const;
    bool operator<(const FQTopicName&) const;
    bool operator!=(const FQTopicName& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const FQTopicName& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const FQTopicName& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const FQTopicName& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct ProvidedInterface
{
    ::std::string name;
    ::std::string id;

    bool operator==(const ProvidedInterface&) const;
    bool operator<(const ProvidedInterface&) const;
    bool operator!=(const ProvidedInterface& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ProvidedInterface& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ProvidedInterface& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ProvidedInterface& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct RequiredInterface
{
    ::jderobot::FQInterfaceName name;
    ::std::string id;

    bool operator==(const RequiredInterface&) const;
    bool operator<(const RequiredInterface&) const;
    bool operator!=(const RequiredInterface& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const RequiredInterface& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const RequiredInterface& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const RequiredInterface& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::jderobot::ProvidedInterface> ProvidesInterfaces;
void __writeProvidesInterfaces(::IceInternal::BasicStream*, const ::jderobot::ProvidedInterface*, const ::jderobot::ProvidedInterface*);
void __readProvidesInterfaces(::IceInternal::BasicStream*, ProvidesInterfaces&);

typedef ::std::vector< ::jderobot::RequiredInterface> RequiresInterfaces;
void __writeRequiresInterfaces(::IceInternal::BasicStream*, const ::jderobot::RequiredInterface*, const ::jderobot::RequiredInterface*);
void __readRequiresInterfaces(::IceInternal::BasicStream*, RequiresInterfaces&);

struct ComponentData
{
    ::jderobot::FQComponentName name;
    ::jderobot::ProvidesInterfaces provides;
    ::jderobot::RequiresInterfaces requires;

    bool operator==(const ComponentData&) const;
    bool operator<(const ComponentData&) const;
    bool operator!=(const ComponentData& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ComponentData& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ComponentData& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ComponentData& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

}

#endif
