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

#include "guinode.h"

/*************************************************************
 * CONSTRUCTORS
 *************************************************************/
GuiNode::GuiNode ( Node n, Point p )
: node(*(new Node(n.getId())))
, point(*(new Point(p.getX(), p.getY()))) {
	this->node.setIdSubautomataSon(n.getIdSubautomataSon());
	this->node.setName(n.getName());
}

GuiNode::GuiNode ( int id, int idSubautomataSon, float x, float y )
: node(*(new Node(id)))
, point(*(new Point(x, y))) {
	this->node.setIdSubautomataSon(idSubautomataSon);
	this->node.setName(ITEM_NAME_NODE);
	this->ellipse = Goocanvas::EllipseModel::create(x, y, RADIUS_NORMAL, RADIUS_NORMAL);
	this->drawIt(1, ITEM_COLOR_BLACK, ITEM_COLOR_BLUE);
	this->ellipseInitial = Goocanvas::EllipseModel::create(x, y, RADIUS_INIT, RADIUS_INIT);
	this->text = Goocanvas::TextModel::create(	ITEM_NAME_NODE,
												x - ITEM_NAME_NODE.size() * PIXELS_PER_LETTER,
												y - Y_NODE_IDEAL, LETTER_WIDTH);
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
GuiNode::~GuiNode () {
	if (itemInitial)
		this->itemInitial->remove();

    if (itemText)
    	this->itemText->remove();
	
	if (item)
		this->item->remove();
}

/*************************************************************
 * SETTERS
 *************************************************************/
void GuiNode::setAsInitial ( bool initial ) {
	this->node.setInitial(initial);
	if (initial)
		this->drawAsInitial(1, ITEM_COLOR_BLACK, ITEM_COLOR_BLUE);
	else
		this->drawAsInitial(0, ITEM_COLOR_BLACK, ITEM_COLOR_BLUE);
}

void GuiNode::setIdSubautomataSon ( int id ) {
	this->node.setIdSubautomataSon(id);
}

void GuiNode::setItems ( const Glib::RefPtr<Goocanvas::Item>& item,
						 Glib::RefPtr<Goocanvas::Item> itemInitial,
						 Glib::RefPtr<Goocanvas::Item> itemText ) {
	this->item = item;
	this->itemInitial = itemInitial;
	this->itemText = itemText;
}

void GuiNode::setCode ( std::string code ) {
	this->node.setCode(code);
}

/*************************************************************
 * GETTERS
 *************************************************************/
Glib::RefPtr<Goocanvas::Item> GuiNode::getItem () {
	return this->item;
}

Glib::RefPtr<Goocanvas::Item> GuiNode::getItemInitial () {
	return this->itemInitial;
}

Glib::RefPtr<Goocanvas::Item> GuiNode::getItemText () {
	return this->itemText;
}

Glib::RefPtr<Goocanvas::EllipseModel> GuiNode::getEllipse () {
	return this->ellipse;
}

Glib::RefPtr<Goocanvas::EllipseModel> GuiNode::getEllipseInitial () {
	return this->ellipseInitial;
}

Glib::RefPtr<Goocanvas::TextModel> GuiNode::getText () {
	return this->text;
}

int GuiNode::getId () {
	return this->node.getId();
}

int GuiNode::getIdSubautomataSon () {
	return this->node.getIdSubautomataSon();
}

std::string GuiNode::getCode () {
	return this->node.getCode();
}

std::string GuiNode::getName () {
	return this->node.getName();
}

Point GuiNode::getPoint () {
	return this->point;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
bool GuiNode::itIsInitial () {
	return this->node.isInitial();
}

bool GuiNode::hasThisItem ( const Glib::RefPtr<Goocanvas::Item>& item ) {
	Glib::RefPtr<Goocanvas::Item> myItem = item;
	return ((this->item == myItem) || (this->itemInitial == myItem) || (this->itemText == myItem));
}

void GuiNode::hide () {
	if (DEBUG)
		std::cout << BEGIN_GREEN << GUIELEM << "Hiding node" << END_COLOR << std::endl;

	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->ellipse->property_visibility() = Goocanvas::ITEM_INVISIBLE;
	this->ellipseInitial->property_visibility() = Goocanvas::ITEM_INVISIBLE;
	this->text->property_visibility() = Goocanvas::ITEM_INVISIBLE;
	#else
	this->ellipse->set_property("visibility", Goocanvas::ITEM_INVISIBLE);
	this->ellipseInitial->set_property("visibility", Goocanvas::ITEM_INVISIBLE);
	this->text->set_property("visibility", Goocanvas::ITEM_INVISIBLE);
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::show () {
	if (DEBUG)
		std::cout << BEGIN_GREEN << GUIELEM << "Showing node" << END_COLOR << std::endl;

	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->ellipse->property_visibility() = Goocanvas::ITEM_VISIBLE;
	this->ellipseInitial->property_visibility() = Goocanvas::ITEM_VISIBLE;
	this->text->property_visibility() = Goocanvas::ITEM_VISIBLE;
	#else
	this->ellipse->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	this->ellipseInitial->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	this->text->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::isVisible () {
	#ifdef GLIBMM_PROPERTIES_ENABLED
	if (this->ellipse->property_visibility())
		std::cout << "ellipse visible" << std::endl;
	else
		std::cout << "ellipse invisible" << std::endl;
	if (this->ellipseInitial->property_visibility())
		std::cout << "ellipse initial visible" << std::endl;
	else
		std::cout << "ellipse initial invisible" << std::endl;
	if (this->text->property_visibility())
		std::cout << "text visible" << std::endl;
	else
		std::cout << "text invisible" << std::endl;
	#else
	if (this->ellipse->get_property("visibility"))
		std::cout << "ellipse visible" << std::endl;
	else
		std::cout << "ellipse invisible" << std::endl;
	if (this->ellipseInitial->get_property("visibility"))
		std::cout << "ellipse initial visible" << std::endl;
	else
		std::cout << "ellipse initial invisible" << std::endl;
	if (this->text->get_property("visibility"))
		std::cout << "text visible" << std::endl;
	else
		std::cout << "text invisible" << std::endl;
	#endif
}

void GuiNode::changeLineWidth ( float newLineWidth ) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->ellipse->property_line_width() = double(newLineWidth);
	#else
	this->ellipse->set_property("line_width", double(newLineWidth));
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::changeLineWidthInitial ( float newLineWidthInitial ) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->ellipseInitial->property_line_width() = newLineWidthInitial;
    #else
    this->ellipseInitial->set_property("line_width", newLineWidthInitial);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::changeText ( std::string newText ) {
	this->node.setName(newText);
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->text->property_text() = newText;
    this->text->property_x() = this->point.getX() - newText.size() * PIXELS_PER_LETTER;
    this->text->property_y() = this->point.getY() - Y_NODE_IDEAL;
    #else
    this->text->set_property("text", newText);
    this->text->set_property("x", this->point.getX() - newText.size() * PIXELS_PER_LETTER);
    this->text->set_property("y", this->point.getY() - Y_NODE_IDEAL);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::moveItems ( double dx, double dy ) {
	this->item->translate(dx, dy);
	this->itemInitial->translate(dx, dy);
	this->text->translate(dx, dy);
	this->point.move(dx, dy);
}

GuiNode GuiNode::copy () {
	GuiNode gnode(this->node.copy(), this->point.copy());
	gnode.setItems(this->item, this->itemInitial, this->itemText);

	return gnode;
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void GuiNode::drawAsInitial ( float line_width, std::string stroke_color, std::string fill_color ) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->ellipseInitial->property_line_width() = line_width;
    this->ellipseInitial->property_stroke_color() = stroke_color;
    #else
    this->ellipseInitial->set_property("line_width", line_width);
    this->ellipseInitial->set_property("stroke_color", Glib::ustring(stroke_color));
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiNode::drawIt ( float line_width, std::string stroke_color, std::string fill_color ) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->ellipse->property_line_width() = line_width;
    this->ellipse->property_stroke_color() = stroke_color;
	this->ellipse->property_fill_color() = fill_color;
    #else
    this->ellipse->set_property("line_width", line_width);
    this->ellipse->set_property("stroke_color", Glib::ustring(stroke_color));
    this->ellipse->set_property("fill_color", Glib::ustring(fill_color));
    #endif //GLIBMM_PROPERTIES_ENABLED
}