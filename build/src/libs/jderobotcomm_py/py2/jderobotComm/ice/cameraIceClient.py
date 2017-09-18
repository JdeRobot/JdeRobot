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

import traceback
import jderobot
import numpy as np
import threading
import Ice
from .threadSensor import ThreadSensor
from jderobotTypes import Image


class Camera:
    '''
        Camera Connector. Recives image from Ice interface when you run update method.
    '''

    def __init__(self, ic, prefix):
        '''
        Camera Contructor.
        Exits When it receives a Exception diferent to Ice.ConnectionRefusedException

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file

        @type ic: Ice Communicator
        @type prefix: String
        '''

        self.lock = threading.Lock()
        self.image = Image()

        try:
            basecamera = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.CameraPrx.checkedCast(basecamera)
            prop = ic.getProperties()
            self.imgFormat = prop.getProperty(prefix+".Format")
            if not self.imgFormat:
                self.imgFormat = "RGB8"

            self.update()

            if not self.proxy:
                print ('Interface ' + prefix + ' not configured')

        except Ice.ConnectionRefusedException:
            print(prefix + ': connection refused')

        except:
            traceback.print_exc()
            exit(-1)

    def update(self):
        '''
        Updates Image.
        '''
        if self.hasproxy():
            img = Image()
            imageData = self.proxy.getImageData(self.imgFormat)
            img.height = imageData.description.height
            img.width = imageData.description.width
            img.format = imageData.description.format

            img.data = np.frombuffer(imageData.pixelData, dtype=np.uint8)
            img.data.shape = img.height, img.width, 3

            img.timeStamp = imageData.timeStamp.seconds + imageData.timeStamp.useconds * 1e-9


            self.lock.acquire()
            self.image = img
            self.lock.release()

    def getImage(self):
        '''
        Returns last Image. 

        @return last JdeRobotTypes Image saved

        '''
        img = Image()      
        if self.hasproxy():
            self.lock.acquire()
            img = self.image
            self.lock.release()
        return img

    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''
        return hasattr(self,"proxy") and self.proxy




class CameraIceClient:
    '''
        Camera Ice Client. Recives image from Ice interface running camera update method in a thread.
    '''
    def __init__(self,ic,prefix, start = False):
        '''
        CameraIceClient Contructor.

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file
        @param start: indicates if start automatically the client

        @type ic: Ice Communicator
        @type prefix: String
        @type start: Boolean
        '''
        self.camera = Camera(ic,prefix)

        self.kill_event = threading.Event()
        self.thread = ThreadSensor(self.camera, self.kill_event)
        #self.thread = ThreadSensor(self.camera)
        self.thread.daemon = True

        if start:
            self.start()

    
    def start(self):
        '''
        Starts the client. If client is stopped you can not start again, Threading.Thread raised error

        '''
        self.kill_event.clear()
        self.thread.start()

    
    def stop(self):
        '''
        Stops the client. If client is stopped you can not start again, Threading.Thread raised error

        '''
        self.kill_event.set()

    def getImage(self):
        '''
        Returns last Image. 

        @return last JdeRobotTypes Image saved

        '''
        return self.camera.getImage()


    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''
        return self.camera.hasproxy()
