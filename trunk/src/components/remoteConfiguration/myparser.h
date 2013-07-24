/*
 *  Copyright (C) 1997-2012 JDE Developers Teameldercare.camRGB
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
			
 */
#ifndef eldercare_XMLPARSER_H
#define eldercare_XMLPARSER_H


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml++/libxml++.h>
#include <iostream>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <jderobot/remoteConfig.h>



namespace eldercare{
	class myparser: public Gtk::Window{
		public:
			myparser(jderobot::remoteConfigPrx configPrx, int id);
			virtual ~myparser();
			int parsePath(std::string filepath);
			int xmlBuild(std::string path);

		private:

			//callbacks
			void parse_sec_nodes(const xmlpp::Node* node, Gtk::VBox *parent);
			void parse_ppal_nodes(const xmlpp::Node* node, Gtk::HBox *parent);
			void alarmsParser(const xmlpp::Node* node);
			void guiParser(const xmlpp::Node* node);

			//alarms callbacks
			void configParser(const xmlpp::Node* node);
			void alarmVolumeParser(const xmlpp::Node* node);
			void alarmPositionParser(const xmlpp::Node* node);
			void alarmPresenceParser(const xmlpp::Node* node);
			void alarmFallParser(const xmlpp::Node* node);
			void testF();
			int imprime(Gtk::Container* w, int tab);

			//prx
			jderobot::remoteConfigPrx prx;
			int id;
		
	};



} //namespace
#endif
