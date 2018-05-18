import sys
import Ice
import rospy
from .ice.motorsIceClient import MotorsIceClient
from .tools import server2int

if (sys.version_info[0] == 2):
    from .ros.publisherMotors import PublisherMotors

def __getMotorsIceClient(jdrc, prefix):
    '''
    Returns a Motors Ice Client. This function should never be used. Use getMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Motors Ice Client

    '''
    print("Publishing "+ prefix +" with ICE interfaces")
    client = MotorsIceClient(jdrc, prefix)
    client.start()
    return client

def __getPublisherMotors(jdrc, prefix):
    '''
    Returns a Motors ROS Publisher. This function should never be used. Use getMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Motors ROS Publisher

    '''
    if (sys.version_info[0] == 2):
        print("Publishing "+  prefix + " with ROS messages")
        topic = jdrc.getConfig().getProperty(prefix+".Topic")

        maxW = jdrc.getConfig().getPropertyWithDefault(prefix+".maxW", 0.5)
        if not maxW:
            maxW = 0.5
            print (prefix+".maxW not provided, the default value is used: "+ repr(maxW))


        maxV = jdrc.getConfig().getPropertyWithDefault(prefix+".maxV", 5)
        if not maxV:
            maxV = 5
            print (prefix+".maxV not provided, the default value is used: "+ repr(maxV))


        client = PublisherMotors(topic, maxV, maxW)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Motorsdisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getMotorsClient (jdrc, prefix):
    '''
    Returns a Motors Client.

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file
    @param node: ROS node

    @type jdrc: Comm Communicator
    @type prefix: String
    @type node: ROS node

    @return None if Motors is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Motorsdisabled, __getMotorsIceClient, __getPublisherMotors]

    return cons[server](jdrc, prefix)
