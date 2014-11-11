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


/*! \file XmlParser.h
    \brief generic typed xml parser based on libxml++
*/

#ifndef XMLREADERPARSER_H_
#define XMLREADERPARSER_H_

#include <libxml++/libxml++.h>
#include <iostream>
#include <cv.h>
#include <eigen3/Eigen/Dense>

namespace Tracker3DPT {

class xmlReaderParser {
public:
	xmlReaderParser(std::string path);
	virtual ~xmlReaderParser();
	bool parse(std::string key, int& value);
	bool parse(std::string key, double& value);
	bool parse(std::string key, float& value);
	bool parse(std::string key, bool& value);
	bool parse(std::string key, cv::Point3f& value);
	//bool parse(std::string key, Eigen::Vector4d& value);
	bool parse(std::string key, bool active, Eigen::Vector4d& pos , Eigen::Vector4d& size, Eigen::Vector4d& dir, int index);
	bool getKeyNumbers(std::string key, int& numbers);




private:
	xmlpp::DomParser parser;
};

} /* namespace Tracker3DPT */

#endif /* XMLREADERPARSER_H_ */
