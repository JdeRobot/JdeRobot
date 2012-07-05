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
*  Authors : Sara Marugán Alonso <smarugan@gsyc.es>,
*            Eduardo Perdices <eperdices@gsyc.es>
*            Alejandro Hernández Cordero <ahcorde@gmail.com>
*	     Agustín Gallardo Díaz <agallard4@gmail.com>
*/

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include "view.h"
#include "module_dlt.h"
#include "module_extrinsics.h"

int main(int argc, char** argv){
	int status;
	Ice::CommunicatorPtr ic;
	calibrator::View * view;
	calibrator::Module_DLT * module_calibrator;
	calibrator::Module_Extrinsics * module_extrinsics;
	calibrator::Module_Rectifier * module_rectifier;

	Gtk::Main run(argc, argv);

	try{
		// Get Properties
		ic = Ice::initialize(argc,argv);
		Ice::PropertiesPtr prop = ic->getProperties();
		std::string worldconf = prop->getProperty("Calibrator.World.File");
		std::string camOutconf = prop->getProperty("Calibrator.Camera.FileOut");


		/* Get cameras */
		Ice::PropertyDict cameras = prop->getPropertiesForPrefix("Calibrator.Camera.Proxy");
		Ice::PropertyDict::iterator p;
	int num_cameras = 0;
	/* Sería feliz eliminando este bucle.. ¿cuál es el método para obtener el tamaño?*/
	for(p = cameras.begin(); p != cameras.end(); ++p)
	{
		std::cout << p->first << " - " << p->second << std::endl;
		num_cameras++;
	}

		Ice::ObjectPrx base[num_cameras];
		jderobot::CameraPrx cprx[num_cameras];
		jderobot::ImageDataPtr data[num_cameras];
		

		int i = 0;
		calibrator::ModelColumns m_Columns;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

		m_refTreeModel = Gtk::ListStore::create(m_Columns);
		Gtk::TreeModel::Row row;

		for(p = cameras.begin(); p != cameras.end(); ++p)
		{
			base[i] = ic->propertyToProxy(p->first);
			if (0==base[i])
				throw "Could not create proxy";
			
			/*cast to CameraPrx */
			cprx[i] = jderobot::CameraPrx::checkedCast(base[i]);
			if (0==cprx[i])
				throw "Invalid proxy";



			/* Load combo */
			i++;
			std::stringstream i_str;
			i_str << i;
			row = *(m_refTreeModel->append());
			row[m_Columns.m_col_id] = i_str.str();
			row[m_Columns.m_col_name] = p->first;		
		}



	
		/*Create Controller and View */
		module_calibrator = new calibrator::Module_DLT(prop);
		module_extrinsics = new calibrator::Module_Extrinsics(prop);
		module_rectifier = new calibrator::Module_Rectifier(prop);

		// Connect with UI
		view = new calibrator::View(m_refTreeModel, module_calibrator, module_extrinsics, module_rectifier);

		int active_camera = 0;
		while(view->isVisible()){
			active_camera = view->get_active_camera();

//			if (view->capture_on == 1) Falta implementarlo para que funcione correctamente con el nuevo modelo
//			{
			/* Get image */
			data[active_camera] = cprx[active_camera]->getImageData();

			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data[active_camera]->description->format);

			if (!fmt)
			throw "Format not supported";

			colorspaces::Image image(data[active_camera]->description->width,
			data[active_camera]->description->height,
			fmt,
			&(data[active_camera]->pixelData[active_camera]));

			view->display(image);
			usleep(10*1000);

		}

		std::cout << "Adios\n";
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		status = 1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		status = 1;
	}

	if (ic)
		ic->destroy();

	return status;
}//namespace
