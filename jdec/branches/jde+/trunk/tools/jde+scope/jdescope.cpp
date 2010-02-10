#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <glibmm.h>
#include <gtkmm.h>
#include <gdkmm.h>
#include <libglademm.h>
#include <jde+/hierarchy.h>
#include <jde+/pointer.h>
#include <jde+/schemafactory.h>
#include <jde+/schemainstance.h>
#include <jde+/schematypes.h>
#include <jde+/message.h>
#include <jde+/message_processor.h>


static const std::string icondir = PIXMAPDIR;

enum icon_enum {
  PSCHEMAICON,
  PSCHEMAROOTICON,
  PSCHEMASMALLICON,
  MSCHEMAICON,
  MSCHEMAROOTICON,
  MSCHEMASMALLICON,
  NICONS
};

static const std::string iconpaths[NICONS] = {
  icondir + "/pschema.svg",
  icondir + "/pschema-root.svg",
  icondir + "/pschema-small.svg",
  icondir + "/mschema.svg",
  icondir + "/mschema-root.svg",
  icondir + "/mschema-small.svg"
};

static Glib::RefPtr<Gdk::Pixbuf> iconpixbufs[NICONS];

Glib::RefPtr<Gdk::Pixbuf> get_icon_pixbuf(const icon_enum icon) {
  if (! iconpixbufs[icon])
    iconpixbufs[icon] = Gdk::Pixbuf::create_from_file(iconpaths[icon]);
  return iconpixbufs[icon];
}

static const Glib::ustring state_color[NSTATES] = 
  {"grey","red","DarkOrange","green"};

class Gui_Processor;

class Jdeapp : public Message_Processor{
public:
  Jdeapp();
  ~Jdeapp();
  
  void run();
 

  /**Message processor interface. Executes on instances threads*/
  /**
     Handles perception messages.
  */
  virtual void process(Pointer<Perception> p);

  /**
     Handles modulation messages.
  */
  virtual void process(Pointer<Modulation> m);

  /**
     Handles arbitrate request messages.
  */
  virtual void process(Pointer<Arbitrate_Req> a);

  /**
     Handles state change messages.
  */
  virtual void process(Pointer<State_Change> s);

  /**
     Handles new state messages.
  */
  virtual void process(Pointer<New_State> n);
protected:
  /*signal handlers*/
  virtual void on_play_button_clicked();
  virtual void on_stop_button_clicked();
  virtual void on_instances_treeview_row_activated(const Gtk::TreeModel::Path& path, 
						   Gtk::TreeViewColumn* column);
  virtual void on_instances_treeview_selection_changed();
  virtual void on_openfactoriesdir_button_clicked();
  virtual void on_selectrootfactory_button_clicked();
  virtual void on_factories_iconview_item_activated(const Gtk::TreeModel::Path& path);

  class InstanceModelColumn : public Gtk::TreeModel::ColumnRecord {
  public:
    InstanceModelColumn() {
      add(sid); add(sid_text_color); add(col_pixbuf);
      //add(interface); add(branch); add(schema_type);
    }

    Gtk::TreeModelColumn<Glib::ustring> sid;
    Gtk::TreeModelColumn<Glib::ustring> sid_text_color;/*shows instance state*/
    Gtk::TreeModelColumn<Glib::ustring> interface;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_pixbuf;
  };

  class FactoryModelColumn : public Gtk::TreeModel::ColumnRecord {
  public:
    FactoryModelColumn() {
      add(interface); add(branch); add(col_pixbuf);
    }

    Gtk::TreeModelColumn<std::string> interface;
    Gtk::TreeModelColumn<std::string> branch;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_pixbuf;
  };

private:
  /**
     Select a path using a filedialog.
     \param path selected path
     \return dialog return code. Gtk::RESPONSE_OK on successfull selection
  */
  int select_path(std::string* path);
  void add_factories_path(std::string& path);
  void select_root_factory(const Gtk::TreeModel::Path& path);
  void draw_message();
  void insert_instance_row(const sid_t father, const sid_t child,
			   Gtk::TreeModel::iterator* father_row_it = NULL,
			   Gtk::TreeModel::iterator* child_row_it = NULL);
  int search_instance_row(const sid_t sid, 
			  Gtk::TreeModel::iterator* row_it);//return 0
							    //if not
							    //found
  void fill_instance_row(const sid_t sid, 
			 Gtk::TreeModel::iterator* row_it);
  void delete_instance_row(const sid_t sid);
  void insert_factory_row(Pointer<Schemafactory> factory);

