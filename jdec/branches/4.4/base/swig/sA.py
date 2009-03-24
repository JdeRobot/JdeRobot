import sys
sys.path.append('../lib/jderobot/swig')
import threading
import schema
import encoders
import motors
import laser
import time

class sA(schema.JDESchema):
    def __init__(self,name):
        super(sA,self).__init__(name)
        self.e = encoders.Encoders(self.name)
        self.m = motors.Motors(self.name)
        self.l = laser.Laser(self.name)
    def iteration(self):
        print "sA encoders:",self.e.x,",",self.e.y,",",self.e.theta
        ldata = laser.intArray_frompointer(self.l.laser)
        print "sA laser[0,90,180]:%d,%d,%d" % (ldata[0],ldata[self.l.number/2],ldata[self.l.number-1])
        self.m.v = 100

    def run_children(self):
        self.e.cast().run()
        self.m.cast().run()
        self.l.cast().run()

    def stop_children(self):
        self.e.cast().stop()
        self.m.cast().stop()
        self.l.cast().stop()

sAinstance = sA('sA')
        
