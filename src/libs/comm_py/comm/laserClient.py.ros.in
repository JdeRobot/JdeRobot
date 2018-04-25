import sys
import Ice
import rospy
from .ice.laserIceClient import LaserIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.listenerLaser import ListenerLaser

def __getLaserIceClient(jdrc, prefix):
    '''
    Returns a Laser Ice Client. This function should never be used. Use getLaserClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Laser Ice Client

    '''
    print("Receiving " + prefix + " LaserData from ICE interfaces")
    client = LaserIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerLaser(jdrc, prefix):
    '''
    Returns a Laser ROS Subscriber. This function should never be used. Use getLaserClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Laser ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + "  LaserData from ROS messages")
        topic  = jdrc.getConfig().getProperty(prefix+".Topic")
        client = ListenerLaser(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Laserdisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getLaserClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getLaserClient (jdrc, prefix):
    '''
    Returns a Laser Client.

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type name: String

    @return None if Laser is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Laserdisabled, __getLaserIceClient, __getListenerLaser]

    return cons[server](jdrc, prefix)
