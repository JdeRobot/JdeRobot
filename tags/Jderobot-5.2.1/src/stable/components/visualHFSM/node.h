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

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

// Definition of this class
class Node {
public:
	// Constructors
	Node ( bool initial );
	Node ( int id );
	Node ( int id, bool initial );

	// Destructor
	virtual ~Node ();

	// Setters
	void setId ( int id );
	void setIdSubautomataSon ( int idSubautomataSon );
	void setInitial ( bool initial );
	void setCode ( std::string code );
	void setName ( std::string name );

	// Getters
	int getId ();
	int getIdSubautomataSon ();
	
	bool isInitial ();
	
	std::string getCode ();
	std::string getName ();

	// Another functions
	Node copy ();
	Node copy ( int newid );
private:
	// Data structure
	int id, idSubautomataSon;
	bool initial;
	std::string name, code;
}; // Class Node

#endif