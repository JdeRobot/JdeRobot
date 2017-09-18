import rospy
from sensor_msgs.msg import Image as ImageROS
import threading
from math import pi as PI
from jderobotTypes import Image
import cv2
from cv_bridge import CvBridge, CvBridgeError




def imageMsg2Image(img, bridge):
    '''
    Translates from ROS Image to JderobotTypes Image. 

    @param img: ROS Image to translate
    @param bridge: bridge to do translation

    @type img: sensor_msgs.msg.Image
    @type brige: CvBridge

    @return a JderobotTypes.Image translated from img

    '''
    image = Image()

    image.width = img.width
    image.height = img.height
    image.format = "RGB8"
    image.timeStamp = img.header.stamp.secs + (img.header.stamp.nsecs *1e-9)
    cv_image = bridge.imgmsg_to_cv2(img, "rgb8")
    image.data = cv_image
    return image

class ListenerCamera:
    '''
        ROS Camera (Image) Subscriber. Camera Client to Receive Images from ROS nodes.
    '''
    def __init__(self, topic):
        '''
        ListenerCamera Constructor.

        @param topic: ROS topic to subscribe
        
        @type topic: String

        '''
        self.topic = topic
        self.data = Image()
        self.sub = None
        self.lock = threading.Lock()

        self.bridge = CvBridge()
        self.start()
 
    def __callback (self, img):
        '''
        Callback function to receive and save Images. 

        @param img: ROS Image received
        
        @type img: sensor_msgs.msg.Image

        '''
        image = imageMsg2Image(img, self.bridge)

        self.lock.acquire()
        self.data = image
        self.lock.release()
        
    def stop(self):
        '''
        Stops (Unregisters) the client.

        '''
        self.sub.unregister()

    def start (self):
        '''
        Starts (Subscribes) the client.

        '''
        self.sub = rospy.Subscriber(self.topic, ImageROS, self.__callback)
        
    def getImage(self):
        '''
        Returns last Image. 

        @return last JdeRobotTypes Image saved

        '''
        self.lock.acquire()
        image = self.data
        self.lock.release()
        
        return image


