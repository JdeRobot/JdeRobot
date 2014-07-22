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

#include "subautomata.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
SubAutomata::SubAutomata ( int id, int idFather ) {
	this->id = id;
	this->idFather = idFather;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
SubAutomata::~SubAutomata () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void SubAutomata::setNodePoint ( int id, Point* p ) {
	this->mapNodePoint[id] = p;
}

void SubAutomata::setTransPoint ( int id, Point* p ) {
	this->mapTransPoint[id] = p;
}

void SubAutomata::setFunctions ( std::string functions ) {
    this->functions = functions;
}

void SubAutomata::setTime ( std::string timing ) {
    this->timing = timing;
}

void SubAutomata::setVariables ( std::string variables ) {
    this->variables = variables;
}

void SubAutomata::setInterfaces ( std::list<std::string>& interfaces ) {
    this->interfaces = interfaces;
}

/*************************************************************
 * GETTERS
 *************************************************************/
int SubAutomata::getId () {
	return this->id;
}

int SubAutomata::getIdFather () {
	return this->idFather;
}

std::string SubAutomata::getFunctions () {
    return this->functions;
}

std::string SubAutomata::getNodeName ( int id ) {
	std::string name = std::string();
	std::list<Node>::iterator nodeListIterator = nodeList.begin();
	while ( (nodeListIterator->getId() != id) &&
			(nodeListIterator != nodeList.end()) )
		nodeListIterator++;

	if (nodeListIterator != nodeList.end())
		name = nodeListIterator->getName();

	return name;
}

std::string SubAutomata::getTime () {
    return this->timing;
}

std::string SubAutomata::getVariables () {
    return this->variables;
}

std::list<std::string>* SubAutomata::getInterfaces () {
    return &this->interfaces;
}

std::list<Node> SubAutomata::getNodeList () {
	return this->nodeList;
}

std::list<Transition> SubAutomata::getTransList () {
	return this->transitionList;
}

Point* SubAutomata::getNodePoint ( int id ) {
	std::map<int, Point*>::iterator itmap = this->mapNodePoint.begin();
	while (itmap != this->mapNodePoint.end()) {
		if (itmap->first == id)
			return itmap->second;
		itmap++;
	}

	return NULL;
}

Point* SubAutomata::getTransPoint ( int id ) {
	std::map<int, Point*>::iterator itmap = this->mapTransPoint.begin();
	while (itmap != this->mapNodePoint.end()) {
		if (itmap->first == id)
			return itmap->second;
		itmap++;
	}

	return NULL;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void SubAutomata::addNode ( Node n, Point* p ) {
	this->nodeList.push_back(n);
	this->mapNodePoint[n.getId()] = p;
}

void SubAutomata::addTransition ( Transition trans, Point* p ) {
	this->transitionList.push_back(trans);
	this->mapTransPoint[trans.getId()] = p;
}