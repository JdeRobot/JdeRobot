
from threading import Lock
import cv2
import numpy as np


'''Max Values supported by OpenCV'''
RGBMAX = (255,255,255)
RGBMIN = (0,0,0)


class RgbFilter:

    def __init__(self):

        self.lock = Lock()

        self.MAX = RGBMAX
        self.MIN = RGBMIN

        self.uLimit = list(RGBMAX)
        self.dLimit = list(RGBMIN)

    def getName(self):
        return 'RGB'

    def setUpLimit (self, r, g, b):
        self.lock.acquire()
        self.uLimit = [r,g,b]
        self.lock.release()


    def getUpLimit (self):
        self.lock.acquire()
        lim = self.uLimit
        self.lock.release()
        return lim

    def setDownLimit(self, r, g, b):
        self.lock.acquire()
        self.dLimit = [r,g,b]
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

        rup,gup,bup = self.getUpLimit()
        rdwn,gdwn,bdwn = self.getDownLimit()

        
        minValues = np.array([rdwn,gdwn,bdwn],dtype=np.uint8)
        maxValues = np.array([rup,gup,bup], dtype=np.uint8)

        mask = cv2.inRange(img, minValues, maxValues)

        res = cv2.bitwise_and(img,img, mask= mask)


        return res
        return img