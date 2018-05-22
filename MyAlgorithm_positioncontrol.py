import threading
import time
from datetime import datetime

import math
import jderobot
from Beacon import Beacon

from parallelIce.cameraClient import CameraClient
from parallelIce.navDataClient import NavDataClient
from parallelIce.cmdvel import CMDVel
from parallelIce.extra import Extra
from parallelIce.pose3dClient import Pose3DClient

time_cycle = 80

class MyAlgorithm(threading.Thread):

    def __init__(self, camera, navdata, pose, cmdvel, extra):
        self.camera = camera
        self.navdata = navdata
        self.pose = pose
        self.cmdvel = cmdvel
        self.extra = extra

        self.beacons=[]
        self.initBeacons()
        self.minError=0.01

        self.stop_event = threading.Event()
        self.kill_event = threading.Event()
        self.lock = threading.Lock()
        threading.Thread.__init__(self, args=self.stop_event)

        self.PID_X = PID()
        self.PID_Y = PID()

    def initBeacons(self):
        self.beacons.append(Beacon('baliza1',jderobot.Pose3DData(0,5,0,0,0,0,0,0),False,False))
        self.beacons.append(Beacon('baliza2',jderobot.Pose3DData(5,0,0,0,0,0,0,0),False,False))
        self.beacons.append(Beacon('baliza3',jderobot.Pose3DData(0,-5,0,0,0,0,0,0),False,False))
        self.beacons.append(Beacon('baliza4',jderobot.Pose3DData(-5,0,0,0,0,0,0,0),False,False))
        self.beacons.append(Beacon('baliza5',jderobot.Pose3DData(10,0,0,0,0,0,0,0),False,False))
        self.beacons.append(Beacon('inicio',jderobot.Pose3DData(0,0,0,0,0,0,0,0),False,False))

    def getNextBeacon(self):
        for beacon in self.beacons:
            if beacon.isReached() == False:
                return beacon

        return None

    def run (self):

        self.stop_event.clear()

        while (not self.kill_event.is_set()):
           
            start_time = datetime.now()

            if not self.stop_event.is_set():
                self.execute()

            finish_Time = datetime.now()

            dt = finish_Time - start_time
            ms = (dt.days * 24 * 60 * 60 + dt.seconds) * 1000 + dt.microseconds / 1000.0
            #print (ms)
            if (ms < time_cycle):
                time.sleep((time_cycle - ms) / 1000.0)

    def stop (self):
        self.stop_event.set()

    def play (self):
        if self.is_alive():
            self.stop_event.clear()
        else:
            self.start()

    def kill (self):
        self.kill_event.set()


    def execute(self):
        next_beacon = self.getNextBeacon()
        for beacon in self.beacons:
            if beacon == next_beacon:
                beacon.setActive(True)
                beacon.setReached(False)

                # Position error compute
                error_x = beacon.getPose().x - self.pose.getPose3D().x
                error_y = beacon.getPose().y - self.pose.getPose3D().y
            
                # Updating PID Controller Values
                PIDX = self.PID_X.GenOut(error_x)
                PIDY = self.PID_Y.GenOut(error_y)
                print('Error eje X: ' + str(error_x) + ', velocidad=' + str(PIDX))
                print('Error eje Y: ' + str(error_y) + ', velocidad=' + str(PIDY)) 
                
                self.cmdvel.sendCMDVel(PIDX,PIDY,0,0,0,0)

                if PIDX == 0 and PIDY == 0:
                    print('ALCANZADA BALIZA' + ' x=' + str(beacon.getPose().x) + ', y=' + str(beacon.getPose().y))
                    print('Posición del drone: x=' + str(self.pose.getPose3D().x) + ', y=' + str(self.pose.getPose3D().y))
                
                    beacon.setActive(False)
                    beacon.setReached(True)

        if next_beacon == None:
            print('FINALIZADO. Todas las balizas alcanzadas.')
            # Stops when the last beacon is reached
            self.cmdvel.sendCMDVel(0,0,0,0,0,0)

class PID:

    def __init__(self, Kp = 1, Kd = 0, Ki = 0):
        # Initialze gains
        self.Kp = Kp
        self.Kd = Kd
        self.Ki = Ki
        
        self.Initialize()

    def Initialize(self):

        self.currtm = time.time()
        self.prevtm = self.currtm

        self.min_Error = 0.1
        self.prev_err = 0

        self.Cp = 0
        self.Ci = 0
        self.Cd = 0

    def GenOut(self, error):
        
        self.currtm = time.time()               # Get current time
        dt = self.currtm - self.prevtm          
        de = error - self.prev_err              # Get delta error

        self.Cp = self.Kp * error               # Set proportional term
        self.Ci += error * dt                   # Set integral term

        self.Cd = 0                             # In case dt <= 0
        if dt > 0:                              # To avoid dividing by 0
            self.Cd = de/dt                     # Set derivative term

        self.prevtm = self.currtm               # Save t value for next call
        self.prev_err = error                   # Save error value for next call

        # Sum the terms (u=e(t))
        u = self.Cp + (self.Ki * self.Ci) + (self.Kd * self.Cd)

        # Output management (drone velocity)
        if abs(u) <= self.min_Error:
            return 0
        elif u < (-1*self.min_Error):
            return -0.5
        elif u > self.min_Error:
            return 0.5

