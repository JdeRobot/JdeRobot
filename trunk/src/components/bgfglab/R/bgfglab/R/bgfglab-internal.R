.onLoad <-
function (libname, pkgname) 
{
  options(bigmemory.typecast.warning=FALSE) #remove cast warning
  registerDoMC(cores=multicore:::detectCores())
}
