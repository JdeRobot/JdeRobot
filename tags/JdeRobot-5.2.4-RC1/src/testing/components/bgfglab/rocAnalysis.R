#!/usr/bin/Rscript

files<-commandArgs(T)

dataList<-list()
for (i in 1:length(files)){
  data<-read.table(files[i],header=T)
  name<-unlist(strsplit(basename(files[i]),"\\."))[1]
  dataList[[name]]<-data
}

#cacl derivatives
for(i in names(dataList)){
  dataList[[i]]$P<-(dataList[[i]]$TP + dataList[[i]]$FN)
  dataList[[i]]$N<-(dataList[[i]]$FP + dataList[[i]]$TN)
  dataList[[i]]$TPR<-(dataList[[i]]$TP / dataList[[i]]$P)
  dataList[[i]]$FPR<-(dataList[[i]]$FP / dataList[[i]]$N)
  dataList[[i]]$ACC<-((dataList[[i]]$TP + dataList[[i]]$TN)/(dataList[[i]]$P + dataList[[i]]$N))
  dataList[[i]]$PPV<-(dataList[[i]]$TP / (dataList[[i]]$TP+dataList[[i]]$FP))
}

#TPR-FPR
png("tpr_fpr.png")
tpr<-vector()
fpr<-vector()
for(i in names(dataList)){
  tpr<-append(tpr,as.double(mean(dataList[[i]]$TPR[!is.nan(dataList[[i]]$TPR)])))
  fpr<-append(fpr,as.double(mean(dataList[[i]]$FPR[!is.nan(dataList[[i]]$FPR)])))
}
plot(fpr,tpr,pch=1:length(fpr),col=1:length(fpr),xlim=c(0,1),ylim=c(0,1))
lines(c(0,1),c(0,1),col="red",lty=2)#random prediction
legend("bottomright",names(dataList),pch=1:length(fpr),col=1:length(fpr))
dev.off()

#ACC-frame
png("acc_frame.png")
layout(matrix(1:length(names(dataList)),byrow=T,nrow=2))
for (i in names(dataList)){
  plot(dataList[[i]]$ACC,main=i,xlab="frame",ylab="accuracy",ylim=c(0,1))
}
dev.off()

#PPV-frame
png("ppv_frame.png")
layout(matrix(1:length(names(dataList)),byrow=T,nrow=2))
for (i in names(dataList)){
  plot(dataList[[i]]$PPV,main=i,xlab="frame",ylab="precission",ylim=c(0,1))
}
dev.off()

