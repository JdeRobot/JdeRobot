#
#  Copyright (C) 1997-2017 JDE Developers Team
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see http://www.gnu.org/licenses/.
#  Authors :
#       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
#
__author__ = 'aitormf'

import sys, os
import yaml
from .properties import Properties


def findConfigFile(filename):
    '''
    Returns filePath or None if it couldn't find the file

    @param filename: Name of the file

    @type filename: String

    @return String with path or None

    '''
    paths = "."
    config_paths = os.getenv("JDEROBOT_CONFIG_PATHS")
    if config_paths:
        paths = paths+":"+config_paths

    for path in paths.split(":"):
        file_path = os.path.join(path, filename)
        if os.path.exists(file_path):
            return file_path

    return None


def load(filename):
    '''
    Returns the configuration as dict

    @param filename: Name of the file

    @type filename: String

    @return a dict with propierties reader from file

    '''
    filepath = findConfigFile(filename)
    prop= None
    if (filepath):
        print ('loading Config file %s' %(filepath))

        with open(filepath, 'r') as stream:
            cfg=yaml.load(stream)
            prop = Properties(cfg) 
    else:
        msg = "Ice.Config file '%s' could not being found" % (filename)
        raise ValueError(msg)

    return prop