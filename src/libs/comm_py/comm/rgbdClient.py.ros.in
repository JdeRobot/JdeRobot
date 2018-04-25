import sys
import Ice
import rospy
from .ice.rgbdIceClient import RgbdIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.listenerRgbd import ListenerRgbd

def __getRgbdIceClient(jdrc, prefix):
    '''
    Returns a Rgbd Ice Client. This function should never be used. Use getRgbdClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Rgbd Ice Client

    '''
    print("Receiving " + prefix + " Rgbd from ICE interfaces")
    client = RgbdIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerRgbd(jdrc, prefix):
    '''
    Returns a Rgbd ROS Subscriber. This function should never be used. Use getRgbdClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Rgbd ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + " Rgbd from ROS messages")
        topicrgb = jdrc.getConfig().getProperty(prefix+".Topicrgb")
        topicd = jdrc.getConfig().getProperty(prefix+".Topicd")
        client = ListenerRgbd(topicrgb, topicd)
        return client
        #print(prefix + ": This Interface doesn't support ROS msg")
        #return None
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Rgbddisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getRgbdClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getRgbdClient (jdrc, prefix):
    '''
    Returns a Rgbd Client.

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type name: String

    @return None if Rgbd is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Rgbddisabled, __getRgbdIceClient, __getListenerRgbd]

    return cons[server](jdrc, prefix)
