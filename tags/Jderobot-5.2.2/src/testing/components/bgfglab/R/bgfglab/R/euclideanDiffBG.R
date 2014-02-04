euclideanDiffBG <-
function (conn, nimage, nrow, ncol, nchannel, idealBG) 
{
    pxDataSize <- nimage * nchannel
    imgsize <- nrow * ncol * nchannel
    npixel <- nrow * ncol
    idealBGPlanar <- splitChannels(idealBG, nchannel)
    bgimgSeq <- readImgSeq(conn, nimage, imgsize)
    meanDiff <- vector("integer", npixel)
    for (i in seq(1, nimage)) {
        bgimgPlanar <- splitChannels(bgimgSeq[, i], nchannel)
        meanDiff <- meanDiff + (sqrt(apply((idealBGPlanar - bgimgPlanar)^2, 
            1, sum))/nimage)
    }
    meanDiff
}
