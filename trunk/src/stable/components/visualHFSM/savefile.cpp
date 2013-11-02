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

#include "savefile.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
SaveFile::SaveFile ( std::string filepath, std::list<GuiSubautomata>* subautomataList,
												std::list<IceInterface>& listInterfaces,
												std::list<std::string> listLibraries ) {
	this->filepath = std::string(filepath);
	this->subautomataList = subautomataList;
	this->listInterfaces = listInterfaces;
	this->listLibraries = listLibraries;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
SaveFile::~SaveFile () {}

/*************************************************************
 * INITIALIZER
 *************************************************************/
void SaveFile::init () {
//	std::locale::global(std::locale(""));

	#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
	try {
	#endif //LIBXMLCPP_EXCEPTIONS_ENABLED

		xmlpp::Document document;
		xmlpp::Element* nodeRoot = document.create_root_node("VisualHFSM", "", "");

		for ( std::list<GuiSubautomata>::iterator subListIterator = this->subautomataList->begin();
			  subListIterator != this->subautomataList->end(); subListIterator++ ) { // for every subautomata
			xmlpp::Element* nodeSubautomata = nodeRoot->add_child("SubAutomata");
			
			std::stringstream ss;
			ss << subListIterator->getId();
			nodeSubautomata->set_attribute("id", ss.str());
			
			ss.str(std::string());
			ss << subListIterator->getIdFather();
			nodeSubautomata->set_attribute("idFather", ss.str());

			std::list<GuiNode>* listGuiNodes = subListIterator->getListGuiNodes();
			for ( std::list<GuiNode>::iterator guiNodesIterator = listGuiNodes->begin();
					guiNodesIterator != listGuiNodes->end(); guiNodesIterator++ ) {
				xmlpp::Element* nodeState = nodeSubautomata->add_child("state");

				if (guiNodesIterator->itIsInitial())
					nodeState->set_attribute("initial", "true");
				else
					nodeState->set_attribute("initial", "false");

				ss.str(std::string());
				ss << guiNodesIterator->getId();
				nodeState->set_attribute("id", ss.str());

				Point point = guiNodesIterator->getPoint();
				ss.str(std::string());
				ss << point.getX();
				xmlpp::Element* nodeStateChild = nodeState->add_child("posx");
				nodeStateChild->set_child_text(ss.str());
				ss.str(std::string());
				ss << point.getY();
				nodeStateChild = nodeState->add_child("posy");
				nodeStateChild->set_child_text(ss.str());

				nodeStateChild = nodeState->add_child("name");
				nodeStateChild->set_child_text(guiNodesIterator->getName());

				ss.str(std::string());
				ss << guiNodesIterator->getIdSubautomataSon();
				nodeStateChild = nodeState->add_child("idSubautomataSon");
				nodeStateChild->set_child_text(ss.str());

				nodeStateChild = nodeState->add_child("code");
				nodeStateChild->set_child_text(guiNodesIterator->getCode());
			}

			std::list<GuiTransition>* listGuiTransition = subListIterator->getListGuiTransitions();
			for ( std::list<GuiTransition>::iterator guiTransIterator = listGuiTransition->begin();
					guiTransIterator != listGuiTransition->end(); guiTransIterator++ ) {
				xmlpp::Element* nodeTransition = nodeSubautomata->add_child("transition");

				ss.str(std::string());
				ss << guiTransIterator->getId();
				nodeTransition->set_attribute("id", ss.str());

				Point point = guiTransIterator->getPoint();
				ss.str(std::string());
				ss << point.getX();
				xmlpp::Element* nodeTransChild = nodeTransition->add_child("posx");
				nodeTransChild->set_child_text(ss.str());
				ss.str(std::string());
				ss << point.getY();
				nodeTransChild = nodeTransition->add_child("posy");
				nodeTransChild->set_child_text(ss.str());

				ss.str(std::string());
				ss << guiTransIterator->getIdOrigin();
				nodeTransChild = nodeTransition->add_child("origin");
				nodeTransChild->set_child_text(ss.str());

				ss.str(std::string());
				ss << guiTransIterator->getIdDestiny();
				nodeTransChild = nodeTransition->add_child("destiny");
				nodeTransChild->set_child_text(ss.str());

				nodeTransChild = nodeTransition->add_child("name");
				nodeTransChild->set_child_text(guiTransIterator->getName());

				nodeTransChild = nodeTransition->add_child("trans");
				nodeTransChild->set_attribute("type", guiTransIterator->getTypeTrans());
				nodeTransChild->set_child_text(guiTransIterator->getCodeTrans());
			}

			xmlpp::Element* nodeChildSubautomata = nodeSubautomata->add_child("iteration_time");
			nodeChildSubautomata->set_child_text(subListIterator->getTime());

			nodeChildSubautomata = nodeSubautomata->add_child("variables");
			nodeChildSubautomata->set_child_text(subListIterator->getVariables());

			nodeChildSubautomata = nodeSubautomata->add_child("functions");
			nodeChildSubautomata->set_child_text(subListIterator->getFunctions());
		}

		xmlpp::Element* nodeLibs = nodeRoot->add_child("libraries");
		for ( std::list<std::string>::iterator listLibsIterator = this->listLibraries.begin();
				listLibsIterator != this->listLibraries.end(); listLibsIterator++ ) {
			xmlpp::Element* nodelib = nodeLibs->add_child("lib");
			nodelib->set_child_text(*listLibsIterator);
		}

		xmlpp::Element* nodeConfig = nodeRoot->add_child("config");
		for ( std::list<IceInterface>::iterator listInterfacesIterator = this->listInterfaces.begin();
				listInterfacesIterator != this->listInterfaces.end(); listInterfacesIterator++ ) {
			xmlpp::Element* nodeIceInterface = nodeConfig->add_child("iceinterface");

			xmlpp::Element* nodeConfigChild = nodeIceInterface->add_child("nameinterface");
			nodeConfigChild->set_child_text(listInterfacesIterator->getName());

			nodeConfigChild = nodeIceInterface->add_child("ip");
			nodeConfigChild->set_child_text(listInterfacesIterator->getIp());

			nodeConfigChild = nodeIceInterface->add_child("port");
			nodeConfigChild->set_child_text(listInterfacesIterator->getPort());

			nodeConfigChild = nodeIceInterface->add_child("interface");
			nodeConfigChild->set_child_text(listInterfacesIterator->getInterface());
		}

		document.write_to_file(this->filepath, "UTF-8");

	#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
	} catch ( const std::exception& ex ) {
		std::cout << "Exception caught: " << ex.what() << std::endl;
	}
	#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
}