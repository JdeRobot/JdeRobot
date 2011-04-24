/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *             Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#include "view.h"

namespace calibrator {

	View::View(Controller * controller): gtkmain(0,0) {

		/*Create controller*/
		this->controller = controller;

		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(this->controller->getGladePath());

		/*Get widgets*/
        refXml->get_widget("mainwindow",mainwindow);
		refXml->get_widget("pos_x",vscale_pos_x);
		refXml->get_widget("pos_y",vscale_pos_y);
		refXml->get_widget("pos_z",vscale_pos_z);
		refXml->get_widget("foa_x",vscale_foa_x);
		refXml->get_widget("foa_y",vscale_foa_y);
		refXml->get_widget("foa_z",vscale_foa_z);
		refXml->get_widget("fx",vscale_fx);
		refXml->get_widget("fy",vscale_fy);
		refXml->get_widget("u0",vscale_u0);
		refXml->get_widget("v0",vscale_v0);
		refXml->get_widget("roll",vscale_roll);
        refXml->get_widget("image",gtk_image);	
        refXml->get_widget("button_center",button_center);	
        refXml->get_widget("button_save",button_save);	
        refXml->get_widget("button_load",button_Load);		
        
		/*Set default config*/
		vscale_pos_x->set_value((double)this->controller->getPos()->X);
		vscale_pos_y->set_value((double)this->controller->getPos()->Y);
		vscale_pos_z->set_value((double)this->controller->getPos()->Z);
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);		
		vscale_fx->set_value((double)this->controller->getFdistX());
		vscale_fy->set_value((double)this->controller->getFdistY());
		vscale_u0->set_value((double)this->controller->getU0());
		vscale_v0->set_value((double)this->controller->getV0());
		vscale_roll->set_value((double)this->controller->getRoll());
		
