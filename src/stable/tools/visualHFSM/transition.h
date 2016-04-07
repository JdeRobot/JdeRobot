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
 *  Authors : Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *            José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef TRANSITION_H
#define TRANSITION_H

#include <iostream>
#include <string>

// Definition of this class
class Transition {
public:
	// Constructors
	Transition ();
	Transition ( int id );
	Transition ( int id, int idOrigin, int idDestiny );

	// Destructor
	virtual ~Transition ();

	// Setters
	void setId ( int id );
	void setIdOrigin ( int idOrigin );
	void setIdDestiny ( int idDestiny );
	void setCode ( std::string code );
	void setName ( std::string name );
	void setTrans ( std::string type, std::string code );

	// Getters
	int getId ();
	int getIdOrigin ();
	int getIdDestiny ();
	std::string getCode ();
	std::string getCodeTrans ();
	std::string getName ();
	std::string getType ();

	// Another functions
	Transition copy ();
	Transition copy ( int newid );
private:
	// Data structure
	int id, idOrigin, idDestiny;
	std::string type, code, codeTrans, name;
}; // Class Transition

#endif