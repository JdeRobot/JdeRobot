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

#ifndef PARAMDICT_JDEROBOTUTIL_H
#define PARAMDICT_JDEROBOTUTIL_H

#include <map>
#include <string>

namespace jderobotutil{
  /**
   * A dictionary to keep key indexed parameters.
   *
   * Parameters and keys are std::string but they may be evaluated as int and float. 
   * A prefix key may be supplied in the constructor so param search would be done using prefix+key.
   */
  class ParamDict: public std::map<std::string, std::string>{
  public:
    /**
     * Constructor
     * \param keyprefix is a prefix used in each search
     * \param params initialization values
     */
    ParamDict(const std::string keyprefix = "", 
	      const std::map<std::string, std::string>& params = std::map<std::string, std::string>::map());
    
    /**
     * Get param indexed by paramkey or empty string
     */
    std::string getParam(const std::string paramkey) const;

    /**
     * Get param indexed by paramkey or defaultValue
     */
    std::string getParamWithDefault(const std::string paramkey, const std::string defaultValue = "") const;

    /**
     * Get param indexed by paramkey as an integer or 0
     */
    int getParamAsInt(const std::string paramkey) const;

    /**
     * Get param indexed by paramkey as an integer or defaultValue
     */
    int getParamAsIntWithDefault(const std::string paramkey, const int defaultValue = 0) const;

    /**
     * Get param indexed by paramkey as a float or 0.0
     */
    float getParamAsFloat(const std::string paramkey) const;

    /**
     * Get param indexed by paramkey as a float or defaultValue
     */
    float getParamAsFloatWithDefault(const std::string paramkey, const float defaultValue = 0.0) const;
    
    /**
     * Get all params for prefix. Prefix is appended to keyprefix
     */
    ParamDict getParamsForPrefix(const std::string prefix) const;

    /**
     * Convert ParamDict to a std::string
     */
    std::string toString() const;
    
    /**
     * Get keyprefix
     */
    const std::string& getKeyPrefix() const { return keyprefix; }

  private:
    std::string keyprefix;
  };

}//namespace

/**
 * output operator (<<) that allows to insert a ParamDict instance to an output stream
 */
std::ostream &operator<<(std::ostream &out, const jderobotutil::ParamDict& param);

/**
 * input operator (>>) that allows to get values for a ParamDict instance from an input stream
 */
std::istream &operator>>(std::istream &in, jderobotutil::ParamDict& param);


#endif //PARAMDICT_JDEROBOTUTIL_H
