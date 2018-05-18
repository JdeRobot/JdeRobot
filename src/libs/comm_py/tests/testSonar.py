#!/usr/bin/env python2
import config
import comm
import sys
import time
import signal

from jderobotTypes import SonarData


if __name__ == '__main__':

    cfg = config.load(sys.argv[1])
    jdrc= comm.init(cfg, "Test")

    client = jdrc.getSonarClient("Test.Sonar")

    sonar = client.getSonarData()
    print (sonar)

    for i in range (10):
        sonar = client.getSonarData()
        print (sonar)
        time.sleep(1)

    jdrc.destroy()