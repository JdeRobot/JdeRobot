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
import sys
import traceback
import easyiceconfig as EasyIce
import jderobot
import threading


class Motors:

    def __init__(self):
        self.lock = threading.Lock()
        self.v = self.w = 0
        try:
            ic = EasyIce.initialize(sys.argv)
            base = ic.propertyToProxy("basic_component.Motors.Proxy")
            self.proxy = jderobot.MotorsPrx.checkedCast(base)

            if not self.proxy:
                print ('Interface Motors not connected')

        except:
            traceback.print_exc()
            exit()

    def setV(self, v):
        self.v = v

    def setW(self, w):
        self.w = w

    def sendVelocities(self):
        if self.proxy:
            self.sendV(self.v)
            self.sendW(self.w)

    def sendV(self, v):
        if self.proxy:
            self.lock.acquire()
            self.proxy.setV(v)
            self.lock.release()

    def sendW(self, w):
        if self.proxy:
            self.lock.acquire()
            self.proxy.setW(w)
            self.lock.release()