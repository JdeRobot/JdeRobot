import sys, Ice
import jderobot
import time
import random

class PointI(jderobot.Visualization):

    def getRGBPoint(self, point, current=None):
        point.x = random.randint(-50, 50)
        point.y = random.randint(-50, 50)
        point.z = random.randint(-50, 50)
        point.r = random.uniform(0, 1)
        point.g = random.uniform(0, 1)
        point.b = random.uniform(0, 1)
        return point

    def clearAll(self, current=None):
        print "Clear All"

try:
    endpoint = "ws -h 0.0.0.0 -p 11000"
    id = Ice.InitializationData()
    ic = Ice.initialize(None, id)
    adapter = ic.createObjectAdapterWithEndpoints("3DViewer", endpoint)
    object = PointI()
    adapter.add(object, ic.stringToIdentity("3DViewerA"))
    adapter.activate()
    print "Listening at port 11000"
    ic.waitForShutdown()
except KeyboardInterrupt:
	del(ic)
	sys.exit()
