import sys
import Ice
import rospy
from .ice.sonarIceClient import SonarIceClient
from .tools import server2int


def __getSonarIceClient(jdrc, prefix):
    '''
    Returns a Laser Ice Client. This function should never be used. Use getLaserClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Laser Ice Client

    '''
    print("Receiving " + prefix + " SonarData from ICE interfaces")
    client = SonarIceClient(jdrc, prefix)
    client.start()
    return client

def __getListenerSonar(jdrc, prefix):
    '''
    Returns a Laser ROS Subscriber. This function should never be used. Use getLaserClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Laser ROS Subscriber

    '''
    print(prefix + ": This Interface doesn't support ROS msg")
    return None

def __Sonardisabled(jdrc, prefix):
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

def getSonarClient (jdrc, prefix):
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

    cons = [__Sonardisabled, __getSonarIceClient, __getListenerSonar]

    return cons[server](jdrc, prefix)
