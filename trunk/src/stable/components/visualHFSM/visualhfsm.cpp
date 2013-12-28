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

#include "visualhfsm.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
VisualHFSM::VisualHFSM ( BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade )
:   Gtk::Dialog(cobject),
    refBuilder(refGlade)
{
    // GETTING WIDGETS
    // Get the menu items
    refBuilder->get_widget("imagemenuitem_new", this->imagemenuitem_new);
    refBuilder->get_widget("imagemenuitem_open", this->imagemenuitem_open);
    refBuilder->get_widget("imagemenuitem_save", this->imagemenuitem_save);
    refBuilder->get_widget("imagemenuitem_saveas", this->imagemenuitem_saveas);
    refBuilder->get_widget("imagemenuitem_quit", this->imagemenuitem_quit);
    refBuilder->get_widget("imagemenuitem_state", this->imagemenuitem_state);
    refBuilder->get_widget("imagemenuitem_transition", this->imagemenuitem_transition);
    refBuilder->get_widget("imagemenuitem_timer", this->imagemenuitem_timer);
    refBuilder->get_widget("imagemenuitem_variables", this->imagemenuitem_variables);
    refBuilder->get_widget("imagemenuitem_libraries", this->imagemenuitem_libraries);
    refBuilder->get_widget("imagemenuitem_configfile", this->imagemenuitem_configfile);
    refBuilder->get_widget("imagemenuitem_generatecode", this->imagemenuitem_generatecode);
    refBuilder->get_widget("imagemenuitem_compile", this->imagemenuitem_compile);
    refBuilder->get_widget("imagemenuitem_about", this->imagemenuitem_about);

    // Get the windows
    refBuilder->get_widget("scrolledwindow_schema", this->scrolledwindow_schema);

    // Get the treeview
    refBuilder->get_widget("treeview", this->treeview);

    // ASSIGNING SIGNALS
    // Of the menu items
    this->imagemenuitem_new->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_new));
    this->imagemenuitem_open->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_open));
    this->imagemenuitem_save->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_save));
    this->imagemenuitem_saveas->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_save_as));
    this->imagemenuitem_quit->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_quit));
    this->imagemenuitem_state->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_state));
    this->imagemenuitem_transition->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_transition));
    this->imagemenuitem_timer->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_timer));
    this->imagemenuitem_variables->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_variables));
    this->imagemenuitem_libraries->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_libraries));
    this->imagemenuitem_configfile->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_configfile));
    this->imagemenuitem_generatecode->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_generate_code));
    this->imagemenuitem_compile->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_compile));
    this->imagemenuitem_about->signal_activate().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_menubar_clicked_about));

    // Of the windows
    this->scrolledwindow_schema->signal_event().connect(
                sigc::mem_fun(*this, &VisualHFSM::on_schema_event));
    
    // Create the canvas    
    this->canvas = Gtk::manage(new Goocanvas::Canvas());
    this->canvas->signal_item_created().connect(sigc::mem_fun(*this,
                                        &VisualHFSM::on_item_created));

    // And the treeview's tree model
    this->refTreeModel = Gtk::TreeStore::create(this->m_Columns);
    this->treeview->set_model(this->refTreeModel);

    //Add the TreeView's view columns:
    this->treeview->append_column("ID", this->m_Columns.m_col_id);
    this->treeview->append_column("Name", this->m_Columns.m_col_name);

    this->state = NONE;

    this->root = Goocanvas::GroupModel::create();
    this->canvas->set_root_item_model(root);
    this->canvas->set_visible(true);
    this->scrolledwindow_schema->add(*(this->canvas));

    Glib::RefPtr<Gdk::Screen> screen = this->get_screen();
    int width = screen->get_width();
    int height = screen->get_height();

    Goocanvas::Bounds bounds;
    this->canvas->get_bounds(bounds);
    bounds.set_x2(width);
    bounds.set_y2(height * 2);
    this->canvas->set_bounds(bounds);

    this->create_menu_transition();
    this->create_menu_item();
    this->create_menu_paste();

    this->copyPressed = false;

    this->id = 1;
    GuiSubautomata guisub(id, 0);
    guisub.setTime(TIME_DEFAULT);
    this->subautomataList.push_back(guisub);
    this->currentSubautomata = this->getSubautomata(id);
    this->id++;

    this->idguinode = 1;
    this->idguitransition = 1;

    system("./getinterfaces.sh /usr/local/include/jderobot/slice > allinterfaces.txt");
    std::ifstream infile("allinterfaces.txt");
    std::string line;
    while ( std::getline(infile, line) ) {
        std::string buff;
        std::string interface;
        std::stringstream ss(line);
        int i = 0;
        while (ss >> buff) {
            if (i == 0) {   // getting the interface
                interface = std::string(buff);
            } else { // getting the file header
                this->mapInterfacesHeader[interface] = buff;
            }
            i++;
        }
    }
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
VisualHFSM::~VisualHFSM () {}

/*************************************************************
 * METHODS FOR SIGNALS
 *************************************************************/
// Save the automata in the specified path
void VisualHFSM::on_save_file ( std::string path ) {
    if (this->lastButton == SAVE_AS) {
        std::string str(".xml");
        if (!this->hasEnding(path, str))
            this->filepath = std::string(path + str);
        else
            this->filepath = std::string(path);
    
        delete this->sfdialog;
    }
    
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Saving file... " << this->filepath << END_COLOR << std::endl;

    SaveFile savefile(this->filepath, &this->subautomataList, this->listInterfaces, this->listLibraries);
    savefile.init();
}

// Load the specified file
void VisualHFSM::on_load_file ( std::string path ) {
    this->filepath = std::string(path);

    delete this->lfdialog;
    
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Loading file... " << this->filepath << END_COLOR << std::endl;

    try {
        MySaxParser parser;
        parser.set_substitute_entities(true);
        parser.parse_file(filepath);

        this->listInterfaces = parser.getConfigFile();
        this->listLibraries = parser.getListLibs();

        this->removeAllGui();
        if (!this->loadSubautomata(parser.getListSubautomata()))
            std::cout << BEGIN_RED << VISUAL << "ERROR loading subautomata" << END_COLOR << std::endl;
    } catch ( const xmlpp::exception& ex ) {
        std::cout << "libxml++ exception: " << ex.what() << std::endl;
    }
}

