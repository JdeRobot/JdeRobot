library(flexclust)

readImg <- function(conn,imgsize){
  readBin(conn,integer(),n=imgsize,size=1,signed=F)
}

#sequence is a matrix with one image per column
readImgSeq <- function(conn,n,imgsize){
  #imgSeq <- NULL
  #for (i in 1:n){
  #  img <- readBin(myconn,integer(),n=imgsize,size=1,signed=F)
  #  if (length(img) != imgsize)#read less than imgsize, so return
  #    break
  #  imgSeq <- rbind(imgSeq,img)
  #}
  matrix(readImg(conn,imgsize*n),ncol=n)
}

writeImg <- function(img, conn){
  writeBin(as.integer(img),conn,size=1)
}

writeImgSeq <- function(imgSeq, conn){
  writeImg(as.vector(imgSeq),conn)
}

calcIdealBGMean <- function(conn){
  imgDumpDataIdx <- 1 #row in dump info file for img data
  if (is.character(conn))
    myconn <- file(conn,"r")
  else
    myconn <- conn
  
  datafiles <- scan(myconn,list(0,0,0,0,""))
  close(myconn)
  
  nframes <- datafiles[[1]][imgDumpDataIdx]
  rows <- datafiles[[2]][imgDumpDataIdx]
  cols <- datafiles[[3]][imgDumpDataIdx]
  channels <- datafiles[[4]][imgDumpDataIdx]
  filename <- datafiles[[5]][imgDumpDataIdx]
  imgsize <- rows*cols*channels
  
  imgSeqFile<-file(filename,"rb")

  imgMean <- array(0.0,dim=c(1,imgsize))
  for (i in 1:nframes)
    imgMean<-imgMean+(readImg(imgSeqFile,imgsize)/nframes)

  close(imgSeqFile)
  as.integer(round(imgMean))
}

calcIdealBGMode <- function(conn,levels){
  imgDumpDataIdx <- 1 #row in dump info file for img data
  if (is.character(conn))
    myconn <- file(conn,"r")
  else
    myconn <- conn
  
  datafiles <- scan(myconn,list(0,0,0,0,""))
  close(myconn)
  
  nframes <- datafiles[[1]][imgDumpDataIdx]
  rows <- datafiles[[2]][imgDumpDataIdx]
  cols <- datafiles[[3]][imgDumpDataIdx]
  channels <- datafiles[[4]][imgDumpDataIdx]
  filename <- datafiles[[5]][imgDumpDataIdx]
  imgsize <- rows*cols*channels

  imgSeqFile<-file(filename,"rb")
  
  imgMode<-array(0,dim=c(levels,imgsize))
  for (i in 1:nframes){
    #read image and adjust values to [0..levels-1]
    img<-round(readImg(imgSeqFile,imgsize)*((levels-1)/255))+1
    imgMode[cbind(img,seq(imgsize))] <- imgMode[cbind(img,seq(imgsize))] + 1
  }
  
  close(imgSeqFile)
  
  maxAndAdjust<-function(v){#get maximun and adjust to range [0..255]
    (which.max(v)-1)*(255/(levels-1))
  }
  apply(imgMode,2,maxAndAdjust)
}

calcIdealBGClustering <- function(conn,radius){
  imgDumpDataIdx <- 1 #row in dump info file for img data
  if (is.character(conn))
    myconn <- file(conn,"r")
  else
    myconn <- conn
  
  datafiles <- scan(myconn,list(0,0,0,0,""))
  close(myconn)
  
  nframes <- datafiles[[1]][imgDumpDataIdx]
  rows <- datafiles[[2]][imgDumpDataIdx]
  cols <- datafiles[[3]][imgDumpDataIdx]
  channels <- datafiles[[4]][imgDumpDataIdx]
  filename <- datafiles[[5]][imgDumpDataIdx]
  imgsize <- rows*cols*channels

  imgSeq <-readImgSeq(filename,nframes,imgsize)

  imgClust <- NULL
  for (i in 1:rows*cols){#pixel index
    pData <- t(imgSeq[i:(i+channels-1),]) #cols RGB
    cl <- qtclust(pData,radius=radius)
    imgClust <-append(imgClust,cl@centers[1,])
  }
  as.integer(round(imgClust))
}
