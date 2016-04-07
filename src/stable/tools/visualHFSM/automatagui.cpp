

#include "automatagui.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
AutomataGui::AutomataGui(int argc, char** argv) : dispatcher(){
	this->app = Gtk::Application::create(argc, argv, "jderobot.visualHFSM.automatagui");
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
AutomataGui::~AutomataGui(){}


int AutomataGui::init(){
	//CREATE REFBUILDER
	this->refBuilder = Gtk::Builder::create();
	try{
		if(access("/usr/local/share/jderobot/glade/visualHFSM/automatagui.glade", F_OK) == 0){
			refBuilder->add_from_file("/usr/local/share/jderobot/glade/visualHFSM/automatagui.glade");
		}else{
			refBuilder->add_from_file("./automatagui.glade");
		}
	}catch (const Glib::FileError& ex){
		std::cerr << "FileError: " << ex.what() << std::endl;
	  	return -1;
	}catch (const Glib::MarkupError& ex){
	  	std::cerr << "MarkupError: " << ex.what() << std::endl;
	  	return -1;
	}catch (const Gtk::BuilderError& ex){
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return -1;
	}

	//GETTING WIDGETS
	refBuilder->get_widget("scrolledwindow_schema", this->scrolledwindow_schema);
	refBuilder->get_widget("treeview", this->treeView);
	refBuilder->get_widget("up_button", this->pUpButton);
	refBuilder->get_widget("check_autofocus", this->checkAutofocus);
	refBuilder->get_widget("DialogDerived", guiDialog);
	if(!guiDialog){
		std::cerr << "Error: couldn't get DialogDerived" << std::endl;
		return -1;
	}

	this->pUpButton->signal_clicked().connect(sigc::mem_fun(*this,
										&AutomataGui::on_up_button_clicked));

	//INIT CANAVS
	this->canvas = Gtk::manage(new Goocanvas::Canvas());
	this->canvas->signal_item_created().connect(sigc::mem_fun(*this,
										&AutomataGui::on_item_created));
	this->root = Goocanvas::GroupModel::create();
	this->canvas->set_root_item_model(root);
	this->canvas->set_visible(true);
	this->scrolledwindow_schema->add(*(this->canvas));

	//INIT TREEVIEW
	this->lastExpanded = "";
	this->refTreeModel = Gtk::TreeStore::create(this->m_Columns);
	this->treeView->set_model(this->refTreeModel);
	this->treeView->append_column("ID", this->m_Columns.m_col_id);
	Gtk::CellRendererText *cell = new Gtk::CellRendererText;
	int column_count = treeView->append_column("Name", *cell);
	Gtk::TreeViewColumn *column = treeView->get_column(column_count-1);
	if (column) {
	#ifdef GLIBMM_PROPERTIES_ENABLED
        column->add_attribute(cell->property_background(), this->m_Columns.m_col_color);
		column->add_attribute(cell->property_text(), this->m_Columns.m_col_name);
	#else
        column->add_attribute(*cell, "background", this->m_Columns.m_col_color);
        column->add_attribute(*cell, "text", this->m_Columns.m_col_name);
	#endif
    }
    this->treeView->signal_row_activated().connect(
    					sigc::mem_fun(*this, &AutomataGui::on_row_activated));

    //Atach handler to the dispatcher
    this->dispatcher.connect(sigc::mem_fun(*this, &AutomataGui::on_notify_received));

    //SETTING CANVAS BOUNDS
	Glib::RefPtr<Gdk::Screen> screen = this->guiDialog->get_screen();
    int width = screen->get_width();
    int height = screen->get_height();

    Goocanvas::Bounds bounds;
    this->canvas->get_bounds(bounds);
    bounds.set_x2(width);
    bounds.set_y2(height * 2);
    this->canvas->set_bounds(bounds);
    return 0;
}


void AutomataGui::run(){			
	this->app->run(*guiDialog);
  	delete guiDialog;
}


void AutomataGui::close(){
	this->app->quit();
}


void AutomataGui::setGuiSubautomataList (std::list<GuiSubautomata> guiSubList){
	this->guiSubautomataList = guiSubList;
}


GuiSubautomata* AutomataGui::getSubautomataWithIdFather(int id){

	std::list<GuiSubautomata>::iterator subListIt = this->guiSubautomataList.begin();
	while(subListIt != this->guiSubautomataList.end()){
		if(subListIt->getIdFather() == id){
			return &*subListIt;
		}
		subListIt++;
	}
	return NULL;
}


GuiSubautomata* AutomataGui::getSubautomata(int id){
	std::list<GuiSubautomata>::iterator subListIt = this->guiSubautomataList.begin();
	while(subListIt != this->guiSubautomataList.end()){
		if(subListIt->getId() == id){
			return &*subListIt;
		}
		subListIt++;
	}
	return NULL;
}


bool AutomataGui::isFirstActiveNode(GuiNode* gnode){
	int sonSubId, nodeId;
	GuiSubautomata* subAux = this->currentGuiSubautomata;

	while(subAux != NULL){
		if (!gnode->itIsInitial())
			return false;
		sonSubId = subAux->getId();
		subAux = this->getSubautomata(subAux->getIdFather());

		if (subAux != NULL){
			nodeId = this->getIdNodeFather(subAux->getId(), sonSubId);
			gnode = subAux->getGuiNode(nodeId);
			if (gnode == NULL){
				std::cerr << "NODE " << nodeId << " not found." << std::endl;
				return false;
			}
		}
	}
	return true;
}


void AutomataGui::loadGuiSubautomata(){
	std::string color;

	std::list<GuiSubautomata>::iterator subListIterator = this->guiSubautomataList.begin();
	while (subListIterator != guiSubautomataList.end()){
		this->currentGuiSubautomata = &(*subListIterator);

		std::list<GuiNode> nodeList = *(subListIterator->getListGuiNodes());
		std::list<GuiNode>::iterator nodeListIterator = nodeList.begin();
		while (nodeListIterator != nodeList.end()){
			this->idGuiNode = nodeListIterator->getId();

			if (nodeListIterator->itIsInitial())
				currentGuiSubautomata->setActiveNode(nodeListIterator->getName());
			if (this->isFirstActiveNode(&*nodeListIterator)){
				nodeListIterator->changeColor(ITEM_COLOR_GREEN);
				color = ITEM_COLOR_GREEN;
			}else{
				color = "white";
			}
			this->create_new_state(&(*nodeListIterator), color);
			nodeListIterator++;
		}

		std::list<GuiTransition> transList = *(subListIterator->getListGuiTransitions());
		std::list<GuiTransition>:: iterator transListIterator = transList.begin();
		while (transListIterator != transList.end()){

			this->create_new_transition(&(*transListIterator));
			transListIterator++;
		}

		if (subListIterator->getIdFather() != 0)
			this->currentGuiSubautomata->hideAll();
		subListIterator++;
	}

	this->currentGuiSubautomata = this->getSubautomataWithIdFather(0);
}


void AutomataGui::create_new_state(GuiNode* gnode, std::string color){
	
	std::string nodeName = gnode->getName();
	if (this->currentGuiSubautomata->getId() == 1){
		Gtk::TreeModel::Row row = *(this->refTreeModel->append());
		row[m_Columns.m_col_id] = this->idGuiNode;
		row[m_Columns.m_col_name] = nodeName;
		row[m_Columns.m_col_color] = color;
	} else {
		this->fillTreeView(nodeName, color, this->refTreeModel->children(),
			this->getIdNodeFather(this->currentGuiSubautomata->getIdFather(),
									this->currentGuiSubautomata->getId()));
	}
	this->type = STATE;
	this->root->add_child(gnode->getEllipse());
	this->type = NONE;
	this->root->add_child(gnode->getText());
	this->root->add_child(gnode->getEllipseInitial());
}


void AutomataGui::create_new_transition(GuiTransition* gtrans){
	int originId = gtrans->getIdOrigin();
	int finalId = gtrans->getIdDestiny();

	if (originId == finalId){	//autotransition
		int transId = gtrans->getId();
		Point origin = *(this->currentGuiSubautomata->getPointPointer(originId));
		Point final = *(this->currentGuiSubautomata->getPointPointer(finalId));

		float xcenter = origin.getX();
		origin.setX(xcenter - RADIUS_NORMAL + 2);
		final.setX(xcenter + RADIUS_NORMAL -2);

		int numberAutotrans = this->currentGuiSubautomata->getNumberOfAutotransitions(origin);

		Point pmidpoint(xcenter, origin.getY() + (2 + 3 * numberAutotrans) * RADIUS_NORMAL);

		GuiTransition transAux(origin, final, pmidpoint, transId);
		this->currentGuiSubautomata->replaceGuiTransition(transAux, transId);

		gtrans = &transAux;
	}
	this->root->add_child(gtrans->getLeftLine());
	this->root->add_child(gtrans->getRightLine());
	this->root->add_child(gtrans->getTextModel());
	this->root->add_child(gtrans->getMidpoint());
}


int AutomataGui::getIdNodeFather ( int subautomataId, int subautSonId ) {
    std::list<GuiSubautomata>::iterator subListIterator = this->guiSubautomataList.begin();
    while ( (subListIterator->getId() != subautomataId) &&
            (subListIterator != this->guiSubautomataList.end()) )
    	subListIterator++;
        
    if (subListIterator == this->guiSubautomataList.end())
		return 0;

    std::list<GuiNode>* guiNodeList = subListIterator->getListGuiNodes();
    std::list<GuiNode>::iterator guiNodeListIterator = guiNodeList->begin();
    while ( (guiNodeListIterator->getIdSubautomataSon() != subautSonId)
    		&& (guiNodeListIterator != guiNodeList->end()) )
   		guiNodeListIterator++;

	if (guiNodeListIterator == guiNodeList->end()){
		return 0;
	}else{
		return guiNodeListIterator->getId();
	}
}

bool AutomataGui::fillTreeView ( std::string nameNode, std::string color, 
									Gtk::TreeModel::Children child, int idNodeFather ) {

    bool added = false;
    Gtk::TreeModel::Children::iterator iter = child.begin();
    while ( !added && (iter != child.end()) ) {
        Gtk::TreeModel::Row therow = *iter;
        if (therow[m_Columns.m_col_id] == idNodeFather) {
            Gtk::TreeModel::Row row = *(refTreeModel->append(therow.children()));
            row[m_Columns.m_col_id] = this->idGuiNode;
            row[m_Columns.m_col_name] = nameNode;
            row[m_Columns.m_col_color] = color;
            added = true;
        } else {
            added = this->fillTreeView(nameNode, color, therow.children(), idNodeFather);
            iter++;
        }
    }
    return added;
}


GuiSubautomata* AutomataGui::getSubautomataByNodeName(std::string name){
	std::list<GuiSubautomata>::iterator subIterator = this->guiSubautomataList.begin();
	while (subIterator != this->guiSubautomataList.end()){

		std::list<GuiNode>::iterator nodeListIter = subIterator->getListGuiNodes()->begin();
		while (nodeListIter != subIterator->getListGuiNodes()->end()){
			if (nodeListIter->getName().compare(name) == 0){
				return &(*subIterator);
			}
			nodeListIter++;
		}
		subIterator++;
	}
	return NULL;
}


void AutomataGui::treeViewAutoFocus(Gtk::TreeModel::Children::iterator iter,
										Glib::ustring name){

	if(this->lastExpanded != name){
		bool checked = this->checkAutofocus->get_active();

		if(this->lastExpanded != "" && checked){
			this->treeView->collapse_row(this->pathLastExp);
		}
		this->pathLastExp = this->refTreeModel->get_path(iter);
		if(checked){
			this->treeView->expand_to_path(this->pathLastExp);
			this->lastExpanded = name;
		}
	}
}


bool AutomataGui::setActiveTreeView(std::string name, bool active,
										Gtk::TreeModel::Children children){

	bool finded = false;
	Gtk::TreeModel::Children::iterator iter = children.begin();
	while(!finded && (iter != children.end())){
		Gtk::TreeModel::Row row = *iter;

		if (row[m_Columns.m_col_name] == name){

			if (active){
				row[m_Columns.m_col_color] = ITEM_COLOR_GREEN;
			}else{
				row[m_Columns.m_col_color] = ITEM_COLOR_WHITE;
			}
			finded = true;

		} else {
			finded = this->setActiveTreeView(name, active, row.children());

			if (finded && active){
				this->treeViewAutoFocus(iter, row[m_Columns.m_col_name]);
			}
		}
		iter++;
	}
	return finded;
}


void AutomataGui::setNodeAsActive(GuiNode* node, GuiSubautomata* subautomata, bool active){
	if(active){
		subautomata->setActiveNode(node->getName());
		node->changeColor(ITEM_COLOR_GREEN);
	}else{
		node->changeColor(ITEM_COLOR_BLUE);
	}

	if(!this->setActiveTreeView(node->getName(), active, this->refTreeModel->children()))
 		std::cerr << "NOT FINDED " << node->getName() << std::endl;	

 	int sonId = node->getIdSubautomataSon();
 	if (sonId != 0){
 		GuiSubautomata* subSon = this->getSubautomata(sonId);
 		std::string nodeName = subSon->getActiveNode();
 		GuiNode* nodeAux = subSon->getGuiNode(nodeName);
 		this->setNodeAsActive(nodeAux, subSon, active);
 	}
}


void AutomataGui::on_notify_received(){

	pthread_mutex_lock(&this->activesNodesNames.lock);
	if (this->activesNodesNames.queue.empty()){
		std::cerr << "ERROR: actives nodes names queue is empty" << std::endl;
		return;
	}
	std::string activeName = this->activesNodesNames.queue.front();
	this->activesNodesNames.queue.pop();
	pthread_mutex_unlock(&this->activesNodesNames.lock);

	GuiSubautomata* subautomata = this->getSubautomataByNodeName(activeName);
	std::string lastActive = subautomata->getActiveNode();
	GuiNode* node;

	node = subautomata->getGuiNode(lastActive);
	this->setNodeAsActive(node, subautomata, false);

	node = subautomata->getGuiNode(activeName);
	this->setNodeAsActive(node, subautomata, true);
}


void AutomataGui::notifySetNodeAsActive(std::string nodeName){
	pthread_mutex_lock(&this->activesNodesNames.lock);
	this->activesNodesNames.queue.push(nodeName);
	pthread_mutex_unlock(&this->activesNodesNames.lock);
	this->dispatcher.emit();
}


 void AutomataGui::on_item_created(const Glib::RefPtr<Goocanvas::Item>& item,
                                   const Glib::RefPtr<Goocanvas::ItemModel>& /* model */){

 	if (this->type == STATE){
 		item->signal_button_press_event().connect(
 			sigc::mem_fun(this, &AutomataGui::on_item_button_press_event));
 		item->signal_enter_notify_event().connect(
                sigc::mem_fun(this, &AutomataGui::on_item_enter_notify_event));
 		item->signal_leave_notify_event().connect(
                sigc::mem_fun(this, &AutomataGui::on_item_leave_notify_event));
 		this->currentGuiSubautomata->setGuiNodeItems(this->idGuiNode, item);
 	}
}


void AutomataGui::showSubautomata(int id){
	this->currentGuiSubautomata->hideAll();
	this->currentGuiSubautomata = this->getSubautomata(id);
	this->currentGuiSubautomata->showAll();
}


//HANDLERS
bool AutomataGui::on_item_button_press_event(const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventButton* event ){

	if ((event->button == 1) && item){
		if (event->type == GDK_2BUTTON_PRESS){
			int idSubautomataSon = this->currentGuiSubautomata->getIdSubautomataSon(item);
			if (idSubautomataSon != 0){
				this->showSubautomata(idSubautomataSon);
			}else{
				std::cout << "This node doesn't have any son." << std::endl;
			}
		}
	}

	return false;
}


bool AutomataGui::on_item_enter_notify_event(const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event){
	if (item){
		this->currentGuiSubautomata->changeGuiNodeWidth(item, 3);
	}
	return false;
}


bool AutomataGui::on_item_leave_notify_event(const Glib::RefPtr<Goocanvas::Item>& item,
                                              GdkEventCrossing* event){
	if (item){
		this->currentGuiSubautomata->changeGuiNodeWidth(item, 1);
	}
	return false;
}


void AutomataGui::on_up_button_clicked (){

	int fatherId = this->currentGuiSubautomata->getIdFather();
	if (fatherId != 0){
		this->showSubautomata(fatherId);
	}else{
		std::cout << "This subautomata doesn't have any parent." << std::endl;
	}
}


void AutomataGui::on_row_activated(const Gtk::TreeModel::Path& path,
									Gtk::TreeViewColumn* /* column */){
	Gtk::TreeModel::iterator iter = this->refTreeModel->get_iter(path);

	if (iter){
		Gtk::TreeModel::Row row = *iter;

		std::stringstream name;
		name << row[m_Columns.m_col_name];
		GuiSubautomata* gsub = this->getSubautomataByNodeName(name.str());

		if (gsub == NULL)
            return;

		if (gsub->getId() != this->currentGuiSubautomata->getId()){
			this->showSubautomata(gsub->getId());
		}
	}else{
		std::cerr << "Couldn't get the row" << std::endl;
	}
}