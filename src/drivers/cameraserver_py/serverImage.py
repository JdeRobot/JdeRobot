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
#       Alberto Martin Florido <almartinflorido@gmail.com>
#       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
#		Francisco Perez Salgado <f.perez475@gmail.com>
#

import threading
import time
import cv2


class ServerImage():

    def __init__(self, uri):
        self._image = None
        self._lock = threading.Lock()
        self.uri = uri

    def read(self):
    	image = cv2.imread(self.uri)
    	self._lock.acquire()
        self._image = image
        self._lock.release()
        return 0, self._image