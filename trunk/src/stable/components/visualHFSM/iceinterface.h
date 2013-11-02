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

#ifndef ICEINTERFACE_H
#define ICEINTERFACE_H

#include <iostream>
#include <stdio.h>

class IceInterface {
public:
	// Constructor
	IceInterface ();
	IceInterface ( std::string name, std::string ip, std::string port, std::string interface );

	// Destructor
	virtual ~IceInterface ();

	// Setters
	void setAll ( std::string name, std::string ip, std::string port, std::string interface );
	void setName ( std::string name );
	void setIp ( std::string ip );
	void setPort ( std::string port );
	void setInterface ( std::string interface );

	// Getters
	std::string getName ();
	std::string getIp ();
	std::string getPort ();
	std::string getInterface ();

	// Another functions
	bool equals ( IceInterface* iceinterface );
	bool equals ( std::string name, std::string ip, std::string port, std::string interface );

private:
	std::string name, ip, port, interface;
};

#endif // ICEINTERFACE_H