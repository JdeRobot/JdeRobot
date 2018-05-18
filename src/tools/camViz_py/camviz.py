import sys, traceback, Ice
import easyiceconfig as EasyIce
import jderobot
import numpy as np
import threading

import cv2


if __name__ == "__main__":
    ic = EasyIce.initialize(sys.argv)
    properties = ic.getProperties()
    basecameraL = ic.propertyToProxy("Camera.Proxy")
    cameraProxy = jderobot.CameraPrx.checkedCast(basecameraL)

    key=-1
    while key != 1048689:
        imageData = cameraProxy.getImageData("RGB8")
        imageData_h = imageData.description.height
        imageData_w = imageData.description.width
        image = np.zeros((imageData_h, imageData_w, 3), np.uint8)
        image = np.frombuffer(imageData.pixelData, dtype=np.uint8)
        image.shape = imageData_h, imageData_w, 3
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        cv2.imshow("Image", image)
        key=cv2.waitKey(30)



