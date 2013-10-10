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

#include "guisubautomata.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
GuiSubautomata::GuiSubautomata ( int id, int idFather ) {
    this->id = id;
    this->idFather = idFather;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
GuiSubautomata::~GuiSubautomata () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void GuiSubautomata::setFunctions ( std::string functions ) {
    this->functions = functions;
}

void GuiSubautomata::setTime ( std::string timing ) {
    this->timing = timing;
}

void GuiSubautomata::setVariables ( std::string variables ) {
    this->variables = variables;
}

void GuiSubautomata::setInterfaces ( std::list<std::string>& interfaces ) {
    this->interfaces = interfaces;
}

void GuiSubautomata::setNodeList ( std::list<GuiNode>* list ) {
    this->nodeList.clear();
    for ( std::list<GuiNode>::iterator nodeListIterator = list->begin();
            nodeListIterator != list->end(); nodeListIterator++ )
        this->nodeList.push_back(*nodeListIterator);
}

void GuiSubautomata::setTransList ( std::list<GuiTransition>* list ) {
    this->transitionList.clear();
    for ( std::list<GuiTransition>::iterator transListIterator = list->begin();
            transListIterator != list->end(); transListIterator++ )
        this->transitionList.push_back(*transListIterator);
}

/*************************************************************
 * GETTERS
 *************************************************************/
int GuiSubautomata::getId () {
    return this->id;
}

int GuiSubautomata::getIdFather () {
    return this->idFather;
}

std::string GuiSubautomata::getFunctions () {
    return this->functions;
}

std::string GuiSubautomata::getTime () {
    return this->timing;
}

std::string GuiSubautomata::getVariables () {
    return this->variables;
}

std::list<std::string>* GuiSubautomata::getInterfaces () {
    return &this->interfaces;
}

std::list<GuiNode>* GuiSubautomata::getListGuiNodes () {
    return &this->nodeList;
}

std::list<GuiTransition>* GuiSubautomata::getListGuiTransitions () {
    return &this->transitionList;
}

std::list<GuiTransition> GuiSubautomata::getAllGuiTransitionsWith (
                                                Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiTransition> guiTransitionList;

    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while (nodeTransIterator != this->transitionList.end()) {
        if (nodeTransIterator->hasThisItem(item))
            guiTransitionList.push_back(*nodeTransIterator);
        nodeTransIterator++;
    }

    return guiTransitionList;
}

std::list<GuiTransition> GuiSubautomata::getAllGuiTransitionsWith ( int id ) {
    std::list<GuiTransition> guiTransitionList;

    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while (nodeTransIterator != this->transitionList.end()) {
        if ( (nodeTransIterator->getIdOrigin() == id) || 
            (nodeTransIterator->getIdDestiny() == id))
            guiTransitionList.push_back(*nodeTransIterator);
        nodeTransIterator++;
    }

    return guiTransitionList;
}

GuiNode* GuiSubautomata::getGuiNode ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        return &*nodeListIterator;

    return NULL;
}

GuiTransition* GuiSubautomata::getGuiTransition ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end())
        return &*nodeTransIterator;

    return NULL;
}

Point GuiSubautomata::getPoint ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        return nodeListIterator->getPoint();

    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    return nodeTransIterator->getPoint();
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void GuiSubautomata::hideAll () {
    if (DEBUG)
        std::cout << BEGIN_GREEN << GUISUB << "Hiding (" << this->id << "). Node list size: " << this->nodeList.size() << END_COLOR << std::endl;

    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( nodeListIterator != this->nodeList.end() ) {
        nodeListIterator->hide();
        nodeListIterator++;
    }

    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( nodeTransIterator != this->transitionList.end() ) {
        nodeTransIterator->hide();
        nodeTransIterator++;
    }
}

void GuiSubautomata::showAll () {
    if (DEBUG)
        std::cout << BEGIN_GREEN << GUISUB << "Showing (" << this->id << "). Node list size: " << this->nodeList.size() << END_COLOR << std::endl;

    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( nodeListIterator != this->nodeList.end() ) {
        nodeListIterator->show();
//        nodeListIterator->isVisible();
        nodeListIterator++;
    }

    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( nodeTransIterator != this->transitionList.end() ) {
        nodeTransIterator->show();
        nodeTransIterator++;
    }
}

void GuiSubautomata::removeAll () {
    if (DEBUG)
        std::cout << BEGIN_GREEN << GUISUB << "Removing (" << this->id << "). Node list size: " << this->nodeList.size() << END_COLOR << std::endl;

    this->nodeList.clear();
    this->transitionList.clear();
}

