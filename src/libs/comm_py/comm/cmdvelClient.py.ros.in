import sys
import Ice
from .ice.cmdvelIceClient import CMDVelIceClient
from .tools import server2int



def __getCMDVelIceClient(jdrc, prefix):
    '''
    Returns a CMDVel Ice Client. This function should never be used. Use getCMDVelClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return CMDVel Ice Client

    '''
    print("Publishing "+ prefix +" with ICE interfaces")
    client = CMDVelIceClient(jdrc, prefix)
    client.start()
    return client

def __getPublisherCMDVel(jdrc, prefix):
    '''
    Returns a CMDVel ROS Publisher. This function should never be used. Use getCMDVelClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return CMDVel ROS Publisher

    '''

    print(prefix + ": This Interface doesn't support ROS msg")
    return None

def __CMDVeldisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getCMDVelClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getCMDVelClient (jdrc, prefix):
    '''
    Returns a CMDVel Client.

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file
    @param node: ROS node

    @type jdrc: Comm Communicator
    @type prefix: String
    @type node: ROS node

    @return None if CMDVel is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__CMDVeldisabled, __getCMDVelIceClient, __getPublisherCMDVel]

    return cons[server](jdrc, prefix)
