from .communicator import Communicator

from .tools import server2int




def init (config, prefix):
    '''
    Starts JdeRobotComm

    @param config: configuration of client

    @type config: dict
    '''
    return Communicator(config, prefix)




    



