#!/usr/bin/Rscript

files<-commandArgs(T)

dataList<-list()
for (i in 1:length(files)){
  data<-read.table(files[i],col.names=c("FP","TP"))
  name<-unlist(strsplit(basename(files[i]),"\\."))[1]
  dataList[[name]]<-data
}


#TP-FP all
#png("tp_fp_all.png")
#for (i in 1:names(dataList)){  
#}
#dev.off()

#TP-FP
png("tp_fp.png")
layout(matrix(1:length(names(dataList)),byrow=T,nrow=2))
for (i in names(dataList)){
  plot(dataList[[i]],main=i,xlim=c(0,20000),ylim=c(0,20000))
}
dev.off()

#frame-TP/FP
png("frame_tpfp.png")
layout(matrix(1:length(names(dataList)),byrow=T,nrow=2))
for (i in names(dataList)){
  plot((dataList[[i]][,2]/dataList[[i]][,1]),main=i,xlab="frame",ylab="TP/FP",ylim=c(0,200))
}
dev.off()


