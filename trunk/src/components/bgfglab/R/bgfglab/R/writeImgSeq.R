writeImgSeq <-
function (imgSeq, conn) 
{
    if (is.character(conn)) 
        myconn <- file(conn, "rb")
    else myconn <- conn
    for (col in 1:ncol(imgSeq)) {
        writeImg(imgSeq[, col], myconn)
    }
}
