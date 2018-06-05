#!/usr/bin/env python2
import config
import comm
import sys
import time
import signal

from jderobotTypes import IRData


if __name__ == '__main__':

    cfg = config.load(sys.argv[1])
    jdrc= comm.init(cfg, "Test")

    client = jdrc.getIRClient("Test.IR")

    ir = client.getIRData()
    print (ir)

    for i in range (10):
        ir = client.getIRData()
        print (ir)
        time.sleep(1)

    jdrc.destroy()