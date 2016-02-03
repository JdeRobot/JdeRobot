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
 *      Victor Arribas Raigadas <.varribas.urjc@gmail.com>
 */


#include <Ice/Ice.h>
#include <easyiceconfig/EasyIce.h>
#include <easyiceconfig/proxies.hpp>
#include <easyiceconfig/EasyProxy.h>
#include <easyiceconfig/debug.hpp>
#include <iostream>


int main(int argc, char* argv[]){
    Ice::CommunicatorPtr ic = EasyIce::initialize(argc, argv);
    Ice::PropertiesPtr prop = ic->getProperties();
    prop->setProperty("Test.Adapter.Endpoints", "tcp -h localhost -p 65000");

    Ice::ObjectPtr obj = new IceMX::Metrics();
    Ice::ObjectAdapterPtr adapter = ic->createObjectAdapter("Test.Adapter");
    adapter->add(obj, ic->stringToIdentity("metric"));

    std::cout<<"\n### EXAMPLE with easyiceconfig::proxies::createProxy() ###"<<std::endl;

    /// Exception due empty proxy
    try{
        easyiceconfig::proxies::createProxy<IceMX::MetricsPrx>(ic, "Test.Proxy", false);
    } catch (Ice::ProxyParseException ex){
        std::cout<<"Expected ProxyParseException\n"<<ex<<std::endl;
    }

    IceMX::MetricsPrx pxr;
    /// All ok (1)
    /// Proxy from string do not create entry at Properties
    pxr = easyiceconfig::proxies::createProxy<IceMX::MetricsPrx>(ic, "metric:tcp -h localhost -p 65000", true);
    easyiceconfig::debug::printProperties(prop);

    /// All ok (2)
    prop->setProperty("Test.Proxy", "metric:tcp -h localhost -p 65000");
    pxr = easyiceconfig::proxies::createProxy<IceMX::MetricsPrx>(ic, "Test.Proxy", false);
    easyiceconfig::debug::printProperties(prop);

    // Using it
    std::cout << "Gathering some info" << std::endl;
    std::cout << pxr->ice_id() << std::endl;
    std::cout << pxr->ice_toString() << std::endl;


    std::cout<<"\n### EXAMPLE with EasyIce::EasyProxy ###"<<std::endl;

    EasyIce::EasyProxy<IceMX::MetricsPrx> proxy(ic, "bad endpoint definition", true);
    if (proxy){
        //do stuff
    }else
        std::cout<<proxy.exception()<<std::endl;


    // copying
    EasyIce::EasyProxy<IceMX::MetricsPrx> p2;
    p2 = EasyIce::EasyProxy<IceMX::MetricsPrx>(ic, "bad endpoint definition", true);
    try{
    p2 = EasyIce::createProxy<IceMX::MetricsPrx>(ic, "bad endpoint definition", true);
    }catch(Ice::Exception){}

    ic->shutdown();
}
