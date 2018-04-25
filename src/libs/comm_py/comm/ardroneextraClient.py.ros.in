import sys
import Ice
from .ice.ardroneextraIceClient import ArDroneExtraIceClient
from .tools import server2int



def __getArDroneExtraIceClient(jdrc, prefix):
    '''
    Returns a ArDroneExtra Ice Client. This function should never be used. Use getArDroneExtraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return ArDroneExtra Ice Client

    '''
    print("Publishing "+ prefix +" with ICE interfaces")
    client = ArDroneExtraIceClient(jdrc, prefix)
    client.start()
    return client

def __getPublisherArDroneExtra(jdrc, prefix):
    '''
    Returns a ArDroneExtra ROS Publisher. This function should never be used. Use getArDroneExtraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return ArDroneExtra ROS Publisher

    '''

    print(prefix + ": This Interface doesn't support ROS msg")
    return None

def __ArDroneExtradisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getArDroneExtraClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getArDroneExtraClient (jdrc, prefix):
    '''
    Returns a ArDroneExtra Client.

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file
    @param node: ROS node

    @type jdrc: Comm Communicator
    @type prefix: String
    @type node: ROS node

    @return None if ArDroneExtra is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__ArDroneExtradisabled, __getArDroneExtraIceClient, __getPublisherArDroneExtra]

    return cons[server](jdrc, prefix)
