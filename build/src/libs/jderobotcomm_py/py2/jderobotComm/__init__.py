import Ice
import rospy

from .laserClient import getLaserClient
from .cameraClient import getCameraClient
from .pose3dClient import getPose3dClient
from .motorsClient import getMotorsClient


def init (ic):
	'''
    Starts ROS Node if it is necessary.

    @param ic: Ice Communicator

    @type ic: Ice Communicator
    '''
	node = None
	rosserver = False

	prop = ic.getProperties()
	nodeName = prop.getPropertyWithDefault("NodeName", "JdeRobot")
	
	l = prop.getPropertiesForPrefix("")
	keys = l.keys()
	for i in keys:
		if (i.endswith(".Server") and l[i] == "2"):
			rosserver = True

	if (rosserver):
		node = rospy.init_node(nodeName, anonymous=True)

	return ic,node

def destroy(ic=None, node=None):
	'''
    Destroys ROS Node and Ice Communicator if it is necessary.

    @param ic: Ice Communicator
    @param node: ROS Node

    @type ic: Ice Communicator
    @type node: ROS Node
    '''
	if node:
		rospy.signal_shutdown("Node Closed")
	if ic:
		ic.destroy()




	



