#
#  Copyright (C) 1997-2016 JDE Developers Team
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
#       Victor Arribas Raigadas <v.arribas.urjc@gmail.com>
#
__author__ = 'varribas'

import os
import platform

if ('Windows')==platform.system():
    JDEROBOT_PATHS = ""
    JDEROBOT_GAZEBO_PLUGINS_PATHS = ""
else:
    # JdeRobot main Ice.Config path
    JDEROBOT_PATHS = "/usr/local/share/jderobot/conf"


    # JdeRobot Gazebo's plugins Ice.Config path
    JDEROBOT_GAZEBO_PLUGINS_BASE_PATH = '/usr/local/share/jderobot/gazebo/plugins'

    gazebo_plugins = list()
    for dir in os.listdir(JDEROBOT_GAZEBO_PLUGINS_BASE_PATH):
        plugin_dir = os.path.join(JDEROBOT_GAZEBO_PLUGINS_BASE_PATH, dir)
        gazebo_plugins.append(plugin_dir)

    JDEROBOT_GAZEBO_PLUGINS_PATHS = ':'.join(str(x) for x in gazebo_plugins)


# "Hardcoded" PATHS
HARDCODED_PATHS = "%s:%s" %(JDEROBOT_PATHS, JDEROBOT_GAZEBO_PLUGINS_PATHS)
