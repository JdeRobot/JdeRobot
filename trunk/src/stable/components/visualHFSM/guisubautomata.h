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

#ifndef GUISUBAUTOMATA_H
#define GUISUBAUTOMATA_H

#include <list>
#include <string>

#include "point.h"
#include "guinode.h"
#include "guitransition.h"
#include "popups/editnodedialog.h"
#include "popups/edittransitiondialog.h"
#include "popups/edittransitioncodedialog.h"
#include "popups/renamedialog.h"
#include "popups/renametransitiondialog.h"

// Definition of this class
class GuiSubautomata {
public:
	// Constructor
	GuiSubautomata ( int id, int idFather );

	// Destructor
	virtual ~GuiSubautomata ();

	// Setters
	void setFunctions ( std::string functions );
	void setTime ( std::string timing );
	void setVariables ( std::string variables );

	void setNodeList ( std::list<GuiNode>* list );
	void setTransList ( std::list<GuiTransition>* list );

	void setToZero ( int idChild );

	// Getters
	int getId ();
	int getIdFather ();
	std::string getFunctions ();
	std::string getTime ();
	std::string getVariables ();
	std::list<GuiNode>* getListGuiNodes ();
	std::list<GuiTransition>* getListGuiTransitions ();
	std::list<GuiTransition> getAllGuiTransitionsWith ( Glib::RefPtr<Goocanvas::Item> item );
	std::list<GuiTransition> getAllGuiTransitionsWith ( int id );
	
	GuiNode* getGuiNode ( Glib::RefPtr<Goocanvas::Item> item );
	GuiTransition* getGuiTransition ( Glib::RefPtr<Goocanvas::Item> item );
	Point getPoint ( Glib::RefPtr<Goocanvas::Item> item );

	// Another functions
	void hideAll ();
	void showAll ();
	void removeAll ();
	bool checkAll ();
	bool isNodeListEmpty ();
	void newGuiNode ( int id, int idSubautomataSon, float x, float y );
	void removeGuiNode ( Glib::RefPtr<Goocanvas::Item> item );
	void removeGuiNode ( int id );
	void newGuiTransition ( Point origin, Point final, int id );
	void newGuiTransition ( Point origin, Point final, Point midpoint, int id );
	void removeGuiTransitionsWith ( Glib::RefPtr<Goocanvas::Item> item );
	void removeGuiTransitionsWith ( int id );
	GuiSubautomata copy ();

	// Methods for accessing nodes
	// Setters for nodes
	void setGuiNodeItems (  const Glib::RefPtr<Goocanvas::Item>& item,
							Glib::RefPtr<Goocanvas::Item> selectedItem,
							Glib::RefPtr<Goocanvas::Item> textItem );
	void setIdSubautomataSon ( int id, const Glib::RefPtr<Goocanvas::Item>& item );

	void setCodeLastGuiNode ( std::string code );
	void setIsInitialLastGuiNode ( bool isInitial );
	void setNameLastGuiNode ( std::string name );

	// Getters for nodes
	Glib::RefPtr<Goocanvas::EllipseModel> getLastEllipse ();
	Glib::RefPtr<Goocanvas::EllipseModel> getLastEllipseInit ();
	Glib::RefPtr<Goocanvas::TextModel> getLastTextNode ();

	std::string getGuiNodeName ( const Glib::RefPtr<Goocanvas::Item>& item );
	std::string getLastGuiNodeName ();
	int getLastGuiNodeIdFatherState ();

	Glib::RefPtr<Goocanvas::Item> getGuiNodeItem ( int id );
	int getGuinodeId ( const Glib::RefPtr<Goocanvas::Item>& item );

	int getFirstIdNode ();

	int getIdSubautomataSon ( const Glib::RefPtr<Goocanvas::Item>& item );

	// Another functions for nodes
	void changeGuiNodeWidth ( const Glib::RefPtr<Goocanvas::Item>& item, float width );
	void checkLastGuiNodeForInitial ();
	void editGuiNode ( Glib::RefPtr<Goocanvas::Item> item );
	void markGuiNodeAsInitial ( Glib::RefPtr<Goocanvas::Item> item );
	void moveGuiNode ( 	const Glib::RefPtr<Goocanvas::Item>& item,
						double dx, double dy );
	void renameGuiNode ( Glib::RefPtr<Goocanvas::Item> item );

	// Methods for accessing transitions
	// Setters for transitions
	void setGuiTransitionItems ( Glib::RefPtr<Goocanvas::Item> itemTransLeft,
							  	 Glib::RefPtr<Goocanvas::Item> itemTransRight,
							  	 const Glib::RefPtr<Goocanvas::Item>& itemMidpoint,
							  	 Glib::RefPtr<Goocanvas::Item> itemOrigin,
							  	 Glib::RefPtr<Goocanvas::Item> itemFinal,
							  	 Glib::RefPtr<Goocanvas::Item> itemText );
	void setNameLastGuiTransition ( std::string name );
	void setCodeLastGuiTransition ( std::string code );
	void setTransGuiTransition ( Glib::RefPtr<Goocanvas::Item> item, std::string type, std::string code );
	void setTransLastGuiTransition ( std::string type, std::string code );

	// Getters for transitions
	Glib::RefPtr<Goocanvas::PolylineModel> getLastLeftLine ();
	Glib::RefPtr<Goocanvas::PolylineModel> getLastRightLine ();
	Glib::RefPtr<Goocanvas::RectModel> getLastMidpoint ();
	Glib::RefPtr<Goocanvas::TextModel> getLastTextTransition ();

	int getNumberOfAutotransitions ( Glib::RefPtr<Goocanvas::Item> item );
	int getNumberOfAutotransitions ( Point point );

	// Another functions for transitions
	void changeGuiTransitionWidth ( const Glib::RefPtr<Goocanvas::Item>& item, float width );
	void editGuiTransition ( Glib::RefPtr<Goocanvas::Item> item );
	void editGuiTransitionCode ( Glib::RefPtr<Goocanvas::Item> item );
	void moveGuiTransition ( const Glib::RefPtr<Goocanvas::Item>& item );
	void moveJustGuiTransition ( const Glib::RefPtr<Goocanvas::Item>& item, float dx, float dy );
	void renameGuiTransition ( Glib::RefPtr<Goocanvas::Item> item );

private:
	// Data structure
	int id, idFather;
	std::string timing, variables, functions, config;
	std::list<GuiNode> nodeList;
	std::list<GuiTransition> transitionList;
}; // Class GuiSubautomata

#endif // GUISUBAUTOMATA_H