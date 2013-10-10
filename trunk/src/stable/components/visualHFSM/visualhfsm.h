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

#ifndef VISUALHFSM_H
#define VISUALHFSM_H

#include <iostream>
#include <stdio.h>
#include <gtkmm/stock.h>
#include <math.h>
#include <sigc++/sigc++.h>
#include <libxml++/libxml++.h>

#include "xmlparser.h"
#include "savefile.h"
#include "generate.h"
#include "guisubautomata.h"
#include "popups/funvardialog.h"
#include "popups/importdialog.h"
#include "popups/loadfiledialog.h"
#include "popups/timerdialog.h"
#include "popups/savefiledialog.h"

typedef enum Button {
    UP,
    TRANSITION,
    STATE,
    SAVE,
    SAVE_AS,
    OPEN,
    INTERFACES,
    TIMER,
    VARIABLES,
    GENERATE_CODE,
    COMPILE,
    ANY
} Button;

typedef enum TypeInitial {
    NORMAL,
    INIT,
    TEXT,
    ALL,
    TRANS_LEFT,
    TRANS_RIGHT,
    TRANS_MIDPOINT,
    NONE
} TypeInitial;

// Definition of this class
class VisualHFSM : public Gtk::Window {
public:
    // Constructor
    VisualHFSM ();
        
    // Destructor
    virtual ~VisualHFSM ();

    // Methods for signals from save and load files
    void on_save_file ( std::string path );
    void on_load_file ( std::string path );

private:
    // Main window
    Gtk::Viewport* viewport_schema;
    Goocanvas::Canvas* canvas;

    // For the draggin item
    Glib::RefPtr<Goocanvas::Item> dragging;
    int drag_x, drag_y;

    // The state to determine the action (create signals or not, add the item to the node...)
    TypeInitial state;

    // The ID for the subautomata created
    int id, idguinode, idguitransition;

    // GroupModel for the items
    Glib::RefPtr<Goocanvas::ItemModel> root;
    // The scrolled window that contains everything
    Gtk::ScrolledWindow* sw;

    // The treeview
    //Tree model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns () { add(m_col_id); add(m_col_name); }

        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };
    ModelColumns m_Columns;

    Gtk::TreeView* treeview;
    Glib::RefPtr<Gtk::TreeStore> refTreeModel;

    // Popup menus
    Glib::RefPtr<Gtk::ActionGroup> actionGroupTransition;
    Glib::RefPtr<Gtk::UIManager> UIManagerTransition;
    Gtk::Menu* menuPopupTransition;

    Glib::RefPtr<Gtk::ActionGroup> actionGroupItem;
    Glib::RefPtr<Gtk::UIManager> UIManagerItem;
    Gtk::Menu* menuPopupItem;

    bool showingMenuPopup, copyPressed;

    Glib::RefPtr<Gtk::ActionGroup> actionGroupPaste;
    Glib::RefPtr<Gtk::UIManager> UIManagerPaste;
    Gtk::Menu* menuPopupPaste;

    // Options for the selected item
    Glib::RefPtr<Goocanvas::Item> selectedItem;
    bool isSelected;
    Glib::RefPtr<Goocanvas::Item> textItem;

    // Options for the creation of transitions
    Glib::RefPtr<Goocanvas::Item> lastItem, theOtherItem, leftItemTrans, rightItemTrans;
    int transitionsCounter;

    // Where the mouse is
    float event_x, event_y;

    // The subautomata list and current subautomata
    std::list <GuiSubautomata> subautomataList;
    GuiSubautomata* currentSubautomata;

    // Buttons
    Gtk::Button *button_up, *button_transition, *button_state, *button_save;
    Gtk::Button *button_save_as, *button_open, *button_interfaces;
    Gtk::Button *button_timer, *button_variables, *button_generate_code, *button_compile;

    // For files (load and save)
    Gtk::Button *button_cancelfile, *button_openfile, *button_savefile;
    std::string filepath;
    SaveFileDialog* sfdialog;
    LoadFileDialog* lfdialog;

    Button lastButton;

    // Internal methods
    int get_all_widgets ();
    void assign_signals ();
    void create_menu_transition ();
    void create_menu_item ();
    void create_menu_paste ();
    void create_new_state ( int idSubautomataSon );
    void create_new_transition ( const Glib::RefPtr<Goocanvas::Item>& item );
    void create_new_transition ( Point origin, Point final, Point midpoint, int id );

    // Methods for signals
    // Of the menus
    void on_menu_transition_rename ();
    void on_menu_transition_edit ();
    void on_menu_transition_remove ();

    void on_menu_state_rename ();
    void on_menu_state_edit ();
    void on_menu_state_markasinitial ();
    void on_menu_state_copy ();
    void on_menu_state_remove ();

    void on_menu_canvas_paste ();

    // Of the treeview

    // Of the schema
    bool on_schema_event ( GdkEvent* event );

    void on_item_created (  const Glib::RefPtr<Goocanvas::Item>& item,
                            const Glib::RefPtr<Goocanvas::ItemModel>& model );
    bool on_item_button_press_event (   const Glib::RefPtr<Goocanvas::Item>& item,
                                        GdkEventButton* event );
    bool on_item_button_release_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                        GdkEventButton* event );
    bool on_item_motion_notify_event (  const Glib::RefPtr<Goocanvas::Item>& item,
                                        GdkEventMotion* event );
    bool on_item_enter_notify_event (   const Glib::RefPtr<Goocanvas::Item>& item,
                                        GdkEventCrossing* event );
    bool on_item_leave_notify_event (   const Glib::RefPtr<Goocanvas::Item>& item,
                                        GdkEventCrossing* event );
    bool on_transition_button_press_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                            GdkEventButton* event );
    bool on_transition_button_release_event (   const Glib::RefPtr<Goocanvas::Item>& item,
                                                GdkEventButton* event );
    bool on_transition_motion_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                             GdkEventMotion* event );
    bool on_transition_enter_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                            GdkEventCrossing* event );
    bool on_transition_leave_notify_event ( const Glib::RefPtr<Goocanvas::Item>& item,
                                            GdkEventCrossing* event );

    // Of buttons
    void on_options_clicked_up ();
    void on_options_clicked_transition ();
    void on_options_clicked_state ();
    void on_options_clicked_save ();
    void on_options_clicked_save_as ();
    void on_options_clicked_open ();
    void on_options_clicked_interfaces ();
    void on_options_clicked_timer ();
    void on_options_clicked_variables ();
    void on_options_clicked_generate_code ();
    void on_options_clicked_compile ();

    // General
    GuiSubautomata* getSubautomata ( int idSubautomata );
    GuiSubautomata* getSubautomataWithIdFather ( int idFather );
    int loadSubautomata ( std::list<SubAutomata> subautomataList );
    void removeAllGui ();
    void removeAllSubautomata ();
    bool hasEnding ( std::string const &fullString, std::string const &ending );
    bool replaceFile ( std::string& str, const std::string& character, std::string to );
    bool replace ( std::string& str, const std::string& from, const std::string& to );

    bool fillTreeView ( std::string nameNode, Gtk::TreeModel::Children child, int idNodeFather );
    bool removeFromTreeView ( int id, Gtk::TreeModel::Children child );
    int getIdNodeInSubautomata ( int subautomataId );

    void removeRecursively ( GuiSubautomata* guisub, GuiNode* gnode );
    void remove ( GuiSubautomata* guisub, GuiNode* gnode );

    int getIdSubautomataWithNode ( int idNode );
}; // Class VisualHFSM

#endif // VISUALHFSM_H