// Receive a list of interfaces for the automata
void VisualHFSM::on_config_text ( std::list<IceInterface>& listInterfaces ) {
    this->listInterfaces = listInterfaces;
    delete this->cfdialog;
}

// Receive a list of libraries for the automata
void VisualHFSM::on_additional_libraries ( std::list<std::string> listLibraries ) {
    this->listLibraries = listLibraries;
    delete this->ldialog;
}

// Receive the signal of name changed in a state for changing it in the treeview
void VisualHFSM::on_change_node_name ( int id, std::string name ) {
    this->changeNameInTreeView(id, name, this->refTreeModel->children());
}

/*************************************************************
 * INTERNAL METHODS
 *************************************************************/
// Create a popup menu for transitions
void VisualHFSM::create_menu_transition () {
    this->actionGroupTransition = Gtk::ActionGroup::create();
    this->actionGroupTransition->add(Gtk::Action::create("ContextMenu", "Context Menu"));
    this->actionGroupTransition->add(Gtk::Action::create("ContextRename", "Rename"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_transition_rename));
    this->actionGroupTransition->add(Gtk::Action::create("ContextEdit", "Edit"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_transition_edit));
    this->actionGroupTransition->add(Gtk::Action::create("ContextCode", "Code"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_transition_code));
    this->actionGroupTransition->add(Gtk::Action::create("ContextRemove", "Remove"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_transition_remove));

    this->UIManagerTransition = Gtk::UIManager::create();
    this->UIManagerTransition->insert_action_group(this->actionGroupTransition);
    add_accel_group(this->UIManagerTransition->get_accel_group());
    Glib::ustring ui_info =
        "<ui>"
        "   <popup name='PopupMenuTransition'>"
        "       <menuitem action='ContextRename'/>"
        "       <menuitem action='ContextEdit'/>"
        "       <menuitem action='ContextCode'/>"
        "       <menuitem action='ContextRemove'/>"
        "  </popup>"
        "</ui>";
    try {
        this->UIManagerTransition->add_ui_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "building menus failed: " << ex.what();
    }

    //Get the menu:
    this->menuPopupTransition = dynamic_cast<Gtk::Menu*>(
                    this->UIManagerTransition->get_widget("/PopupMenuTransition")); 
    if (!this->menuPopupTransition)
        g_warning("menu not found");
}

// Create a popup menu for states
void VisualHFSM::create_menu_item () {
    this->actionGroupItem = Gtk::ActionGroup::create();
    this->actionGroupItem->add(Gtk::Action::create("ContextMenu", "Context Menu"));
    this->actionGroupItem->add(Gtk::Action::create("ContextRename", "Rename"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_state_rename));
    this->actionGroupItem->add(Gtk::Action::create("ContextEdit", "Edit"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_state_edit));
    this->actionGroupItem->add(Gtk::Action::create("ContextMarkAsInitial", "Mark as initial"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_state_markasinitial));
    this->actionGroupItem->add(Gtk::Action::create("ContextCopy", "Copy"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_state_copy));
    this->actionGroupItem->add(Gtk::Action::create("ContextRemove", "Remove"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_state_remove));

    this->UIManagerItem = Gtk::UIManager::create();
    this->UIManagerItem->insert_action_group(this->actionGroupItem);
    add_accel_group(this->UIManagerItem->get_accel_group());
    Glib::ustring ui_info =
        "<ui>"
        "   <popup name='PopupMenu'>"
        "       <menuitem action='ContextRename'/>"
        "       <menuitem action='ContextEdit'/>"
        "       <menuitem action='ContextMarkAsInitial'/>"
        "       <menuitem action='ContextCopy'/>"
        "       <menuitem action='ContextRemove'/>"
        "  </popup>"
        "</ui>";
    try {
        this->UIManagerItem->add_ui_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "building menus failed: " << ex.what();
    }

    //Get the menu:
    this->menuPopupItem = dynamic_cast<Gtk::Menu*>(this->UIManagerItem->get_widget("/PopupMenu")); 
    if(!this->menuPopupItem)
        g_warning("menu not found");
}

// Create a popup menu for paste states in the canvas
void VisualHFSM::create_menu_paste () {
    this->actionGroupPaste = Gtk::ActionGroup::create();
    this->actionGroupPaste->add(Gtk::Action::create("ContextMenu", "Context Menu"));
    this->actionGroupPaste->add(Gtk::Action::create("ContextPaste", "Paste"),
                        sigc::mem_fun(this, &VisualHFSM::on_menu_canvas_paste));

    this->UIManagerPaste = Gtk::UIManager::create();
    this->UIManagerPaste->insert_action_group(this->actionGroupPaste);
    add_accel_group(this->UIManagerPaste->get_accel_group());
    Glib::ustring ui_infoPaste =
        "<ui>"
        "   <popup name='PopupMenuPaste'>"
        "       <menuitem action='ContextPaste'/>"
        "  </popup>"
        "</ui>";
    try {
        this->UIManagerPaste->add_ui_from_string(ui_infoPaste);
    } catch(const Glib::Error& ex) {
        std::cerr << "building menu paste failed: " << ex.what();
    }

    //Get the menu:
    this->menuPopupPaste = dynamic_cast<Gtk::Menu*>(
                    this->UIManagerPaste->get_widget("/PopupMenuPaste")); 
    if(!this->menuPopupPaste)
        g_warning("menu paste not found");
}

// Create a new state, receiving the ID of the subautomata son
void VisualHFSM::create_new_state ( int idSubautomataSon ) {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Creating state with ID: " << this->idguinode << END_COLOR << std::endl;

    this->currentSubautomata->newGuiNode(this->idguinode, idSubautomataSon,
                                                this->event_x, this->event_y);

    if (this->currentSubautomata->getId() == 1) {
        Gtk::TreeModel::Row row = *(refTreeModel->append());
        row[m_Columns.m_col_id] = this->idguinode;
        row[m_Columns.m_col_name] = this->nameNode;
    } else {
        this->fillTreeView(this->nameNode, refTreeModel->children(),
                    this->getIdNodeInSubautomata(this->currentSubautomata->getIdFather()));
    }


    this->state = NORMAL;
    this->root->add_child(this->currentSubautomata->getLastEllipse());

    this->state = TEXT;
    this->root->add_child(this->currentSubautomata->getLastTextNode());
    
    this->state = INIT;
    this->root->add_child(this->currentSubautomata->getLastEllipseInit());

    this->currentSubautomata->checkLastGuiNodeForInitial();

    this->state = ALL;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Done!" << END_COLOR << std::endl;
}

