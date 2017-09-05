import sys 
import os
sys.path.append(os.path.abspath("/home/nigel/Documents/Yolod/Code/DarkflowTest"))
#sys.path.append(os.path.abspath("/usr/lib/python2.7"))
#sys.path += ['', '/opt/ros/kinetic/lib/python2.7/dist-packages', '/usr/lib/python2.7', '/usr/lib/python2.7/plat-x86_64-linux-gnu', '/usr/lib/python2.7/lib-tk', '/usr/local/lib/python2.7/dist-packages/dateparser-0.6.0-py2.7.egg', '/usr/local/lib/python2.7/dist-packages/tzlocal-1.4-py2.7.egg', '/usr/local/lib/python2.7/dist-packages/regex-2017.5.26-py2.7-linux-x86_64.egg', '/usr/local/lib/python2.7/dist-packages/ruamel.yaml-0.13.14-py2.7-linux-x86_64.egg', '/usr/local/lib/python2.7/dist-packages/typing-3.6.1-py2.7.egg', '/usr/local/lib/python2.7/dist-packages/ruamel.ordereddict-0.4.9-py2.7-linux-x86_64.egg', '/usr/lib/python2.7/dist-packages', '/usr/local/lib/python2.7/dist-packages/tzlocal-1.4-py2.7.egg', '/usr/local/lib/python2.7/dist-packages/regex-2017.5.26-py2.7-linux-x86_64.egg', '/usr/local/lib/python2.7/dist-packages/ruamel.yaml-0.13.14-py2.7-linux-x86_64.egg', '/usr/local/lib/python2.7/dist-packages/typing-3.6.1-py2.7.egg', '/usr/local/lib/python2.7/dist-packages/ruamel.ordereddict-0.4.9-py2.7-linux-x86_64.egg', '/usr/local/lib/python2.7/site-packages', '/usr/local/lib/python2.7/dist-packages', '/usr/lib/python2.7/dist-packages/PILcompat', '/usr/lib/python2.7/dist-packages/gtk-2.0']
#print (sys.path)

#'/home/nigel/.virtualenvs/cv/lib/python2.7', '/home/nigel/.virtualenvs/cv/lib/python2.7/plat-x86_64-linux-gnu', '/home/nigel/.virtualenvs/cv/lib/python2.7/lib-tk', '/home/nigel/.virtualenvs/cv/lib/python2.7/lib-old', '/home/nigel/.virtualenvs/cv/lib/python2.7/lib-dynload', '/home/nigel/.virtualenvs/cv/local/lib/python2.7/site-packages', '/home/nigel/.virtualenvs/cv/lib/python2.7/site-packages'


from darkflow.net.build import TFNet

import sys, traceback, Ice
import easyiceconfig as EasyIce
import jderobot
import numpy as np
import threading
import cv2



if __name__ == "__main__":
    print ("Camera is ready to get yolod!")
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
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        
        options = {"model": "cfg/yolo.cfg", "load": "bin/yolo.weights", "threshold": 0.1}
        tfnet = TFNet(options)
        imgcv = image
        result = tfnet.return_predict(imgcv)        
        for item in result:
            tlx = item['topleft']['x']
            tly = item['topleft']['y']
            brx = item['bottomright']['x']
            bry = item['bottomright']['y']
            label = item['label']
            cv2.rectangle(imgcv, (tlx, tly), (brx, bry), (0,255,0), 3)
            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(imgcv, label, (tlx, tly), font, 2, (255,255,255), 1, cv2.LINE_AA)
        cv2.imshow("Image", imgcv)
        key=cv2.waitKey(30)