// Is node list empty?
bool GuiSubautomata::isNodeListEmpty () {
    return this->nodeList.empty();
}

bool GuiSubautomata::findInterface ( std::string interface ) {
    bool found = false;

    std::list<std::string>::iterator interfacesIterator = this->interfaces.begin();
    while ( (!found) && (interfacesIterator != this->interfaces.end()) ) {
        std::size_t pos = interfacesIterator->find(interface);
        found = (pos != std::string::npos);
        interfacesIterator++;
    }

    return found;
}

void GuiSubautomata::newGuiNode ( int id, int idSubautomataSon, float x, float y ) {
    GuiNode gnode(id, idSubautomataSon, x, y);
    this->nodeList.push_back(gnode);
//    delete gnode;
}

// Removes the node with the Goocanvas::Item 'item'
void GuiSubautomata::removeGuiNode ( Glib::RefPtr<Goocanvas::Item> item ) {
    this->removeGuiTransitionsWith(item);
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    bool isInit = nodeListIterator->itIsInitial();

    if (nodeListIterator != this->nodeList.end()) {
        nodeListIterator = this->nodeList.erase(nodeListIterator);

        if ((!this->nodeList.empty()) && (isInit)) {
            if (nodeListIterator == this->nodeList.end())
                this->nodeList.begin()->setAsInitial(true);
            else
                nodeListIterator->setAsInitial(true);
        }
    }
}

// Removes the node with the id 'id'
void GuiSubautomata::removeGuiNode ( int id ) {
    this->removeGuiTransitionsWith(id);
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->getId() != id) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    bool isInit = nodeListIterator->itIsInitial();

    if (nodeListIterator != this->nodeList.end()) {
        nodeListIterator = this->nodeList.erase(nodeListIterator);

        if ((!this->nodeList.empty()) && (isInit)) {
            if (nodeListIterator == this->nodeList.end())
                this->nodeList.begin()->setAsInitial(true);
            else
                nodeListIterator->setAsInitial(true);
        }
    }
}

// Creates a new GuiTransition
void GuiSubautomata::newGuiTransition ( Point origin, Point final, int id ) {
    GuiTransition gtransition(origin, final, id);
    this->transitionList.push_back(gtransition);
}

void GuiSubautomata::newGuiTransition ( Point origin, Point final, Point midpoint, int id ) {
    GuiTransition gtransition(origin, final, midpoint, id);
    this->transitionList.push_back(gtransition);
}

void GuiSubautomata::removeGuiTransitionsWith ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while (nodeTransIterator != this->transitionList.end()) {
        if (nodeTransIterator->hasThisItem(item)) {
            nodeTransIterator = this->transitionList.erase(nodeTransIterator);
        } else if (nodeTransIterator != this->transitionList.end()) {
            nodeTransIterator++;
        }
    }
}

void GuiSubautomata::removeGuiTransitionsWith ( int id ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while (nodeTransIterator != this->transitionList.end()) {
        if ( (nodeTransIterator->getIdOrigin() != id) && 
            (nodeTransIterator->getIdDestiny() != id) ) {
            nodeTransIterator = this->transitionList.erase(nodeTransIterator);
        } else if (nodeTransIterator != this->transitionList.end()) {
            nodeTransIterator++;
        }
    }
}

GuiSubautomata GuiSubautomata::copy () {
    std::list<GuiNode> gnodelist;
    for ( std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
            nodeListIterator != this->nodeList.end(); nodeListIterator++ )
        gnodelist.push_back(nodeListIterator->copy());

    std::list<GuiTransition> gtranslist;
    for ( std::list<GuiTransition>::iterator transListIterator = this->transitionList.begin();
            transListIterator != this->transitionList.end(); transListIterator++ )
        gtranslist.push_back(transListIterator->copy());

    GuiSubautomata gsubautomata(this->id, this->idFather);
    gsubautomata.setFunctions(std::string(this->functions));
    gsubautomata.setTime(std::string(this->timing));
    gsubautomata.setVariables(std::string(this->variables));
    gsubautomata.setInterfaces(this->interfaces);
    gsubautomata.setNodeList(&gnodelist);
    gsubautomata.setTransList(&gtranslist);

    std::cout << "hasta aquiiiiiiii???" << std::endl;

    return gsubautomata;
}

/*************************************************************
 * METHODS FOR ACCESSING NODES
 *************************************************************/

 /*************************************************************
 * SETTERS FOR NODES
 *************************************************************/
