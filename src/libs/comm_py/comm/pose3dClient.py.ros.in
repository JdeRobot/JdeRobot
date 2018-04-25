import sys
import Ice
import rospy
from .ice.pose3dIceClient import Pose3dIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.listenerPose3d import ListenerPose3d



def __getPoseIceClient(jdrc, prefix):
    '''
    Returns a Pose3D Ice Client. This function should never be used. Use getPose3dClient instead of this

    @@param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Pose3D Ice Client

    '''
    print("Receiving " + prefix + " from ICE interfaces")
    client = Pose3dIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerPose(jdrc, prefix):
    '''
    Returns a Pose3D ROS Subscriber. This function should never be used. Use getPose3dClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Pose3D ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + " from ROS messages")
        topic = jdrc.getConfig().getProperty(prefix+".Topic")
        client = ListenerPose3d(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None


def __Posedisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getPose3dClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getPose3dClient (jdrc, prefix):
    '''
    Returns a Pose3D Client.

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None if pose3d is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Posedisabled, __getPoseIceClient, __getListenerPose]

    return cons[server](jdrc, prefix)
