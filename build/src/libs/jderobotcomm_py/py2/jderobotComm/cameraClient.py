import sys
import Ice
import rospy
from .ice.cameraIceClient import CameraIceClient

if (sys.version_info[0] == 2):
    from .ros.listenerCamera import ListenerCamera

def __getCameraIceClient(ic, prefix):
    '''
    Returns a Camera Ice Client. This function should never be used. Use getCameraClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Camera Ice Client

    '''
    print("Receiving " + prefix + " Image from ICE interfaces")
    client = CameraIceClient(ic, prefix)
    client.start()
    return client

def __getListenerCamera(ic, prefix):
    '''
    Returns a Camera ROS Subscriber. This function should never be used. Use getCameraClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Camera ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + " Image from ROS messages")
        prop = prop = ic.getProperties()
        topic = prop.getPropertyWithDefault(prefix+".Topic","");
        client = ListenerCamera(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Cameradisabled(ic, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getCameraClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getCameraClient (ic, prefix, node=None):
    '''
    Returns a Camera Client.

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file
    @param node: ROS node

    @type ic: Ice Communicator
    @type prefix: String
    @type node: ROS node

    @return None if Camera is disabled

    '''
    prop = prop = ic.getProperties()
    server = prop.getPropertyAsIntWithDefault(prefix+".Server",0)

    cons = [__Cameradisabled, __getCameraIceClient, __getListenerCamera]

    return cons[server](ic, prefix)