		/*Create callbacks*/
		vscale_pos_x->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_x_changed));
		vscale_pos_y->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_y_changed));
		vscale_pos_z->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_z_changed));
		vscale_foa_x->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_x_changed));
		vscale_foa_y->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_y_changed));
		vscale_foa_z->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_z_changed));
		vscale_fx->signal_value_changed().connect(sigc::mem_fun(this,&View::fx_changed));
		vscale_fy->signal_value_changed().connect(sigc::mem_fun(this,&View::fy_changed));
		vscale_u0->signal_value_changed().connect(sigc::mem_fun(this,&View::u0_changed));
		vscale_v0->signal_value_changed().connect(sigc::mem_fun(this,&View::v0_changed));
		vscale_roll->signal_value_changed().connect(sigc::mem_fun(this,&View::roll_changed));
		button_center->signal_clicked().connect(sigc::mem_fun(this,&View::button_center_clicked));
	    button_save->signal_clicked().connect(sigc::mem_fun(this,&View::button_save_clicked));
	    button_Load->signal_clicked().connect(sigc::mem_fun(this,&View::button_Load_clicked));	    
	    
      m_table.resize(10,10);
      window = NULL;
	}

	View::~View() {
		delete this->controller;
	}

  bool View::isVisible(){
    return mainwindow->is_visible();
  }

  void View::display(const colorspaces::Image& image)
  {
		/*Change button*/

		/*Manage image*/
		this->controller->drawWorld(image);

		/*Set image*/
		colorspaces::ImageRGB8 img_rgb8(image);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step); 
    gtk_image->clear();
    gtk_image->set(imgBuff);

		/*Show window*/
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }

	void View::pos_x_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value()); 
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);
		button_KRT_clicked();
  }

	void View::pos_y_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value()); 
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);
		button_KRT_clicked();
  }

	void View::pos_z_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value());
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z); 
		button_KRT_clicked();
  }

	void View::foa_x_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value());
		button_KRT_clicked(); 
  }

	void View::foa_y_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value()); 
        button_KRT_clicked();
  }

	void View::foa_z_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value()); 
        button_KRT_clicked();
  }

	void View::fx_changed(){
		this->controller->setFdistX((float)vscale_fx->get_value()); 
		button_KRT_clicked();
  }

	void View::fy_changed(){
		this->controller->setFdistY((float)vscale_fy->get_value()); 
		button_KRT_clicked();
  }

	void View::u0_changed(){
		this->controller->setU0((float)vscale_u0->get_value()); 
		button_KRT_clicked();
  }

	void View::v0_changed(){
		this->controller->setV0((float)vscale_v0->get_value()); 
		button_KRT_clicked();
  }

	void View::roll_changed(){
		this->controller->setRoll((float)vscale_roll->get_value()); 
		button_KRT_clicked();
  }

	void View::button_center_clicked(){
		this->controller->changeDrawCenter();
  }
  
  void View::button_save_clicked()
  {
      Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_OPEN);
      dialog.set_transient_for(*mainwindow);

      //Add response buttons the the dialog:
      dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
      dialog.add_button("Select", Gtk::RESPONSE_OK);

      int result = dialog.run();

      //Handle the response:
      switch(result){
        case(Gtk::RESPONSE_OK):
        {
          //std::cout << "Select clicked." << std::endl;
          std::cout << "Folder selected: " << dialog.get_filename() << std::endl;
          this->controller->saveParameters(dialog.get_filename().data());
          break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
          std::cout << "Cancel clicked." << std::endl;
          break;
        }
        default:
        {
          std::cout << "Unexpected button clicked." << std::endl;
          break;
        }
      }
  }
  
    void View::button_Load_clicked()
    {
    
		int i=0;
		FILE *worldconfig;
		
        Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_transient_for(*mainwindow);
		
        //Add response buttons the the dialog:
        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dialog.add_button("Select", Gtk::RESPONSE_OK);

        int result = dialog.run();

        //Handle the response:
        switch(result){
            case(Gtk::RESPONSE_OK):{
              //std::cout << "Select clicked." << std::endl;
              std::cout << "Folder selected: " << dialog.get_filename() << std::endl;
                worldconfig=fopen(dialog.get_filename().data(),"r");
                
                if(worldconfig==NULL){
                    cerr << "Calibrator: World configuration configuration file error: " << dialog.get_filename().data() << endl;
                }else{
                    this->controller->resetLines();
                    do{
                        i=this->controller->load_world_line(worldconfig);
                    } while(i!=EOF);
                fclose(worldconfig);
                }

                this->controller->load_world();
              break;
            }
            case(Gtk::RESPONSE_CANCEL):{
              std::cout << "Cancel clicked." << std::endl;
              break;
            }
            default:{
              std::cout << "Unexpected button clicked." << std::endl;
              break;
            }
        }
    }
  
    void View::button_KRT_clicked(){
        

        TPinHoleCamera camera = this->controller->getCam();
        gchar *str;

        if(window!=NULL)
            delete window;

        window = new Gtk::Window();
        window->set_visible(true);
        window->set_title("KRT");
        
        str = g_strdup_printf ("|\t%.1lf", camera.k11);
        labelK[0].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k12);
        labelK[1].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k13);
        labelK[2].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.k14);
        labelK[3].set_text(str); g_free (str);
        str = g_strdup_printf ("|\t%.1lf", camera.k21);
        labelK[4].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k22);
        labelK[5].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k23);
        labelK[6].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.k24);
        labelK[7].set_text(str); g_free (str);
        str = g_strdup_printf ("|\t%.1lf", camera.k31);
        labelK[8].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k32);
        labelK[9].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.k33);
        labelK[10].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.k34);
        labelK[11].set_text(str); g_free (str);
        labelK[12].set_text("K Matrix");

        m_table.attach(labelK[12],1 ,2 ,0, 1 );
        m_table.attach(labelK[0] ,1 ,2 ,1, 2 );
        m_table.attach(labelK[1] ,2 ,3 ,1, 2 );
        m_table.attach(labelK[2] ,3 ,4 ,1, 2 );
        m_table.attach(labelK[3] ,4 ,5 ,1, 2 );

        m_table.attach(labelK[4] ,1 ,2 ,2 , 3 );
        m_table.attach(labelK[5] ,2 ,3 ,2 , 3 );
        m_table.attach(labelK[6] ,3 ,4 ,2 , 3 );
        m_table.attach(labelK[7] ,4 ,5 ,2 , 3 );

        m_table.attach(labelK[8] ,1 ,2 ,3 , 4 );
        m_table.attach(labelK[9] ,2 ,3 ,3 , 4 );
        m_table.attach(labelK[10] ,3 ,4 ,3 , 4 );
        m_table.attach(labelK[11] ,4 ,5 ,3 , 4 );


        str = g_strdup_printf ("|\t%.1lf", camera.rt11);
        labelRT[0].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt12);
        labelRT[1].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt13);
        labelRT[2].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.rt14);
        labelRT[3].set_text(str); g_free (str);
        str = g_strdup_printf ("|\t%.1lf", camera.rt21);
        labelRT[4].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt22);
        labelRT[5].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt23);
        labelRT[6].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.rt24);
        labelRT[7].set_text(str); g_free (str);
        str = g_strdup_printf ("|\t%.1lf", camera.rt31);
        labelRT[8].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt32);
        labelRT[9].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf", camera.rt33);
        labelRT[10].set_text(str); g_free (str);
        str = g_strdup_printf ("%.1lf\t|", camera.rt34);
        labelRT[11].set_text(str); g_free (str);
        labelRT[12].set_text("RT Matrix");

        m_table.attach(labelRT[12],1 ,2 ,4, 5 );
        m_table.attach(labelRT[0] ,1 ,2 ,5, 6 );
        m_table.attach(labelRT[1] ,2 ,3 ,5, 6 );
        m_table.attach(labelRT[2] ,3 ,4 ,5, 6 );
        m_table.attach(labelRT[3] ,4 ,5 ,5, 6 );

        m_table.attach(labelRT[4] ,1 ,2 ,6 , 7 );
        m_table.attach(labelRT[5] ,2 ,3 ,6 , 7 );
        m_table.attach(labelRT[6] ,3 ,4 ,6 , 7 );
        m_table.attach(labelRT[7] ,4 ,5 ,6 , 7 );

        m_table.attach(labelRT[8] ,1 ,2 ,7 , 8 );
        m_table.attach(labelRT[9] ,2 ,3 ,7 , 8 );
        m_table.attach(labelRT[10] ,3 ,4 ,7 , 8 );
        m_table.attach(labelRT[11],4 ,5 ,7 , 8 );
        
        window->add(m_table);
        window->show_all_children();
        
        printf("\tK Matrix:\n\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n",camera.k11,camera.k12,camera.k13,camera.k14);
        printf("\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n",camera.k21,camera.k22,camera.k23,camera.k24);
        printf("\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n\n",camera.k31,camera.k32,camera.k33,camera.k34);
        printf("\tR&T Matrix:\n\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n",camera.rt11,camera.rt12,camera.rt13,camera.rt14);
        printf("\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n",camera.rt21,camera.rt22,camera.rt23,camera.rt24);
        printf("\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n",camera.rt31,camera.rt32,camera.rt33,camera.rt34);
        printf("\t| %.1f\t%.1f\t%.1f\t%.1f\t|\n\n",camera.rt41,camera.rt42,camera.rt43,camera.rt44);
        
       
    }

  
}//namespace