// Create a new transition between 'item' and 'lastItem'
void VisualHFSM::create_new_transition ( const Glib::RefPtr<Goocanvas::Item>& item ) {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Creating transition" << END_COLOR << std::endl;

    this->theOtherItem = item;

    Point porigin = this->currentSubautomata->getPoint(this->lastItem);
    Point pfinal = this->currentSubautomata->getPoint(this->theOtherItem);

    if (porigin.equals(pfinal)) { // autotransition!
        float xcenter = porigin.getX();
        porigin.setX(xcenter - RADIUS_NORMAL + 2);
        pfinal.setX(xcenter + RADIUS_NORMAL - 2);

        int numberAutotrans = this->currentSubautomata->getNumberOfAutotransitions(item);

        Point pmidpoint(xcenter, porigin.getY() + (2 + 3 * numberAutotrans) * RADIUS_NORMAL);

        this->currentSubautomata->newGuiTransition(porigin, pfinal, pmidpoint, this->idguitransition);
    } else {
        this->currentSubautomata->newGuiTransition(porigin, pfinal, this->idguitransition);
    }

    this->idguitransition++;

    this->state = TRANS_LEFT;
    this->root->add_child(this->currentSubautomata->getLastLeftLine());

    this->state = TRANS_RIGHT;
    this->root->add_child(this->currentSubautomata->getLastRightLine());

    this->state = TEXT;
    this->root->add_child(this->currentSubautomata->getLastTextTransition());

    this->state = TRANS_MIDPOINT;
    this->root->add_child(this->currentSubautomata->getLastMidpoint());

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Done!" << END_COLOR << std::endl;
}

// Create a new transition when loading an xml file
void VisualHFSM::create_new_transition ( Point origin, Point final, Point midpoint, int idTransition ) {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Creating transition" << END_COLOR << std::endl;

    if (origin.equals(final)) { // autotransition!
        float xcenter = origin.getX();
        origin.setX(xcenter - RADIUS_NORMAL + 2);
        final.setX(xcenter + RADIUS_NORMAL - 2);

        int numberAutotrans = this->currentSubautomata->getNumberOfAutotransitions(origin);

        Point pmidpoint(xcenter, origin.getY() + (2 + 3 * numberAutotrans) * RADIUS_NORMAL);

        this->currentSubautomata->newGuiTransition(origin, final, pmidpoint, idTransition);
    } else {
        this->currentSubautomata->newGuiTransition(origin, final, midpoint, idTransition);
    }

    this->state = TRANS_LEFT;
    this->root->add_child(this->currentSubautomata->getLastLeftLine());

    this->state = TRANS_RIGHT;
    this->root->add_child(this->currentSubautomata->getLastRightLine());

    this->state = TEXT;
    this->root->add_child(this->currentSubautomata->getLastTextTransition());

    this->state = TRANS_MIDPOINT;
    this->root->add_child(this->currentSubautomata->getLastMidpoint());

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Done!" << END_COLOR << std::endl;
}

/*************************************************************
 * OF THE MENUS (TRANSITIONS)
 *************************************************************/
// Rename the selected transition
void VisualHFSM::on_menu_transition_rename () {
    this->currentSubautomata->renameGuiTransition(this->selectedItem);
}

// Edit the selected transition
void VisualHFSM::on_menu_transition_edit () {
    this->currentSubautomata->editGuiTransition(this->selectedItem);
}

// Edit the code of the selected transition
void VisualHFSM::on_menu_transition_code () {
    this->currentSubautomata->editGuiTransitionCode(this->selectedItem);
}

// Remove the selected transition
void VisualHFSM::on_menu_transition_remove () {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Removing transition..." << END_COLOR << std::endl;

    GuiTransition* gtransition = this->currentSubautomata->getGuiTransition(this->selectedItem);
    if (gtransition != NULL) {
        root->remove_child(root->find_child(gtransition->getLeftLine()));
        root->remove_child(root->find_child(gtransition->getRightLine()));
        root->remove_child(root->find_child(gtransition->getMidpoint()));
        root->remove_child(root->find_child(gtransition->getTextModel()));
        
        this->currentSubautomata->removeGuiTransitionsWith(this->selectedItem);
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Done!" << END_COLOR << std::endl;
    } else if (DEBUG) {
        std::cout << BEGIN_YELLOW << VISUAL << "Impossible to remove!" << END_COLOR << std::endl;
    }
}

/*************************************************************
 * OF THE MENUS (STATES)
 *************************************************************/
// Rename a state
void VisualHFSM::on_menu_state_rename () {
    this->copyPressed = false;

    std::list<GuiNode>* listGuinodes = this->currentSubautomata->getListGuiNodes();
    std::list<GuiNode>::iterator nodeListIterator = listGuinodes->begin();
    while ( (!nodeListIterator->hasThisItem(this->selectedItem)) &&
            (nodeListIterator != listGuinodes->end()) )
        nodeListIterator++;

    if (nodeListIterator != listGuinodes->end()) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << GUISUB << "Renaming node" << END_COLOR << std::endl;

        this->rdialog = new RenameDialog(&*nodeListIterator);
        this->rdialog->init();
        this->rdialog->signal_change_node().connect(sigc::mem_fun(this,
                                                &VisualHFSM::on_change_node_name));
    }
}

// Edit the code of a state
void VisualHFSM::on_menu_state_edit () {
    this->copyPressed = false;

    this->currentSubautomata->editGuiNode(this->selectedItem);
}

// Mark the state as initial
void VisualHFSM::on_menu_state_markasinitial () {
    this->copyPressed = false;

    this->currentSubautomata->markGuiNodeAsInitial(this->selectedItem);
}

// Just put the 'copyPressed' variable to true
void VisualHFSM::on_menu_state_copy () {
    this->copyPressed = true;
}

