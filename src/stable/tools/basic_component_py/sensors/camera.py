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
import sys
import traceback
import easyiceconfig as EasyIce
import jderobot
import numpy as np
import threading


class Camera:

    def __init__(self):
        self.lock = threading.Lock()

        try:
            ic = EasyIce.initialize(sys.argv)
            basecamera = ic.propertyToProxy("basic_component.Camera1.Proxy")
            self.cameraProxy = jderobot.CameraPrx.checkedCast(basecamera)

            if self.cameraProxy:
                self.image = self.cameraProxy.getImageData("RGB8")
                self.height = self.image.description.height
                self.width = self.image.description.width

                self.trackImage = np.zeros((self.height, self.width, 3), np.uint8)
                self.trackImage.shape = self.height, self.width, 3

                self.thresoldImage = np.zeros((self.height, self.width, 1), np.uint8)
                self.thresoldImage.shape = self.height, self.width,

            else:
                print ('Interface camera not connected')

        except:
            traceback.print_exc()
            exit()
            #status = 1

    def update(self):
        self.lock.acquire()
        self.updateCamera()
        self.lock.release()

    def updateCamera(self):
        if self.cameraProxy:
            self.image = self.cameraProxy.getImageData("RGB8")
            self.height = self.image.description.height
            self.width = self.image.description.width

    def getImage(self):
        if self.cameraProxy:
            self.lock.acquire()
            img = np.zeros((self.height, self.width, 3), np.uint8)
            img = np.frombuffer(self.image.pixelData, dtype=np.uint8)
            img.shape = self.height, self.width, 3

            self.lock.release()
            return img

        return None