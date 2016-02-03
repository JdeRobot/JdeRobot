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

#ifndef EASYICECONFIG_PROXIES__HPP
#define EASYICECONFIG_PROXIES__HPP

#include <Ice/Communicator.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <boost/shared_ptr.hpp>

namespace easyiceconfig {
namespace proxies{


template<typename Prx>
/**
 * @brief A proxy creator that raise any error/fail as exception
 * @return a valid proxy
 */
Prx
createProxy(const Ice::CommunicatorPtr &ic, const std::string proxyStr, bool stringIsAlreadyProxy){
    Ice::ObjectPrx base;

    if (stringIsAlreadyProxy){
        base = ic->stringToProxy(proxyStr); // Return: The proxy, or nil if str is an empty string.
    }else{
        base = ic->propertyToProxy(proxyStr); // Return: The proxy, or nil if property do not exist.
    }

    if (0 == base){
        throw Ice::ProxyParseException("easyiceconfig/proxy.hpp", 47, "Proxy arguments unmet: passed configuration is empty.");
    }

    // unsafe (try-catch) to maintain original Exceptions
    Prx proxy = Prx::checkedCast(base);

    return proxy;
}




template<typename Prx>
/**
 * EasyProxy simplifies Ice Proxy creation and exception managenemt.
 * It handles exceptions to provide a simple if scoped logic.
 * On error, original exception is retained allowing printing and throwing.
 *
 * Usage:
 *   EasyProxy<ClassPrx> proxy(ic,"X.Proxy", false)
 *   if (proxy)
 *     // do stuff
 *   else
 *     std::cout<<proxy.exception()
 *
 */
class EasyProxy {
public:
    /**
     * Constructor to allow lazy initialization
     */
    EasyProxy(){}

    EasyProxy(const Ice::CommunicatorPtr &ic, const std::string proxyStr, bool stringIsAlreadyProxy){
        try {
            proxy = easyiceconfig::proxies::createProxy<Prx>(ic, proxyStr, stringIsAlreadyProxy);
        }catch (Ice::Exception& e){
            ex = boost::shared_ptr<Ice::Exception>(e.ice_clone());
        }
    }

    virtual ~EasyProxy(){
    }

    operator bool (){
        return proxy != 0;
    }

    /**
     * key operator to make class transparent by implement "proxy pattern"
     * @return exactly same as templated class
     */
    typename Prx::element_type* operator-> () const{
        return proxy.operator->();
    }

    /**
     * complements "proxy pattern" by allow direct assignment.
     */
    Prx& operator= (const Prx &prx){
        proxy = prx;
        return proxy;
    }

    EasyProxy<Prx>& operator= (const EasyProxy<Prx> &other){
        proxy = other.proxy;
        ex = other.ex;

        return *this;
    }

    Ice::Exception& exception(){
        assert(ex != 0);
        return *ex;
    }


protected:
    Prx proxy;
    boost::shared_ptr<Ice::Exception> ex;

};

}} //NS

#endif // EASYICECONFIG_PROXIES__HPP