// Remove a state
void VisualHFSM::on_menu_state_remove () {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Removing state..." << END_COLOR << std::endl;
    
    this->copyPressed = false;

    GuiNode* gnode = this->currentSubautomata->getGuiNode(this->selectedItem);
    if (gnode != NULL) {
        this->removeFromTreeView(gnode->getId(), this->refTreeModel->children());
        this->removeRecursively(this->currentSubautomata, gnode);

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Done!" << END_COLOR << std::endl;
    } else if (DEBUG) {
        std::cout << BEGIN_YELLOW << VISUAL << "Impossible to remove!" << END_COLOR << std::endl;
    }
}

// Paste a state
void VisualHFSM::on_menu_canvas_paste () {
    this->copyPressed = true;
    GuiNode* gnode = this->currentSubautomata->getGuiNode(this->selectedItem);
    this->nameNode = gnode->getName();
    this->create_new_state(0);
    this->currentSubautomata->setNameLastGuiNode(this->nameNode);
    this->currentSubautomata->setCodeLastGuiNode(gnode->getCode());
    this->idguinode++;
}

/*************************************************************
 * OF THE TREEVIEW
 *************************************************************/
// Fills the treeview with a new state
bool VisualHFSM::fillTreeView ( std::string nameNode, Gtk::TreeModel::Children child, int idNodeFather ) {
    bool cont = true;
    Gtk::TreeModel::Children::iterator iter = child.begin();
    while ( cont && (iter != child.end()) ) {
        Gtk::TreeModel::Row therow = *iter;
        if (therow[m_Columns.m_col_id] == idNodeFather) {
            Gtk::TreeModel::Row row = *(refTreeModel->append(therow.children()));
            row[m_Columns.m_col_id] = this->idguinode;
            row[m_Columns.m_col_name] = nameNode;
            cont = false;
        } else {
            cont = this->fillTreeView(nameNode, therow.children(), idNodeFather);
            iter++;
        }
    }

    return cont;
}

// Remove from the treeview
bool VisualHFSM::removeFromTreeView ( int id, Gtk::TreeModel::Children child ) {
    bool cont = true;
    Gtk::TreeModel::Children::iterator iter = child.begin();
    while ( cont && (iter != child.end()) ) {
        Gtk::TreeModel::Row therow = *iter;
        if (therow[m_Columns.m_col_id] == id) {
            refTreeModel->erase(therow);
            cont = false;
        } else {
            cont = this->removeFromTreeView(id, therow.children());
            iter++;
        }
    }

    return cont;
}

// Change a name of a state in the treeview
bool VisualHFSM::changeNameInTreeView ( int id, std::string nameNode, Gtk::TreeModel::Children child ) {
    bool cont = true;
    Gtk::TreeModel::Children::iterator iter = child.begin();
    while ( cont && (iter != child.end()) ) {
        Gtk::TreeModel::Row therow = *iter;
        if (therow[m_Columns.m_col_id] == id) {
            therow[m_Columns.m_col_name] = nameNode;
            cont = false;
        } else {
            cont = this->changeNameInTreeView(id, nameNode, therow.children());
            iter++;
        }
    }

    return cont;
}

/*************************************************************
 * OF THE SCHEMA
 *************************************************************/
// Schema events: create an state, goes up to the subautomata father
// and shows the popup menu paste
bool VisualHFSM::on_schema_event ( GdkEvent* event ) {
    if ((event->button.x != 0) && (event->button.y != 0)) {
        this->event_x = event->button.x;
        this->event_y = event->button.y;
    }

    if ((lastButton == STATE) && (event->type == GDK_BUTTON_RELEASE)
            && (event->button.button == 1)) { // create an state
        this->nameNode = std::string("state");
        this->create_new_state(0);
        this->idguinode++;
    } else if ((event->button.button == 2) && (event->type == GDK_BUTTON_RELEASE)) {
        this->on_menubar_clicked_up();
    } else if ((event->button.button == 3) && this->copyPressed && !this->showingMenuPopup) {
        if (menuPopupPaste) {
            std::cout << BEGIN_GREEN << VISUAL << "Showing popup paste" << END_COLOR << std::endl;
            menuPopupPaste->popup(((GdkEventButton*)event)->button, ((GdkEventButton*)event)->time);
        }
    } else if ((event->button.button == 3) && this->showingMenuPopup) {
        this->showingMenuPopup = false;
    }

    return false;
}

// Method called when an item is created, both states and transitions
void VisualHFSM::on_item_created ( const Glib::RefPtr<Goocanvas::Item>& item,
                                   const Glib::RefPtr<Goocanvas::ItemModel>& /* model */) {
    if (this->state == NORMAL) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;
            
        item->signal_button_press_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_button_press_event));
        item->signal_button_release_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_button_release_event));
        item->signal_motion_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_motion_notify_event));
        item->signal_enter_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_enter_notify_event));
        item->signal_leave_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_leave_notify_event));

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item NORMAL created" << END_COLOR << std::endl;

        this->selectedItem = item;
    } else if (this->state == INIT) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;

        item->signal_enter_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_item_enter_notify_event));

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item INIT created" << END_COLOR << std::endl;

        this->currentSubautomata->setGuiNodeItems(this->selectedItem, item, this->textItem);
    } else if (this->state == TEXT) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item TEXT created" << END_COLOR << std::endl;

        this->textItem = item;
    } else if (this->state == TRANS_LEFT) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item TRANS_LEFT created" << END_COLOR << std::endl;

        this->leftItemTrans = item;
    } else if (this->state == TRANS_RIGHT) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item TRANS_RIGHT created" << END_COLOR << std::endl;

        this->rightItemTrans = item;
    } else if (this->state == TRANS_MIDPOINT) {
        Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
        if (group)
            return;

        item->signal_button_press_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_transition_button_press_event));
        item->signal_button_release_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_transition_button_release_event));
        item->signal_motion_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_transition_motion_notify_event));
        item->signal_enter_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_transition_enter_notify_event));
        item->signal_leave_notify_event().connect(
                sigc::mem_fun(this, &VisualHFSM::on_transition_leave_notify_event));

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Item TRANS_MIDPOINT created" << END_COLOR << std::endl;

        this->currentSubautomata->setGuiTransitionItems(this->leftItemTrans,
                                                this->rightItemTrans, item, this->lastItem,
                                                this->theOtherItem, this->textItem);
    }
}