// Sets the items for a node with the Goocanvas::Item 'item'
void GuiSubautomata::setGuiNodeItems (  const Glib::RefPtr<Goocanvas::Item>& item,
                                        Glib::RefPtr<Goocanvas::Item> selectedItem,
                                        Glib::RefPtr<Goocanvas::Item> textItem ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    nodeListIterator->setItems(item, selectedItem, textItem);
}

void GuiSubautomata::setIdSubautomataSon ( int id, const Glib::RefPtr<Goocanvas::Item>& item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        nodeListIterator->setIdSubautomataSon(id);
}

void GuiSubautomata::setCodeLastGuiNode ( std::string code ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    nodeListIterator->setCode(code);
}

void GuiSubautomata::setIsInitialLastGuiNode ( bool isInitial ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    nodeListIterator->setAsInitial(isInitial);
}

void GuiSubautomata::setNameLastGuiNode ( std::string name ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    nodeListIterator->changeText(name);
}

/*************************************************************
 * GETTERS FOR NODES
 *************************************************************/
Glib::RefPtr<Goocanvas::EllipseModel> GuiSubautomata::getLastEllipse () {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    return nodeListIterator->getEllipse();
}

Glib::RefPtr<Goocanvas::EllipseModel> GuiSubautomata::getLastEllipseInit () {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    return nodeListIterator->getEllipseInitial();
}

Glib::RefPtr<Goocanvas::TextModel> GuiSubautomata::getLastTextNode () {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    return nodeListIterator->getText();
}

std::string GuiSubautomata::getLastGuiNodeName () {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    return nodeListIterator->getName();
}

Glib::RefPtr<Goocanvas::Item> GuiSubautomata::getGuiNodeItem ( int id ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (nodeListIterator->getId() != id) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    return nodeListIterator->getItem();
}

int GuiSubautomata::getGuinodeId ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    return nodeListIterator->getId();
}

int GuiSubautomata::getFirstIdNode () {
    return this->nodeList.begin()->getId();
}

int GuiSubautomata::getIdSubautomataSon ( const Glib::RefPtr<Goocanvas::Item>& item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        return nodeListIterator->getIdSubautomataSon();

    return 0;
}

/*************************************************************
 * ANOTHER FUNCTIONS FOR NODES
 *************************************************************/
// Changes the node width with the Goocanvas::Item 'item'
void GuiSubautomata::changeGuiNodeWidth ( const Glib::RefPtr<Goocanvas::Item>& item, float width ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        nodeListIterator->changeLineWidth(width);
}

void GuiSubautomata::checkLastGuiNodeForInitial () {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.end();
    nodeListIterator--;
    if (this->nodeList.size() == 1)
        nodeListIterator->setAsInitial(true);
    else
        nodeListIterator->setAsInitial(false);
}

// Edits the node with the Goocanvas::Item 'item'
void GuiSubautomata::editGuiNode ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) && (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Editing transition" << END_COLOR << std::endl;
        
        EditNodeDialog* endialog = new EditNodeDialog(&*nodeListIterator);
        endialog->init();
    }
}

// Marks the node with the Goocanvas::Item 'item' as initial
// If there is another node marked as initial, it first removes that mark
void GuiSubautomata::markGuiNodeAsInitial ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->itIsInitial()) && (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;
    if (nodeListIterator != this->nodeList.end())
        nodeListIterator->setAsInitial(false);

    nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
                (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;
    if (nodeListIterator != this->nodeList.end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Marked node as initial" << END_COLOR << std::endl;
        
        nodeListIterator->setAsInitial(true);
    }
}

// Moves the node (all its items)
void GuiSubautomata::moveGuiNode (  const Glib::RefPtr<Goocanvas::Item>& item,
                                    double dx, double dy ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end())
        nodeListIterator->moveItems(dx, dy);
}

// Renames the node with the Goocanvas::Item 'item'
void GuiSubautomata::renameGuiNode ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) && (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Renaming node" << END_COLOR << std::endl;

        RenameDialog* rdialog = new RenameDialog(&*nodeListIterator);
        rdialog->init();
    }
}

/*************************************************************
 * METHODS FOR ACCESSING TRANSITIONS
 *************************************************************/

/*************************************************************
 * SETTERS FOR TRANSITIONS
 *************************************************************/
