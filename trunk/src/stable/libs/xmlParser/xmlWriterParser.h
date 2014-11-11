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


#ifndef XMLWRITERPARSER_H_
#define XMLWRITERPARSER_H_

#include <libxml++/libxml++.h>
#include <iostream>
#include <cv.h>
#include <eigen3/Eigen/Dense>



namespace Tracker3DPT {

class xmlWriterParser {
public:
	xmlWriterParser(std::string rootNode, std::string fileName);
	virtual ~xmlWriterParser();
	void saveToFile();
	void printXmlFile();
	void addKey(std::string key, std::string value);
	void addKey(std::string key, cv::Point3f value);
	void addKey(std::string key, double value);
	void addKey(std::string key, float value);
	void addKey(std::string key, int value);
	void addKey(std::string key, bool value);
	void addKey(std::string key, Eigen::Vector4d value);




private:
	xmlpp::Document document; /**< xml document*/
	std::string fileName; /**< fileName to save the xml file*/

	void splitString(std::string value, std::vector<std::string>& array);
	bool elementExists(std::string key);
	xmlpp::Element* lookForKeyAndMakeway(std::string key);


};

} /* namespace Tracker3DPT */

#endif /* XMLWRITERPARSER_H_ */