// Button press events: goes deep to the subautomata child
// or shows the popup menu for a state
bool VisualHFSM::on_item_button_press_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventButton* event ) {
    if ((event->button == 1) && item) {
        if (event->type == GDK_2BUTTON_PRESS) {
            if (DEBUG)
                std::cout << BEGIN_GREEN << VISUAL << "Going deep" << END_COLOR << std::endl;
            
            this->currentSubautomata->hideAll();
            int idSubautomataSon = this->currentSubautomata->getIdSubautomataSon(item);
            if (idSubautomataSon != 0) {
                if (DEBUG)
                    std::cout << BEGIN_GREEN << VISUAL << "To the automata with id: " << idSubautomataSon << END_COLOR << std::endl;
                
                this->currentSubautomata = this->getSubautomata(idSubautomataSon);
                this->currentSubautomata->showAll();
            } else {
                if (DEBUG)
                    std::cout << BEGIN_GREEN << VISUAL << "Creating a new subautomata with id: " << id << END_COLOR << std::endl;
                
                this->currentSubautomata->setIdSubautomataSon(id, item);
                GuiSubautomata newSubautomata(id, this->currentSubautomata->getId());
                newSubautomata.setTime(TIME_DEFAULT);
                subautomataList.push_back(newSubautomata);
                this->currentSubautomata = this->getSubautomata(id);
                id++;
            }

            lastButton = ANY;
        } else if (event->type == GDK_BUTTON_PRESS) {
            dragging = item;
            drag_x = (double) event->x;
            drag_y = (double) event->y;
            if (this->lastButton != TRANSITION)
                this->lastButton = ANY;
        }
    } else if ((event->button == 3) && item) {
        if(menuPopupItem) {
            this->showingMenuPopup = true;
            this->selectedItem = item;
            
            if (DEBUG)
                std::cout << BEGIN_GREEN << VISUAL << "Showing menu popup" << END_COLOR << std::endl;
            
            menuPopupItem->popup(event->button, event->time);
        }
    }
    
    return false;
}

// Release events: reset the drag and create a new transition (just in case)
bool VisualHFSM::on_item_button_release_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                GdkEventButton* event) {
    if ((event->button == 1)) {
        dragging.reset();

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Button release event" << END_COLOR << std::endl;

        if (this->lastButton == TRANSITION) {
            if (transitionsCounter % 2 == 0) {
                this->lastItem = item;
            } else {
                this->create_new_transition(item);
                this->lastItem = item;

                this->state = NONE;
            }
            transitionsCounter++;
        }
    }
    
    return false;
}

// Notify event to move a state
bool VisualHFSM::on_item_motion_notify_event (  const Glib::RefPtr<Goocanvas::Item>& item,
                                                GdkEventMotion* event) {
    if (item && dragging && (item == dragging)) {
        double new_x = event->x;
        double new_y = event->y;

        this->currentSubautomata->moveGuiNode(item, new_x - drag_x, new_y - drag_y);
        this->currentSubautomata->moveGuiTransition(item);
    }
    
    return false;
}

// Notify event when entering a state (change the state's width)
bool VisualHFSM::on_item_enter_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event) {
    if (item) {
        if (this->state == ALL) {
            this->state = NONE;
        } else if (this->state == NONE) {
            if (DEBUG)
                std::cout << BEGIN_GREEN << VISUAL << "Changing node width to 3" << END_COLOR << std::endl;
            
            this->currentSubautomata->changeGuiNodeWidth(item, 3);
        }
    }

    return false;
}

// Notify event when leaving a state (change the state's width)
bool VisualHFSM::on_item_leave_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event) {
    if (item) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Changing node width to 1" << END_COLOR << std::endl;
        
        this->currentSubautomata->changeGuiNodeWidth(item, 1);
    }

    return false;
}

// Button press event for: start dragging or show the popup menu transition
bool VisualHFSM::on_transition_button_press_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                    GdkEventButton* event ) {
    if ((event->button == 1) && item && (event->type == GDK_BUTTON_PRESS)) {
        dragging = item;
        drag_x = (double) event->x;
        drag_y = (double) event->y;
        if (this->lastButton != TRANSITION)
            this->lastButton = ANY;
    } else if ((event->button == 3) && item) {
        if (menuPopupTransition) {
            this->selectedItem = item;

            if (DEBUG)
                std::cout << BEGIN_GREEN << VISUAL << "Showing menu transition popup" << END_COLOR << std::endl;

            menuPopupTransition->popup(event->button, event->time);
        }
    }
    
    return false;
}

// Release events: reset the drag
bool VisualHFSM::on_transition_button_release_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                      GdkEventButton* event) {
    if (event->button == 1)
        dragging.reset();
    
    return false;
}

// Notify event to move a transition
bool VisualHFSM::on_transition_motion_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                     GdkEventMotion* event) {
    if (item && dragging && (item == dragging)) {
        double new_x = event->x;
        double new_y = event->y;

        this->currentSubautomata->moveJustGuiTransition(item, new_x - drag_x, new_y - drag_y);
    }
    
    return false;
}

// Notify event when entering a transition (change the transition's width)
bool VisualHFSM::on_transition_enter_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                    GdkEventCrossing* event) {
    if (item) {
        if (this->state == NONE) {
            if (DEBUG)
                std::cout << BEGIN_GREEN << VISUAL << "Changing transition width to 3" << END_COLOR << std::endl;

            this->currentSubautomata->changeGuiTransitionWidth(item, 3);
        }
    }

    return false;
}

// Notify event when leaving a transition (change the transition's width)
bool VisualHFSM::on_transition_leave_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                                     GdkEventCrossing* event) {
    if (item) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Changing transition width to 1" << END_COLOR << std::endl;

        this->currentSubautomata->changeGuiTransitionWidth(item, 1);
    }

    return false;
}

/*************************************************************
 * OF THE MENU
 *************************************************************/
// New automata, remove all
// TODO: remove the treeview
void VisualHFSM::on_menubar_clicked_new () {
    this->removeAllGui();
    this->removeAllSubautomata();
    this->filepath = std::string("");
}

// Open an automata previously saved
void VisualHFSM::on_menubar_clicked_open () {
    lastButton = OPEN;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Open automata" << END_COLOR << std::endl;

    this->lfdialog = new LoadFileDialog();
    this->lfdialog->init();
    this->lfdialog->signal_path().connect(sigc::mem_fun(this,
                                        &VisualHFSM::on_load_file));
}

