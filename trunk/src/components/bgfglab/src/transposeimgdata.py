#!/usr/bin/python

import sys
import os
import numpy



def transposeImageData(inputFileName, outputFileName, nrow, ncol):
    imgData = numpy.fromfile(file=inputFileName,dtype=numpy.uint8,count=nrow*ncol).reshape(nrow,ncol)
    imgData.transpose().tofile(outputFileName)


def usage():
    sys.stderr.write("Usage: %s inputFileName outputFileName nimages/nrow imgsize/ncol\n" % os.path.basename(sys.argv[0]))
    sys.exit(-1)

def main():
    if len(sys.argv) != 5:
        usage()
        
    nimages = 0
    imgsize = 0
    try:
        nimages = int(sys.argv[3])
        imgsize = int(sys.argv[4])
    except:
        usage()

    transposeImageData(sys.argv[1],sys.argv[2],nimages,imgsize)


if __name__ == "__main__":
    main()
