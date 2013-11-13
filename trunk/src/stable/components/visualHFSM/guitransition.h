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

#ifndef GUITRANSITION_H
#define GUITRANSITION_H

#include <list>

#include <gtkmm-3.0/gtkmm.h>
#include <goocanvasmm-2.0/goocanvasmm.h>

#include "common.h"
#include "point.h"
#include "transition.h"

// Definition of this class
class GuiTransition {
public:
	// Constructors
	GuiTransition ( Transition transition, Point p );
	GuiTransition ( Point porigin, Point pfinal, int id );
	GuiTransition ( Point porigin, Point pfinal, Point pmidpoint, int id );

	// Destructor
	virtual ~GuiTransition ();

	// Setters
	void setItems ( Glib::RefPtr<Goocanvas::Item> itemTransLeft,
					Glib::RefPtr<Goocanvas::Item> itemTransRight,
					Glib::RefPtr<Goocanvas::Item> itemMidpoint,
					Glib::RefPtr<Goocanvas::Item> itemOrigin,
					Glib::RefPtr<Goocanvas::Item> itemFinal,
					Glib::RefPtr<Goocanvas::Item> itemText );
	void setIds ( int idOrigin, int idDestiny );
	void setTrans ( std::string type, std::string code );

	// Getters
	Glib::RefPtr<Goocanvas::Item> getItemMidpoint ();
	Glib::RefPtr<Goocanvas::Item> getItemOrigin ();
	Glib::RefPtr<Goocanvas::Item> getItemFinal ();
	Glib::RefPtr<Goocanvas::Item> getItemText ();
	Glib::RefPtr<Goocanvas::Item> getItemTransLeft ();
	Glib::RefPtr<Goocanvas::Item> getItemTransRight ();
	Glib::RefPtr<Goocanvas::Item> getTheOther ( Glib::RefPtr<Goocanvas::Item> item );

	Glib::RefPtr<Goocanvas::PolylineModel> getLeftLine ();
	Glib::RefPtr<Goocanvas::PolylineModel> getRightLine ();
	
	Glib::RefPtr<Goocanvas::RectModel> getMidpoint ();
	Glib::RefPtr<Goocanvas::TextModel> getTextModel ();
	
	Point getPoint ();

	int getId ();
	int getIdOrigin ();
	int getIdDestiny ();
	std::string getCodeTrans ();
	std::string getName ();
	std::string getTypeTrans ();

	// Another functions
	bool hasThisItem ( Glib::RefPtr<Goocanvas::Item> item );
	bool isOrigin ( Glib::RefPtr<Goocanvas::Item> item );

	void hide ();
	void show ();

	void changeLineWidth ( float newLineWidth );
	void changeText ( std::string newText );
	void moveLeftItem ( int index, double posx, double posy );
	void moveMidpoint ( float dx, float dy, Point leftpoint, Point rightpoint );
	void moveRightItem ( int index, double posx, double posy );

	GuiTransition copy ();

private:
	// Data structure
	Glib::RefPtr<Goocanvas::PolylineModel> leftline, rightline;
	Glib::RefPtr<Goocanvas::RectModel> midpoint;
	Glib::RefPtr<Goocanvas::TextModel> text;
	Glib::RefPtr<Goocanvas::Item> itemTransLeft, itemTransRight, itemMidpoint;
	Glib::RefPtr<Goocanvas::Item> itemOrigin, itemFinal, itemText;
	Transition transition;
	Point point;
}; // Class GuiTransition

#endif // GUITRANSITION_H