import sys
import config
import cv2
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

from serverImage import ServerImage


img_formats = ["bmp", "dib", "gif", "jpeg", "jpg", "jpe", "jp2", "png", "pbm", "pgm", "ppm", "sr", "ras", "tiff", "tif"]
video_formats = ["mp4", "avi"]

if __name__== "__main__":
    cfg = config.load(sys.argv[1])

    topic = cfg.getProperty("camServer.Topic")
    fps = cfg.getPropertyWithDefault("camServer.FrameRate",12)
    uri = cfg.getProperty("camServer.Uri")
    name = cfg.getProperty("camServer.Name")

    if type(uri) == str and "." in uri and uri.split(".")[-1] in img_formats:
        print "loading image: ", uri.split("/")[-1]
        camera = ServerImage(uri)
    elif type(uri) == int:
        print "loading camera ..."
        camera = cv2.VideoCapture(uri)
    elif type(uri) == str and "." in uri and uri.split(".")[-1] in video_formats:
        print "loading video: ", uri.split("/")[-1]
        camera = cv2.VideoCapture(uri)
    else:
        print "[Format error]"
        sys.exit()
            
    bridge = CvBridge()
    pub = rospy.Publisher(topic, Image, queue_size=1)
    rospy.init_node(name)
    r = rospy.Rate(fps)

    while not rospy.is_shutdown():
        retval, img = camera.read()
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        msg = bridge.cv2_to_imgmsg(imgRGB, "rgb8")

        pub.publish(msg)
        r.sleep()



