/*
 *  Copyright (C) 2014 JdeRobot developers
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
 *
 *  Authors : Roberto Calvo Palomino <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include "ns.h"

namespace jderobot
{

ns::ns(Ice::CommunicatorPtr& ic, std::string proxy)
{

	Ice::ObjectPrx namingService = ic->stringToProxy(proxy);

	if (0==namingService)
		throw "namingService: Could not create proxy with namingService";

	mNamingService = NamingServicePrx::checkedCast(namingService);

	if (0==mNamingService)
		throw "NamingService: Invalid proxy to remote namingService";
	else
		jderobot::Logger::getInstance()->info("NamingService connection OK! - " + proxy);

}
ns::~ns()
{
}
void ns::bind (std::string name, std::string Endpoint, std::string interface )
{
	NamingNode* node = new NamingNode();

	std::vector<std::string> elems;
	split(Endpoint, ' ', elems);

	node->name = name;
	node->interfaceName = interface;
	node->ip = elems[2];
	node->port = boost::lexical_cast<int>(elems[4]);
	node->protocol = elems[0];

	jderobot::Logger::getInstance()->info("ns::bind:: " + node->name + " - " + node->interfaceName + " - " + node->protocol + " - " +
					node->ip + ":" + boost::lexical_cast<std::string>(node->port) );

	mNamingService->bind(node);

	mBinds.push_back(node->name);
}

void ns::unbind (std::string name)
{

	NamingNodePtr node = new NamingNode();
	node->name = name;

	mNamingService->unbind(node);

	for (std::vector<std::string>::iterator it = mBinds.begin(); it != mBinds.end(); it++)
	{
		if (name.compare(*it))
			mBinds.erase(it);
	}
}

void ns::unbindAll()
{
	NamingNodePtr node = new NamingNode();
	for (std::vector<std::string>::iterator it = mBinds.begin(); it != mBinds.end(); it++)
	{
		node->name = *it;
		mNamingService->unbind(node);
	}
	mBinds.clear();
}

jderobot::NodeContainerPtr ns::resolveByName(std::string name)
{
	return mNamingService->resolveByName(name);
}

jderobot::NodeContainerPtr ns::resolveByInterface(std::string name)
{
	return mNamingService->resolveByInterface(name);
}

std::string ns::getProxyStr (const NamingNode& node)
{
	std::stringstream proxy;
	proxy << node.name << ":" << node.protocol;
	proxy << " -h " << node.ip << " -p " << node.port;

	return proxy.str();
}


std::vector<std::string>& ns::split(const std::string &s, char delim, std::vector<std::string> &elems) {

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}




}
