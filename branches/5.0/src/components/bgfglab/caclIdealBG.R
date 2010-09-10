#!/usr/bin/Rscript

source(bgstats.R)

args<-commandArgs(T)

if (length(args) != 3){
  stop("Usage: caclIdealBGClustering.R <dumpfile> <idealbg> <radius>")
}

#get args
dumpfile<-args[1]
idealbgfile<-args[2]
radius<-as.integer(args[3])

#read dump info file
dumpinfo<-readDumpDataInfo(dumpfile)
imgIdx<-1 #dumpfile first line is real image data

idealBG<-calcIdealBGClustering(dumpinfo$filename[imgIdx],
                               dumpinfo$nimage[imgIdx],
                               dumpinfo$nrow[imgIdx],
                               dumpinfo$ncol[imgIdx],
                               dumpinfo$nchannel[imgIdx],
                               radius)
writeImg(idealBG,idealfile)

