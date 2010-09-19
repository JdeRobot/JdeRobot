library(flexclust)
library(bigmemory)
library(biganalytics)
library(multicore)
library(foreach)
library(doMC)

options(bigmemory.typecast.warning=FALSE) #remove cast warning
registerDoMC(cores=multicore:::detectCores())

readImg <- function(conn,imgsize){
  readBin(conn,integer(),n=imgsize,size=1,signed=F)
}

#sequence is a big.matrix with one image per column
readImgSeq <- function(conn,n,imgsize){
  if (is.character(conn))
    myconn <- file(conn,"rb")
  else
    myconn <- conn

  imgSeq<-big.matrix(imgsize,n,type="short",shared="F")
  for (col in 1:n){
    imgSeq[,col]<-readImg(myconn,imgsize)
  }
  imgSeq
}

#read dump data info file produced by bgfglab dump
readDumpDataInfo<-function(conn){
  if (is.character(conn))
    myconn <- file(conn,"r")
  else
    myconn <- conn
  
  dumpinfo <- scan(myconn,list(filename="",nimage=0,nrow=0,ncol=0,nchannel=0))
  close(myconn)
  dumpinfo
} 
  
writeImg <- function(img, conn){
  writeBin(as.integer(img),conn,size=1)
}

#imgSeq must be a big.matrix
writeImgSeq <- function(imgSeq, conn){
  if (is.character(conn))
    myconn <- file(conn,"rb")
  else
    myconn <- conn

  for (col in 1:ncol(imgSeq)){
    writeImg(imgSeq[,col],myconn)
  }
}

idealBGMean <- function(conn,nimage,nrow,ncol,nchannel){
  if (is.character(conn))
    myconn <- file(conn,"rb")
  else
    myconn <- conn

  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol

  imgMean<-vector("numeric",imgsize)
  
  for (i in 1:nimage)
    imgMean<-imgMean+(readImg(myconn,imgsize)/nimage)

  as.integer(round(imgMean))
}

idealBGMeanMC <- function(conn,nimage,nrow,ncol,nchannel,rowsbyjob){
  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol

  if ((imgsize %% rowsbyjob) != 0){
    stop("imgsize should be divisible by rowsbyjob")
  }
  imgSeq<-readImgSeq(conn,nimage,imgsize)
  imgMean<-foreach(i=seq(1:nrow(imgSeq),by=rowsbyjob),.combine=c) %dopar% apply(imgSeq[i:(i+rowsbyjob-1),],1,mean)
  as.integer(round(imgMean))
}

idealBGMode <- function(conn,nimage,nrow,ncol,nchannel,levels){
  if (is.character(conn))
    myconn <- file(conn,"rb")
  else
    myconn <- conn

  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol

  imgMode<-array(0,dim=c(levels,imgsize)) #each col has level entries to count every occurence
  for (i in 1:nimage){
    #read image and adjust values to [0..levels-1]
    img<-round(readImg(myconn,imgsize)*((levels-1)/255))+1
    #indexing [[level,pixel]]
    imgMode[cbind(img,seq(imgsize))] <- imgMode[cbind(img,seq(imgsize))] + 1
  }
  
  maxAndAdjust<-function(v){#get maximun and adjust to range [0..255]
    (which.max(v)-1)*(255/(levels-1))
  }
  apply(imgMode,2,maxAndAdjust)
}

idealBGClustering <- function(conn,nimage,nrow,ncol,nchannel,radius){
  pxDataSize<-nimage*nchannel
  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol

  imgSeq<-readImgSeq(conn,nimage,imgsize)
  
  imgClust<-NULL
  centers<-NULL
  for (i in seq(1,imgsize,by=nchannel)){#row index
    #read pixel data, each channel in a column
    pxData<-t(imgSeq[i:(i+nchannel-1),])
     cl <- try(qtclust(pxData,radius=radius),TRUE)
     #cl<-try(kcca(pData,2),TRUE)
     if (isS4(cl)){#qtclust returned a solution
       centers<-cl@centers[1,]
     }else{#just one, so center is the mean
       centers<-apply(pxData,2,mean)
     }
     imgClust<-append(imgClust,centers)
   }
   as.integer(round(imgClust))
 }

