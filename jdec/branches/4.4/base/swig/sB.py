import sys
sys.path.append('../lib/jderobot/swig')
#import threading
import pyschema
import encoders
import time

class sB(pyschema.PyJDESchema):
    def __init__(self,name):
        super(sB,self).__init__(name)
        self.erobot = encoders.Encoders(self)
        self.ecalc = encoders.Encoders(self,"encoders-calc",1)
        	
    def iteration(self):
        self.ecalc.x = self.erobot.x/100
        self.ecalc.y = self.erobot.y/100
        self.ecalc.theta = self.erobot.theta
        print "sB robot encoders[x,y,@]:%f,%f,%f" % (self.erobot.x,self.erobot.y,self.erobot.theta)
        print "sB calc encoders[x,y,@]:%f,%f,%f" % (self.ecalc.x,self.ecalc.y,self.ecalc.theta)
        
    def run_children(self):
        self.erobot.run()

    def stop_children(self):
        self.erobot.stop()

sBinstance = sB('python-sB')
sBinstance.init()
