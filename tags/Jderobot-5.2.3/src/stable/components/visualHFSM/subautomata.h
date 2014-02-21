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

#ifndef SUBAUTOMATA_H
#define SUBAUTOMATA_H

#include <list>
#include <map>
#include <iostream>
#include <string>

#include "node.h"
#include "transition.h"
#include "point.h"

// Definition of this class
class SubAutomata {
public:
	// Constructor
	SubAutomata ( int id, int idFather );

	// Destructor
	virtual ~SubAutomata ();

	// Setters
	void setNodePoint ( int id, Point* p );
	void setTransPoint ( int id, Point* p );

	void setFunctions ( std::string functions );
	void setTime ( std::string timing );
	void setVariables ( std::string variables );
	void setInterfaces ( std::list<std::string>& interfaces );

	// Getters
	int getId ();
	int getIdFather ();

	std::string getFunctions ();
	std::string getNodeName ( int id );
	std::string getTime ();
	std::string getVariables ();
	std::list<std::string>* getInterfaces ();

	std::list<Node> getNodeList ();
	std::list<Transition> getTransList ();

	Point* getNodePoint ( int id );
	Point* getTransPoint ( int id );

	// Another functions
	void addNode ( Node n, Point* p );
	void addTransition ( Transition trans, Point* p );
private:
	// Data structure
	int id, idFather;
	std::map<int, Point*> mapNodePoint, mapTransPoint;
	std::list<Node> nodeList;
	std::list<Transition> transitionList;
	std::string timing, variables, functions;
	std::list<std::string> interfaces;
}; // Class SubAutomata

#endif // SUBAUTOMATA_H