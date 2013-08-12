import sys, traceback, Ice
import jderobot 
import cv2  
import numpy as np
import threading  

class Sensor:
    def __init__(self):
        try:
            ic  =     ic = Ice.initialize(sys.argv)
            base = ic.stringToProxy("cameraA:default -h localhost -p 9999")
            self.cameraProxy = jderobot.CameraPrx.checkedCast(base)
            if not self.cameraProxy:
                raise RuntimeError("Invalid proxy")
        
            self.image = self.cameraProxy.getImageData();
            self.height= self.image.description.height
            self.width = self.image.description.width
                
            self.lock = threading.Lock()  

        except:
            traceback.print_exc()
            status = 1
    def update(self):
        self.lock.acquire();
        self.image = self.cameraProxy.getImageData();

        self.lock.release();

        
    def getImage(self):
        self.lock.acquire();
        img = np.zeros((self.height, self.width, 3), np.uint8)
        img = np.frombuffer(self.image.pixelData, dtype=np.uint8)
        img.shape = self.height, self.width, 3
        self.lock.release();
        """
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray = cv2.Canny(gray, 150, 200)
        gray = cv2.cvtColor(gray, cv2.COLOR_GRAY2RGB)
        """
        return img;
