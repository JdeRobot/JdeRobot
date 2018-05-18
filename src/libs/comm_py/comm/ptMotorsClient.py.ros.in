import sys
import Ice
from .ice.ptMotorsIceClient import PTMotorsIceClient
from .tools import server2int



def __getPTMotorsIceClient(jdrc, prefix):
    '''
    Returns a PTMotors Ice Client. This function should never be used. Use getPTMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return PTMotors Ice Client

    '''
    print("Publishing "+ prefix +" with ICE interfaces")
    client = PTMotorsIceClient(jdrc, prefix)
    client.start()
    return client

def __getPublisherPTMotors(jdrc, prefix):
    '''
    Returns a PTMotors ROS Publisher. This function should never be used. Use getPTMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return PTMotors ROS Publisher

    '''

    print(prefix + ": This Interface doesn't support ROS msg")
    return None

def __PTMotorsdisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getPTMotorsClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getPTMotorsClient (jdrc, prefix):
    '''
    Returns a PTMotors Client.

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file
    @param node: ROS node

    @type jdrc: Comm Communicator
    @type prefix: String
    @type node: ROS node

    @return None if PTMotors is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__PTMotorsdisabled, __getPTMotorsIceClient, __getPublisherPTMotors]

    return cons[server](jdrc, prefix)
