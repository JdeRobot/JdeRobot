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

#ifndef GUINODE_H
#define GUINODE_H

#include <list>
#include <string>
#include <iostream>

#include <gtkmm-3.0/gtkmm.h>
#include <goocanvasmm-2.0/goocanvasmm.h>
#include <boost/shared_ptr.hpp>

#include "common.h"
#include "point.h"
#include "node.h"

// Definition of this class
class GuiNode {
public:
	// Constructors
	GuiNode ( Node node, Point p );
	GuiNode ( int id, int idSubautomataSon, float x, float y );
		
	// Destructor
	virtual ~GuiNode ();

	// Setters
	void setAsInitial ( bool initial );
	void setIdSubautomataSon ( int id );
	void setItems ( const Glib::RefPtr<Goocanvas::Item>& item,
					Glib::RefPtr<Goocanvas::Item> itemInitial,
					Glib::RefPtr<Goocanvas::Item> itemText );
	void setCode ( std::string code );

	// Getters
	Glib::RefPtr<Goocanvas::Item> getItem ();
	Glib::RefPtr<Goocanvas::Item> getItemInitial ();
	Glib::RefPtr<Goocanvas::Item> getItemText ();

	Glib::RefPtr<Goocanvas::EllipseModel> getEllipse ();
	Glib::RefPtr<Goocanvas::EllipseModel> getEllipseInitial ();
	
	Glib::RefPtr<Goocanvas::TextModel> getText ();
	
	int getId ();
	int getIdSubautomataSon ();
	std::string getCode ();
	std::string getName ();
	Point getPoint ();

	// Another functions
	bool hasThisItem ( const Glib::RefPtr<Goocanvas::Item>& item );
	bool itIsInitial ();

	void hide ();
	void show ();
	void isVisible ();

	void changeLineWidth ( float newLineWidth );
	void changeLineWidthInitial ( float newLineWidthInitial );
	void changeText ( std::string newText );
	void moveItems ( double dx, double dy );

	GuiNode copy ();

private:
	// Data structure
	Node node;
	Point point;
	Glib::RefPtr<Goocanvas::EllipseModel> ellipse, ellipseInitial;
	Glib::RefPtr<Goocanvas::TextModel> text;
	Glib::RefPtr<Goocanvas::Item> item, itemInitial, itemText;

	// Private methods
	void drawAsInitial ( float line_width, std::string stroke_color, std::string fill_color );
	void drawIt (float line_width, std::string stroke_color, std::string fill_color);
};

#endif