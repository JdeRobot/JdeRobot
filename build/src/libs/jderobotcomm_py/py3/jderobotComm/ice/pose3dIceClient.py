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
from math import asin, atan2, pi
from jderobotTypes import Pose3d


class Pose3D:
    '''
        Pose3d Connector. Recives Pose3d from Ice interface when you run update method.
    '''

    def __init__(self, ic, prefix):
        '''
        Pose3d Contructor.
        Exits When it receives a Exception diferent to Ice.ConnectionRefusedException

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file

        @type ic: Ice Communicator
        @type prefix: String
        '''
        self.lock = threading.Lock()

        self.pose = Pose3d()

        try:
            base = ic.propertyToProxy(prefix+".Proxy")
            self.proxy = jderobot.Pose3DPrx.checkedCast(base)
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
        Updates Pose3d.
        '''
        pos = Pose3d()
        if self.hasproxy():
            pose = self.proxy.getPose3DData()
            pos.yaw = self.quat2Yaw(pose.q0, pose.q1, pose.q2, pose.q3)
            pos.pitch = self.quat2Pitch(pose.q0, pose.q1, pose.q2, pose.q3)
            pos.roll = self.quat2Roll(pose.q0, pose.q1, pose.q2, pose.q3)
            pos.x = pose.x
            pos.y = pose.y
            pos.z = pose.z 
            pos.h = pose.h
            pos.q = [pose.q0, pose.q1, pose.q2, pose.q3]

        self.lock.acquire()
        self.pose = pos
        self.lock.release()

    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''
        return hasattr(self,"proxy") and self.proxy

    def getPose3d(self):
        '''
        Returns last Pose3d. 

        @return last JdeRobotTypes Pose3d saved

        '''	   
        self.lock.acquire()
        pose = self.pose
        self.lock.release()
        return pose


    def quat2Yaw(self, qw, qx, qy, qz):
        '''
        Translates from Quaternion to Yaw. 

        @param qw,qx,qy,qz: Quaternion values

        @type qw,qx,qy,qz: float

        @return Yaw value translated from Quaternion

        '''
        rotateZa0=2.0*(qx*qy + qw*qz)
        rotateZa1=qw*qw + qx*qx - qy*qy - qz*qz
        rotateZ=0.0
        if(rotateZa0 != 0.0 and rotateZa1 != 0.0):
            rotateZ=atan2(rotateZa0,rotateZa1)
        return rotateZ

    def quat2Pitch(self, qw, qx, qy, qz):
        '''
        Translates from Quaternion to Pitch. 

        @param qw,qx,qy,qz: Quaternion values

        @type qw,qx,qy,qz: float

        @return Pitch value translated from Quaternion

        '''
        rotateYa0=-2.0*(qx*qz - qw*qy)
        rotateY=0.0
        if(rotateYa0 >= 1.0):
            rotateY = pi/2.0
        elif(rotateYa0 <= -1.0):
            rotateY = -pi/2.0
        else:
            rotateY = asin(rotateYa0)

        return rotateY

    def quat2Roll (self, qw, qx, qy, qz):
        '''
        Translates from Quaternion to Roll. 

        @param qw,qx,qy,qz: Quaternion values

        @type qw,qx,qy,qz: float

        @return Roll value translated from Quaternion

        '''
        rotateXa0=2.0*(qy*qz + qw*qx)
        rotateXa1=qw*qw - qx*qx - qy*qy + qz*qz
        rotateX=0.0

        if(rotateXa0 != 0.0 and rotateXa1 != 0.0):
            rotateX=atan2(rotateXa0, rotateXa1)
        return rotateX





class Pose3dIceClient:
    '''
        Pose3d Ice Client. Recives Pose3d from Ice interface running Pose3d update method in a thread.
    '''
    def __init__(self,ic,prefix, start = False):
        '''
        Pose3dIceClient Contructor.

        @param ic: Ice Communicator
        @param prefix: prefix name of client in config file
        @param start: indicates if start automatically the client

        @type ic: Ice Communicator
        @type prefix: String
        @type start: Boolean
        '''
        self.pose3d = Pose3D(ic,prefix)

        self.kill_event = threading.Event()
        self.thread = ThreadSensor(self.pose3d, self.kill_event)
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

    def getPose3d(self):
        '''
        Returns last Pose3d. 

        @return last JdeRobotTypes Pose3d saved

        '''
        return self.pose3d.getPose3d()

    def hasproxy (self):
        '''
        Returns if proxy has ben created or not. 

        @return if proxy has ben created or not (Boolean)

        '''
        return self.pose3d.hasproxy()
