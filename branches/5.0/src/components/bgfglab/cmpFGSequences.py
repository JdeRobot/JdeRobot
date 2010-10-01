#!/usr/bin/python

import sys
import opencv
import opencv.highgui

def usage():
    sys.stderr.write("Usage: %s seqlen fgseqideal fgseq\n" % os.path.basename(sys.argv[0]))

def main():
    if len(sys.argv) != 4:
        usage()
        sys.exit(-1)

    seqlength = int(sys.argv[1])
    fgseqideal = sys.argv[2]
    fgseq = sys.argv[3]

    for i in range(seqlength):
        fgmask = opencv.highgui.cvLoadImage(fgseq % (i+1),0)
        fgmaskideal = opencv.highgui.cvLoadImage(fgseqideal % (i+1),0)
        res = opencv.cvCloneMat(fgmask)
        opencv.cvAnd(fgmask,fgmaskideal,res)
        tp = opencv.cvCountNonZero(res)
        opencv.cvNot(fgmaskideal,fgmaskideal)
        opencv.cvAnd(fgmask,fgmaskideal,res)
        fp = opencv.cvCountNonZero(res)
        print('%d %d' %  (fp,tp))


if __name__ == "__main__":
    main()
