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

#ifndef GENERATE_H
#define GENERATE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "subautomata.h"

 typedef enum TabEnum {
    T_ZERO,
    T_ONE,
    T_TWO,
    T_THREE,
    T_FOUR,
    T_FIVE,
    T_SIX
} TabEnum;

class Generate {
public:
	// Constructor
	Generate ( std::list<SubAutomata> subautomataList, std::string cpppath,
								std::string cfgpath, std::string cmakepath,
								std::string configfile );

	// Destructor
	virtual ~Generate ();

	// Another functions
	int init ();

private:
	std::list<SubAutomata> subautomataList;
	std::string path, cfgpath, cmakepath, configfile;
	std::fstream fs;
	std::map<TabEnum, std::string> mapTab;

	void generateHeaders ();
	void generateGenericHeaders ();
	void generateSpecificHeaders ();
	void generateEnums ();
	void generateVariables ();
	void generateFunctions ();
	void generateSubautomatas ();
	void generateMain ();

	void generateCfg ();

	void generateCmake ();

	std::string getCppName ();

}; // Class Generate

#endif // GENERATE_H