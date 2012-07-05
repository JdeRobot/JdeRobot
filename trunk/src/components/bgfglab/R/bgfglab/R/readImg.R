readImg <-
function (conn, imgsize) 
{
    readBin(conn, integer(), n = imgsize, size = 1, signed = F)
}
