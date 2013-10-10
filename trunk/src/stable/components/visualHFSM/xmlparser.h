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

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <map>
#include <iostream>
#include <stdlib.h>
#include <libxml++/libxml++.h>

#include "common.h"
#include "point.h"
#include "node.h"
#include "transition.h"
#include "subautomata.h"

typedef enum Element {
    E_VISUALHFSM,
    E_SUBAUTOMATA,
    E_STATE,
    E_POSX,
    E_POSY,
    E_NAME,
    E_IDSUBAUTOMATASON,
    E_CODE,
    E_TRANSITION,
    E_ORIGIN,
    E_DESTINY,
    E_TRANS, 
    E_LIBRARIES,
    E_LIB,
    E_ITERATION,
    E_VARS,
    E_FUNCTIONS
} Element;

// Definition of this class
class MySaxParser : public xmlpp::SaxParser {
public:
	// Constructor
	MySaxParser ();

	// Destructor
    virtual ~MySaxParser ();

    // Getters
    std::list<SubAutomata> getListSubautomata ();

protected:
    // Overrides:
    virtual void on_start_document ();
    virtual void on_end_document ();
    virtual void on_start_element ( const Glib::ustring& name,
                                  const AttributeList& attributes );
    virtual void on_end_element ( const Glib::ustring& name );
    virtual void on_characters ( const Glib::ustring& characters );
    virtual void on_comment ( const Glib::ustring& text );
    virtual void on_warning ( const Glib::ustring& text );
    virtual void on_error ( const Glib::ustring& text );
    virtual void on_fatal_error ( const Glib::ustring& text );

private:
    // Data structure
    std::list <SubAutomata> subautomataList;
    std::map<std::string, Element> mapStringValues;

    bool state;
    float x;
    std::string type;

    Element option;

    SubAutomata* subautomata;
    Transition* transition;
    Node* node;
    Point* point;

    std::list<std::string> listInterfaces;
    std::string code;

    // Private functions
    SubAutomata* getSubautomataWithId ( int id );
}; // MySaxParser

#endif // XMLPARSER_H