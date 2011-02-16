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

#ifndef BGFGLAB_VIEWGTK_H
#define BGFGLAB_VIEWGTK_H

#include <jderobotutil/paramdict.h>
#include <gbxutilacfr/exceptions.h>
#include <gtkmm.h>
#include <gtkmm/socket.h>
#include <libglademm.h>
#include <colorspaces/colorspacesmm.h>
#include <memory>
#include <vector>
#include "widget.h"
#include "controller.h" //class View

namespace bgfglab {
  class ViewGtk: public View {
  public:
    ViewGtk(Controller &controller) throw();
    virtual ~ViewGtk() throw() {}
    virtual void update(const jderobotutil::Subject* o,
			jderobotutil::ObserverArg* arg = 0)
      throw(gbxutilacfr::Exception);
  private:
    void drawImage(Gtk::DrawingArea* drawingArea, 
		   const colorspaces::Image& image);    

    Gtk::Main gtkmain;
    Glib::RefPtr<Gnome::Glade::Xml> refXml;

    //main window
    Widget<Gtk::Window> mainwindow;
    Widget<Gtk::MenuToolButton> menutoolbuttonSelectBGModel;
    //Gtk::Menu selectBGModelMenu;
    //std::vector< Glib::RefPtr<Gtk::MenuItem> > menutoolbuttonSelectBGModelItems;
    Widget<Gtk::ToggleToolButton> toolbuttonApplyMaskToImage;
    Widget<Gtk::ToggleToolButton> toolbuttonDumpData;
    Widget<Gtk::DrawingArea> drawingareaBg;
    Widget<Gtk::DrawingArea> drawingareaFgMask;
    Widget<Gtk::DrawingArea> drawingareaImage;
    Widget<Gtk::TextView> textviewAlgInfo;
    Widget<Gtk::Statusbar> statusbarMain;
    bool onDrawingAreaBgExposeEvent(GdkEventExpose* event);
    bool onDrawingAreaFgMaskExposeEvent(GdkEventExpose* event);
    bool onDrawingAreaImageExposeEvent(GdkEventExpose* event);
    void onMenutoolbuttonSelectBGModelClicked();
    void updateMenutoolbuttonItems();
    void onMenutoolbuttonSelectBGModelMenuItemClicked(const std::string algDesc);
    void onToolbuttonDumpDataToggled();

    void setBGModel(const std::string modelName, const jderobotutil::ParamDict& param) throw();

    //algorithm selection dialog
    //Tree model columns for comboboxBGModel
    class ModelColumns : public Gtk::TreeModel::ColumnRecord{
    public:
      ModelColumns()
	{ add(m_col_name);
	  add(m_col_desc); }
      
      Gtk::TreeModelColumn<std::string> m_col_name;
      Gtk::TreeModelColumn<std::string> m_col_desc;
    };

    Widget<Gtk::Dialog> dialogBGModelSelection;
    Widget<Gtk::ComboBox> comboboxBGModel;
    Glib::RefPtr<Gtk::ListStore> comboboxBGModelLSRef;
    Widget<Gtk::TextView> textviewAlgParam;
    Widget<Gtk::Button> buttonBGModelCancel;
    Widget<Gtk::Button> buttonBGModelApply;
    Widget<Gtk::Button> buttonBGModelAccept;
    void updateDialogBGModelSelection();
    void updateComboboxBGModelItems();
    void onComboboxBGModelChanged();
    void onButtonBGModelCancelClicked();
    void onButtonBGModelApplyClicked();
    void onButtonBGModelAcceptClicked();

    //dump data file chooser dialog
    Widget<Gtk::FileChooserDialog> filechooserdialogDumpData;
    Widget<Gtk::CheckButton> checkbuttonImgDump;
    Widget<Gtk::CheckButton> checkbuttonBgDump;
    Widget<Gtk::CheckButton> checkbuttonFgMaskDump;
    Widget<Gtk::Button> buttonDumpDataCancel;
    Widget<Gtk::Button> buttonDumpDataAccept;
    void onButtonDumpDataCancelClicked();
    void onButtonDumpDataAcceptClicked();
    

    class PImpl;
    std::auto_ptr<PImpl> pImpl;
  };
  
}//namespace

#endif /*BGFGLAB_VIEWGTK_H*/