idealBGClusteringMC <- function(conn,nimage,nrow,ncol,nchannel,radius){
  pxDataSize<-nimage*nchannel
  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol
  
  imgSeq<-readImgSeq(conn,nimage,imgsize)
  imgClust<-foreach(i=seq(1,imgsize,by=nchannel),.combine=c) %dopar% {
    pxData<-t(imgSeq[i:(i+nchannel-1),])
    cl <- try(qtclust(pxData,radius),TRUE)
    centers<-NULL
    if (isS4(cl)){#qtclust returned a solution
      centers<-cl@centers[1,]
    }else{#just one, so center is the mean
      centers<-apply(pxData,2,mean)
    }
    centers
  }
  as.integer(round(imgClust))
}

euclideanDiffBG <- function(conn,nimage,nrow,ncol,nchannel,idealBG){
  pxDataSize<-nimage*nchannel
  imgsize<-nrow*ncol*nchannel
  npixel<-nrow*ncol

  idealBGPlanar<-splitChannels(idealBG,nchannel)

  bgimgSeq<-readImgSeq(conn,nimage,imgsize)
  meanDiff<-vector("integer",npixel)
  for (i in seq(1,nimage)){
    bgimgPlanar<-splitChannels(bgimgSeq[,i],nchannel)
    meanDiff<-meanDiff+(sqrt(apply((idealBGPlanar-bgimgPlanar)^2,1,sum))/nimage)
  }
  meanDiff
}

# image is fitted in a column matrix
# data is P0ch0P0ch1...P0chN...
# return a matrix with one channel per column
splitChannels <- function(img,nchannel){
  if (length(img) %% nchannel != 0){
    stop("img length not divisible by nchannel")
  }
  
  channelMat<-matrix(img,ncol=nchannel)
  for (i in 1:nchannel){
    channelMat[,i]<-img[seq(i,length(img),by=nchannel)]
  }
  channelMat
}

# list contains each channel in an element
# return an image with channels interleaved P0ch0P0ch1...P0chN..
mixChannels <- function(list){
  nchannels<-length(list)
  nrow<-dim(list[[1]])[1]
  ncol<-dim(list[[1]])[2]*nchannels
  img<-matrix(0,nrow,ncol)
  for (i in 1:nchannels){
    img[,seq(i,ncol,by=nchannels)]<-list[[i]]
  }
  img
}


# image is a matrix with dim height x (width x nchannels)
# data is P0ch0P0ch1...P0chN... in range 0..255
# return a matrix where channels have been fused to get one element so height x width
# channel data is adjusted to the range 0..levels-1 and then fused
# ch0+(ch1+levels)+..+(chN+levels*N)
#fuseChannels <- function(img, nchannels, levels){
#  imgAdj<-round(img*((levels-1)/255))
#  channelList<-splitChannels(img,nchannels)
#  fusedImg<-array(0,dim=dim(channelList[[1]]))
#  for (i in 1:nchannels){
#    fusedImg<-fusedImg+(channelList[[i]]*(levels^(i-1)))
#  }
#  fusedImg
#}

# image is  a matrix with dim height x width
# data is P0P1... in range 0..levels-1
# return the reviously fused image with data in range 0..255
#unfuseChannels <- function(img, nchannels, levels){
#  channelList<-NULL
  
#  channelList<-splitChannels(img,nchannels)
#  fusedImg<-array(0,dim=dim(channelList[[1]]))
#  for (i in 1:nchannels){
#    fusedImg<-fusedImg+(channelList[[i]]*(levels^(i-1)))
#  }
#  fusedImg
#}


