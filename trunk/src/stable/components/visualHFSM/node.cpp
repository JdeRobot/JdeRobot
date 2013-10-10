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

#include "node.h"

/*************************************************************
 * CONSTRUCTORS
 *************************************************************/
Node::Node ( bool initial ) {
	this->initial = initial;
}

Node::Node ( int id ) {
	this->id = id;
}

Node::Node ( int id, bool initial ) {
	this->id = id;
	this->initial = initial;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Node::~Node () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void Node::setId ( int id ) {
	this->id = id;
}

void Node::setIdSubautomataSon ( int idIdSubautomataSon ) {
	this->idSubautomataSon = idIdSubautomataSon;
}

void Node::setInitial ( bool initial ) {
	this->initial = initial;
}

void Node::setCode ( std::string code ) {
	this->code = code;
}

void Node::setName ( std::string name ) {
	this->name = name;
}

/*************************************************************
 * GETTERS
 *************************************************************/
int Node::getId () {
	return this->id;
}

int Node::getIdSubautomataSon () {
	return this->idSubautomataSon;
}

bool Node::isInitial () {
	return this->initial;
}

std::string Node::getCode () {
	return this->code;
}

std::string Node::getName () {
	return this->name;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
Node Node::copy () {
	Node newnode(this->id);
	newnode.setIdSubautomataSon(this->idSubautomataSon);
	newnode.setInitial(this->initial);
	newnode.setCode(this->code);
	newnode.setName(this->name);

	return newnode;
}

Node Node::copy ( int newid ) {
	Node newnode(newid);
	newnode.setIdSubautomataSon(this->idSubautomataSon);
	newnode.setInitial(this->initial);
	newnode.setCode(this->code);
	newnode.setName(this->name);

	return newnode;
}