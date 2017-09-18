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
import threading
import Ice
from .threadSensor import ThreadSensor
from jderobotTypes import CMDVel


class MotorsIceClient:

    def __init__(self, ic, prefix):
        self.lock = threading.Lock()
        prop = ic.getProperties()
        
        maxWstr = prop.getProperty(prefix+".maxW")
        if maxWstr:
            self.maxW = float(maxWstr)
        else:
            self.maxW = 0.5
            print (prefix+".maxW not provided, the default value is used: "+ repr(self.maxW))
                

        maxVstr = prop.getProperty(prefix+".maxV")
        if maxWstr:
            self.maxV = float(maxVstr)
        else:
            self.maxV = 5
            print (prefix+".maxV not provided, the default value is used: "+ repr(self.maxV))

        try:
            base = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.MotorsPrx.checkedCast(base)


            if not self.proxy:
                print ('Interface ' + prefix + ' not configured')

        except Ice.ConnectionRefusedException:
            print(prefix + ': connection refused')

        except:
            traceback.print_exc()
            exit(-1)


    def start(self):
        pass
    def stop(self):
        pass
        
    def sendVelocities(self, vel):
        if self.hasproxy():
            self.lock.acquire()
            self.proxy.setV(vel.vx)
            self.proxy.setW(vel.az)
            self.proxy.setL(vel.vy)
            self.lock.release()

    def getMaxW(self):
        return self.maxW

    def getMaxV(self):
        return self.maxV

    def sendV(self, v):
        self.sendVX(v)

    def sendL(self, l):
        self.sendVY(l)

    def sendW(self, w):
        self.sendAZ(w)

    def sendVX(self, vx):
        if self.hasproxy():
            self.lock.acquire()
            self.proxy.setV(vx)
            self.lock.release()

    def sendVY(self, vy):
        if self.hasproxy():
            self.lock.acquire()
            self.proxy.setL(vy)
            self.lock.release()

    def sendAZ(self, az):
        if self.hasproxy():
            self.lock.acquire()
            self.proxy.setW(az)
            self.lock.release()

    def hasproxy (self):
        return hasattr(self,"proxy") and self.proxy