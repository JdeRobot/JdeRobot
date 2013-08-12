idealBGClusteringMC <-
function (conn, nimage, nrow, ncol, nchannel, radius) 
{
    pxDataSize <- nimage * nchannel
    imgsize <- nrow * ncol * nchannel
    npixel <- nrow * ncol
    imgSeq <- readImgSeq(conn, nimage, imgsize)
    imgClust <- foreach(i = seq(1, imgsize, by = nchannel), .combine = c) %dopar% 
        {
            pxData <- t(imgSeq[i:(i + nchannel - 1), ])
            cl <- try(qtclust(pxData, radius), TRUE)
            centers <- NULL
            if (isS4(cl)) {
                centers <- cl@centers[1, ]
            }
            else {
                centers <- apply(pxData, 2, mean)
            }
            centers
        }
    as.integer(round(imgClust))
}