  Hierarchy hierarchy;

  Glib::RefPtr<Gnome::Glade::Xml> refXml;
  std::vector<std::string> paths;
  std::string root_interface_name;
  std::string root_branch;

  //message reception
  std::queue<Pointer<Message> > msg_queues[2];
  std::queue<Pointer<Message> > *msg_read_queue,*msg_write_queue;
  Glib::Mutex msg_queue_mutex;
  Glib::Dispatcher new_message;

  //windows
  Gtk::Window *main_window;
  Gtk::Dialog *factories_dialog;

  //log textview
  Gtk::TextView *log_textview;
  Glib::RefPtr<Gtk::TextBuffer> refLogBuffer;

  //instance textview
  Gtk::TextView *instance_textview;
  Glib::RefPtr<Gtk::TextBuffer> refInstanceBuffer;

  //instance treeview
  InstanceModelColumn instance_column;
  Glib::RefPtr<Gtk::TreeStore> instances_refTreeModel;
  Gtk::TreeView *instances_treeview;
  std::map<sid_t,Gtk::TreeModel::Path> sid2treestore_path;

  //factory iconview
  FactoryModelColumn factory_column;
  Glib::RefPtr<Gtk::ListStore> factories_refListModel;
  Gtk::IconView *factories_iconview;
  bool root_factory_selected;
  Gtk::TreeModel::Path root_factory_path;
};


Jdeapp::Jdeapp()
  :hierarchy(true),root_factory_selected(false) { //FIXME: permitir activar desactivar modo depuración
  //create gui
  refXml = Gnome::Glade::Xml::create("jdescope.glade");
  
  //get windows
  refXml->get_widget("main_window",main_window);
  refXml->get_widget("factories_dialog",factories_dialog);

  //connect button signals on main_window
  Gtk::ToolButton *play_button,*stop_button;
  
  refXml->get_widget("play_toolbutton",play_button);
  play_button->signal_clicked().connect(sigc::mem_fun(*this,&Jdeapp::on_play_button_clicked));
  refXml->get_widget("stop_toolbutton",stop_button);
  stop_button->signal_clicked().connect(sigc::mem_fun(*this,&Jdeapp::on_stop_button_clicked));

  //connect button signals on factories_dialog
  Gtk::ToolButton *openfactoriesdir_button,
    *selectrootfactory_button;

  refXml->get_widget("openfactoriesdir_toolbutton",openfactoriesdir_button);
  openfactoriesdir_button->signal_clicked().connect(sigc::mem_fun(*this,
								  &Jdeapp::on_openfactoriesdir_button_clicked));
  refXml->get_widget("selectrootfactory_toolbutton",selectrootfactory_button);
  selectrootfactory_button->signal_clicked().connect(sigc::mem_fun(*this,
								   &Jdeapp::on_selectrootfactory_button_clicked));

  //create log buffer and set on log_textview
  refXml->get_widget("log_textview",log_textview);
  refLogBuffer = Gtk::TextBuffer::create();
  log_textview->set_buffer(refLogBuffer);

  //create instance buffer and set on instance_textview
  refXml->get_widget("instance_textview",instance_textview);
  refInstanceBuffer = Gtk::TextBuffer::create();
  instance_textview->set_buffer(refInstanceBuffer);

  //create instances treemodel and set treeview
  instances_refTreeModel = Gtk::TreeStore::create(instance_column);//create model
  refXml->get_widget("instances_treeview",instances_treeview);
  instances_treeview->set_model(instances_refTreeModel);//set view
  instances_treeview->set_reorderable();
  //user defined cell renderer. View sid with foreground color set on
  //schema state changes
  Gtk::CellRendererText* pSidRenderer = Gtk::manage(new Gtk::CellRendererText());
  Gtk::TreeViewColumn* pColumn = Gtk::manage(new Gtk::TreeView::Column("Instance"));
  pColumn->pack_start(instance_column.col_pixbuf,false);
  pColumn->pack_start(*pSidRenderer);
  // int cols_count = instances_treeview->append_column("SID",*pSidRenderer);
//   Gtk::TreeViewColumn* pColumn = instances_treeview->get_column(cols_count-1);
  if (pColumn){
    pColumn->add_attribute(pSidRenderer->property_text(),instance_column.sid);
    pColumn->add_attribute(pSidRenderer->property_foreground(),instance_column.sid_text_color);
  }
  instances_treeview->append_column(*pColumn);

  Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = instances_treeview->get_selection();
  //on row selected draw schema info
  refTreeSelection->signal_changed().connect(sigc::mem_fun(*this,
							   &Jdeapp::on_instances_treeview_selection_changed));
  //on double click draw schema gui
  instances_treeview->signal_row_activated().connect(sigc::mem_fun(*this,
								   &Jdeapp::on_instances_treeview_row_activated));

  //create factories listmodel and set iconview
  factories_refListModel = Gtk::ListStore::create(factory_column);
  factories_refListModel->set_sort_column(factory_column.interface,Gtk::SORT_ASCENDING);
  refXml->get_widget("factories_iconview",factories_iconview);
  factories_iconview->set_model(factories_refListModel);
  factories_iconview->set_markup_column(factory_column.interface);
  factories_iconview->set_pixbuf_column(factory_column.col_pixbuf);
  factories_iconview->signal_item_activated().connect(sigc::mem_fun(*this,
								    &Jdeapp::on_factories_iconview_item_activated) );

  //connect new_message to draw a message when processed
  new_message.connect(sigc::mem_fun(*this, &Jdeapp::draw_message));

  //init queue ptr
  msg_read_queue = msg_queues;
  msg_write_queue = msg_queues+1;

  //connects hierarchy with gui
  hierarchy.append_processor(this);
}
  
