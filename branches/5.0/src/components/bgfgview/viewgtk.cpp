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
#include <tr1/memory>
#include <string>
#include <opencv/cvaux.h>
#include "viewgtk.h"
#include "model.h"

namespace bgfgview {
  const std::string gladepath = std::string(GLADE_DIR) + std::string("/bgfgview.glade");

  typedef std::map<std::string, std::tr1::shared_ptr<BGModelFactory> > BGModelFactoryMap;

  class ViewGtk::PImpl{
  public:
    BGModelFactoryMap bgmodelF;
    //std::auto_ptr<BGModel> currentModel;
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
      INIT_WIDGET(drawingareaBg, refXml),
      INIT_WIDGET(drawingareaFgMask, refXml),
      INIT_WIDGET(drawingareaImage, refXml),
      INIT_WIDGET(statusbarMain, refXml),
      INIT_WIDGET(dialogBGModelSelection, refXml),
      INIT_WIDGET(comboboxBGModel, refXml),
      INIT_WIDGET(frameCvFGD, refXml),
      INIT_WIDGET(frameCvMoG, refXml),
      INIT_WIDGET(frameExp, refXml),
      INIT_WIDGET(frameMean, refXml),
      INIT_WIDGET(frameMode, refXml),
      INIT_WIDGET(buttonBGModelCancel, refXml),
      INIT_WIDGET(buttonBGModelApply, refXml),
      INIT_WIDGET(buttonBGModelAccept, refXml),
      pImpl(new PImpl())
  {
    //main window
    menutoolbuttonSelectBGModel->signal_clicked().connect(sigc::mem_fun(this,&ViewGtk::onMenutoolbuttonSelectBGModelClicked));


    toolbuttonApplyMaskToImage->set_active(false);
        
    drawingareaBg->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaBgExposeEvent));
    drawingareaFgMask->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaFgMaskExposeEvent));
    drawingareaImage->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaImageExposeEvent));
    mainwindow->show();

    //algorithm selection dialog
    comboboxBGModel->signal_changed().connect(sigc::mem_fun(this, &ViewGtk::onComboboxBGModelChanged));
    buttonBGModelCancel->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelCancelClicked));
    buttonBGModelApply->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelApplyClicked));
    buttonBGModelAccept->signal_clicked().connect(sigc::mem_fun(this, &ViewGtk::onButtonBGModelAcceptClicked));
    dialogBGModelSelection->hide();

    //populate bgmodelF
    //OpenCv FGD
    BGModelFactory* bgmF;
    bgmF = new BGModelCvFGDFactory();
    pImpl->bgmodelF.insert(make_pair(bgmF->description,bgmF));
    //OpenCv Mog
    bgmF = new BGModelCvMoGFactory();
    pImpl->bgmodelF.insert(make_pair(bgmF->description,bgmF));
    //Exp
    bgmF = new BGModelExpFactory();
    pImpl->bgmodelF.insert(make_pair(bgmF->description,bgmF));
    //Mean
    bgmF = new BGModelMeanFactory();
    pImpl->bgmodelF.insert(make_pair(bgmF->description,bgmF));
    //Mode
    bgmF = new BGModelModeFactory();
    pImpl->bgmodelF.insert(make_pair(bgmF->description,bgmF));

    //update algorithm selection button
    updateMenutoolbuttonItems();

  }

  void ViewGtk::update(const jderobotutil::Subject* o, jderobotutil::ObserverArg* arg)
    throw (gbxutilacfr::Exception) {

    //image drawing is done on expose events send with these calls
    drawingareaBg->queue_draw();
    drawingareaFgMask->queue_draw();
    drawingareaImage->queue_draw();
    
    //get iterations per second
    std::stringstream ss;
    ss << controller.model().bgModelIps().ips();
    statusbarMain->push(ss.str());    

    mainwindow->resize(1, 1);

    //FIXME: gui use bgfgview thread, we have to move this to another thread
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }

  void ViewGtk::drawImage(Gtk::DrawingArea* drawingArea, 
			  const colorspaces::Image& image) {
    
    Glib::RefPtr<Gdk::Window> window = drawingArea->get_window();
    if (window) {
      /*convert to RGB*/
      colorspaces::ImageRGB888 img_rgb888(image);
      Glib::RefPtr<Gdk::Pixbuf> imgBuff =
	Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,
				      Gdk::COLORSPACE_RGB,
				      false,
				      8,
				      img_rgb888.width,
				      img_rgb888.height,
				      img_rgb888.step);

      const Glib::RefPtr< const Gdk::GC > gc; /*empty*/
      window->draw_pixbuf(gc,
			  imgBuff,
			  0, 0, /*starting point from imgBuff*/
			  0, 0, /*starting point into drawable*/
			  imgBuff->get_width(),
			  imgBuff->get_height(),
			  Gdk::RGB_DITHER_NONE, 0, 0);
      drawingArea->set_size_request(img_rgb888.width,
				    img_rgb888.height);
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
    //show algorithm selection dialog
    dialogBGModelSelection->show();
  }
  
  void ViewGtk::updateMenutoolbuttonItems() {
    BGModelFactoryMap::const_iterator bgmodelIt;

    Gtk::Menu* selectBGModelMenu = Gtk::manage(new Gtk::Menu());

    for(bgmodelIt = pImpl->bgmodelF.begin();
	bgmodelIt != pImpl->bgmodelF.end();
	bgmodelIt++){
      Gtk::MenuItem* itemp = Gtk::manage(new Gtk::MenuItem(bgmodelIt->first));
      itemp->signal_activate().connect(sigc::bind<std::string>(sigc::mem_fun(this, &ViewGtk::onMenutoolbuttonSelectBGModelMenuItemClicked),bgmodelIt->first));
      selectBGModelMenu->append(*itemp);
    }

    menutoolbuttonSelectBGModel->set_menu(*selectBGModelMenu);
  }

  void ViewGtk::setBGModel(const BGModelFactory& m){
    IplImage tmpImg(controller.model().getBGImage());
    CvBGStatModel* newBgModel = m.createModel(&tmpImg);
    controller.setBGModel(newBgModel);
    pImpl->bgmodelF["Current"] = 
      std::tr1::shared_ptr<BGModelFactory>(m.clone());
  }

  void ViewGtk::onMenutoolbuttonSelectBGModelMenuItemClicked(const std::string bgmodelDesc){
    //apply set algorithm
    BGModelFactoryMap::const_iterator bgmodelIt = 
      pImpl->bgmodelF.find(bgmodelDesc);
    if (bgmodelIt != pImpl->bgmodelF.end()){
      setBGModel(*(bgmodelIt->second));
    }
  }

  void ViewGtk::updateComboboxBGModelItems(){
    BGModelFactoryMap::const_iterator bgmodelIt;
    
    ModelColumns comboboxBGModelCols;
    comboboxBGModelLSRef = Gtk::ListStore::create(comboboxBGModelCols);
    Gtk::TreeModel::iterator tmIt;
    bool hasCurrent = false;
    Gtk::TreeModel::iterator currIt;
    
    comboboxBGModel->clear();
    comboboxBGModel->set_model(comboboxBGModelLSRef);

    Gtk::TreeModel::Row r;
    for(bgmodelIt = pImpl->bgmodelF.begin();
	bgmodelIt != pImpl->bgmodelF.end();
	bgmodelIt++){
      tmIt = comboboxBGModelLSRef->append();
      r = *tmIt;
      r[comboboxBGModelCols.m_col_name] = bgmodelIt->first;
      r[comboboxBGModelCols.m_col_desc] = bgmodelIt->second->description;
      if (bgmodelIt->first.compare("Current") == 0){
	hasCurrent = true;
	currIt = tmIt;
      }
    }

    
    comboboxBGModel->pack_start(comboboxBGModelCols.m_col_name);
    comboboxBGModel->pack_start(comboboxBGModelCols.m_col_desc);
    if (hasCurrent)
      comboboxBGModel->set_active(currIt);
  }

  void ViewGtk::updateDialogBGModelSelection(){
    //hide parameter frames
    frameCvFGD->hide();
    frameCvMoG->hide();
    frameExp->hide();
    frameMean->hide();
    frameMode->hide();
  }

  void ViewGtk::onComboboxBGModelChanged(){
    ModelColumns comboboxBGModelCols;
    Gtk::TreeModel::iterator it = comboboxBGModel->get_active();

    //hide parameter frames
    frameCvFGD->hide();
    frameCvMoG->hide();
    frameExp->hide();
    frameMean->hide();
    frameMode->hide();

    if (it){
      Gtk::TreeModel::Row row = *it;
      if (row){
	std::string name = row[comboboxBGModelCols.m_col_name];
	BGModelFactoryMap::const_iterator bgmodelFIt = pImpl->bgmodelF.find(name);
	
	// if (bgmodelFIt != pImpl->bgmodelF.end()){
	//   //cast to get bgmodelF type
	//   BGModelCvFGDFactory* bgmCvFGDFactoryPtr = dynamic_cast<BGModelCvFGDFactory*>(bgmodelFIt->second.get());
	//   if (bgmCvFGDFactoryPtr != 0){
	//     frameCvFGD->show();
	//     goto onComboboxBGModelChanged_end;
	//   }

	//   BGModelCvMoGFactory* bgmCvMoGFactoryPtr = dynamic_cast<BGModelCvMoGFactory*>(bgmodelFIt->second.get());
	//   if (bgmCvMoGFactoryPtr != 0){
	//     frameCvMoG->show();
	//     goto onComboboxBGModelChanged_end;
	//   }

	//   BGModelExpFactory* bgmExpFactoryPtr = dynamic_cast<BGModelExpFactory*>(bgmodelFIt->second.get());
	//   if (bgmExpFactoryPtr != 0){
	//     frameExp->show();
	//     goto onComboboxBGModelChanged_end;
	//   }

	//   BGModelMeanFactory* bgmMeanFactoryPtr = dynamic_cast<BGModelMeanFactory*>(bgmodelFIt->second.get());
	//   if (bgmMeanFactoryPtr != 0){
	//     frameMean->show();
	//     goto onComboboxBGModelChanged_end;
	//   }

	//   BGModelModeFactory* bgmModeFactoryPtr = dynamic_cast<BGModelModeFactory*>(bgmodelFIt->second.get());
	//   if (bgmModeFactoryPtr != 0){
	//     frameMode->show();	    
	//     goto onComboboxBGModelChanged_end;
	//   }
	// }
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
	BGModelFactoryMap::const_iterator bgmodelFIt = pImpl->bgmodelF.find(name);
	
	if (bgmodelFIt != pImpl->bgmodelF.end())
	  setBGModel(*(bgmodelFIt->second));
      }
    }
  }


	//   //cast to get bgmodelF type
	//   BGModelCvFGDFactory* bgmCvFGDFactoryPtr = dynamic_cast<BGModelCvFGDFactory*>(bgmodelFIt->second.get());
	//   if (bgmCvFGDFactoryPtr != 0){
	//     //get params, build factory
	    

	//     goto onButtonBGModelApplyClicked_end;
	//   }

	//   BGModelCvMoGFactory* bgmCvMoGFactoryPtr = dynamic_cast<BGModelCvMoGFactory*>(bgmodelFIt->second.get());
	//   if (bgmCvMoGFactoryPtr != 0){
	    
	//     goto onButtonBGModelApplyClicked_end;
	//   }

	//   BGModelExpFactory* bgmExpFactoryPtr = dynamic_cast<BGModelExpFactory*>(bgmodelFIt->second.get());
	//   if (bgmExpFactoryPtr != 0){
	    
	//     goto onButtonBGModelApplyClicked_end;
	//   }

	//   BGModelMeanFactory* bgmMeanFactoryPtr = dynamic_cast<BGModelMeanFactory*>(bgmodelFIt->second.get());
	//   if (bgmMeanFactoryPtr != 0){
	    
	//     goto onButtonBGModelApplyClicked_end;
	//   }

	//   BGModelModeFactory* bgmModeFactoryPtr = dynamic_cast<BGModelModeFactory*>(bgmodelFIt->second.get());
	//   if (bgmModeFactoryPtr != 0){
	        
	//     goto onButtonBGModelApplyClicked_end;
	//   }
	//}
	//}
	//}
  //onButtonBGModelApplyClicked_end:
    //check errors, createModel
  
	//}

  void ViewGtk::onButtonBGModelAcceptClicked(){
    onButtonBGModelApplyClicked();
    dialogBGModelSelection->hide();
  }

}//namespace
