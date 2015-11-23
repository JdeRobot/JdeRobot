/*
 *  Copyright (C) 1997-2015 JDE Developers Team
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
 *  Authors : 
 *       Alberto Martín Florido <almartinflorido@gmail.com>	
 */

#ifndef ARDRONEPARSER_H
#define ARDRONEPARSER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define ATTR_SET ".<xmlattr>"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ardrone_config.h"

class ArDroneParser
{
    public:
	ArDroneParser();
        ArDroneParser(int id);
        int readFile(std::string filepath,ArDroneConfig *conf);
        void writeFile(std::string filepath,ArDroneConfig *conf);
        virtual ~ArDroneParser();
    private:
        int id;
};

#endif // ARDRONEPARSER_H

