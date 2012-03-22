readImgSeq <-
function (conn, n, imgsize) 
{
    if (is.character(conn)) 
        myconn <- file(conn, "rb")
    else myconn <- conn
    imgSeq <- big.matrix(imgsize, n, type = "short", shared = "F")
    for (col in 1:n) {
        imgSeq[, col] <- readImg(myconn, imgsize)
    }
    imgSeq
}
