// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.3.0
// Generated from file `containers.ice'

#ifndef ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_containers_h__
#define ___users_eperdices_GO2012_naoserver_src_interfaces_cpp_jderobot_containers_h__

#include <IceE/ProxyF.h>
#include <IceE/ObjectF.h>
#include <IceE/Exception.h>
#include <IceE/ScopedArray.h>
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

typedef ::std::vector< ::Ice::Byte> ByteSeq;

typedef ::std::vector< ::Ice::Int> IntSeq;

typedef ::std::vector< ::Ice::Float> seqFloat;

}

#endif
