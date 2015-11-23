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

#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <libxml++/libxml++.h>

#include "guisubautomata.h"
#include "iceinterface.h"

// Definition of this class
class SaveFile {
public:
	// Constructor
	SaveFile ( std::string filepath, std::list<GuiSubautomata>* subautomataList,
				std::list<IceInterface>& listInterfaces, std::list<std::string> listLibraries );

	// Destructor
	virtual ~SaveFile ();

	// Initializer
	void init ();

private:
	// Data structure
	std::string filepath;
	std::list<IceInterface> listInterfaces;
	std::list<GuiSubautomata>* subautomataList;
	std::list<std::string> listLibraries;
};

#endif // SAVEFILE_H