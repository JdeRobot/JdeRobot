splitChannels <-
function (img, nchannel) 
{
    if (length(img)%%nchannel != 0) {
        stop("img length not divisible by nchannel")
    }
    channelMat <- matrix(img, ncol = nchannel)
    for (i in 1:nchannel) {
        channelMat[, i] <- img[seq(i, length(img), by = nchannel)]
    }
    channelMat
}
