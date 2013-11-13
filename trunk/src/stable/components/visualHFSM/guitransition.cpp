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

#include "guitransition.h"

/*************************************************************
 * CONSTRUCTORS
 *************************************************************/
GuiTransition::GuiTransition ( Transition t, Point p )
: transition(*(new Transition(t.getId(), t.getIdOrigin(), t.getIdDestiny())))
, point(*(new Point(p.getX(), p.getY()))) {
	this->transition.setTrans(t.getType(), t.getCode());
	this->transition.setName(t.getName());
}

GuiTransition::GuiTransition ( Point porigin, Point pfinal, int id )
: transition(*(new Transition(id)))
, point(porigin.midpoint(pfinal)) {
	Point origin = porigin.calculateGoodArrowPosition(pfinal);
    Point final = pfinal.calculateGoodArrowPosition(porigin);

	this->leftline = Goocanvas::PolylineModel::create(origin.getX(), origin.getY(),
													  this->point.getX(), this->point.getY());
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->leftline->property_line_width() = LINE_WIDTH;
    #else
    this->leftline->set_property("line_width", LINE_WIDTH);
    #endif //GLIBMM_PROPERTIES_ENABLED

    this->rightline = Goocanvas::PolylineModel::create(this->point.getX(), this->point.getY(),
    												   final.getX(), final.getY());
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->rightline->property_line_width() = LINE_WIDTH;
    this->rightline->property_arrow_length() = ARROW_LENGTH;
    this->rightline->property_arrow_tip_length() = ARROW_TIP_LENGTH;
    this->rightline->property_arrow_width() = ARROW_WIDTH;
    this->rightline->property_end_arrow() = END_ARROW;
    #else
    this->rightline->set_property("line_width", LINE_WIDTH);
    this->rightline->set_property("arrow_length", ARROW_LENGTH);
    this->rightline->set_property("arrow_tip_length", ARROW_TIP_LENGTH);
    this->rightline->set_property("arrow_width", ARROW_WIDTH);
    this->rightline->set_property("end_arrow", END_ARROW);
    #endif //GLIBMM_PROPERTIES_ENABLED

    this->midpoint = Goocanvas::RectModel::create(this->point.getX() - SQUARE_SIDE_MID,
    											  this->point.getY() - SQUARE_SIDE_MID,
    											  SQUARE_SIDE, SQUARE_SIDE);
    #ifdef GLIBMM_PROPERTIES_ENABLED
	this->midpoint->property_line_width() = LINE_WIDTH;
	this->midpoint->property_fill_color() = ITEM_COLOR_RED;
	#else
	this->midpoint->set_property("line_width", LINE_WIDTH);
	this->midpoint->set_property("fill_color", Glib::ustring(ITEM_COLOR_RED));
	#endif //GLIBMM_PROPERTIES_ENABLED

    this->text = Goocanvas::TextModel::create(
    						ITEM_NAME_TRANSITION,
    						this->point.getX() - ITEM_NAME_TRANSITION.size() * PIXELS_PER_LETTER,
    						this->point.getY() - Y_TRANS_IDEAL, LETTER_WIDTH);

	this->transition.setName(ITEM_NAME_TRANSITION);
}

