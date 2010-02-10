#!/usr/bin/python
#-*- coding: Latin-1 -*-

import loader
import schema
import sys
import time

def main():
    root_interface = sys.argv[1]
    for i in sys.argv[2:]:
        print 'Loading %s...' % i
        if i.endswith('.so'):
            if loader.load_so(i) == None:
                print 'Error loading %s' % i
        else:
            __import__(i)
            
    sr = loader.search_schema_reg(root_interface)
    if sr == None:
        print 'Error: Can\'t find a schema implementing %s' % root_interface
        sys.exit(1)

    root = sr.instance(1,1)
    while True:
        root.init()
        time.sleep(1)

    sr.del_instance(root)
    sys.exit(0)


if __name__ == "__main__":
    main()

    
