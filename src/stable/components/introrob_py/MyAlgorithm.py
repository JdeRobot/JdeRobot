from sensors import sensor
import math
import jderobot


class MyAlgorithm():
    def __init__(self, sensor):
        self.sensor = sensor
        self.one=True

    def execute(self):
        # Add your code here
        tmp = self.sensor.getNavdata()
        print "State: " +str(tmp.state)
        print "Altitude: " +str(tmp.altd)
        print "Vehicle: " +str(tmp.vehicle)
        print "Battery %: " +str(tmp.batteryPercent)



