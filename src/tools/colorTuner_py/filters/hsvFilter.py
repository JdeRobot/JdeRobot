
from threading import Lock
import cv2
import numpy as np

'''Max Values supported by OpenCV'''
HSVMAX = (359,255,255)
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
        mask = None
        res = None

        hup=hup//2
        hdwn=hdwn//2
        if hdwn <= hup:
            # http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html
            minValues = np.array([hdwn,sdwn,vdwn],dtype=np.uint8)
            maxValues = np.array([hup,sup,vup], dtype=np.uint8)

            mask = cv2.inRange(hsv, minValues, maxValues)
            res = cv2.bitwise_and(img,img, mask= mask)
        else:
            # http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html
            # red goes from 240 to 20 degreess aprox
            maxValues1 = np.array([hup,sup,vup],dtype=np.uint8)
            minValues1 = np.array([0,sdwn,vdwn],dtype=np.uint8)
            maxValues2 = np.array([180,sup,vup], dtype=np.uint8)
            minValues2 = np.array([hdwn,sdwn,vdwn],dtype=np.uint8)

            mask1 = cv2.inRange(hsv, minValues1, maxValues1)
            mask2 = cv2.inRange(hsv, minValues2, maxValues2)

            r1 = cv2.bitwise_and(img,img, mask= mask1)
            r2 = cv2.bitwise_and(img,img, mask= mask2)
            res = cv2.bitwise_or(r1, r2)
        


        return res