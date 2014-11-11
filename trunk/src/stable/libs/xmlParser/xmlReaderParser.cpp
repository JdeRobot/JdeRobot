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

#include "xmlReaderParser.h"

namespace Tracker3DPT {

xmlReaderParser::xmlReaderParser(std::string path) {

	this->parser.set_substitute_entities();
	this->parser.parse_file(path);
	/*if (this->parser.get_validate()){
		std::cout << path << " document has a valid xml format." << std::endl;
	}
	else{
		std::cout << "ERROR: " << path << " document has not a valid xml format." << std::endl;

	}*/


}

xmlReaderParser::~xmlReaderParser() {

}


bool xmlReaderParser::parse(std::string key, int& value) {
	xmlpp::NodeSet n=parser.get_document()->get_root_node()->find(key);
	if (!n.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(n[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value;
		return true;
	}
	return false;
}


bool xmlReaderParser::parse(std::string key, double& value) {
	xmlpp::NodeSet n=parser.get_document()->get_root_node()->find(key);
	if (!n.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(n[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value;
		return true;
	}
	return false;
}


bool xmlReaderParser::parse(std::string key, float& value){
	xmlpp::NodeSet n=parser.get_document()->get_root_node()->find(key);
	if (!n.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(n[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value;
		return true;
	}
	return false;
}



bool xmlReaderParser::parse(std::string key, bool& value) {
	xmlpp::NodeSet n=parser.get_document()->get_root_node()->find(key);
	if (!n.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(n[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> std::boolalpha >> value;
		return true;
	}
	return false;
}

bool xmlReaderParser::parse(std::string key, cv::Point3f& value){
	xmlpp::NodeSet nx=parser.get_document()->get_root_node()->find(key+"/x");
	if (!nx.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nx[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value.x;
	}
	else{
		return false;
	}
	xmlpp::NodeSet ny=parser.get_document()->get_root_node()->find(key+"/y");
	if (!ny.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(ny[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value.y;
	}
	else{
		return false;
	}
	xmlpp::NodeSet nz=parser.get_document()->get_root_node()->find(key+"/z");
	if (!nz.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nz[0]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> value.z;
		return true;
	}
	else{
		return false;
	}
	return false;
}
/*bool xmlReaderParser::parse(std::string key, Eigen::Vector4d& value){
	return false;
}*/


bool xmlReaderParser::parse(std::string key, bool active, Eigen::Vector4d& pos , Eigen::Vector4d& size, Eigen::Vector4d& dir, int index){
	//active
	xmlpp::NodeSet nActive=parser.get_document()->get_root_node()->find(key+"/Active");
	if (!nActive.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nActive[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> std::boolalpha >> active;
	}
	else{
		return false;
	}

	//pos
	xmlpp::NodeSet nPosX0=parser.get_document()->get_root_node()->find(key+"/Pos/x0");
	if (!nPosX0.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nPosX0[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> pos[0];
	}
	else
		return false;

	xmlpp::NodeSet nPosX1=parser.get_document()->get_root_node()->find(key+"/Pos/x1");
	if (!nPosX1.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nPosX1[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> pos[1];
	}
	else
		return false;

	xmlpp::NodeSet nPosX2=parser.get_document()->get_root_node()->find(key+"/Pos/x2");
	if (!nPosX2.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nPosX2[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> pos[2];
	}
	else
		return false;

	xmlpp::NodeSet nPosX3=parser.get_document()->get_root_node()->find(key+"/Pos/x3");
	if (!nPosX3.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nPosX3[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> pos[3];
	}
	else
		return false;

	//Size
	xmlpp::NodeSet nSizeX0=parser.get_document()->get_root_node()->find(key+"/Size/x0");
	if (!nSizeX0.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nSizeX0[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >>size[0];
	}
	else
		return false;


	xmlpp::NodeSet nSizeX1=parser.get_document()->get_root_node()->find(key+"/Size/x1");
	if (!nSizeX1.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nSizeX1[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> size[1];
	}
	else
		return false;

	xmlpp::NodeSet nSizeX2=parser.get_document()->get_root_node()->find(key+"/Size/x2");
	if (!nSizeX2.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nSizeX2[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> size[2];
	}
	else
		return false;

	xmlpp::NodeSet nSizeX3=parser.get_document()->get_root_node()->find(key+"/Size/x3");
	if (!nSizeX3.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nSizeX3[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> size[3];
	}
	else
		return false;

	//Dir
	xmlpp::NodeSet nDirX0=parser.get_document()->get_root_node()->find(key+"/Dir/x0");
	if (!nDirX0.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nDirX0[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >>dir[0];
	}
	else
		return false;

	xmlpp::NodeSet nDirX1=parser.get_document()->get_root_node()->find(key+"/Dir/x1");
	if (!nDirX1.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nDirX1[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> dir[1];
	}
	else
		return false;

	xmlpp::NodeSet nDirX2=parser.get_document()->get_root_node()->find(key+"/Dir/x2");
	if (!nDirX2.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nDirX2[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> dir[2];
	}
	else
		return false;

	xmlpp::NodeSet nDirX3=parser.get_document()->get_root_node()->find(key+"/Dir/x3");
	if (!nDirX3.empty()){
		const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*(nDirX3[index]->get_children().begin()));
		std::istringstream sTemp(nodeTextValue->get_content());
		sTemp >> dir[3];
	}
	else
		return false;

	return false;
}


bool xmlReaderParser::getKeyNumbers(std::string key, int& numbers){
	xmlpp::NodeSet n=parser.get_document()->get_root_node()->find(key);
	if (!n.empty()){
		numbers=n.size();
		return true;
	}
	return false;
}

} /* namespace Tracker3DPT */
