.noGenerics <- TRUE

.onLoad <- function(libname, pkgname) 
{
  library.dynam("PointLoc", pkgname, libname)
}

.onUnload <- function(libpath)
  library.dynam.unload("PointLoc", libpath)
