import sys
import Ice
from .ice.navdataIceClient import NavdataIceClient
from .tools import server2int



def __getNavdataIceClient(jdrc, prefix):
    '''
    Returns a Navdata Ice Client. This function should never be used. Use getNavdataClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Navdata Ice Client

    '''
    print("Publishing "+ prefix +" with ICE interfaces")
    client = NavdataIceClient(jdrc, prefix)
    client.start()
    return client

def __getPublisherNavdata(jdrc, prefix):
    '''
    Returns a Navdata ROS Publisher. This function should never be used. Use getNavdataClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return Navdata ROS Publisher

    '''

    print(prefix + ": This Interface doesn't support ROS msg")
    return None

def __Navdatadisabled(jdrc, prefix):
    '''
    Prints a warning that the client is disabled. This function should never be used. Use getNavdataClient instead of this

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file

    @type jdrc: Comm Communicator
    @type prefix: String

    @return None

    '''
    print(prefix + " Disabled")
    return None

def getNavdataClient (jdrc, prefix):
    '''
    Returns a Navdata Client.

    @param jdrc: Comm Communicator
    @param prefix: Name of client in config file
    @param node: ROS node

    @type jdrc: Comm Communicator
    @type prefix: String
    @type node: ROS node

    @return None if Navdata is disabled

    '''
    server = jdrc.getConfig().getProperty(prefix+".Server")
    server = server2int(server)

    cons = [__Navdatadisabled, __getNavdataIceClient, __getPublisherNavdata]

    return cons[server](jdrc, prefix)