GuiTransition::GuiTransition ( Point porigin, Point pfinal, Point pmidpoint, int id )
: transition(*(new Transition(id)))
, point(pmidpoint) {
	Point origin = porigin.calculateGoodArrowPosition(pmidpoint);
    Point final = pfinal.calculateGoodArrowPosition(pmidpoint);

	this->leftline = Goocanvas::PolylineModel::create(origin.getX(), origin.getY(),
													  pmidpoint.getX(), pmidpoint.getY());
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->leftline->property_line_width() = LINE_WIDTH;
    #else
    this->leftline->set_property("line_width", LINE_WIDTH);
    #endif //GLIBMM_PROPERTIES_ENABLED

    this->rightline = Goocanvas::PolylineModel::create(pmidpoint.getX(), pmidpoint.getY(),
    												   final.getX(), final.getY());
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->rightline->property_line_width() = LINE_WIDTH;
    this->rightline->property_arrow_length() = ARROW_LENGTH;
    this->rightline->property_arrow_tip_length() = ARROW_TIP_LENGTH;
    this->rightline->property_arrow_width() = ARROW_WIDTH;
    this->rightline->property_end_arrow() = END_ARROW;
    #else
    this->rightline->set_property("line_width", LINE_WIDTH);
    this->rightline->set_property("arrow_length", ARROW_LENGTH);
    this->rightline->set_property("arrow_tip_length", ARROW_TIP_LENGTH);
    this->rightline->set_property("arrow_width", ARROW_WIDTH);
    this->rightline->set_property("end_arrow", END_ARROW);
    #endif //GLIBMM_PROPERTIES_ENABLED

    this->midpoint = Goocanvas::RectModel::create(pmidpoint.getX() - SQUARE_SIDE_MID,
    											  pmidpoint.getY() - SQUARE_SIDE_MID,
    											  SQUARE_SIDE, SQUARE_SIDE);
    #ifdef GLIBMM_PROPERTIES_ENABLED
	this->midpoint->property_line_width() = LINE_WIDTH;
	this->midpoint->property_fill_color() = ITEM_COLOR_RED;
	#else
	this->midpoint->set_property("line_width", LINE_WIDTH);
	this->midpoint->set_property("fill_color", Glib::ustring(ITEM_COLOR_RED));
	#endif //GLIBMM_PROPERTIES_ENABLED

    this->text = Goocanvas::TextModel::create(
    						ITEM_NAME_TRANSITION,
    						pmidpoint.getX() - ITEM_NAME_TRANSITION.size() * PIXELS_PER_LETTER,
    						pmidpoint.getY() - Y_TRANS_IDEAL, LETTER_WIDTH);
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
GuiTransition::~GuiTransition () {
	if (this->itemTransLeft)
		this->itemTransLeft->remove();
	
	if (this->itemTransRight)
		this->itemTransRight->remove();
	
	if (this->itemMidpoint)
		this->itemMidpoint->remove();
	
	if (this->itemText)
		this->itemText->remove();
}

/*************************************************************
 * SETTERS
 *************************************************************/
void GuiTransition::setItems ( Glib::RefPtr<Goocanvas::Item> itemTransLeft,
								Glib::RefPtr<Goocanvas::Item> itemTransRight,
								Glib::RefPtr<Goocanvas::Item> itemMidpoint,
								Glib::RefPtr<Goocanvas::Item> itemOrigin,
								Glib::RefPtr<Goocanvas::Item> itemFinal,
								Glib::RefPtr<Goocanvas::Item> itemText ) {
	this->itemTransLeft = itemTransLeft;
	this->itemTransRight = itemTransRight;
	this->itemMidpoint = itemMidpoint;
	this->itemOrigin = itemOrigin;
	this->itemFinal = itemFinal;
	this->itemText = itemText;
}

void GuiTransition::setIds ( int idOrigin, int idDestiny ) {
	this->transition.setIdOrigin(idOrigin);
	this->transition.setIdDestiny(idDestiny);
}

void GuiTransition::setTrans ( std::string type, std::string code ) {
	this->transition.setTrans(type, code);
}

/*************************************************************
 * GETTERS
 *************************************************************/
Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemMidpoint () {
	return this->itemMidpoint;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemOrigin () {
	return this->itemOrigin;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemFinal () {
	return this->itemFinal;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemText () {
	return this->itemText;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemTransLeft () {
	return this->itemTransLeft;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getItemTransRight () {
	return this->itemTransRight;
}

Glib::RefPtr<Goocanvas::Item> GuiTransition::getTheOther ( Glib::RefPtr<Goocanvas::Item> item ) {
	if (item == this->itemOrigin)
		return this->itemFinal;
	else
		return this->itemOrigin;
}

Glib::RefPtr<Goocanvas::PolylineModel> GuiTransition::getLeftLine () {
	return this->leftline;
}

Glib::RefPtr<Goocanvas::PolylineModel> GuiTransition::getRightLine () {
	return this->rightline;
}

Glib::RefPtr<Goocanvas::RectModel> GuiTransition::getMidpoint () {
	return this->midpoint;
}

Glib::RefPtr<Goocanvas::TextModel> GuiTransition::getTextModel () {
	return this->text;
}

Point GuiTransition::getPoint () {
	return this->point;
}

int GuiTransition::getId () {
	return this->transition.getId();
}

int GuiTransition::getIdOrigin () {
	return this->transition.getIdOrigin();
}

int GuiTransition::getIdDestiny () {
	return this->transition.getIdDestiny ();
}

std::string GuiTransition::getCodeTrans () {
	return this->transition.getCode();
}

std::string GuiTransition::getName () {
	return this->transition.getName();
}

std::string GuiTransition::getTypeTrans () {
	return this->transition.getType();
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
bool GuiTransition::hasThisItem ( Glib::RefPtr<Goocanvas::Item> item ) {
	return ((this->itemMidpoint == item) ||
			(this->itemOrigin == item) || (this->itemFinal == item));
}

bool GuiTransition::isOrigin ( Glib::RefPtr<Goocanvas::Item> item ) {
	return (this->itemOrigin == item);
}

void GuiTransition::hide () {
	if (DEBUG)
		std::cout << BEGIN_GREEN << GUIELEM << "Hiding transition" << END_COLOR << std::endl;

	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->leftline->property_visibility() = Goocanvas::ITEM_HIDDEN;
	this->rightline->property_visibility() = Goocanvas::ITEM_HIDDEN;
	this->midpoint->property_visibility() = Goocanvas::ITEM_HIDDEN;
	this->text->property_visibility() = Goocanvas::ITEM_HIDDEN;
	#else
	this->leftline->set_property("visibility", Goocanvas::ITEM_HIDDEN);
	this->rightline->set_property("visibility", Goocanvas::ITEM_HIDDEN);
	this->midpoint->set_property("visibility", Goocanvas::ITEM_HIDDEN);
	this->text->set_property("visibility", Goocanvas::ITEM_HIDDEN);
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiTransition::show () {
	if (DEBUG)
		std::cout << BEGIN_GREEN << GUIELEM << "Showing transition" << END_COLOR << std::endl;

	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->leftline->property_visibility() = Goocanvas::ITEM_VISIBLE;
	this->rightline->property_visibility() = Goocanvas::ITEM_VISIBLE;
	this->midpoint->property_visibility() = Goocanvas::ITEM_VISIBLE;
	this->text->property_visibility() = Goocanvas::ITEM_VISIBLE;
	#else
	this->leftline->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	this->rightline->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	this->midpoint->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	this->text->set_property("visibility", Goocanvas::ITEM_VISIBLE);
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiTransition::changeLineWidth ( float newLineWidth ) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
	this->midpoint->property_line_width() = newLineWidth;
	#else
	this->midpoint->set_property("line_width", newLineWidth);
	#endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiTransition::changeText ( std::string newText ) {
	double x;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    x = this->midpoint->property_x();
    #else
    x = this->midpoint->get_property("x");
    #endif //GLIBMM_PROPERTIES_ENABLED

	this->transition.setName(newText);
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->text->property_text() = newText;
    this->text->property_x() = x - newText.size() * PIXELS_PER_LETTER;
    #else
    this->text->set_property("text", newText);
    this->text->set_property("x", x - newText.size() * PIXELS_PER_LETTER);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiTransition::moveLeftItem ( int index, double posx, double posy ) {
	Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->leftline->property_points();
    #else
    points = this->leftline->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->leftline->property_points() = points;
    #else
    this->leftline->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void GuiTransition::moveMidpoint ( float dx, float dy, Point leftpoint, Point rightpoint ) {
	this->midpoint->translate(dx, dy);
	this->text->translate(dx, dy);
	this->point.move(dx, dy);

	Point origin = leftpoint.calculateGoodArrowPosition(this->point);
    Point final = rightpoint.calculateGoodArrowPosition(this->point);

    this->moveLeftItem(0, origin.getX(), origin.getY());
    this->moveLeftItem(1, this->point.getX(), this->point.getY());

    this->moveRightItem(0, this->point.getX(), this->point.getY());
    this->moveRightItem(1, final.getX(), final.getY());
}

void GuiTransition::moveRightItem ( int index, double posx, double posy ) {
	Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->rightline->property_points();
    #else
    points = this->rightline->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->rightline->property_points() = points;
    #else
    this->rightline->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

GuiTransition GuiTransition::copy () {
	GuiTransition gtransition(this->transition.copy(), this->point.copy());
	gtransition.setItems(this->itemTransLeft, this->itemTransRight, this->itemMidpoint,
							this->itemOrigin, this->itemFinal, this->itemText);

	return gtransition;
}
