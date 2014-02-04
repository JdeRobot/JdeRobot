#!/usr/bin/python

import sys
import opencv
import opencv.highgui

def usage():
    sys.stderr.write("Usage: %s seqlen fgseqideal fgseq\n" % os.path.basename(sys.argv[0]))

def cmpFG(fgmask,fgmaskideal):
    '''Returns tuple (tp,tn,fp,fn)'''
    notfgmask = opencv.cvCloneMat(fgmask)
    opencv.cvNot(fgmask,notfgmask)
    notfgmaskideal = opencv.cvCloneMat(fgmask)
    opencv.cvNot(fgmaskideal,notfgmaskideal)
    res = opencv.cvCloneMat(fgmask)

    opencv.cvAnd(fgmask,fgmaskideal,res)
    tp = opencv.cvCountNonZero(res)

    opencv.cvAnd(notfgmask,notfgmaskideal,res)
    tn = opencv.cvCountNonZero(res)

    opencv.cvAnd(fgmask,notfgmaskideal,res)
    fp = opencv.cvCountNonZero(res)

    opencv.cvAnd(notfgmask,fgmaskideal,res)
    fn = opencv.cvCountNonZero(res)
    return (tp,tn,fp,fn)
    

def main():
    if len(sys.argv) != 4:
        usage()
        sys.exit(-1)

    seqlength = int(sys.argv[1])
    fgseqideal = sys.argv[2]
    fgseq = sys.argv[3]

    print('TP TN FP FN')
    for i in range(seqlength):
        fgmask = opencv.highgui.cvLoadImage(fgseq % (i+1),0)
        fgmaskideal = opencv.highgui.cvLoadImage(fgseqideal % (i+1),0)
        print('%d %d %d %d' %  cmpFG(fgmask,fgmaskideal))


if __name__ == "__main__":
    main()
