import sys
import threading
import config
import comm
import cv2
from comm.ros.listenerCamera import ListenerCamera

def showImage(client):
    key = -1
    while key not in [13, 27]:
        imageData =  client.getImage()
        cv2.imshow("Image", cv2.cvtColor(imageData.data, cv2.COLOR_RGB2BGR))
        key=cv2.waitKey(30)

if __name__ == "__main__":
    try:
        cfg = config.load(sys.argv[1])
        Server = cfg.getProperty("Cameraview.Camera.Server")
        rosserver = comm.server2int(Server) == 2
        jdrc = comm.init(cfg, "Cameraview")
        client = jdrc.getCameraClient("Cameraview.Camera")
        showImage(client)

    except IndexError:
        print("Please pass the configuration file.")
        sys.exit(1)
    except KeyError as e:
        print("Key Error in config file for {}".format(e))
        sys.exit(1)

