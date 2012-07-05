idealBGMeanMC <-
function (conn, nimage, nrow, ncol, nchannel, rowsbyjob) 
{
    imgsize <- nrow * ncol * nchannel
    npixel <- nrow * ncol
    if ((imgsize%%rowsbyjob) != 0) {
        stop("imgsize should be divisible by rowsbyjob")
    }
    imgSeq <- readImgSeq(conn, nimage, imgsize)
    imgMean <- foreach(i = seq(1:nrow(imgSeq), by = rowsbyjob), 
        .combine = c) %dopar% apply(imgSeq[i:(i + rowsbyjob - 
        1), ], 1, mean)
    as.integer(round(imgMean))
}