Jdeapp::~Jdeapp() {}

void Jdeapp::run(){
  Gtk::Window *main_window;

  refXml->get_widget("main_window",main_window);
  Gtk::Main::run(*main_window);
}

void Jdeapp::on_play_button_clicked() {
  int rc = factories_dialog->run();

  factories_dialog->hide();
  if ( rc != Gtk::RESPONSE_OK)
    return;
  
  if (root_interface_name.size() == 0) {
    Gtk::MessageDialog dialog("No root selected",false,Gtk::MESSAGE_ERROR);
    dialog.run();
    return;
  }

  std::cout << "Starting hierarchy" << std::endl;
  hierarchy.active(root_interface_name,root_branch);

  //swap play/stop buttons
  Gtk::ToolButton *play_button,*stop_button;
  refXml->get_widget("play_toolbutton",play_button);
  refXml->get_widget("stop_toolbutton",stop_button);
  play_button->set_sensitive(false);
  stop_button->set_sensitive(true);
}

void Jdeapp::on_stop_button_clicked(){
  hierarchy.stop();

  //swap play/stop buttons
  Gtk::ToolButton *play_button,*stop_button;
  refXml->get_widget("play_toolbutton",play_button);
  refXml->get_widget("stop_toolbutton",stop_button);
  play_button->set_sensitive(true);
  stop_button->set_sensitive(false);
}

void Jdeapp::on_instances_treeview_row_activated(const Gtk::TreeModel::Path& path, 
						 Gtk::TreeViewColumn* column){
  // Gtk::TreeModel::iterator iter = instances_refTreeModel->get_iter(path);
//   if(iter) {
//     Gtk::TreeModel::Row row = *iter;
//     refInstanceBuffer->set_text("Instance " + row[instance_column.sid] + "\n");
    
//   }
}

void Jdeapp::on_instances_treeview_selection_changed(){
  Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = 
    instances_treeview->get_selection();
  Gtk::TreeModel::iterator iter = 
    refTreeSelection->get_selected();
  if (iter){
    Gtk::TreeModel::Row row = *iter;
    refInstanceBuffer->set_text("Instance " + row[instance_column.sid] + "\n");
  }
}

void Jdeapp::on_openfactoriesdir_button_clicked() {
  std::string p;
  
  if (select_path(&p) == Gtk::RESPONSE_OK)
    add_factories_path(p);
}

void Jdeapp::on_selectrootfactory_button_clicked() {
  typedef std::list<Gtk::TreeModel::Path> type_list_paths;
  type_list_paths selected = factories_iconview->get_selected_items();

  if(!selected.empty()) //select first
    select_root_factory(*selected.begin());
}

void Jdeapp::on_factories_iconview_item_activated(const Gtk::TreeModel::Path& path) {
  select_root_factory(path);
}

void Jdeapp::process(Pointer<Perception> p){
  Glib::Mutex::Lock l(msg_queue_mutex);
  Pointer<Message> m = p;
  
  msg_write_queue->push(m);
  if (msg_write_queue->size() == 1)//first signals
    new_message();
}

void Jdeapp::process(Pointer<Modulation> m){
  Glib::Mutex::Lock l(msg_queue_mutex);
  
  msg_write_queue->push(m);
  if (msg_write_queue->size() == 1)//first signals
    new_message();
}

