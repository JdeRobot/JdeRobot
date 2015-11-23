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

#include "NamingServiceJdeRobot.h"

namespace NamingService
{

const std::string serializeFile = "TableNamingService.jde";

NamingServiceJdeRobot::NamingServiceJdeRobot(std::string& propertyPrefix): mNodes(new jderobot::NodeContainer())
{
	load();
}

void NamingServiceJdeRobot::bind (const jderobot::NamingNodePtr& node, const Ice::Current&)
{
	if (exists(node))
	{
		jderobot::Logger::getInstance()->warning("Name already bind " + node->name + " replace it ...");
		remove(node);
	}

	mNodes->nodes.push_back(node);

	jderobot::Logger::getInstance()->info("Bind:: " + node->name + " - " + node->interfaceName + " - " + node->protocol + " - " +
			node->ip + ":" + boost::lexical_cast<std::string>(node->port) );

	save();
}

void NamingServiceJdeRobot::unbind (const jderobot::NamingNodePtr& node, const Ice::Current& )
{
	if (! exists(node))
	{
		jderobot::Logger::getInstance()->error("Name not exist to unbind " + node->name);
		jderobot::NameNotExistException ex ("Name not exist to unbind " + node->name);
		throw ex;
	}

	if (remove(node))
	{
		jderobot::Logger::getInstance()->info("unbind:: " + node->name + " - " + node->interfaceName + " - " + node->protocol + " - " +
				node->ip + ":" + boost::lexical_cast<std::string>(node->port) );

		save();
	}
	else
		jderobot::Logger::getInstance()->error("unbind:: there was an error with unbind node" );

}

jderobot::NodeContainerPtr NamingServiceJdeRobot::resolveByName (const std::string& name, const Ice::Current&)
{

	jderobot::NodeContainerPtr result = new jderobot::NodeContainer();

	for (std::vector<jderobot::NamingNodePtr>::iterator it = mNodes->nodes.begin(); it != mNodes->nodes.end(); it++)
	{
		if (it->_ptr->name.compare(name)==0)
			result->nodes.push_back(mNodes->nodes[it - mNodes->nodes.begin()]);
	}

	if (result->nodes.size() == 0)
	{
		jderobot::Logger::getInstance()->error("Name not exist to resolveByName " + name);
		jderobot::NameNotExistException ex ("Name not exist to resolveByName " + name);
		throw ex;
	}

	return result;
}

jderobot::NodeContainerPtr NamingServiceJdeRobot::resolveByInterface (const std::string& interface, const Ice::Current&)
{
	jderobot::NodeContainerPtr result = new jderobot::NodeContainer();

	for (std::vector<jderobot::NamingNodePtr>::iterator it = mNodes->nodes.begin(); it != mNodes->nodes.end(); it++)
	{
		if (it->_ptr->interfaceName.compare(interface) == 0)
			result->nodes.push_back(mNodes->nodes[it - mNodes->nodes.begin()]);
	}

	if (result->nodes.size() == 0)
	{
		jderobot::Logger::getInstance()->error("Interface not exist to resolveByInterface " + interface);
		jderobot::InterfaceNotExistException ex ("Interface not exist to resolveByInterface " + interface);
		throw ex;
	}

	return result;
}

bool NamingServiceJdeRobot::exists (const jderobot::NamingNodePtr& node)
{
	for (std::vector<jderobot::NamingNodePtr>::iterator it = mNodes->nodes.begin(); it != mNodes->nodes.end(); it++)
	{
		if (node->name.compare(it->_ptr->name)==0)
			return true;
	}
	return false;
}

bool NamingServiceJdeRobot::remove (const jderobot::NamingNodePtr& node)
{
	for (std::vector<jderobot::NamingNodePtr>::iterator it = mNodes->nodes.begin(); it != mNodes->nodes.end(); it++)
	{
			if (node->name.compare(it->_ptr->name)==0)
		{
			mNodes->nodes.erase(it);
			return true;
		}
	}
	return false;
}

bool NamingServiceJdeRobot::save ()
{
	std::ofstream out(serializeFile.c_str());

	for (std::vector<jderobot::NamingNodePtr>::iterator it = mNodes->nodes.begin(); it != mNodes->nodes.end(); it++)
	{
		std::stringstream ss;
		ss << it->_ptr->name << "&" << it->_ptr->interfaceName << "&" << it->_ptr->ip << "&" << it->_ptr->port << "&" << it->_ptr->protocol << std::endl;
		out.write(ss.str().c_str(), ss.str().length());
	}
	out.close();
	return true;
}

bool NamingServiceJdeRobot::load ()
{
	std::ifstream in(serializeFile.c_str());

	jderobot::Logger::getInstance()->info ("Trying to get persistent data from " + serializeFile);
	std::string line;
	while (std::getline(in, line))
	{
		std::vector<std::string> elems;
		split(line, '&', elems);

		if (elems.size() != 5)
		{
			jderobot::Logger::getInstance()->warning("Restore error line (not process): " + line);
			continue;
		}

		jderobot::NamingNodePtr node = new jderobot::NamingNode();
		node->name = elems[0];
		node->interfaceName = elems[1];
		node->ip = elems[2];
		node->port = boost::lexical_cast<int>(elems[3]);
		node->protocol = elems[4];

		mNodes->nodes.push_back(node);

		jderobot::Logger::getInstance()->info("Restore node: " + node->name + " - " + node->interfaceName + " - " + node->protocol + " - " +
					node->ip + ":" + boost::lexical_cast<std::string>(node->port) );

	}
	return true;
}

std::vector<std::string>& NamingServiceJdeRobot::split(const std::string &s, char delim, std::vector<std::string> &elems) {

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


}
