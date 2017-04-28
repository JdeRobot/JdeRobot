import sys
import Ice
import rospy
from .ice.motorsIceClient import MotorsIceClient

if (sys.version_info[0] == 2):
    from .ros.publisherMotors import PublisherMotors

def __getMotorsIceClient(ic, prefix):
    '''
    Returns a Motors Ice Client. This function should never be used. Use getMotorsClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Motors Ice Client

    '''
    print("Publishing Motors with ICE interfaces")
    client = MotorsIceClient(ic, prefix)
    client.start()
    return client

def __getPublisherMotors(ic, prefix):
    '''
    Returns a Motors ROS Publisher. This function should never be used. Use getMotorsClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return Motors ROS Publisher

    '''
    if (sys.version_info[0] == 2):
        print("Publishing Motors with ROS messages")
        prop = prop = ic.getProperties()
        topic = prop.getPropertyWithDefault(prefix+".Topic","")

        maxWstr = prop.getProperty(prefix+".maxW")
        if maxWstr:
            maxW = float(maxWstr)
        else:
            maxW = 0.5
            print (prefix+".maxW not provided, the default value is used: "+ repr(maxW))
                

        maxVstr = prop.getProperty(prefix+".maxV")
        if maxWstr:
            maxV = float(maxVstr)
        else:
            maxV = 5
            print (prefix+".maxV not provided, the default value is used: "+ repr(maxV))

        
        client = PublisherMotors(topic, maxV, maxW)
        return client
    else:
        print(prefix + ": ROS msg are diabled for python "+ sys.version_info[0])
        return None

def __Motorsdisabled(ic, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getMotorsClient instead of this

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file

    @type ic: Ice Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getMotorsClient (ic, prefix, node=None):
    '''
    Returns a Motors Client.

    @param ic: Ice Communicator
    @param prefix: prefix name of client in config file
    @param node: ROS node

    @type ic: Ice Communicator
    @type prefix: String
    @type node: ROS node

    @return None if Motors is disabled

    '''
    prop = prop = ic.getProperties()
    server = prop.getPropertyAsIntWithDefault(prefix+".Server",0)

    cons = [__Motorsdisabled, __getMotorsIceClient, __getPublisherMotors]

    return cons[server](ic, prefix)