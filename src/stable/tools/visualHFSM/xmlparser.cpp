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

#include "xmlparser.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
MySaxParser::MySaxParser () : xmlpp::SaxParser() {
    this->mapStringValues["VisualHFSM"] = E_VISUALHFSM;
    this->mapStringValues["SubAutomata"] = E_SUBAUTOMATA;
    this->mapStringValues["state"] = E_STATE;
    this->mapStringValues["posx"] = E_POSX;
    this->mapStringValues["posy"] = E_POSY;
    this->mapStringValues["name"] = E_NAME;
    this->mapStringValues["idSubautomataSon"] = E_IDSUBAUTOMATASON;
    this->mapStringValues["code"] = E_CODE;
    this->mapStringValues["transition"] = E_TRANSITION;
    this->mapStringValues["origin"] = E_ORIGIN;
    this->mapStringValues["destiny"] = E_DESTINY;
    this->mapStringValues["transcode"] = E_TRANSCODE;
    this->mapStringValues["trans"] = E_TRANS;
    this->mapStringValues["libraries"] = E_LIBRARIES;
    this->mapStringValues["lib"] = E_LIB;
    this->mapStringValues["iteration_time"] = E_ITERATION;
    this->mapStringValues["variables"] = E_VARS;
    this->mapStringValues["functions"] = E_FUNCTIONS;
    this->mapStringValues["config"] = E_CONFIGFILE;
    this->mapStringValues["iceinterface"] = E_ICEINTERFACE;
    this->mapStringValues["nameinterface"] = E_INTERFACENAME;
    this->mapStringValues["ip"] = E_INTERFACEIP;
    this->mapStringValues["port"] = E_INTERFACEPORT;
    this->mapStringValues["interface"] = E_INTERFACEINTERFACE;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
MySaxParser::~MySaxParser () {}

/*************************************************************
 * GETTERS
 *************************************************************/
std::list<SubAutomata> MySaxParser::getListSubautomata () {
    return this->subautomataList;
}

std::list<IceInterface>& MySaxParser::getConfigFile () {
    return this->listConfig;
}

std::list<std::string> MySaxParser::getListLibs () {
    return this->listLibraries;
}

/*************************************************************
 * OVERRIDES
 *************************************************************/
void MySaxParser::on_start_document () {
//    std::cout << "on_start_document()" << std::endl;
}

void MySaxParser::on_end_document () {
//    std::cout << "on_end_document()" << std::endl;
}

void MySaxParser::on_start_element ( const Glib::ustring& name,
                                     const AttributeList& attributes ) {
    switch (this->mapStringValues[name.c_str()]) {
        case E_VISUALHFSM :
            this->option = E_VISUALHFSM;
            break;
        case E_SUBAUTOMATA: {
            int idSubautomata, idFather;
            for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin();
                                                            iter != attributes.end(); iter++) {
                if (iter->name.compare("id") == 0) {
                    std::stringstream toint(iter->value);
                    toint >> idSubautomata;
                } else if (iter->name.compare("idFather") == 0) {
                    std::stringstream toint(iter->value);
                    toint >> idFather;
                }
            }
            SubAutomata newsubautomata(idSubautomata, idFather);
            this->subautomataList.push_back(newsubautomata);
            this->subautomata = this->getSubautomataWithId(idSubautomata);
            this->option = E_SUBAUTOMATA;
            break;
        }
        case E_STATE: {
            int idNode;
            bool initial;
            for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin();
                                                            iter != attributes.end(); iter++) {
                if (iter->name.compare("id") == 0) {
                    std::stringstream toint(iter->value);
                    toint >> idNode;
                } else if (iter->name.compare("initial") == 0) {
                    if (iter->value.compare("true") == 0)
                        initial = true;
                    else
                        initial = false;
                }
            }
            this->node = new Node(idNode, initial);
            this->state = true;
            this->option = E_STATE;
            break;
        }
        case E_POSX:
            this->option = E_POSX;
            break;
        case E_POSY:
            this->option = E_POSY;
            break;
        case E_NAME:
            this->option = E_NAME;
            break;
        case E_IDSUBAUTOMATASON:
            this->option = E_IDSUBAUTOMATASON;
            break;
        case E_CODE:
            this->option = E_CODE;
            this->code.clear();
            break;
        case E_TRANSITION: {
            int idTransition;
            for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin();
                                                            iter != attributes.end(); iter++) {
                if (iter->name.compare("id") == 0) {
                    std::stringstream toint(iter->value);
                    toint >> idTransition;
                }
            }
            this->transition = new Transition(idTransition);
            this->option = E_TRANSITION;
            break;
        }
        case E_ORIGIN:
            this->option = E_ORIGIN;
            break;
        case E_DESTINY:
            this->option = E_DESTINY;
            break;
        case E_TRANSCODE:
            this->option = E_TRANSCODE;
            this->code.clear();
            break;
        case E_TRANS: {
            for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin();
                                                            iter != attributes.end(); iter++) {
                if (iter->name.compare("type") == 0) {
                    this->type = iter->value;
                }
            }
            this->option = E_TRANS;
            this->code.clear();
            break;
        }
        case E_LIBRARIES:
            this->option = E_LIBRARIES;
            this->listLibraries.clear();
            break;
        case E_LIB:
            this->option = E_LIB;
            this->code.clear();
            break;
        case E_ITERATION:
            this->option = E_ITERATION;
            break;
        case E_VARS:
            this->option = E_VARS;
            this->code.clear();
            break;
        case E_FUNCTIONS:
            this->option = E_FUNCTIONS;
            this->code.clear();
            break;
        case E_CONFIGFILE:
            this->option = E_CONFIGFILE;
            break;
        case E_ICEINTERFACE:
            this->iceinterface = new IceInterface();
            this->option = E_ICEINTERFACE;
            break;
        case E_INTERFACENAME:
            this->option = E_INTERFACENAME;
            break;
        case E_INTERFACEIP:
            this->option = E_INTERFACEIP;
            break;
        case E_INTERFACEPORT:
            this->option = E_INTERFACEPORT;
            break;
        case E_INTERFACEINTERFACE:
            this->option = E_INTERFACEINTERFACE;
            break;
        default:
            break;
    }
}

