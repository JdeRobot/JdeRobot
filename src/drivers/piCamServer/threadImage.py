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
#
import threading
import time
from datetime import datetime
import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera

class ThreadImage(threading.Thread):

    def __init__(self, camera, fps, uri):
        self.camera = camera
	self.uri = uri
        self._kill_event = threading.Event()
        self._image = None
        self._time_cycle = 1000/fps #time_cycle in ms
        self._lock = threading.Lock()
        threading.Thread.__init__(self, args=self._kill_event)
        self._kill_event.clear()

        self.__read_image()

    def run(self):
        while (not self._kill_event.is_set()):
            start_time = datetime.now()

            self.__read_image()

            finish_Time = datetime.now()

            dt = finish_Time - start_time
            ms = (dt.days * 24 * 60 * 60 + dt.seconds) * 1000 + dt.microseconds / 1000.0
            #print (ms)
            if (ms < self._time_cycle):
                time.sleep((self._time_cycle - ms) / 1000.0)

    def __read_image(self):
	if self.uri == 2:
		rawCapture = PiRGBArray(self.camera, size=(640, 480))
		self.camera.capture(rawCapture, format="rgb")
		imgRGB = rawCapture.array
	else:
		retval, img = self.camera.read()
		imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        
	self._lock.acquire()
	self._image = imgRGB
	self._lock.release()

    def get_image(self, format="RGB"):
        self._lock.acquire()
        img = self._image
        self._lock.release()
        return img

    
    def stop(self):
        '''
        Stops the client. If client is stopped you can not start again, Threading.Thread raised error

        '''
        self._kill_event.set()

