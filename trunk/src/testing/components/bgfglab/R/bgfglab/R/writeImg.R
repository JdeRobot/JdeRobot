writeImg <-
function (img, conn) 
{
    writeBin(as.integer(img), conn, size = 1)
}
