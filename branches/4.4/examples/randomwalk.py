#!/usr/bin/python

import sys
prefix='..'
sys.path.append(prefix + '/lib/jderobot/swig')
import hierarchy
import laser
import motors
import time

def main():
    myH = hierarchy.JDEHierarchy(sys.argv,prefix + '/share/jderobot/conf/randomwalk.conf')
    if myH == None:
        print('Initialization failed...')
        sys.exit(-1)

    myL = laser.LaserPrx('laser',myH.root_schema_get())
    if myL == None:
        print("Can't get laser prx")
        sys.exit(-1);
    myL.run()
    
    myM = motors.MotorsPrx('motors',myH.root_schema_get())
    if myM == None:
        print("Can't get motors prx")
        sys.exit(-1);
    myM.run()
    
    threshold = 500
    right_laser = 0
    front_laser = int(myL.number/2)
    left_laser = myL.number

    #from swig carray.i library
    laserv = laser.intArray_frompointer(myL.laser)

    while True:
        right_obst = threshold
        left_obst = threshold
        for i in range(0,myL.number):
            if i < front_laser:
                right_obst = min(laserv[i],right_obst)
            else:
                left_obst = min(laserv[i],left_obst)
        
        if (right_obst < threshold) or (left_obst < threshold):#turn
            if right_obst < left_obst:#obstacle approaching on right
                myM.v = 0
                myM.w = 50 #turn left
            else:#obstacle approaching on left
                myM.v = 0
                myM.w = 50 #turn right
        else:#no obstacle approaching
            myM.v = 100
            myM.w = 0
        print('L: %d, F: %d, R: %d' % (laserv[left_laser],
                                       laserv[front_laser],
                                       laserv[right_laser]))
        time.sleep(0.33) #~3 times/s


if __name__ == '__main__':
    main()
