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

#ifndef BGFGVIEW_VIEW_H
#define BGFGVIEW_VIEW_H

#include <gbxutilacfr/exceptions.h>
#include <gtkmm.h>
#include <gtkmm/socket.h>
#include <libglademm.h>
#include <colorspacesmm.h>
#include <memory>
#include "widget.h"
#include "controller.h" //class View

namespace bgfgview {
  class ViewGtk: public View {
  public:
    ViewGtk(Controller &controller) throw();
    virtual ~ViewGtk() throw() {}
    virtual void update(const jderobotutil::Subject* o,
			jderobotutil::ObserverArg* arg = 0)
      throw(gbxutilacfr::Exception);
  private:
    void drawImage(const Gtk::DrawingArea* drawingArea, 
		   const colorspaces::Image& image);    

    void setBGModel(const BGModel& m);

    Gtk::Main gtkmain;
    Glib::RefPtr<Gnome::Glade::Xml> refXml;

    //main window
    Widget<Gtk::Window> mainwindow;
    Widget<Gtk::MenuToolButton> menutoolbuttonSelectBGModel;
    Widget<Gtk::ToggleToolButton> toolbuttonApplyMaskToImage;
    Widget<Gtk::DrawingArea> drawingareaBg;
    Widget<Gtk::DrawingArea> drawingareaFgMask;
    Widget<Gtk::DrawingArea> drawingareaImage;
    Widget<Gtk::Statusbar> statusbarMain;
    bool onDrawingAreaBgExposeEvent(GdkEventExpose* event);
    bool onDrawingAreaFgMaskExposeEvent(GdkEventExpose* event);
    bool onDrawingAreaImageExposeEvent(GdkEventExpose* event);
    void onMenutoolbuttonSelectBGModelClicked();
    void updateMenutoolbuttonItems();
    void onMenutoolbuttonSelectBGModelMenuItemClicked(const std::string algDesc);

    //algorithm selection dialog
    //Tree model columns for comboboxBGModel
    class ModelColumns : public Gtk::TreeModel::ColumnRecord{
    public:
      ModelColumns()
	{ add(m_col_id); }//add(m_col_desc); }
      
      //Gtk::TreeModelColumn<BGModel::BGModelID> m_col_id;
      Gtk::TreeModelColumn<std::string> m_col_desc;
    };

    Widget<Gtk::Dialog> dialogBGModelSelection;
    Widget<Gtk::ComboBox> comboboxBGModel;
    Glib::RefPtr<Gtk::ListStore> comboboxBGModelLSRef;
    Widget<Gtk::Frame> frameCvFGD;
    Widget<Gtk::Frame> frameCvMog;
    Widget<Gtk::Frame> frameExp;
    Widget<Gtk::Frame> frameMean;
    Widget<Gtk::Frame> frameMode;
    Widget<Gtk::Button> buttonBGModelCancel;
    Widget<Gtk::Button> buttonBGModelApply;
    Widget<Gtk::Button> buttonBGModelAccept;
    void updateDialogBGModelSelection();
    void updateComboboxBGModelItems();
    void onComboboxBGModelChanged();
    void onButtonBGModelCancelClicked();
    void onButtonBGModelApplyClicked();
    void onButtonBGModelAcceptClicked();

    class PImpl;
    std::auto_ptr<PImpl> pImpl;
  };
  
}//namespace

#endif /*BGFGVIEW_VIEW_H*/