void GuiSubautomata::setGuiTransitionItems ( Glib::RefPtr<Goocanvas::Item> itemTransLeft,
                                             Glib::RefPtr<Goocanvas::Item> itemTransRight,
                                             const Glib::RefPtr<Goocanvas::Item>& itemMidpoint,
                                             Glib::RefPtr<Goocanvas::Item> itemOrigin,
                                             Glib::RefPtr<Goocanvas::Item> itemFinal,
                                             Glib::RefPtr<Goocanvas::Item> itemText ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    nodeTransIterator->setItems(itemTransLeft, itemTransRight, itemMidpoint,
                                itemOrigin, itemFinal, itemText);
    nodeTransIterator->setIds(this->getGuinodeId(itemOrigin), this->getGuinodeId(itemFinal));
}

void GuiSubautomata::setNameLastGuiTransition ( std::string name ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    nodeTransIterator->changeText(name);
}

void GuiSubautomata::setTransGuiTransition ( Glib::RefPtr<Goocanvas::Item> item, std::string type, std::string code ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end())
        nodeTransIterator->setTrans(type, code);
}

void GuiSubautomata::setTransLastGuiTransition ( std::string type, std::string code ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    nodeTransIterator->setTrans(type, code);
}

/*************************************************************
 * GETTERS FOR TRANSITIONS
 *************************************************************/
Glib::RefPtr<Goocanvas::PolylineModel> GuiSubautomata::getLastLeftLine () {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    return nodeTransIterator->getLeftLine();
}

Glib::RefPtr<Goocanvas::PolylineModel> GuiSubautomata::getLastRightLine () {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    return nodeTransIterator->getRightLine();
}

Glib::RefPtr<Goocanvas::RectModel> GuiSubautomata::getLastMidpoint () {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    return nodeTransIterator->getMidpoint();
}

Glib::RefPtr<Goocanvas::TextModel> GuiSubautomata::getLastTextTransition () {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.end();
    nodeTransIterator--;
    return nodeTransIterator->getTextModel();
}

/*************************************************************
 * ANOTHER FUNCTIONS FOR TRANSITIONS
 *************************************************************/
void GuiSubautomata::changeGuiTransitionWidth ( const Glib::RefPtr<Goocanvas::Item>& item, float width ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end())
        nodeTransIterator->changeLineWidth(width);
}

void GuiSubautomata::editGuiTransition ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Editing transition" << END_COLOR << std::endl;

        EditTransitionDialog* etdialog = new EditTransitionDialog(&*nodeTransIterator);
        etdialog->init();
    }
}

// Moves the transitions of the item (all its items)
void GuiSubautomata::moveGuiTransition ( const Glib::RefPtr<Goocanvas::Item>& item ) {
    std::list<GuiNode>::iterator nodeListIterator = this->nodeList.begin();
    while ( (!nodeListIterator->hasThisItem(item)) &&
            (nodeListIterator != this->nodeList.end()) )
        nodeListIterator++;

    if (nodeListIterator != this->nodeList.end()) {
        std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
        while (nodeTransIterator != this->transitionList.end()) {
            while ( (!nodeTransIterator->hasThisItem(item)) &&
                    (nodeTransIterator != this->transitionList.end())) {
                nodeTransIterator++;
            }

            if (nodeTransIterator != this->transitionList.end()) {
                Point ppoint = this->getPoint(item);

                Point point = ppoint.calculateGoodArrowPosition(this->getPoint(nodeTransIterator->getItemMidpoint()));

                if (nodeTransIterator->isOrigin(item))
                    nodeTransIterator->moveLeftItem(0, point.getX(), point.getY());
                else
                    nodeTransIterator->moveRightItem(1, point.getX(), point.getY());

                nodeTransIterator++;
            }
        }
    }
}

void GuiSubautomata::moveJustGuiTransition ( const Glib::RefPtr<Goocanvas::Item>& item,
                                             float dx, float dy ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end()) {
        nodeTransIterator->moveMidpoint(dx, dy,
                            this->getPoint(nodeTransIterator->getItemOrigin()),
                            this->getPoint(nodeTransIterator->getItemFinal()));
    }
}

void GuiSubautomata::renameGuiTransition ( Glib::RefPtr<Goocanvas::Item> item ) {
    std::list<GuiTransition>::iterator nodeTransIterator = this->transitionList.begin();
    while ( (!nodeTransIterator->hasThisItem(item)) &&
            (nodeTransIterator != this->transitionList.end()) )
        nodeTransIterator++;

    if (nodeTransIterator != this->transitionList.end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Renaming transition" << END_COLOR << std::endl;
        
        RenameDialogTransition* rdialog = new RenameDialogTransition(&*nodeTransIterator);
        rdialog->init();
    }
}