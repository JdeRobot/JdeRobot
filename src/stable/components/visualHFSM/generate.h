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

#ifndef GENERATE_H
#define GENERATE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "subautomata.h"
#include "iceinterface.h"

 typedef enum TabEnum {
    T_ZERO,
    T_ONE,
    T_TWO,
    T_THREE,
    T_FOUR,
    T_FIVE,
    T_SIX,
    T_SEVEN
} TabEnum;

class Generate {
public:
	// Constructor
	Generate ( std::list<SubAutomata> subautomataList, std::string cpppath,
								std::string cfgpath, std::string cmakepath,
								std::list<IceInterface>* listInterfaces,
								std::map<std::string, std::string> mapInterfacesHeader,
								std::list<std::string> listLibraries );

	// Destructor
	virtual ~Generate ();

	// Another functions
	int init ();
	int init_py ();
private:
	std::list<SubAutomata> subautomataList;
	std::string path, cfgpath, cmakepath;
	std::list<IceInterface>* listInterfaces;
	std::fstream fs;
	std::map<TabEnum, std::string> mapTab;
	std::map<std::string, std::string> mapInterfacesHeader;
	std::list<std::string> listLibraries;

	void generateHeaders ();
	void generateGenericHeaders ();
	void generateSpecificHeaders ();
	void generateEnums ();
	void generateVariables ();
	void generateShutDown();
	void generateFunctions ();
	void generateCreateGuiSubautomataList();
	void generateSubautomatas ();
	void generateAutomataGui();
	void generateMain ();

	void generateCfg ();
	void generateCmake ();

	void generateHeaders_py ();
	void generateGenericHeaders_py();
	void generateSpecificHeaders_py();
	void generateAutomataClass_py();
	void generateAutomataInit_py();
	void generateEnums_py();
	void generateVariables_py();
	void generateShutDown_py();
	void generateSubautomatas_py();
	void generateConnectToProxys_py();
	void generateDestroyIc_py();
	void generateStart_py();
	void generaitJoin_py();
	void generateMain_py();

	std::string getCppName ();

}; // Class Generate

#endif // GENERATE_H