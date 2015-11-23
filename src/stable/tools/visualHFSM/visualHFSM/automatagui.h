#ifndef AUTOMATAGUI_H
#define AUTOMATAGUI_H

#include <unistd.h>
#include <iostream>
#include <err.h>
#include <pthread.h>
#include <queue>
#include <gtkmm-3.0/gtkmm.h>

#include "guisubautomata.h"


typedef enum ItemType {
	NONE,
	STATE,
} ItemType;

//Class definition
class AutomataGui {
public:

	AutomataGui(int argc, char** argv);

	virtual ~AutomataGui();

	int init();
	void setGuiSubautomataList(std::list<GuiSubautomata> guiSubList);
	void loadGuiSubautomata();
	void run();
	void notifySetNodeAsActive(std::string nodeName);
	void close();

private:

	Glib::RefPtr<Gtk::Builder> refBuilder;
	Gtk::Dialog *guiDialog;
	Glib::RefPtr<Gtk::Application> app;

	//Main window
	Goocanvas::Canvas* canvas;
	Gtk::ScrolledWindow* scrolledwindow_schema;
	Gtk::Button* pUpButton;
	Gtk::CheckButton* checkAutofocus;
	Glib::RefPtr<Goocanvas::ItemModel> root;

	//Allow MultiThreading
	Glib::Dispatcher dispatcher;
	std::string activeNodeName;

	class Queue{
	public:
		std::queue<std::string> queue;
		pthread_mutex_t lock;
	};
	Queue activesNodesNames;

	class ModelColumns : public Gtk::TreeModel::ColumnRecord{
	public:
		ModelColumns () {
			add(m_col_id);
			add(m_col_name);
			add(m_col_color);
		}
		Gtk::TreeModelColumn<int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_col_color;
	};
	ModelColumns m_Columns;
	ModelColumns m_Columns2;	// <---DON't DELETE THIS

	Gtk::TreeView* treeView;
	Glib::RefPtr<Gtk::TreeStore> refTreeModel;
	Glib::ustring lastExpanded; 
	Gtk::TreeModel::Path pathLastExp;

	std::list<GuiSubautomata> guiSubautomataList;
	GuiSubautomata* currentGuiSubautomata;

	ItemType type;

	//IDs for the subautomata created
	int idGuiNode, idGuiTrans;

	GuiSubautomata* getSubautomataWithIdFather(int id);
	GuiSubautomata* getSubautomata(int id);
	bool isFirstActiveNode(GuiNode* gnode);
	void create_new_state(GuiNode* gnode, std::string color);
	void create_new_transition(GuiTransition* gtrans);
	int getIdNodeFather(int subautomataId, int subautSonId);
	bool fillTreeView(std::string nameNode, std::string color, 
						Gtk::TreeModel::Children child, int idNodeFather);
	GuiSubautomata* getSubautomataByNodeName(std::string name);
	void showSubautomata(int id);
	bool setActiveTreeView(std::string name, bool active,
								Gtk::TreeModel::Children children);
	void treeViewAutoFocus(Gtk::TreeModel::Children::iterator iter, Glib::ustring name);
	void setNodeAsActive(GuiNode* node, GuiSubautomata* subautomata, bool active);

	//Handlers
	void on_notify_received();
	void on_up_button_clicked ();
	void on_item_created(const Glib::RefPtr<Goocanvas::Item>& item, 
							const Glib::RefPtr<Goocanvas::ItemModel>& model);
	bool on_item_button_press_event(const Glib::RefPtr<Goocanvas::Item>& item,
                            GdkEventButton* event);
	bool on_item_enter_notify_event(const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event);
	bool on_item_leave_notify_event(const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event);
	void on_row_activated(const Gtk::TreeModel::Path& path,
							Gtk::TreeViewColumn* /* column */);
};

#endif // AUTOMATAGUI_H