void Jdeapp::process(Pointer<Arbitrate_Req> a){
  Glib::Mutex::Lock l(msg_queue_mutex);
  
  msg_write_queue->push(a);
  if (msg_write_queue->size() == 1)//first signals
    new_message();
}

void Jdeapp::process(Pointer<State_Change> s){
  Glib::Mutex::Lock l(msg_queue_mutex);
  
  Pointer<Message> m = Pointer<Message>(s);

  msg_write_queue->push(m);
  if (msg_write_queue->size() == 1)//first signals
    new_message();
}

void Jdeapp::process(Pointer<New_State> n){
  Glib::Mutex::Lock l(msg_queue_mutex);
  
  msg_write_queue->push(n);
  if (msg_write_queue->size() == 1)//first signals
    new_message();
}

int Jdeapp::select_path(std::string* path){
  Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  int rc;

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button("Select", Gtk::RESPONSE_OK);

  if ((rc = dialog.run()) == Gtk::RESPONSE_OK)
    *path = dialog.get_filename();
  return rc;
}

void Jdeapp::add_factories_path(std::string& path) {
  std::vector<Pointer<Schemafactory> > loaded_factories;
  std::vector<Pointer<Schemafactory> >::iterator lf_it;

  loaded_factories = hierarchy.load_factories(path);
  for (lf_it = loaded_factories.begin();
       lf_it != loaded_factories.end();
       lf_it++)
    insert_factory_row(*lf_it);
}

void Jdeapp::select_root_factory(const Gtk::TreeModel::Path& path) {
  Gtk::TreeModel::iterator iter;

  //undo icon change
  if (root_interface_name.size() > 0) {//another root factory was
				       //selected before
    iter = factories_refListModel->get_iter(root_factory_path);
    if (iter){
      Gtk::TreeModel::Row row = *iter;
      std::vector<std::string> branch_prefs;

      branch_prefs.push_back(row[factory_column.branch]);
      Pointer<Schemafactory> f = hierarchy.get_factory(row[factory_column.interface],
						       branch_prefs);
      
      if (f->get_interface_type() == PSCHEMA)
	row[factory_column.col_pixbuf] = get_icon_pixbuf(PSCHEMAICON);
      else
	row[factory_column.col_pixbuf] = get_icon_pixbuf(MSCHEMAICON);
    }
  }

  //select new root
  iter = factories_refListModel->get_iter(path);
  if (iter){
    Gtk::TreeModel::Row row = *iter;
    std::vector<std::string> branch_prefs;

    branch_prefs.push_back(row[factory_column.branch]);
    Pointer<Schemafactory> f = hierarchy.get_factory(row[factory_column.interface],
						     branch_prefs);

    root_interface_name = row[factory_column.interface];
    root_branch = row[factory_column.branch];
    if (f->get_interface_type() == PSCHEMA)
      row[factory_column.col_pixbuf] = get_icon_pixbuf(PSCHEMAROOTICON);
    else
      row[factory_column.col_pixbuf] = get_icon_pixbuf(MSCHEMAROOTICON);
    root_factory_path = path;
  }
}

void Jdeapp::draw_message(){
  {
    Glib::Mutex::Lock l(msg_queue_mutex);

    //swap queues and unlock
    std::queue<Pointer<Message> > *swap = msg_write_queue;
    msg_write_queue = msg_read_queue;
    msg_read_queue = swap;
  }

  Gtk::TreeModel::iterator father_it,child_it;
  Gtk::TreeModel::Row row;
  
  //draw queued msg
  while ( ! msg_read_queue->empty() ) {
    Pointer<Message> msg = msg_read_queue->front();

    msg_read_queue->pop();

    //guess message type and draw it
    Perception* p = dynamic_cast<Perception*>(msg.get());
    if (p != NULL){
      refLogBuffer->insert_at_cursor("mensaje perceptivo de " +
				     sid_to_str(msg->src) +
				     " a " + sid_to_str(msg->dst) + "\n");
      continue;
    }
    
    
    Modulation* m = dynamic_cast<Modulation*>(msg.get());
    if (m != NULL){
      refLogBuffer->insert_at_cursor("mensaje modulacion de " +
				     sid_to_str(msg->src) +
				     " a " + sid_to_str(msg->dst) + "\n");
      if (!search_instance_row(msg->dst,&child_it))
	insert_instance_row(msg->src,msg->dst,&father_it,&child_it);/*insert new row (child)*/
      continue;
    }

    Arbitrate_Req* a = dynamic_cast<Arbitrate_Req*>(msg.get());
    if (a != NULL){
      refLogBuffer->insert_at_cursor("mensaje arbitraje de " +
				     sid_to_str(msg->src) + " a " +
				     sid_to_str(msg->dst) + "\n");
      continue;
    }

    State_Change* s = dynamic_cast<State_Change*>(msg.get());
    if (s != NULL){
      refLogBuffer->insert_at_cursor("mensaje cambio estado de " +
				     sid_to_str(msg->src) + " a " +
				     sid_to_str(msg->dst) + "\n");
      if (!search_instance_row(msg->dst,&child_it))
	insert_instance_row(msg->src,msg->dst,&father_it,&child_it);
      continue;
    }

    New_State* n = dynamic_cast<New_State*>(msg.get());
    if (n != NULL){
      refLogBuffer->insert_at_cursor("mensaje nuevo estado de " +
				     sid_to_str(msg->src) + " a " +
				     sid_to_str(msg->dst) + "\n");
      // if (n->new_state == SLEPT){//delete row when goes slept
// 	delete_instance_row(msg->src);
//       }else{
      if (search_instance_row(msg->src,&child_it)){
	row = *child_it;
	row[instance_column.sid_text_color] = state_color[n->new_state];//update color
      }
	//}
      continue;
    }

    
  }
}

