import rospy
from sensor_msgs.msg import Image as ImageROS
import threading
from math import pi as PI
from jderobotTypes import Image
from .threadPublisher import ThreadPublisher
from cv_bridge import CvBridge, CvBridgeError



class PublisherCamera:
    '''
        ROS Image Publisher.
    '''
    def __init__(self, topic, camera):
        '''
        PublisherCamera Constructor.

        @param topic: ROS topic to publish
        
        @type topic: String

        '''
        self.camera = camera
        self.bridge = CvBridge()

        self.topic = topic
        self.pub = self.pub = rospy.Publisher(self.topic, ImageROS, queue_size=1)

        self.kill_event = threading.Event()
        self.thread = ThreadPublisher(self, self.kill_event)

        self.thread.daemon = True
        self.start()
 
    def publish (self):
        '''
        Function to publish Image. 
        '''
        img = self.camera.get_image()
        msg = self.bridge.cv2_to_imgmsg(img.data, "rgb8")
        
        self.pub.publish(tw)
        
    def stop(self):
        '''
        Stops (Unregisters) the server. If server is stopped you can not start again, Threading.Thread raised error

        '''
        self.kill_event.set()
        self.pub.unregister()

    def start (self):
        '''
        Starts (Subscribes) the server. If server is stopped you can not start again, Threading.Thread raised error

        '''
        self.kill_event.clear()
        self.thread.start()