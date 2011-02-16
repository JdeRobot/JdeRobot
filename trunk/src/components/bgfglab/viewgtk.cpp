/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include <libglademm.h>
#include <cairomm/context.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <opencv/cvaux.h>
#include "viewgtk.h"
#include "model.h"
#include "bgmodelfactory.h"

namespace bgfglab {
  const std::string gladepath = std::string(GLADE_DIR) + std::string("/bgfglab.glade");

  class ViewGtk::PImpl{
  public:
    std::string currentModelName;
    jderobotutil::ParamDict currentModelParam;
  };


  ViewGtk::ViewGtk(Controller &controller) throw ()
    : View(controller),
      gtkmain(0, 0), //FIXME: do we need gtk params?? maybe we can get them from config file
      refXml(Gnome::Glade::Xml::create(gladepath)), //FIXME: check for errors main window
      INIT_WIDGET(mainwindow, refXml),
      INIT_WIDGET(menutoolbuttonSelectBGModel, refXml),
      //selectBGModelMenu(),
      //menutoolbuttonSelectBGModelItems(),
      INIT_WIDGET(toolbuttonApplyMaskToImage, refXml),
      INIT_WIDGET(toolbuttonDumpData, refXml),
      INIT_WIDGET(drawingareaBg, refXml),
      INIT_WIDGET(drawingareaFgMask, refXml),
      INIT_WIDGET(drawingareaImage, refXml),
      INIT_WIDGET(textviewAlgInfo, refXml),
      INIT_WIDGET(statusbarMain, refXml),
      INIT_WIDGET(dialogBGModelSelection, refXml),
      INIT_WIDGET(comboboxBGModel, refXml),
      INIT_WIDGET(textviewAlgParam, refXml),
      INIT_WIDGET(buttonBGModelCancel, refXml),
      INIT_WIDGET(buttonBGModelApply, refXml),
      INIT_WIDGET(buttonBGModelAccept, refXml),
      INIT_WIDGET(filechooserdialogDumpData, refXml),
      INIT_WIDGET(checkbuttonImgDump, refXml),
      INIT_WIDGET(checkbuttonBgDump, refXml),
      INIT_WIDGET(checkbuttonFgMaskDump, refXml),
      INIT_WIDGET(buttonDumpDataCancel, refXml),
      INIT_WIDGET(buttonDumpDataAccept, refXml),
      pImpl(new PImpl())
  {
    //main window
    menutoolbuttonSelectBGModel->signal_clicked().connect(sigc::mem_fun(this,&ViewGtk::onMenutoolbuttonSelectBGModelClicked));

    toolbuttonApplyMaskToImage->set_active(false);
        
    drawingareaBg->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaBgExposeEvent));
    drawingareaFgMask->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaFgMaskExposeEvent));
    drawingareaImage->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaImageExposeEvent));
    textviewAlgInfo->get_buffer()->set_text(controller.model().bgModelParam().toString());
    mainwindow->show();

    //algorithm selection dialog
    comboboxBGModel->signal_changed().connect(sigc::mem_fun(this, &ViewGtk::onComboboxBGModelChanged));
    buttonBGModelCancel->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelCancelClicked));
    buttonBGModelApply->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelApplyClicked));
    buttonBGModelAccept->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelAcceptClicked));
    dialogBGModelSelection->hide();

    //update algorithm selection button
    updateMenutoolbuttonItems();

    //dump data file chooser dialog
    toolbuttonDumpData->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::onToolbuttonDumpDataToggled));
    buttonDumpDataCancel->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonDumpDataCancelClicked));  
    buttonDumpDataAccept->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonDumpDataAcceptClicked));
    filechooserdialogDumpData->hide();

  }

  void ViewGtk::update(const jderobotutil::Subject* o, jderobotutil::ObserverArg* arg)
    throw (gbxutilacfr::Exception) {

    //image drawing is done on expose events send with these calls
    drawingareaBg->queue_draw();
    drawingareaFgMask->queue_draw();
    drawingareaImage->queue_draw();
    
    //get iterations per second
    std::stringstream ss;
    ss << controller.model().bgModelIps().ips() << " fps";  

    //dump status
    int dumpedFrames = 0;
    toolbuttonDumpData->set_active(controller.model().isDumpingData(&dumpedFrames));
    ss << " | " << dumpedFrames << " f. dumped";
    statusbarMain->pop();
    statusbarMain->push(ss.str()); 

    mainwindow->resize(1, 1);

    //FIXME: gui use bgfglab thread, we have to move this to another thread
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }

  void ViewGtk::drawImage(Gtk::DrawingArea* drawingArea, 
			  const colorspaces::Image& image) {
    
    Glib::RefPtr<Gdk::Window> window = drawingArea->get_window();
    if (window) {
      /*convert to RGB*/
      colorspaces::ImageRGB8 img_rgb8(image);
      Glib::RefPtr<Gdk::Pixbuf> imgBuff =
	Gdk::Pixbuf::create_from_data((const guint8*) img_rgb8.data,
				      Gdk::COLORSPACE_RGB,
				      false,
				      8,
				      img_rgb8.width,
				      img_rgb8.height,
				      img_rgb8.step);

      const Glib::RefPtr< const Gdk::GC > gc; /*empty*/
      window->draw_pixbuf(gc,
			  imgBuff,
			  0, 0, /*starting point from imgBuff*/
			  0, 0, /*starting point into drawable*/
			  imgBuff->get_width(),
			  imgBuff->get_height(),
			  Gdk::RGB_DITHER_NONE, 0, 0);
      drawingArea->set_size_request(img_rgb8.width,
				    img_rgb8.height);
    }
  }

  bool ViewGtk::onDrawingAreaBgExposeEvent(GdkEventExpose* event){
    drawImage(drawingareaBg.get(), controller.model().getBGImage()); //display bg image
    return true;
  }

  bool ViewGtk::onDrawingAreaFgMaskExposeEvent(GdkEventExpose* event){
    drawImage(drawingareaFgMask.get(), controller.model().getFGMaskImage()); //display fg mask
    return true;
  }

  bool ViewGtk::onDrawingAreaImageExposeEvent(GdkEventExpose* event){
    colorspaces::Image curr(controller.model().getCurrentImage());

    if (toolbuttonApplyMaskToImage->get_active()){//apply mask
      cv::Mat tmpImg;
      controller.model().getCurrentImage().copyTo(tmpImg,controller.model().getFGMaskImage());
      curr = colorspaces::Image(tmpImg, controller.model().getCurrentImage().format());
    }

    drawImage(drawingareaImage.get(), curr); //display image.
    return true;
  }

  void ViewGtk::onMenutoolbuttonSelectBGModelClicked(){
    //update combobox entries
    updateComboboxBGModelItems();
    //show AlgInfo textbuffer
    textviewAlgParam->get_buffer()->set_text(controller.model().bgModelParam().toString());
    //show algorithm selection dialog
    dialogBGModelSelection->show();
  }
  
  void ViewGtk::updateMenutoolbuttonItems() {
    // BGModelFactoryMap::const_iterator bgmodelIt;

    // Gtk::Menu* selectBGModelMenu = Gtk::manage(new Gtk::Menu());

    // for(bgmodelIt = pImpl->bgmodelF.begin();
    // 	bgmodelIt != pImpl->bgmodelF.end();
    // 	bgmodelIt++){
    //   Gtk::MenuItem* itemp = Gtk::manage(new Gtk::MenuItem(bgmodelIt->first));
    //   itemp->signal_activate().connect(sigc::bind<std::string>(sigc::mem_fun(this, &ViewGtk::onMenutoolbuttonSelectBGModelMenuItemClicked),bgmodelIt->first));
    //   selectBGModelMenu->append(*itemp);
    // }

    // menutoolbuttonSelectBGModel->set_menu(*selectBGModelMenu);
  }

  void ViewGtk::onMenutoolbuttonSelectBGModelMenuItemClicked(const std::string bgmodelDesc){
    //constroller.setBGModel(bgmodelDesc);
  }

  void ViewGtk::updateComboboxBGModelItems(){
    BGModelFactory::FactoryDict::const_iterator fIt;
    
    ModelColumns comboboxBGModelCols;
    comboboxBGModelLSRef = Gtk::ListStore::create(comboboxBGModelCols);
    Gtk::TreeModel::iterator tmIt;
    bool hasCurrent = false;
    Gtk::TreeModel::iterator currIt;
    
    comboboxBGModel->clear();
    comboboxBGModel->set_model(comboboxBGModelLSRef);

    Gtk::TreeModel::Row r;
    for(fIt = BGModelFactory::factories.begin();
	fIt != BGModelFactory::factories.end();
	fIt++){
      tmIt = comboboxBGModelLSRef->append();
      r = *tmIt;
      r[comboboxBGModelCols.m_col_name] = fIt->first;
      r[comboboxBGModelCols.m_col_desc] = fIt->second->description;
    }

    // if (pImpl->currentModelName.lenght() > 0){
    //   hasCurrent = true;
    //   tmIt = comboboxBGModelLSRef->append();
    //   r = *tmIt;
    //   r[comboboxBGModelCols.m_col_name] = pImpl->currentModelName;
    //   r[comboboxBGModelCols.m_col_desc] = "Last model selected";
    //   currIt = tmIt;
    // }

    
    comboboxBGModel->pack_start(comboboxBGModelCols.m_col_name);
    comboboxBGModel->pack_start(comboboxBGModelCols.m_col_desc);
    // if (hasCurrent)
    //   comboboxBGModel->set_active(currIt);
  }

  void ViewGtk::updateDialogBGModelSelection(){}

  void ViewGtk::onComboboxBGModelChanged(){
    ModelColumns comboboxBGModelCols;
    Gtk::TreeModel::iterator it = comboboxBGModel->get_active();

    if (it){
      Gtk::TreeModel::Row row = *it;
      if (row){
	std::string name = row[comboboxBGModelCols.m_col_name];
	BGModelFactory::FactoryDict::const_iterator fIt = BGModelFactory::factories.find(name);
	//FIXME: get default values for algorithms??
      }
    }
  onComboboxBGModelChanged_end:
    dialogBGModelSelection->resize(1,1);
  }

  void ViewGtk::onButtonBGModelCancelClicked(){
    dialogBGModelSelection->hide();
  }

  void ViewGtk::onButtonBGModelApplyClicked(){
    ModelColumns comboboxBGModelCols;
    BGModelFactory* newF;
    Gtk::TreeModel::iterator it = comboboxBGModel->get_active();

    if (it){
      Gtk::TreeModel::Row row = *it;
      if (row){
	std::string name = row[comboboxBGModelCols.m_col_name];
	jderobotutil::ParamDict param;
	std::stringstream textss(textviewAlgParam->get_buffer()->get_text());
	textss >> param;
	setBGModel(name,param);
      }
    }
  }

  void ViewGtk::onButtonBGModelAcceptClicked(){
    onButtonBGModelApplyClicked();
    dialogBGModelSelection->hide();
  }

  void ViewGtk::onToolbuttonDumpDataToggled(){
    bool toggled = toolbuttonDumpData->get_active();
    filechooserdialogDumpData->set_visible(toggled);
    //if not active stop dump data via controller
    if (!toggled)
      controller.stopDumpData();
  }

  void ViewGtk::onButtonDumpDataCancelClicked(){
    filechooserdialogDumpData->hide();
  }

  void ViewGtk::onButtonDumpDataAcceptClicked(){
    std::string filename;

    filename = filechooserdialogDumpData->get_filename();
    //activate dump data via controller
    controller.startDumpData(filename,
			     -1,-1,//FIXME: set values from dialog
			     checkbuttonImgDump->get_active(),
			     checkbuttonBgDump->get_active(),
			     checkbuttonFgMaskDump->get_active());
    filechooserdialogDumpData->hide();
  }

  void ViewGtk::setBGModel(const std::string modelName, const jderobotutil::ParamDict& param) throw(){
    //Glib::RefPtr< Gtk::TextBuffer > buffer();
    textviewAlgInfo->get_buffer()->set_text(param.toString());
    textviewAlgInfo->show();
    controller.setBGModel(modelName,param);
  }

}//namespace
