import sys, traceback, Ice
import easyiceconfig as EasyIce
import jderobot
import numpy as np
import threading
from interfaces.cameraClient import CameraClient

import cv2


if __name__ == "__main__":
    ic = EasyIce.initialize(sys.argv)
    properties = ic.getProperties()
    cameraRGB = CameraClient(ic, "rgbdViz.CameraRGB")
    cameraDepth = CameraClient(ic, "rgbdViz.CameraDEPTH")

    key=-1
    while key != 1048689:
        rgb = cameraRGB.getImage()
        depth = cameraDepth.getImage()

        image = cv2.cvtColor(rgb, cv2.COLOR_RGB2BGR)
        cv2.imshow("RGB", image)


        layers = cv2.split(depth)
        colord = cv2.cvtColor(layers[0],cv2.COLOR_GRAY2BGR)
        cv2.imshow("DEPTH", colord)

        key=cv2.waitKey(30)

    cameraDepth.stop()
    cameraRGB.stop()



