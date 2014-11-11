/*
 *  Copyright (C) 1997-2014 JDE Developers Team
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */


/*! \file xmlWriterParser.h
    \brief xml parser used to write xml configurations
*/

#include "xmlWriterParser.h"

namespace Tracker3DPT {

/**
 * \brief ConfigTracker constructor
 * This method initializes the xml document
 *
 *  @param rootNode The root node name for the xml file
 *  @param fileName The filename of the generated xml file
 */
xmlWriterParser::xmlWriterParser(std::string rootNode, std::string fileName) {
	xmlpp::Element* r=this->document.create_root_node(rootNode);
	r->set_child_text("");
	this->fileName=fileName;
}

/**
 * \brief ConfigTracker destructor
 */
xmlWriterParser::~xmlWriterParser() {
}

/**
 *  \brief Method to save the generated xml to a file
 */
void xmlWriterParser::saveToFile(){
	this->document.write_to_file(this->fileName);
}

/**
 *  \brief Method to print of screen the generated xml
 */
void xmlWriterParser::printXmlFile(){
	std::string whole = this->document.write_to_string();
	std::cout << "XML built at runtime: " << std::endl << whole << std::endl;
}

/**
 * \brief Method to split string using "/" as delimitator
 *
 * @param value The input string
 * @param array The output string array once splitted
 */
void xmlWriterParser::splitString(std::string value, std::vector<std::string>& array){
	size_t pos = 0;
	std::string token;
	while ((pos = value.find("/")) != std::string::npos) {
		token = value.substr(0, pos);
		array.push_back(token);
		value.erase(0, pos + 1);
	}
	array.push_back(value);
}

/**
 * \brief Method that checks if a key exsits
 *
 * @param key The searched key
 * @return True if the element exists
 */
bool xmlWriterParser::elementExists(std::string key){
	xmlpp::NodeSet n=this->document.get_root_node()->find(key);
	return (n.size()!=0);
}

/**
 *  \brief Method that look for a key and if it does not exists this make the correponding way
 *
 * @param key The searched key
 * @return the parent node of the searched key
 */
xmlpp::Element* xmlWriterParser::lookForKeyAndMakeway(std::string key){
	std::vector<std::string> s;

	splitString(key,s);
	if (s.size()!=1){
		std::string completeKey(s[0]);
		std::string parentKey(s[0]);
		for (int i=0; i!= s.size()-1; i++){
			if (!elementExists(completeKey)){
				if (i==0){
					//we have to create the node under the parent document.
					xmlpp::Element* n1= document.get_root_node();
					xmlpp::Element* n2= n1->add_child(parentKey);
					n2->set_child_text("");
				}
				else{
					xmlpp::NodeSet ns1=document.get_root_node()->find(parentKey);
					xmlpp::Element* n1 = ns1[0]->add_child(s[i]);
					n1->set_child_text("");
				}
			}
			if (i!=s.size()-2)
				completeKey+="/"+s[i+1];
			if (i!=0)
				parentKey+="/"+s[i];
		}
		xmlpp::NodeSet ns1=document.get_root_node()->find(parentKey);
		xmlpp::Element* n1 = ns1[0]->add_child(s[s.size()-1]);
		return n1;
	}
	else{
		xmlpp::Element* n1= document.get_root_node();
		xmlpp::Element* n2= n1->add_child(s[0]);
		return n2;
	}
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (string)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, std::string value){

	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	n1->set_child_text(value);
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (cv::Point3f)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, cv::Point3f value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	n1->set_child_text("");
	xmlpp::Element* nx = n1->add_child("x");
	xmlpp::Element* ny = n1->add_child("y");
	xmlpp::Element* nz = n1->add_child("z");
	std::stringstream isX;
	isX << value.x;
	nx->set_child_text(isX.str());
	std::stringstream isY;
	isY << value.y;
	ny->set_child_text(isY.str());
	std::stringstream isZ;
	isZ << value.z;
	nz->set_child_text(isZ.str());

}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (double)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, double value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	std::stringstream ss;
	ss << value;
	n1->set_child_text(ss.str());
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (float)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, float value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	std::stringstream ss;
	ss << value;
	n1->set_child_text(ss.str());
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (int)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, int value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	std::stringstream ss;
	ss << value;
	n1->set_child_text(ss.str());
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (bool)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, bool value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	if (value)
		n1->set_child_text("True");
	else
		n1->set_child_text("False");
}

/**
 * \brief Overloaded method that includes into the xml document a key (complete path) and its value (Eigen::Vector4d)
 *
 * @param key The new key
 * @param value The value of the new key
 */
void xmlWriterParser::addKey(std::string key, Eigen::Vector4d value){
	xmlpp::Element* n1=lookForKeyAndMakeway(key);
	n1->set_child_text("");
	xmlpp::Element* nx0 = n1->add_child("x0");
	xmlpp::Element* nx1 = n1->add_child("x1");
	xmlpp::Element* nx2 = n1->add_child("x2");
	xmlpp::Element* nx3 = n1->add_child("x3");
	std::stringstream ssX0;
	ssX0 << value[0];
	nx0->set_child_text(ssX0.str());
	std::stringstream ssX1;
	ssX1 << value[1];
	nx1->set_child_text(ssX1.str());
	std::stringstream ssX2;
	ssX2 << value[2];
	nx2->set_child_text(ssX2.str());
	std::stringstream ssX3;
	ssX3 << value[3];
	nx3->set_child_text(ssX3.str());
}


} /* namespace Tracker3DPT */
