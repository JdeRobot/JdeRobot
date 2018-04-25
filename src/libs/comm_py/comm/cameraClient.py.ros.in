import sys
import Ice
import rospy
from .ice.cameraIceClient import CameraIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.listenerCamera import ListenerCamera

def __getCameraIceClient(jdrc, prefix):
    '''
    Returns a Camera Ice Client. This function should never be used. Use getCameraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Camera Ice Client

    '''
    print("Receiving " + prefix + " Image from ICE interfaces")
    client = CameraIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerCamera(jdrc, prefix):
    '''
    Returns a Camera ROS Subscriber. This function should never be used. Use getCameraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Camera ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + " Image from ROS messages")
        topic = jdrc.getConfig().getProperty(prefix+".Topic")
        client = ListenerCamera(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Cameradisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getCameraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getCameraClient (jdrc, prefix):
    '''
    Returns a Camera Client.

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type name: String

    @return None if Camera is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Cameradisabled, __getCameraIceClient, __getListenerCamera]

    return cons[server](jdrc, prefix)
