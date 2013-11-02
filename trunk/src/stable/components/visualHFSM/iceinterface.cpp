/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "iceinterface.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
IceInterface::IceInterface () {}

IceInterface::IceInterface ( std::string name, std::string ip, std::string port, std::string interface ) {
	this->name = name;
	this->ip = ip;
	this->port = port;
	this->interface = interface;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
IceInterface::~IceInterface () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void IceInterface::setAll ( std::string name, std::string ip, std::string port, std::string interface ) {
	this->name = name;
	this->ip = ip;
	this->port = port;
	this->interface = interface;
}

void IceInterface::setName ( std::string name ) {
	this->name = name;
}

void IceInterface::setIp ( std::string ip ) {
	this->ip = ip;
}

void IceInterface::setPort ( std::string port ) {
	this->port = port;
}

void IceInterface::setInterface ( std::string interface ) {
	this->interface = interface;
}

/*************************************************************
 * GETTERS
 *************************************************************/
std::string IceInterface::getName () {
	return this->name;
}

std::string IceInterface::getIp () {
	return this->ip;
}

std::string IceInterface::getPort () {
	return this->port;
}

std::string IceInterface::getInterface () {
	return this->interface;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
bool IceInterface::equals ( IceInterface* iceinterface ) {
	return (this->name.compare(iceinterface->getName()) == 0);
}

bool IceInterface::equals ( std::string name, std::string ip,
							std::string port, std::string interface ) {
	return ( (this->name.compare(name) == 0) && (this->ip.compare(ip) == 0) &&
			(this->port.compare(port) == 0) && (this->interface.compare(interface) == 0) );
}