/*
 *  Copyright (C) 2015 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *  Authors :
 *       Victor Arribas Raigadas <.varribas.urjc@gmail.com>
 */

#ifndef EASYICECONFIG_EASYPROXY_H
#define EASYICECONFIG_EASYPROXY_H


#include <easyiceconfig/proxies.hpp>

namespace EasyIce{

/// http://en.cppreference.com/w/cpp/language/using_declaration
/// Option 1
///   just add this element to current namespace
//using easyiceconfig::proxies::EasyProxy;

/// Option 2 (fallback)
///   import whole namespace in current one
using namespace easyiceconfig::proxies;


}//NS


#endif // EASYICECONFIG_EASYPROXY_H

