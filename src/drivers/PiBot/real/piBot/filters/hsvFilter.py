
from threading import Lock
import cv2
import numpy as np

'''Max Values supported by OpenCV'''
HSVMAX = (179,255,255)
HSVMIN = (0,0,0)


class HsvFilter:

    def __init__(self):

        self.lock = Lock()

        self.MAX = HSVMAX
        self.MIN = HSVMIN

        self.uLimit = list(HSVMAX)
        self.dLimit = list(HSVMIN)

    def getName(self):
        return 'HSV'

    def setUpLimit (self, h, s, v):
        self.lock.acquire()
        self.uLimit = [h,s,v]
        self.lock.release()


    def getUpLimit (self):
        self.lock.acquire()
        lim = self.uLimit
        self.lock.release()
        return lim

    def setDownLimit(self, h, s, v):
        self.lock.acquire()
        self.dLimit = [h,s,v]
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

        hup,sup,vup = self.getUpLimit()
        hdwn,sdwn,vdwn = self.getDownLimit()

        hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

        # http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html
        minValues = np.array([hdwn,sdwn,vdwn],dtype=np.uint8)
        maxValues = np.array([hup,sup,vup], dtype=np.uint8)

        mask = cv2.inRange(hsv, minValues, maxValues)

        res = cv2.bitwise_and(img,img, mask= mask)


        return res