// Save an automata
void VisualHFSM::on_menubar_clicked_save () {
    lastButton = SAVE;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Save automata" << END_COLOR << std::endl;

    if (this->filepath.empty())
        this->on_menubar_clicked_save_as();
    else
        this->on_save_file(this->filepath);
}

// Save as (if you want to change the name and/or location)
void VisualHFSM::on_menubar_clicked_save_as () {
    lastButton = SAVE_AS;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Save as..." << END_COLOR << std::endl;

    this->sfdialog = new SaveFileDialog();
    this->sfdialog->init();
    this->sfdialog->signal_path().connect(sigc::mem_fun(this,
                                        &VisualHFSM::on_save_file));
}

// Close the application
void VisualHFSM::on_menubar_clicked_quit () {
    this->hide(); // hide() will cause main::run() to end
}

// New state to be generated
void VisualHFSM::on_menubar_clicked_state () {
    lastButton = STATE;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Create a state?" << END_COLOR << std::endl;
}

// New transition to be generated
void VisualHFSM::on_menubar_clicked_transition () {
    lastButton = TRANSITION;
    transitionsCounter = 0;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Create a transition?" << END_COLOR << std::endl;
}

// Insert the timer for this automata
void VisualHFSM::on_menubar_clicked_timer () {
    lastButton = TIMER;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Timer of this automata" << END_COLOR << std::endl;

    TimerDialog* tdialog = new TimerDialog(this->currentSubautomata);
    tdialog->init();
}

// Insert variables and functions for this automata
void VisualHFSM::on_menubar_clicked_variables () {
    lastButton = VARIABLES;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Variables for this automata" << END_COLOR << std::endl;

    FunVarDialog* fvdialog = new FunVarDialog(this->currentSubautomata);
    fvdialog->init();
}

// Insert libraries for the project
void VisualHFSM::on_menubar_clicked_libraries () {
    lastButton = LIBRARIES;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Additional libraries for this automata" << END_COLOR << std::endl;

    this->ldialog = new LibrariesDialog(this->listLibraries);
    this->ldialog->init();
    this->ldialog->signal_libraries().connect(sigc::mem_fun(this,
                                        &VisualHFSM::on_additional_libraries));
}

// Create a new config file for the project
void VisualHFSM::on_menubar_clicked_configfile () {
    lastButton = VARIABLES;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Config file for this automata" << END_COLOR << std::endl;

    this->cfdialog = new ConfigFileDialog(this->listInterfaces, this->mapInterfacesHeader);
    this->cfdialog->init();
    this->cfdialog->signal_config().connect(sigc::mem_fun(this,
                                        &VisualHFSM::on_config_text));
}

// Generate the code for the project
void VisualHFSM::on_menubar_clicked_generate_code () {
    lastButton = GENERATE_CODE;

    if (this->filepath.compare(std::string("")) != 0) { // if it is not saved, save it please!
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Generating code..." << END_COLOR << std::endl;
        
        if (this->checkAll()) { // all ok!
            SaveFile savefile(this->filepath, &this->subautomataList, this->listInterfaces, this->listLibraries);
            savefile.init();

            MySaxParser parser;
            parser.set_substitute_entities(true);
            parser.parse_file(this->filepath);
            
            this->listInterfaces = parser.getConfigFile();
            this->listLibraries = parser.getListLibs();

            std::string cpppath(this->filepath);
            std::string cfgpath(this->filepath);
            std::string cmakepath(this->filepath);
            if ( (this->replace(cpppath, std::string(".xml"), std::string(".cpp"))) &&
                    (this->replace(cfgpath, std::string(".xml"), std::string(".cfg"))) &&
                    (this->replaceFile(cmakepath, std::string("/"), std::string("CMakeLists.txt"))) ) {
                Generate generate(parser.getListSubautomata(), cpppath, cfgpath, cmakepath,
                            &(this->listInterfaces), this->mapInterfacesHeader, this->listLibraries);
                generate.init();
            } else {
                std::cout << BEGIN_GREEN << VISUAL << "Impossible to generate code" << END_COLOR << std::endl;
            }
        } else { // show problems
            std::cout << BEGIN_RED << VISUAL << "There are some empty important fields that cannot be empty. Possibilities:" << END_COLOR << std::endl;
            std::cout << BEGIN_RED << VISUAL << "- Node names" << END_COLOR << std::endl;
            std::cout << BEGIN_RED << VISUAL << "- Transition names and mode of transition" << END_COLOR << std::endl;
            std::cout << BEGIN_RED << VISUAL << "- Repeated interfaces" << END_COLOR << std::endl;
            std::cout << BEGIN_RED << VISUAL << "- Iteration time for each subautomata" << END_COLOR << std::endl;
        }

    } else {
        std::cout << BEGIN_YELLOW << VISUAL << "You must save the project first" << END_COLOR << std::endl;
    }

}

// Compile the project
void VisualHFSM::on_menubar_clicked_compile () {
    lastButton = COMPILE;

    std::string cpppath(this->filepath);
    this->replace(cpppath, std::string(".xml"), std::string(".cpp"));

    struct stat buffer;

    if ( (this->filepath.compare(std::string("")) != 0) &&
                    (stat (cpppath.c_str(), &buffer) == 0) ) { // check the .xml and .cpp files
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Starting compilation..." << END_COLOR << std::endl;
        
        std::string directory(this->filepath);

        size_t last_pos = directory.find_last_of(std::string("/"));
        if (last_pos == std::string::npos) {
            std::cout << "Impossible to compile" << std::endl;
            return;
        }

        directory.erase(last_pos + 1, std::string::npos);
        std::string directoryBuild(directory + "build");

        std::string hoption("-H" + directory);
        std::string boption("-B" + directoryBuild);
        
        std::string cmake("cmake " + hoption + " " + boption);
        std::string make("make -C " + directoryBuild);

        system(cmake.c_str());
        system(make.c_str());
        
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Please, check if the compilation was succesful" << END_COLOR << std::endl;
    } else {
        std::cout << BEGIN_YELLOW << VISUAL << "You must save the project and generate the code first" << END_COLOR << std::endl;
    }

}

