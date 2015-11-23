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

#include "ardrone_config.h"

ArDroneConfig::ArDroneConfig()
{
    //ctor
}

ArDroneConfig::~ArDroneConfig()
{
    //dtor
}

void ArDroneConfig::clearParameters()
{
	parameters.clear();
}
bool ArDroneConfig::setParameterValue(std::string key, double value)
{
	std::pair<std::map<std::string,double>::iterator,bool> ret;
	ret=parameters.insert(std::pair<std::string,double>(key,value));
	if(ret.second==false){
		return false;
	}
	std::cout << "PUT Parameter: " << key << " value: " << value << std::endl;
	return true;
}
double ArDroneConfig::getParameterValue(std::string key)
{
	std::map<std::string,double>::iterator it;
	it=parameters.find(key);
	if(it==parameters.end()){
		return -1;
	}
	std::cout << "GET Parameter: " << key << " value: " << it->second << std::endl;
	return it->second;
}
std::vector<double> ArDroneConfig::getValuesAsArray()
{
	std::vector<double> values;
	for(std::map<std::string,double>::const_iterator it=parameters.begin(); it!=parameters.end(); ++it)
	{
		values.push_back(it->second);
	}
	return values;
}
std::vector<std::string> ArDroneConfig::getParametersAsArray()
{
	std::vector<std::string> values;
	for(std::map<std::string,double>::const_iterator it=parameters.begin(); it!=parameters.end(); ++it)
	{
		values.push_back(it->first);
	}
	return values;
}

void ArDroneConfig::printParameters()
{
	std::vector<std::string> keys=this->getParametersAsArray();

	
	for(std::vector<std::string>::size_type i = 0; i != keys.size(); i++) {
		std::string key=keys[i];
		std::cout << key << " = "<< this->getParameterValue(key) << std::endl;
    
	}
}

