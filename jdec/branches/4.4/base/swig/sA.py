import sys
sys.path.append('../lib/jderobot/swig')
import threading
import schema
import encoders
import motors
import time

class sA(schema.JDESchema):
    def iteration(self):
        e = encoders.Encoders()
        m = motors.Motors()
        print "sA iteration(",self.name,"):",e.x,",",e.y,",",e.theta
        m.v = 1000

sAinstance = sA('sA')
        
