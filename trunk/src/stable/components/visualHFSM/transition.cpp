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

#include "transition.h"

/*************************************************************
 * CONSTRUCTORS
 *************************************************************/
Transition::Transition () {}

Transition::Transition ( int id ) {
	this->id = id;
}

Transition::Transition ( int id, int idOrigin, int idDestiny ) {
	this->id = id;
	this->idOrigin = idOrigin;
	this->idDestiny = idDestiny;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Transition::~Transition () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void Transition::setId ( int id ) {
	this->id = id;
}

void Transition::setIdOrigin ( int idOrigin ) {
	this->idOrigin = idOrigin;
}

void Transition::setIdDestiny ( int idDestiny ) {
	this->idDestiny = idDestiny;
}

void Transition::setName ( std::string name ) {
	this->name = name;
}

void Transition::setTrans ( std::string type, std::string code ) {
	this->type = type;
	this->code = code;
}

/*************************************************************
 * GETTERS
 *************************************************************/
int Transition::getId () {
	return this->id;
}

int Transition::getIdOrigin () {
	return this->idOrigin;
}

int Transition::getIdDestiny () {
	return this->idDestiny;
}

std::string Transition::getCode () {
	return this->code;
}

std::string Transition::getName () {
	return this->name;
}

std::string Transition::getType () {
	return this->type;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
Transition Transition::copy () {
	Transition newtransition(this->id);
	newtransition.setIdOrigin(this->idOrigin);
	newtransition.setIdDestiny(this->idDestiny);
	newtransition.setName(this->name);
	newtransition.setTrans(this->type, this->code);

	return newtransition;
}

Transition Transition::copy ( int newid ) {
	Transition newtransition(newid);
	newtransition.setIdOrigin(this->idOrigin);
	newtransition.setIdDestiny(this->idDestiny);
	newtransition.setName(this->name);
	newtransition.setTrans(this->type, this->code);

	return newtransition;
}