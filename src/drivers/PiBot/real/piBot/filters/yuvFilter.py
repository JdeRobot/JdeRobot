
from threading import Lock

import cv2
import numpy as np

'''Max Values supported by OpenCV'''
YUVMAX = (255,255,255)
YUVMIN = (0,0,0)



class YuvFilter:

    def __init__(self):

        self.lock = Lock()

        self.MAX = YUVMAX
        self.MIN = YUVMIN

        self.uLimit = list(YUVMAX)
        self.dLimit = list(YUVMIN)

    def getName(self):
        return 'YUV'

    def setUpLimit (self, y, u, v):
        self.lock.acquire()
        self.uLimit = [y,u,v]
        self.lock.release()


    def getUpLimit (self):
        self.lock.acquire()
        lim = self.uLimit
        self.lock.release()
        return lim

    def setDownLimit(self, y, u, v):
        self.lock.acquire()
        self.dLimit = [y,u,v]
        self.lock.release()

    def getDownLimit(self):
        self.lock.acquire()
        lim = self.dLimit
        self.lock.release()
        return lim

    def getMAX(self):
        return self.MAX

    def getMIN(self):
        return self.MIN

    def apply (self, img):

        
        yup,uup,vup = self.getUpLimit()
        ydwn,udwn,vdwn = self.getDownLimit()

        ''' We convert RGB as BGR because OpenCV 
        with RGB pass to YVU instead of YUV'''

        yuv = cv2.cvtColor(img, cv2.COLOR_BGR2YUV)
        
        minValues = np.array([ydwn,udwn,vdwn],dtype=np.uint8)
        maxValues = np.array([yup,uup,vup], dtype=np.uint8)

        mask = cv2.inRange(yuv, minValues, maxValues)

        res = cv2.bitwise_and(img,img, mask= mask)


        return res