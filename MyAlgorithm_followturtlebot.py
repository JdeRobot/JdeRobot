import threading
import time
from datetime import datetime
import cv2
import numpy as np

from sensors.cameraFilter import CameraFilter
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

        self.stop_event = threading.Event()
        self.kill_event = threading.Event()
        self.lock = threading.Lock()
        threading.Thread.__init__(self, args=self.stop_event)

        self.PID_X = PID()
        self.PID_Y = PID()

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
        position_error_x,position_error_y = self.ImageProcessing()

        if (position_error_x != None or position_error_y != None):
               
            PIDX = self.PID_X.GenOut(position_error_x)
            PIDY = self.PID_Y.GenOut(position_error_y)
            print('X Axis Error: ' + str(position_error_x) + ', velocity=' + str(PIDX))
            print('Y Axis Error: ' + str(position_error_y) + ', velocity=' + str(PIDY))
                    
            self.cmdvel.sendCMDVel(PIDX,PIDY,0,0,0,0)

            if PIDX == 0 and PIDY == 0:
                print('TURTEBLOT REACHED')
                print('Position: x=' + str(self.pose.getPose3D().x) + ', y=' + str(self.pose.getPose3D().y))

        else:
            # The drone stops...
            print("TurtleBot is not visible. Waiting until it appears...")
            self.cmdvel.sendCMDVel(0,0,0,0,0,0)

    def ImageProcessing(self):
        input_image = self.camera.getImage()
        if input_image is not None:
            # Camera's image processing
            self.camera.setColorImage(input_image)  
            smooth_image = cv2.GaussianBlur(input_image,(5,5),0)
            HSV_smooth_image = cv2.cvtColor(smooth_image, cv2.COLOR_RGB2HSV)
            lower_boundary = np.array([55,190,210], dtype = "uint8")
            upper_boundary = np.array([65,200,255], dtype = "uint8")
            mask = cv2.inRange(HSV_smooth_image,lower_boundary,upper_boundary)
            self.camera.setThresoldImage(mask)
            input_image_copy = input_image

            mask_copy = np.copy(mask)
            im2, contours, hierarchy = cv2.findContours(mask_copy,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
            if contours != []: 
                contour = sorted(contours, key = cv2.contourArea, reverse = True)[0]
                x,y,w,h = cv2.boundingRect(contour)
                rectangle = cv2.rectangle(input_image_copy, (x,y), (x+w,y+h),(255,0,0),2)
                self.camera.setColorImage(rectangle)

                # Center compute
                x_p = round(x + 0.5*w)
                y_p = round(y + 0.5*h)

                # Center drawing
                center = cv2.rectangle(input_image_copy, (x_p,y_p), (x_p+2, y_p+2),(255,0,0), 2)
                self.camera.setColorImage(center)
                y = -(x_p-160)/320                   # Pixels position error compute 
                x = -(y_p-120)/240                   # (Assuming: drone->(120,160))
                # Drone's x and y values in the world extrapolated from their values in the image.
                print("TurtleBot detected. Distance = " + str(x) + "m(x), " + str(y) + "m(y)")
                return x,y
            else:
                # In case no contour is detected
                return None,None

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

        self.minError = 0.15
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
        if abs(u) <= self.minError:
            return 0
        elif u < (-1*self.minError):
            return -0.5
        elif u > self.minError:
            return 0.5
                
    






