/*
 *
 *  Copyright (C) 1997-2014 JDE Developers Team
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
 *  Author : Robeto Calvo <rocapal@gsyc.urjc.es>
 *
 */


#ifndef NAMING_SERVICE_ICE
#define NAMING_SERVICE_ICE

#include <common.ice>

module jderobot {

	exception NameAlreadyExistException extends JderobotException {};
	exception NameNotExistException extends JderobotException {};
	exception InterfaceNotExistException extends JderobotException {};
	
	class NamingNode
	{
		string name;
		string interfaceName;
		string protocol;
		string ip;
		int port;
		
	};
	
	sequence<NamingNode> nodeList;

	class NodeContainer
	{	
		nodeList nodes;
	};

	interface NamingService
	{
		void bind(NamingNode node) throws NameAlreadyExistException;
		void unbind(NamingNode node) throws NameAlreadyExistException, NameNotExistException;
		
		idempotent NodeContainer resolveByName (string name) throws NameNotExistException;
		idempotent NodeContainer resolveByInterface (string interfaceName) throws InterfaceNotExistException;
	};

}; /*module*/

#endif /*NAMING_SERVICE_ICE*/