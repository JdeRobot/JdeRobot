import sys
import Ice
import rospy
from .ice.bumperIceClient import BumperIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.listenerBumper import ListenerBumper

def __getBumperIceClient(jdrc, prefix):
    '''
    Returns a Bumper Ice Client. This function should never be used. Use getBumperClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Bumper Ice Client

    '''
    print("Receiving " + prefix + " BumperData from ICE interfaces")
    client = BumperIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerBumper(jdrc, prefix):
    '''
    Returns a Bumper ROS Subscriber. This function should never be used. Use getBumperClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Bumper ROS Subscriber

    '''
    if (sys.version_info[0] == 2):
        print("Receiving " + prefix + "  BumperData from ROS messages")
        topic  = jdrc.getConfig().getProperty(prefix+".Topic")
        client = ListenerBumper(topic)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Bumperdisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getBumperClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print( prefix + " Disabled")
    return None

def getBumperClient (jdrc, prefix):
    '''
    Returns a Bumper Client.

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type name: String

    @return None if Bumper is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Bumperdisabled, __getBumperIceClient, __getListenerBumper]

    return cons[server](jdrc, prefix)
