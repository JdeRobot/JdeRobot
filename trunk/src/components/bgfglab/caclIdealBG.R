#!/usr/bin/Rscript

args<-commandArgs(T)

if (length(args) != 7){
  stop("Usage: caclIdealBG.R <imgseqfile> <nimage> <nrow> <ncol> <nchannel> <radius> <outidealbgfile>")
}

#get args
imgseqfile<-args[1]
nimage<-as.integer(args[2])
nrow<-as.integer(args[3])
ncol<-as.integer(args[4])
nchannel<-as.integer(args[5])
radius<-as.integer(args[6])
outidealbgfile<-args[7]


library(bgfglab)

idealBG<-idealBGClusteringMC(imgseqfile,nimage,nrow,ncol,nchannel,radius)
writeImg(idealBG,outidealbgfile)

