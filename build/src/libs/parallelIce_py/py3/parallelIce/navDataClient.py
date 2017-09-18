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
from parallelIce.threadSensor import ThreadSensor


class NavData:

    def __init__(self, ic, prefix):
        self.lock = threading.Lock()

        try:
            base = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.NavdataPrx.checkedCast(base)
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
        if self.hasproxy():
            navData = self.proxy.getNavdata()

            self.lock.acquire()
            self.navData = navData
            self.lock.release()

    def hasproxy (self):
        return hasattr(self,"proxy") and self.proxy

    def getNavdata(self):	   
        if self.hasproxy():
            self.lock.acquire()
            navData = self.navData
            self.lock.release()
            return navData

        return None




class NavDataClient:
    def __init__(self,ic,prefix, start = False):
        self.navdata = NavData(ic,prefix)

        self.kill_event = threading.Event()
        self.thread = ThreadSensor(self.navdata, self.kill_event)
        self.thread.daemon = True

        if start:
            self.start()

    # if client is stopped you can not start again, Threading.Thread raised error
    def start(self):
        self.kill_event.clear()
        self.thread.start()

    # if client is stopped you can not start again
    def stop(self):
        self.kill_event.set()

    def getNavdata(self):
        return self.navdata.getNavdata()