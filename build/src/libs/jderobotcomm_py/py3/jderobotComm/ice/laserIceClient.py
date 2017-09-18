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
#       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
#

import traceback
import jderobot
import threading
import Ice
from .threadSensor import ThreadSensor
from jderobotTypes import LaserData


class Laser:
    '''
        Laser Connector. Recives LaserData from Ice interface when you run update method.
    '''

    def __init__(self, ic, prefix):
        '''
        Laser Contructor.
        Exits When it receives a Exception diferent to Ice.ConnectionRefusedException

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file

        @type ic: Ice Communicator
        @type prefix: String
        '''
        self.lock = threading.Lock()
        self.laser = LaserData()

        try:
            base = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.LaserPrx.checkedCast(base)
            prop = ic.getProperties()

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
        Updates LaserData.
        '''
        if self.hasproxy():
            laserD = LaserData()
            values = []
            data = self.proxy.getLaserData()

            #laserD.values = laser.distanceData
            for i in range (data.numLaser):
                values.append(data.distanceData[i] / 1000) 

            laserD.maxAngle = data.maxAngle
            laserD.minAngle = data.minAngle
            laserD.maxRange = data.maxRange
            laserD.minRange = data.minRange
            laserD.values = values


            self.lock.acquire()
            self.laser = laserD
            self.lock.release()

    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''

        return hasattr(self,"proxy") and self.proxy

    def getLaserData(self):
        '''
        Returns last LaserData. 

        @return last JdeRobotTypes LaserData saved

        '''     
        if self.hasproxy():
            self.lock.acquire()
            laser = self.laser
            self.lock.release()
            return laser

        return None




class LaserIceClient:
    '''
        Laser Ice Client. Recives LaserData from Ice interface running Laser update method in a thread.
    '''
    def __init__(self,ic,prefix, start = False):
        '''
        LaserIceClient Contructor.

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file
        @param start: indicates if start automatically the client

        @type ic: Ice Communicator
        @type prefix: String
        @type start: Boolean
        '''
        self.laser = Laser(ic,prefix)

        self.kill_event = threading.Event()
        self.thread = ThreadSensor(self.laser, self.kill_event)
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

    def getLaserData(self):
        '''
        Returns last LaserData. 

        @return last JdeRobotTypes LaserData saved

        '''
        return self.laser.getLaserData()

    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''
        return self.laser.hasproxy()