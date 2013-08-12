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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include "paramdict.h"
#include <sstream>

namespace jderobotutil{
  ParamDict::ParamDict(const std::string keyprefix, 
		       const std::map<std::string, std::string>& params)
    : std::map<std::string, std::string>(params), keyprefix(keyprefix) {}
  
  std::string ParamDict::getParam(const std::string paramkey) const{
    return getParamWithDefault(paramkey,std::string());
  }
  
  std::string ParamDict::getParamWithDefault(const std::string paramkey, std::string defaultValue) const{
    ParamDict::const_iterator pIt(this->find(keyprefix+paramkey));
    if (pIt != this->end())
      return pIt->second;
    return defaultValue;
  }
  
  int ParamDict::getParamAsInt(const std::string paramkey) const{
    return getParamAsIntWithDefault(paramkey,0);
  }
  
  int ParamDict::getParamAsIntWithDefault(const std::string paramkey, const int defaultValue) const{
    std::string p(getParam(paramkey));
    if (p.length() == 0)
      return defaultValue;
    
    std::istringstream iss(p);
    int value;
    iss >> value;
    if (iss.fail())
      return defaultValue;
    return value;
  }
  
  float ParamDict::getParamAsFloat(const std::string paramkey) const{
    return getParamAsFloatWithDefault(paramkey,0.0f);
  }
  
  float ParamDict::getParamAsFloatWithDefault(const std::string paramkey, const float defaultValue) const{
    std::string p(getParam(paramkey));
    if (p.length() == 0)
      return defaultValue;
    
    std::istringstream iss(p);
    float value;
    iss >> value;
    if (iss.fail())
      return defaultValue;
    return value;
  }

  ParamDict ParamDict::getParamsForPrefix(const std::string prefix) const{
    ParamDict paramsForPrefix(keyprefix+prefix);
    ParamDict::const_iterator pIt;

    for (pIt = this->begin();
	 pIt != this->end();
	 pIt++){
      if (pIt->first.find(keyprefix+prefix) == 0) //startswith
	paramsForPrefix.insert(ParamDict::value_type(pIt->first,pIt->second));
    }
    return paramsForPrefix;
  }
  
  std::string ParamDict::toString() const{
    std::stringstream ss;
    ss << *this;
    return ss.str();
  } 
}

std::ostream &operator<<(std::ostream &out, const jderobotutil::ParamDict& param){
  jderobotutil::ParamDict::const_iterator pIt;
  std::string key;
  int keyprefixlen = param.getKeyPrefix().length();
  
  for( pIt = param.begin(); pIt != param.end(); pIt++){
    key = pIt->first;
    if ((keyprefixlen > 0) && (key.find(param.getKeyPrefix()) == 0))//erase keyprefix
      key = key.substr(keyprefixlen);
    out << key << '=' << pIt->second << std::endl;
  }
  return out;
}

std::istream &operator>>(std::istream &in, jderobotutil::ParamDict& param){
  std::string line;
  while(!in.eof()){
    std::getline(in,line);
    //if data read check for comments
    if (line.length() > 0){
      int commentpos = line.find_first_of('#');
      if (commentpos != std::string::npos)
	line = line.erase(commentpos);
    }

    //split line by =
    if (line.length() > 0){
      int delpos = line.find_first_of('=');
      if (delpos != std::string::npos){
	std::string key,value;
	key = line.substr(0,delpos);
	value = line.substr(delpos+1);
	param[key] = value;
      }
    }
  }
  return in;
}
