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

#include <jcm.h>
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
jderobot::FQExecutableName::operator==(const FQExecutableName& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(executable != __rhs.executable)
    {
        return false;
    }
    if(host != __rhs.host)
    {
        return false;
    }
    return true;
}

bool
jderobot::FQExecutableName::operator<(const FQExecutableName& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(executable < __rhs.executable)
    {
        return true;
    }
    else if(__rhs.executable < executable)
    {
        return false;
    }
    if(host < __rhs.host)
    {
        return true;
    }
    else if(__rhs.host < host)
    {
        return false;
    }
    return false;
}

void
jderobot::FQExecutableName::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(executable);
    __os->write(host);
}

void
jderobot::FQExecutableName::__read(::IceInternal::BasicStream* __is)
{
    __is->read(executable);
    __is->read(host);
}

bool
jderobot::FQComponentName::operator==(const FQComponentName& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(platform != __rhs.platform)
    {
        return false;
    }
    if(component != __rhs.component)
    {
        return false;
    }
    return true;
}

bool
jderobot::FQComponentName::operator<(const FQComponentName& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(platform < __rhs.platform)
    {
        return true;
    }
    else if(__rhs.platform < platform)
    {
        return false;
    }
    if(component < __rhs.component)
    {
        return true;
    }
    else if(__rhs.component < component)
    {
        return false;
    }
    return false;
}

void
jderobot::FQComponentName::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(platform);
    __os->write(component);
}

void
jderobot::FQComponentName::__read(::IceInternal::BasicStream* __is)
{
    __is->read(platform);
    __is->read(component);
}

bool
jderobot::FQInterfaceName::operator==(const FQInterfaceName& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(platform != __rhs.platform)
    {
        return false;
    }
    if(component != __rhs.component)
    {
        return false;
    }
    if(iface != __rhs.iface)
    {
        return false;
    }
    return true;
}

bool
jderobot::FQInterfaceName::operator<(const FQInterfaceName& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(platform < __rhs.platform)
    {
        return true;
    }
    else if(__rhs.platform < platform)
    {
        return false;
    }
    if(component < __rhs.component)
    {
        return true;
    }
    else if(__rhs.component < component)
    {
        return false;
    }
    if(iface < __rhs.iface)
    {
        return true;
    }
    else if(__rhs.iface < iface)
    {
        return false;
    }
    return false;
}

void
jderobot::FQInterfaceName::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(platform);
    __os->write(component);
    __os->write(iface);
}

void
jderobot::FQInterfaceName::__read(::IceInternal::BasicStream* __is)
{
    __is->read(platform);
    __is->read(component);
    __is->read(iface);
}

bool
jderobot::FQTopicName::operator==(const FQTopicName& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(platform != __rhs.platform)
    {
        return false;
    }
    if(component != __rhs.component)
    {
        return false;
    }
    if(iface != __rhs.iface)
    {
        return false;
    }
    if(topic != __rhs.topic)
    {
        return false;
    }
    return true;
}

bool
jderobot::FQTopicName::operator<(const FQTopicName& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(platform < __rhs.platform)
    {
        return true;
    }
    else if(__rhs.platform < platform)
    {
        return false;
    }
    if(component < __rhs.component)
    {
        return true;
    }
    else if(__rhs.component < component)
    {
        return false;
    }
    if(iface < __rhs.iface)
    {
        return true;
    }
    else if(__rhs.iface < iface)
    {
        return false;
    }
    if(topic < __rhs.topic)
    {
        return true;
    }
    else if(__rhs.topic < topic)
    {
        return false;
    }
    return false;
}

void
jderobot::FQTopicName::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(platform);
    __os->write(component);
    __os->write(iface);
    __os->write(topic);
}

void
jderobot::FQTopicName::__read(::IceInternal::BasicStream* __is)
{
    __is->read(platform);
    __is->read(component);
    __is->read(iface);
    __is->read(topic);
}

bool
jderobot::ProvidedInterface::operator==(const ProvidedInterface& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(id != __rhs.id)
    {
        return false;
    }
    return true;
}

bool
jderobot::ProvidedInterface::operator<(const ProvidedInterface& __rhs) const
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
    if(id < __rhs.id)
    {
        return true;
    }
    else if(__rhs.id < id)
    {
        return false;
    }
    return false;
}

void
jderobot::ProvidedInterface::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(name);
    __os->write(id);
}

void
jderobot::ProvidedInterface::__read(::IceInternal::BasicStream* __is)
{
    __is->read(name);
    __is->read(id);
}

bool
jderobot::RequiredInterface::operator==(const RequiredInterface& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(id != __rhs.id)
    {
        return false;
    }
    return true;
}

bool
jderobot::RequiredInterface::operator<(const RequiredInterface& __rhs) const
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
    if(id < __rhs.id)
    {
        return true;
    }
    else if(__rhs.id < id)
    {
        return false;
    }
    return false;
}

void
jderobot::RequiredInterface::__write(::IceInternal::BasicStream* __os) const
{
    name.__write(__os);
    __os->write(id);
}

void
jderobot::RequiredInterface::__read(::IceInternal::BasicStream* __is)
{
    name.__read(__is);
    __is->read(id);
}

void
jderobot::__writeProvidesInterfaces(::IceInternal::BasicStream* __os, const ::jderobot::ProvidedInterface* begin, const ::jderobot::ProvidedInterface* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
jderobot::__readProvidesInterfaces(::IceInternal::BasicStream* __is, ::jderobot::ProvidesInterfaces& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

void
jderobot::__writeRequiresInterfaces(::IceInternal::BasicStream* __os, const ::jderobot::RequiredInterface* begin, const ::jderobot::RequiredInterface* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
jderobot::__readRequiresInterfaces(::IceInternal::BasicStream* __is, ::jderobot::RequiresInterfaces& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 4);
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
jderobot::ComponentData::operator==(const ComponentData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(provides != __rhs.provides)
    {
        return false;
    }
    if(requires != __rhs.requires)
    {
        return false;
    }
    return true;
}

bool
jderobot::ComponentData::operator<(const ComponentData& __rhs) const
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
    if(provides < __rhs.provides)
    {
        return true;
    }
    else if(__rhs.provides < provides)
    {
        return false;
    }
    if(requires < __rhs.requires)
    {
        return true;
    }
    else if(__rhs.requires < requires)
    {
        return false;
    }
    return false;
}

void
jderobot::ComponentData::__write(::IceInternal::BasicStream* __os) const
{
    name.__write(__os);
    if(provides.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::jderobot::__writeProvidesInterfaces(__os, &provides[0], &provides[0] + provides.size());
    }
    if(requires.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::jderobot::__writeRequiresInterfaces(__os, &requires[0], &requires[0] + requires.size());
    }
}

void
jderobot::ComponentData::__read(::IceInternal::BasicStream* __is)
{
    name.__read(__is);
    ::jderobot::__readProvidesInterfaces(__is, provides);
    ::jderobot::__readRequiresInterfaces(__is, requires);
}
