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
        #                         L  E  E  M  E  
        # /////////////////////////////////////////////////////////////////////
        # DESCOMENTAR LAS PARTES INDICADAS SEGÚN EL VÍDEO CON EL QUE SE QUIERA 
        # EJECUTAR EL CÓDIGO. ACTUALMENTE SE ENCUENTRA LISTO PARA EJECUTARSE
        # CON EL VÍDEO 'pelotas_azul_roja'
        # /////////////////////////////////////////////////////////////////////
        input_image = self.camera.getImage()
        if input_image is not None:
            self.camera.setColorImage(input_image)  
            smooth_image = cv2.GaussianBlur(input_image,(5,5),0)
            HSV_smooth_image = cv2.cvtColor(smooth_image, cv2.COLOR_RGB2HSV)
            lower_boundary = np.array([110,155,0], dtype = "uint8")
            upper_boundary = np.array([179,255,255], dtype = "uint8")

            # Pelotas Roja y Azul
            # -------------------
            lower_boundary1 = np.array([170,207,148], dtype = "uint8")
            upper_boundary1 = np.array([179,255,255], dtype = "uint8")
            lower_boundary2 = np.array([109,0,0], dtype = "uint8")
            upper_boundary2 = np.array([128,255,255], dtype = "uint8")
            # -------------------

            # Drone 1
            # -------------------
            #lower_boundary1 = np.array([166,100,66], dtype = "uint8")
            #upper_boundary1 = np.array([179,235,255], dtype = "uint8")
            #lower_boundary2 = np.array([100,132,90], dtype = "uint8")
            #upper_boundary2 = np.array([152,205,255], dtype = "uint8")
            # -------------------

            mask = cv2.inRange(HSV_smooth_image,lower_boundary,upper_boundary)
            mask1 = cv2.inRange(HSV_smooth_image,lower_boundary1,upper_boundary1)
            mask2 = cv2.inRange(HSV_smooth_image,lower_boundary2,upper_boundary2)
            self.camera.setThresoldImage(mask)
            input_image_copy = input_image

            mask_copy1 = np.copy(mask1)
            im2, contours, hierarchy = cv2.findContours(mask_copy1,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
            if contours != []: # En caso de que no detecte contornos
                contour = sorted(contours, key = cv2.contourArea, reverse = True)[0]
                x,y,w,h = cv2.boundingRect(contour)
                rectangle = cv2.rectangle(input_image_copy, (x,y), (x+w,y+h),(0,255,0),2)
                self.camera.setColorImage(rectangle)

                # Drone 1
                # -------------------
                #contours = sorted(contours, key = cv2.contourArea, reverse = True)
                #for contour in contours:
                #    if cv2.contourArea(contour) > 800:
                #        contours.remove(contour)
                #contours = contours[0:2]
                
                #if len(contours) == 2:
                #    x1,y1,w1,h1 = cv2.boundingRect(contours[0])
                #    x2,y2,w2,h2 = cv2.boundingRect(contours[1])
                #    rectangle1 = cv2.rectangle(input_image_copy, (x1,y1), (x1+w1,y1+h1),(0,255,0),2)
                #    rectangle2 = cv2.rectangle(input_image_copy, (x2,y2), (x2+w2,y2+h2),(0,255,0),2)
                #    self.camera.setColorImage(rectangle1)
                #    self.camera.setColorImage(rectangle2)
                #elif len(contours) == 1: 
                #    x1,y1,w1,h1 = cv2.boundingRect(contours[0])
                #    rectangle1 = cv2.rectangle(input_image_copy, (x1,y1), (x1+w1,y1+h1),(0,255,0),2) 
                #    self.camera.setColorImage(rectangle1)
                # -------------------

            mask_copy2 = np.copy(mask2)
            im2_2, contours2, hierarchy2 = cv2.findContours(mask_copy2,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
            if contours2 != []: # En caso de que no detecte contornos
                contour = sorted(contours2, key = cv2.contourArea, reverse = True)[0] 
                # Ordenamos los contornos por su área (de mayor a menor)
                x,y,w,h = cv2.boundingRect(contour)
                rectangle = cv2.rectangle(input_image_copy, (x,y), (x+w,y+h),(255,117,20),2)
                self.camera.setColorImage(rectangle)

                # Drone 1
                # -------------------
                #contours2 = sorted(contours2, key = cv2.contourArea, reverse = True)
                #i = 0
                #for contour in contours2:
                #    if cv2.contourArea(contour) < 800 and i == 0:
                #        x,y,w,h = cv2.boundingRect(contour)
                #        rectangle = cv2.rectangle(input_image_copy, (x,y), (x+w,y+h),(255,117,20),2)
                #        self.camera.setColorImage(rectangle)
                #        i += 1
                # -------------------