/*
  Insert a child row. If father not created yet, append it to the top
  level. If both already exist, return only iterators.
*/
void Jdeapp::insert_instance_row(sid_t father, sid_t child,
				 Gtk::TreeModel::iterator* father_row_it,
				 Gtk::TreeModel::iterator* child_row_it){
  if (!search_instance_row(father,father_row_it)){//create a new row
						  //at top the level
    *father_row_it = instances_refTreeModel->append();
    sid2treestore_path[father] = 
      instances_refTreeModel->get_path(*father_row_it);//save
							     //path
    fill_instance_row(father,father_row_it);
  }

  if (!search_instance_row(child,child_row_it)){//child not in,
						   //create it
    Gtk::TreeModel::Row father_row = **father_row_it;
    *child_row_it = 
      instances_refTreeModel->append(father_row.children());
    Gtk::TreeModel::Path path = instances_refTreeModel->get_path(*child_row_it);
    sid2treestore_path[child] = path;//save path
    instances_treeview->expand_to_path(path);//expand to row
    fill_instance_row(child,child_row_it);
  }
}
    

int Jdeapp::search_instance_row(const sid_t sid, 
				Gtk::TreeModel::iterator* row_it){
  std::map<sid_t,Gtk::TreeModel::Path>::iterator s2p_it = 
    sid2treestore_path.find(sid);

  if (s2p_it != sid2treestore_path.end()){
    *row_it = instances_refTreeModel->get_iter(s2p_it->second);
    return (*row_it == true);
  }
  return 0;//not found
}

void Jdeapp::fill_instance_row(const sid_t sid, 
			       Gtk::TreeModel::iterator* row_it){
  Gtk::TreeModel::Row row = **row_it;
  Pointer<Schemainstance> i = hierarchy.get_instance(sid);

  row[instance_column.sid] = sid_to_str(sid);
  row[instance_column.sid_text_color] = state_color[SLEPT];
  row[instance_column.interface] = i->get_interface_name();
  if ( i->get_interface_type() == PSCHEMA)
    row[instance_column.col_pixbuf] = get_icon_pixbuf(PSCHEMASMALLICON);
  else
    row[instance_column.col_pixbuf] = get_icon_pixbuf(MSCHEMASMALLICON);
}

void Jdeapp::delete_instance_row(const sid_t sid){
  Gtk::TreeModel::iterator row_it;

  if (search_instance_row(sid,&row_it)){
    instances_refTreeModel->erase(row_it);
  }
  sid2treestore_path.erase(sid);
}

void Jdeapp::insert_factory_row(Pointer<Schemafactory> factory) {
  Gtk::TreeModel::Row row = *(factories_refListModel->append());
  row[factory_column.interface] = factory->get_interface_name();
  row[factory_column.branch] = factory->get_implementation_branch();
  if (factory->get_interface_type() == PSCHEMA)
    row[factory_column.col_pixbuf] = get_icon_pixbuf(PSCHEMAICON);
  else
    row[factory_column.col_pixbuf] = get_icon_pixbuf(MSCHEMAICON);
}

int main(int argc, char* argv[]) {
  Glib::thread_init();
  Gtk::Main kit(argc, argv);
  Jdeapp app;

  app.run();
}
