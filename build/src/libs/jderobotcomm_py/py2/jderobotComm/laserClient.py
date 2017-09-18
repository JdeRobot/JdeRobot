import sys
import Ice
import rospy
from .ice.laserIceClient import LaserIceClient

if (sys.version_info[0] == 2):
    from .ros.listenerLaser import ListenerLaser

def __getLaserIceClient(ic, prefix):
    '''
    Returns a Laser Ice Client. This function should never be used. Use getLaserClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Laser Ice Client

    '''
    print("Receiving " + prefix + " LaserData from ICE interfaces")
    client = LaserIceClient(ic, prefix)
    client.start()
    return client

def __getListenerLaser(ic, prefix):
    '''
    Returns a Laser ROS Subscriber. This function should never be used. Use getLaserClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Laser ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + "  LaserData from ROS messages")
        prop = prop = ic.getProperties()
        topic = prop.getPropertyWithDefault(prefix+".Topic","");
        client = ListenerLaser(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Laserdisabled(ic, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getLaserClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getLaserClient (ic, prefix, node=None):
    '''
    Returns a Laser Client.

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file
    @param node: ROS node

    @type ic: Ice Communicator
    @type prefix: String
    @type node: ROS node

    @return None if Laser is disabled

    '''
    prop = prop = ic.getProperties()
    server = prop.getPropertyAsIntWithDefault(prefix+".Server",0)

    cons = [__Laserdisabled, __getLaserIceClient, __getListenerLaser]

    return cons[server](ic, prefix)