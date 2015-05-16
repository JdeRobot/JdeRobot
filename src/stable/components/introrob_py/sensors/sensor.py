#
#  Copyright (C) 1997-2015 JDE Developers Team
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
#
import sys, traceback, Ice
import jderobot
import numpy as np
import threading  

class Sensor:
    def __init__(self):
        self.lock = threading.Lock()

        self.playButton=False

        self.cmd=jderobot.CMDVelData()
        self.cmd.linearX=self.cmd.linearY=self.cmd.linearZ=0
        self.cmd.angularZ=0
        ''' With values distinct to 0 in the next fields, the ardrone not enter in hover mode'''
        self.cmd.angularX=0.5
        self.cmd.angularY=1.0


        try:

            ic = Ice.initialize(sys.argv)
            properties = ic.getProperties()
            basecamera = ic.propertyToProxy("Introrob.Camera.Proxy")
            self.cameraProxy = jderobot.CameraPrx.checkedCast(basecamera)
            if not self.cameraProxy:
                raise RuntimeError("Camera -> invalid proxy")

            self.image = self.cameraProxy.getImageData("RGB8")
            self.height= self.image.description.height
            self.width = self.image.description.width

            basecmdVel = ic.propertyToProxy("Introrob.CMDVel.Proxy")
            self.cmdVelProxy=jderobot.CMDVelPrx.checkedCast(basecmdVel)
            if not self.cmdVelProxy:
                raise RuntimeError("CMDVel -> invalid proxy")

            basenavdata = ic.propertyToProxy("Introrob.Navdata.Proxy")
            self.navdataProxy = jderobot.NavdataPrx.checkedCast(basenavdata)
            if not self.navdataProxy:
                raise RuntimeError("Navdata -> invalid proxy")
            self.navdata=self.navdataProxy.getNavdata()

            if self.navdata.vehicle == 10 :
                self.virtualDrone = True
            else:
                self.virtualDrone = False

            baseextra = ic.propertyToProxy("Introrob.Extra.Proxy")
            self.extraProxy=jderobot.ArDroneExtraPrx.checkedCast(baseextra)
            if not self.extraProxy:
                raise RuntimeError("CMDVel -> invalid proxy")

            basepose3D = ic.propertyToProxy("Introrob.Pose3D.Proxy")
            self.pose3DProxy=jderobot.Pose3DPrx.checkedCast(basepose3D)
            if not self.pose3DProxy:
                raise RuntimeError("Pose3D -> invalid proxy")
            self.pose=jderobot.Pose3DData()
        except:
            traceback.print_exc()
            status = 1
            
    def update(self):
        self.lock.acquire()
        self.image = self.cameraProxy.getImageData("RGB8")
        self.height= self.image.description.height
        self.width = self.image.description.width
        self.navdata=self.navdataProxy.getNavdata()
        self.pose=self.pose3DProxy.getPose3DData()
        self.lock.release()
    
    def getNavdata(self):
        self.lock.acquire()
        tmp=self.navdata
        self.lock.release()
        return tmp
    
    def getImage(self):
        self.lock.acquire()
        img = np.zeros((self.height, self.width, 3), np.uint8)
        img = np.frombuffer(self.image.pixelData, dtype=np.uint8)
        img.shape = self.height, self.width, 3
        self.lock.release()
        return img;
    
    def takeoff(self):
        self.lock.acquire()
        self.extraProxy.takeoff()
        self.lock.release()
        
    def land(self):
        self.lock.acquire()
        self.extraProxy.land()
        self.lock.release()
        
    def toggleCam(self):
        self.lock.acquire()
        self.extraProxy.toggleCam()
        self.lock.release()
              
    def reset(self):
        self.lock.acquire()
        self.extraProxy.reset()  
        self.lock.release()
        
    def record(self,record):
        print 'record'
        if self.navdata.vehicle == 1:
            self.extraProxy.recordOnUsb(record)

    def setVX(self,vx):
        self.cmd.linearX=vx

    def setVY(self,vy):
        self.cmd.linearY=vy
        
    def setVZ(self,vz):
        self.cmd.linearZ=vz

    def setYaw(self,yaw):
        self.cmd.angularZ=yaw        
        
    def setRoll(self,roll):
        self.cmd.angularX=roll 
        
    def setPitch(self,pitch):
        self.cmd.angularY=pitch                 
                
    def Velocities(self):
        self.lock.acquire();
        self.cmdVelProxy.setCMDVelData(self.cmd)
        self.lock.release();  

    def sendVelocities(self):
        self.lock.acquire();
        self.cmdVelProxy.setCMDVelData(self.cmd)
        self.lock.release();

    def CMDVel(self,vx,vy,vz,yaw,roll,pitch):
        cmd=jderobot.CMDVelData()
        cmd.linearX=vy
        cmd.linearY=vx
        cmd.linearZ=vz
        cmd.angularZ=yaw
        cmd.angularX=cmd.angularY=1.0
        self.lock.acquire();
        self.cmdVelProxy.setCMDVelData(cmd)
        self.lock.release();
    
    def getPose3D(self):
        self.lock.acquire()
        tmp=self.pose
        self.lock.release()
        return self.pose
    
    def isPlayButton(self):
        return self.playButton
    
    def setPlayButton(self,value):
        self.playButton=value

    def isVirtual(self):
        return self.virtualDrone