// About
// TODO: generate it
void VisualHFSM::on_menubar_clicked_about () {
    lastButton = COMPILE;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "About" << END_COLOR << std::endl;
}

// Goes up to the upper level of the automata (pressing the middle button of the mouse)
void VisualHFSM::on_menubar_clicked_up () { // Deprecated
    lastButton = UP;

    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Going up" << END_COLOR << std::endl;

    GuiSubautomata* guisub = this->getSubautomata(this->currentSubautomata->getIdFather());
    if (guisub != NULL) {
        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "It is ok to get the father" << END_COLOR << std::endl;

        if (this->currentSubautomata->isNodeListEmpty()) {
            guisub->setToZero(this->currentSubautomata->getId());
            std::list<GuiSubautomata>::iterator listGuisubIterator = this->subautomataList.begin();
            while ( (listGuisubIterator->getId() != this->currentSubautomata->getId()) &&
                    (listGuisubIterator != this->subautomataList.end()) )
                listGuisubIterator++;

            if (listGuisubIterator != this->subautomataList.end())
                this->subautomataList.erase(listGuisubIterator);
        } else {
            this->currentSubautomata->hideAll();
        }

        guisub->showAll();
        this->currentSubautomata = guisub;

        if (DEBUG)
            std::cout << BEGIN_GREEN << VISUAL << "Got the father" << END_COLOR << std::endl;
    }
}

/*************************************************************
 * IN GENERAL
 *************************************************************/
// Get a specified subautomata
GuiSubautomata* VisualHFSM::getSubautomata ( int idSubautomata ) {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Getting subautomata with ID: " << idSubautomata << END_COLOR << std::endl;

    std::list<GuiSubautomata>::iterator subautomataListIterator = this->subautomataList.begin();
    while ( (subautomataListIterator->getId() != idSubautomata) &&
            (subautomataListIterator != this->subautomataList.end()) )
        subautomataListIterator++;

    if (subautomataListIterator != this->subautomataList.end())
        return &*subautomataListIterator;

    return NULL;
}

// Get a specified subautomata with an idFather
GuiSubautomata* VisualHFSM::getSubautomataWithIdFather ( int idFather ) {
    if (DEBUG)
        std::cout << BEGIN_GREEN << VISUAL << "Getting subautomata with ID father: " << idFather << END_COLOR << std::endl;

    std::list<GuiSubautomata>::iterator subautomataListIterator = this->subautomataList.begin();
    while ( (subautomataListIterator->getIdFather() != idFather) &&
            (subautomataListIterator != this->subautomataList.end()) )
        subautomataListIterator++;

    if (subautomataListIterator != this->subautomataList.end())
        return &*subautomataListIterator;

    return NULL;
}

// Load a subautomata
int VisualHFSM::loadSubautomata ( std::list<SubAutomata> subList ) {
    this->removeAllSubautomata();
    std::list<SubAutomata>::iterator subListIterator = subList.begin();
    while ( subListIterator != subList.end() ) {
        int id = subListIterator->getId();
        int idFather = subListIterator->getIdFather();
        
        GuiSubautomata guisub(id, idFather);
        this->subautomataList.push_back(guisub);
        this->currentSubautomata = this->getSubautomata(id);
        this->id = id + 1;

        this->currentSubautomata->setFunctions(subListIterator->getFunctions());
        this->currentSubautomata->setTime(subListIterator->getTime());
        this->currentSubautomata->setVariables(subListIterator->getVariables());

        std::list<Node> nodeList = subListIterator->getNodeList();
        std::list<Node>::iterator nodeListIterator = nodeList.begin();
        while ( nodeListIterator != nodeList.end() ) {
            int idNode = nodeListIterator->getId();
            Point* nodePoint = subListIterator->getNodePoint(idNode);
            this->event_x = nodePoint->getX();
            this->event_y = nodePoint->getY();
            this->idguinode = idNode;
            this->nameNode = std::string(nodeListIterator->getName());
            this->create_new_state(nodeListIterator->getIdSubautomataSon());
            this->currentSubautomata->setIsInitialLastGuiNode(nodeListIterator->isInitial());
            this->currentSubautomata->setNameLastGuiNode(nodeListIterator->getName());
            this->currentSubautomata->setCodeLastGuiNode(nodeListIterator->getCode());
            nodeListIterator++;
        }

        std::list<Transition> transList = subListIterator->getTransList();
        std::list<Transition>::iterator transListIterator = transList.begin();
        while ( transListIterator != transList.end() ) {
            int idTransition = transListIterator->getId();
            Point* ptransition = subListIterator->getTransPoint(idTransition);
            
            int idOrigin = transListIterator->getIdOrigin();
            Point* porigin = subListIterator->getNodePoint(idOrigin);
            
            int idDestiny = transListIterator->getIdDestiny();
            Point* pdestiny = subListIterator->getNodePoint(idDestiny);
            
            this->lastItem = this->currentSubautomata->getGuiNodeItem(idOrigin);
            this->theOtherItem = this->currentSubautomata->getGuiNodeItem(idDestiny);

            this->create_new_transition(*porigin, *pdestiny, *ptransition, idTransition);
            this->currentSubautomata->setTransLastGuiTransition(transListIterator->getType(), transListIterator->getCodeTrans());
            this->currentSubautomata->setNameLastGuiTransition(transListIterator->getName());
            this->currentSubautomata->setCodeLastGuiTransition(transListIterator->getCode());
            this->state = NONE;
            
            transListIterator++;
        }

        if (idFather != 0)
            this->currentSubautomata->hideAll();

        subListIterator++;
    }

    this->currentSubautomata = this->getSubautomataWithIdFather(0);

    return 1;
}

