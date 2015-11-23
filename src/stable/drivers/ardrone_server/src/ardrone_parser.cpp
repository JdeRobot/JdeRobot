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

#include "ardrone_parser.h"
#include <fstream>

using namespace boost;
using namespace boost::property_tree;


std::string float2string(float number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}


float string2float(std::string val)
{
	float temp;
	std::istringstream(val) >> temp;
	return temp;
}



ArDroneParser::ArDroneParser()
{


}

ArDroneParser::ArDroneParser(int id)
{
    //ctor
    this->id=id;
}

ArDroneParser::~ArDroneParser()
{
    //dtor
}


const ptree& empty_ptree(){
    static ptree t;
    return t;
}

void ArDroneParser::writeFile(std::string filepath,ArDroneConfig *conf)
{
	using boost::property_tree::ptree;
	ptree pt;

	boost::property_tree::ptree rootNode;

	std::vector<std::string> keys=conf->getParametersAsArray();
	for(std::vector<std::string>::size_type i = 0; i != keys.size(); i++) {
		std::string key=keys[i];
		boost::property_tree::ptree node;
		node.put("<xmlattr>.name", key);
		node.put("<xmlattr>.value", float2string(conf->getParameterValue(key)));
		rootNode.add_child("param",node);
	}
	rootNode.put("<xmlattr>.name", "ardrone_server");
	pt.add_child("component", rootNode);
	boost::property_tree::xml_writer_settings<char> settings('\t', 1);
	write_xml(filepath, pt, std::locale(), settings);	
}

int ArDroneParser::readFile(std::string filepath,ArDroneConfig *conf)
{

	ptree tree;
	read_xml(filepath, tree);
	const ptree & formats = tree.get_child("component", empty_ptree());
	BOOST_FOREACH(const ptree::value_type & f, formats){
		std::string at = f.first + ATTR_SET;
		std::string name,value;
		const ptree & attributes = f.second.get_child("<xmlattr>", empty_ptree());

		BOOST_FOREACH(const ptree::value_type &v, attributes){
		    std::string first(v.first.data());
		    if(first.compare("name")==0)
		    {
		    	name=v.second.data();
		    }
		    if(first.compare("value")==0)
		    {
		    	value=v.second.data();
		    	conf->setParameterValue(name,string2float(value));
		    }		    
		}
	}	
	
}