void MySaxParser::on_end_element ( const Glib::ustring& name ) {
    switch (this->mapStringValues[name]) {
        case E_STATE:
            this->state = false;
            this->subautomata->addNode(this->node->copy(), this->point->copyAsPointer());
            break;
        case E_CODE:
            this->node->setCode(this->code);
            break;
        case E_TRANSITION:
            this->subautomata->addTransition(this->transition->copy(), this->point->copyAsPointer());
            break;
        case E_TRANSCODE:
            this->transition->setCode(this->code);
            break;
        case E_TRANS:
            this->transition->setTrans(this->type, this->code);
            break;
        case E_LIB:
            this->listLibraries.push_back(this->code);
            break;
        case E_VARS:
            this->subautomata->setVariables(this->code);
            break;
        case E_FUNCTIONS:
            this->subautomata->setFunctions(this->code);
            break;
        case E_ICEINTERFACE:
            this->listConfig.push_back(*this->iceinterface);
            break;
        default:
            break;
    }
}

void MySaxParser::on_characters ( const Glib::ustring& text ) {
    switch (this->option) {
        case E_VISUALHFSM:
            break;
        case E_SUBAUTOMATA:
            break;
        case E_STATE:
            break;
        case E_POSX: {
            this->x = atof(text.c_str());
            break;
        }
        case E_POSY: {
            this->point = new Point(this->x, atof(text.c_str()));
            break;
        }
        case E_NAME: {
            if (this->state)
                this->node->setName(text);
            else
                this->transition->setName(text);
            break;
        }
        case E_IDSUBAUTOMATASON: {
            int idSubautomataSon;
            std::stringstream toint(text);
            toint >> idSubautomataSon;
            this->node->setIdSubautomataSon(idSubautomataSon);
            break;
        }
        case E_CODE: {
            this->code += text;
            break;
        }
        case E_TRANSCODE: {
            this->code += text;
            break;
        }
        case E_TRANSITION:
            break;
        case E_ORIGIN: {
            int origin;
            std::stringstream toint(text);
            toint >> origin;
            this->transition->setIdOrigin(origin);
            break;
        }
        case E_DESTINY: {
            int destiny;
            std::stringstream toint(text);
            toint >> destiny;
            this->transition->setIdDestiny(destiny);
            break;
        }
        case E_TRANS: {
            this->code += text;
            break;
        }
        case E_LIB: {
            this->code += text;
            break;
        }
        case E_ITERATION: {
            this->subautomata->setTime(text);
            break;
        }
        case E_VARS: {
            this->code += text;
            break;
        }
        case E_FUNCTIONS: {
            this->code += text;
            break;
        }
        case E_CONFIGFILE: {
//            this->configfile += text;
            break;
        }
        case E_INTERFACENAME: {
            this->iceinterface->setName(text);
            break;
        }
        case E_INTERFACEIP: {
            this->iceinterface->setIp(text);
            break;
        }
        case E_INTERFACEPORT: {
            this->iceinterface->setPort(text);
            break;
        }
        case E_INTERFACEINTERFACE: {
            this->iceinterface->setInterface(text);
            break;
        }
        default:
            break;
    }
}

void MySaxParser::on_comment ( const Glib::ustring& text ) {
//    std::cout << "on_comment(): " << text << std::endl;
}

void MySaxParser::on_warning ( const Glib::ustring& text ) {
    std::cout << "on_warning(): " << text << std::endl;
}

void MySaxParser::on_error ( const Glib::ustring& text ) {
    std::cout << "on_error(): " << text << std::endl;
}

void MySaxParser::on_fatal_error ( const Glib::ustring& text ) {
    std::cout << "on_fatal_error(): " << text << std::endl;
}

/*************************************************************
 * PRIVATE FUNCTIONS
 *************************************************************/
SubAutomata* MySaxParser::getSubautomataWithId ( int id ) {
    std::list<SubAutomata>::iterator subautomataListIterator = this->subautomataList.begin();
    while ( (subautomataListIterator->getId() != id) &&
            (subautomataListIterator != this->subautomataList.end()) )
        subautomataListIterator++;

    if (subautomataListIterator != this->subautomataList.end())
        return &*subautomataListIterator;

    return NULL;
}