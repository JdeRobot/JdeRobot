import sys
sys.path.append('../lib/jderobot/swig')
#import threading
import pyschema
import encoders
import motors
import laser
import time

class sA(pyschema.PyJDESchema):
    def __init__(self,name):
        super(sA,self).__init__(name)
        self.e = encoders.Encoders(self.name,"encoders-calc")
        self.m = motors.Motors(self.name)
        self.l = laser.Laser(self.name)
		
    def iteration(self):
        print "sA encoders[x,y,@]:%f,%f,%f" % (self.e.x,self.e.y,self.e.theta)
        ldata = laser.intArray_frompointer(self.l.laser)
        print "sA laser[0,90,180]:%d,%d,%d" % (ldata[0],ldata[self.l.number/2],ldata[self.l.number-1])
        self.m.v = 500

    def run_children(self):
        self.e.run()
        self.m.run()
        self.l.run()

    def stop_children(self):
        self.e.stop()
        self.m.stop()
        self.l.stop()

sAinstance = sA('python-sA')
sAinstance.init()