// Remove the GUI
void VisualHFSM::removeAllGui () {
    for ( std::list<GuiSubautomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ ) {
        std::list<GuiNode>* nodeList = subListIterator->getListGuiNodes();
        for ( std::list<GuiNode>::iterator nodeListIterator = nodeList->begin();
                nodeListIterator != nodeList->end(); nodeListIterator++ ) {
            this->root->remove_child(this->root->find_child(nodeListIterator->getEllipse()));
            this->root->remove_child(this->root->find_child(nodeListIterator->getEllipseInitial()));
            this->root->remove_child(this->root->find_child(nodeListIterator->getText()));
        }

        std::list<GuiTransition>* transList = subListIterator->getListGuiTransitions();
        for ( std::list<GuiTransition>::iterator transListIterator = transList->begin();
                transListIterator != transList->end(); transListIterator++ ) {
            this->root->remove_child(this->root->find_child(transListIterator->getLeftLine()));
            this->root->remove_child(this->root->find_child(transListIterator->getRightLine()));
            this->root->remove_child(this->root->find_child(transListIterator->getMidpoint()));
            this->root->remove_child(this->root->find_child(transListIterator->getTextModel()));
        }
    }
}

// Remove all subautomata
void VisualHFSM::removeAllSubautomata () {
    for ( std::list<GuiSubautomata>::iterator subListIterator = this->subautomataList.begin();
            subListIterator != this->subautomataList.end(); subListIterator++ )
        subListIterator->removeAll();

    this->subautomataList.clear();
}

// Get an ID of a node with a specified subautomata
int VisualHFSM::getIdNodeInSubautomata ( int subautomataId ) {
    std::list<GuiSubautomata>::iterator subListIterator = this->subautomataList.begin();
    while ( (subListIterator->getId() != subautomataId) &&
            (subListIterator != this->subautomataList.end()) )
        subListIterator++;

    std::list<GuiNode>* guiNodeList = subListIterator->getListGuiNodes();
    std::list<GuiNode>::iterator guiNodeListIterator = guiNodeList->begin();
    while ( (guiNodeListIterator->getIdSubautomataSon() != this->currentSubautomata->getId()) &&
            (guiNodeListIterator != guiNodeList->end()) )
        guiNodeListIterator++;

    return guiNodeListIterator->getId();
}

// Get an ID of a subautomata with a scpecified node
int VisualHFSM::getIdSubautomataWithNode ( int idNode ) {
    int id = 0;
    std::list<GuiSubautomata>::iterator subListIterator = this->subautomataList.begin();
    while ( (id == 0) && (subListIterator != this->subautomataList.end()) ) {
        std::list<GuiNode>* nodeList = subListIterator->getListGuiNodes();
        std::list<GuiNode>::iterator nodeListIterator = nodeList->begin();
        while ( (id == 0) && (nodeListIterator != nodeList->end()) ) {
            if (nodeListIterator->getId() == idNode)
                id = subListIterator->getId();
            else
                nodeListIterator++;
        }
        subListIterator++;
    }

    return id;
}

// Remove a node and its associated transitions
void VisualHFSM::remove ( GuiSubautomata* guisub, GuiNode* gnode ) {
    root->remove_child(root->find_child(gnode->getEllipse()));
    root->remove_child(root->find_child(gnode->getEllipseInitial()));
    root->remove_child(root->find_child(gnode->getText()));

    std::list<GuiTransition> nodeTrans =
                    guisub->getAllGuiTransitionsWith(gnode->getId());
    std::list<GuiTransition>::iterator nodeTransIterator = nodeTrans.begin();

    while ( nodeTransIterator != nodeTrans.end() ) {
        root->remove_child(root->find_child(nodeTransIterator->getLeftLine()));
        root->remove_child(root->find_child(nodeTransIterator->getRightLine()));
        root->remove_child(root->find_child(nodeTransIterator->getMidpoint()));
        root->remove_child(root->find_child(nodeTransIterator->getTextModel()));
        nodeTransIterator++;
    }

    guisub->removeGuiNode(gnode->getId());
}

// Removes a node recursively (including its subautomata children)
void VisualHFSM::removeRecursively ( GuiSubautomata* guisub, GuiNode* gnode ) {
    int idSubautomataSon = gnode->getIdSubautomataSon();
    if (idSubautomataSon != 0) {
        GuiSubautomata* subautomata = this->getSubautomata(idSubautomataSon);
        std::list<GuiNode> guiNodeList = *(subautomata->getListGuiNodes());
        for ( std::list<GuiNode>::iterator guiNodeListIterator = guiNodeList.begin();
                guiNodeListIterator != guiNodeList.end(); guiNodeListIterator++ ) {
            this->removeRecursively(subautomata, &*guiNodeListIterator);
        }
    }
    
    this->remove(guisub, gnode);
}

// Check if a string has the specified ending (usually for .xml endings)
bool VisualHFSM::hasEnding ( std::string const &fullString, std::string const &ending ) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(),
                                                        ending.length(), ending));
    } else {
        return false;
    }
}

// Replace in the 'str', 'from' to 'to'
bool VisualHFSM::replace ( std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;

    str.replace(start_pos, from.length(), to);
    
    return true;
}

// Replace in 'str', 'character' to 'to'
bool VisualHFSM::replaceFile ( std::string& str, const std::string& character, std::string to ) {
    size_t last_pos = str.find_last_of(character);
    if (last_pos == std::string::npos)
        return false;
    
    str.replace(last_pos + 1, std::string::npos, to);

    return true;
}

// Check if it is all ok
bool VisualHFSM::checkAll () {
    // The interfaces are all different
    for ( std::list<IceInterface>::iterator first = this->listInterfaces.begin();
            first != this->listInterfaces.end(); first++ ) {
        std::list<IceInterface>::iterator aux = first;
        aux++;
        for ( std::list<IceInterface>::iterator second = aux;
                second != this->listInterfaces.end(); second++ ) {
            if (first->equals(&*second))
                return false;
        }
    }

    std::list<GuiSubautomata>::iterator subautomataListIterator = this->subautomataList.begin();
    while ( subautomataListIterator != this->subautomataList.end() ) {
        // If there are no empty automatons
        if ( subautomataListIterator->isNodeListEmpty() &&
                (this->currentSubautomata->getId() != subautomataListIterator->getId()) ) {
            GuiSubautomata* gsub = this->getSubautomata(subautomataListIterator->getIdFather());
            gsub->setToZero(subautomataListIterator->getId());
            subautomataListIterator = this->subautomataList.erase(subautomataListIterator);
        } else {
            // The subautomata is all ok
            if ( !subautomataListIterator->checkAll() )
                return false;

            // The timing for the automata is not empty
            if (subautomataListIterator->getTime().compare("") == 0)
                return false;

            subautomataListIterator++;
        }
    }

    return true;
}