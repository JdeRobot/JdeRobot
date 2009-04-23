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
        self.e = encoders.Encoders(self,"encoders-calc")
        self.m = motors.Motors(self)
        self.l = laser.Laser(self)
		
    def iteration(self):
        edata = encoders.floatArray_frompointer(self.e.robot)
        print "sA encoders[x,y,@]:%f,%f,%f" % (edata[encoders.ROBOT_X],
                                               edata[encoders.ROBOT_Y],
                                               edata[encoders.ROBOT_THETA])
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
