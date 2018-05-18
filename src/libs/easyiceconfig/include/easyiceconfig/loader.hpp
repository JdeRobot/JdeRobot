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
 *       Victor Arribas Raigadas <.varribas.urjc@gmail.com>
 */

#ifndef EASYICECONFIG_LOADER_H
#define EASYICECONFIG_LOADER_H


#include <string>
#include <easyiceconfig/stdutils.hpp>

#include <Ice/Communicator.h>
#include <Ice/Properties.h>
#include <Ice/Initialize.h>
#include <Ice/LocalException.h>


namespace easyiceconfig {
namespace loader {


const std::string ENV_PATH_NAME = "ICE_CONFIG_PATH";

const std::string CONFIG_PATH_NAME = "JDEROBOT_CONFIG_PATHS";



/**
 * @brief Find filename into all defined search paths.
 * Order is:
 * 1. current dir
 * 2. $ICE_CONFIG_PATH
 * 3. jderobot paths (*)
 *
 * @return empty if file was not found.
 */
std::string findConfigFile(const std::string& filename);

/**
 * @brief Loads Ice configuration from passed file.
 *
 * @return new Ice::Properties or passed one.
 */
Ice::PropertiesPtr loadIceConfig(std::string filename,
                            Ice::PropertiesPtr properties = Ice::createProperties());


/**
 * @brief Perform a full initialize of Ice::Properties just as Ice does it.
 * @see Ice::createProperties
 * Additionally, parses non `--Ice.A=B` command line arguments.
 * Implies `loadIceConfig` + parseCommandLineOptions
 *
* @return new Ice::Properties or passed one.
 */
Ice::PropertiesPtr initializeProperties(Ice::StringSeq args,
                            Ice::PropertiesPtr properties = Ice::createProperties());


}}//NS


#endif // EASYICECONFIG_LOADER_H
