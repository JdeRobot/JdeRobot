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


class CMDVel:

    def __init__(self, ic, prefix):
        self.lock = threading.Lock()
        prop = ic.getProperties()

        self.cmd=jderobot.CMDVelData()
        self.cmd.linearX=self.cmd.linearY=self.cmd.linearZ=0
        self.cmd.angularZ=0
        ''' With values distinct to 0 in the next fields, the ardrone not enter in hover mode'''
        self.cmd.angularX=0.0
        self.cmd.angularY=0.0


        maxlinx = prop.getProperty(prefix+".Xmax")
        if maxlinx:
            self.MAX_LINX = float(maxlinx)
        else:
            self.MAX_LINX = 0.1
            print (prefix+".Xmax not provided, the default value is used: "+ repr(self.MAX_LINX))

        maxliny = prop.getProperty(prefix+".Ymax")
        if maxliny:
            self.MAX_LINY = float(maxliny)
        else:
            self.MAX_LINY = 0.3
            print (prefix+".Ymax not provided, the default value is used: "+ repr(self.MAX_LINY))

        maxlinz = prop.getProperty(prefix+".Zmax")
        if maxlinz:
            self.MAX_LINZ = float(maxlinz)
        else:
            self.MAX_LINZ = 0.3
            print (prefix+".Zmax not provided, the default value is used: "+ repr(self.MAX_LINZ))

        maxangz = prop.getProperty(prefix+".Yawmax")
        if maxangz:
            self.MAX_ANGZ = float(maxangz)
        else:
            self.MAX_ANGZ = 0.4
            print (prefix+".Yawmax not provided, the default value is used: "+ repr(self.MAX_ANGZ))


        try:
            base = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.CMDVelPrx.checkedCast(base)
            prop = ic.getProperties()

            if not self.proxy:
                print ('Interface ' + prefix + ' not configured')

        except Ice.ConnectionRefusedException:
            print(prefix + ': connection refused')

        except:
            traceback.print_exc()
            exit(-1)


    def setVX(self,vx):
        self.lock.acquire()
        self.cmd.linearX=vx
        self.lock.release()

    def setVY(self,vy):
        self.lock.acquire()
        self.cmd.linearY=vy
        self.lock.release()

    def setVZ(self,vz):
        self.lock.acquire()
        self.cmd.linearZ=vz
        self.lock.release()

    def setAngularZ(self,az):
        self.lock.acquire()
        self.cmd.angularZ=az
        self.lock.release()

    def setAngularX(self,ax):
        self.lock.acquire()
        self.cmd.angularX=ax
        self.lock.release()
        
    def setAngularY(self,ay):
        self.lock.acquire()
        self.cmd.angularY=ay
        self.lock.release()

    def setYaw(self,yaw):
       self.setAngularZ(yaw)

    def setRoll(self,roll):
        self.angularX(roll)
        
    def setPitch(self,pitch):
        self.angularY(pitch)

    def setCMD (self, cmd):
        self.lock.acquire()
        self.cmd = cmd
        self.lock.release()

    def sendVelocities(self):
        if self.hasproxy():
            self.lock.acquire()
            tmp = self.cmd
            self.lock.release()
            self.sendCMDVel(tmp.linearX,tmp.linearY,tmp.linearZ,tmp.angularX, tmp.angularY, tmp.angularZ)

    def sendCMDVel(self,vx,vy,vz,ax,ay,az):
        cmd=jderobot.CMDVelData()
        if abs(vx) > self.MAX_LINX:
            if vx > 0:
                cmd.linearX = self.MAX_LINX
            else:
                cmd.linearX = -self.MAX_LINX
        else:
            cmd.linearX = vx

        if abs(vy) > self.MAX_LINY:
            if vy > 0:
                cmd.linearY = self.MAX_LINY
            else:
                cmd.linearY = -self.MAX_LINY
        else:
            cmd.linearY = vy

        if abs(vz) > self.MAX_LINZ:
            if vz > 0:
                cmd.linearZ = self.MAX_LINZ
            else:
                cmd.linearZ = -self.MAX_LINZ
        else:
            cmd.linearZ = vz

        if abs(az) > self.MAX_ANGZ:
            if az > 0:
                cmd.angularZ = self.MAX_ANGZ
            else:
                cmd.angularZ = -self.MAX_ANGZ
        else:
            cmd.angularZ = az

        cmd.angularX = ax
        cmd.angularY = ay

        if self.hasproxy():
            self.lock.acquire()
            self.proxy.setCMDVelData(cmd)
            self.lock.release()

    def hasproxy (self):
        return hasattr(self,"proxy") and self.proxy