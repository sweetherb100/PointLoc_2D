#' Point.Locator
#' Point locator with ADTree, it works on 2D triangular mesh
#' @param locations, a vector containing the x-y coordinates of the points to search (x1,y1,x2,y2...)
#' @param num_loc, an integer containing the number of the point to be located
#' @param meshobj, the 2D mesh
#' @return a vector containing the index of the triangle where the point are.
#' @examples library(rgl)
#' library(PointLoc)
#' nodes = rbind(c(0,0), c(10,0), c(10,10), c(0,10),cbind(runif(20, min = 0, max = 10), runif(20, min = 0, max = 10)))
#' mesh = create.MESH.2D(nodes, segments = rbind(c(1,2),c(2,3),c(3,4),c(4,1)))
#' mesh_2 = refine.MESH.2D(mesh,maximum_area = 1.)
#' mesh_2$order = 1
#' plot(mesh_2)
#' num_nodes = length(mesh_2$nodes)/2
#' loc <- c(2,3)
#' res = Point.Locator(loc, 1, mesh_2)
#' points(loc[1],loc[2], lwd = 3, col = 'green')
#' points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
#' points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
#' points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
#' lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
#' lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
#' lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")

Point.Locator<-function(locations, num_loc, meshobj)
{
  # Point.Locator Compute a solution for a Spatial Spline problem 
  #
  #     Arguments:
  #
  #     Output:

  # Indexes in C++ starts from 0, in R from 1, opportune transformation
  ##TO BE CHANGED SOON: LOW PERFORMANCES, IMPLIES COPY OF PARAMETERS
  meshobj$triangles = meshobj$triangles - 1
  meshobj$edges = meshobj$edges - 1
  meshobj$neighbors = meshobj$neighbors - 1
    
  ## Set propr type for correct C++ reading
  storage.mode(locations) <- "double"
  storage.mode(meshobj$points) <- "double"
  storage.mode(meshobj$triangles) <- "integer"
  storage.mode(meshobj$edges) <- "integer"
  storage.mode(meshobj$neighbors) <- "integer"
  storage.mode(meshobj$order) <- "integer"
  storage.mode(num_loc)<- "integer"
  
  ## Call C++ function
  bigsol <- .Call("PointLoc_c", locations, num_loc, meshobj, 
                  meshobj$order,
                  package = "PointLoc")
  
  ## Reset them correctly
  #fdobj$basis$params$mesh$triangles = fdobj$basis$params$mesh$triangles + 1
  #fdobj$basis$params$mesh$edges = fdobj$basis$params$mesh$edges + 1
  #fdobj$basis$params$mesh$neighbors = fdobj$basis$params$mesh$neighbors + 1
  return(bigsol)
}

#' Point.Locator.Naive
#' Point locator with Naive algorithm (triangle by triangle), it works on 2D triangular mesh
#' @param locations, a vector containing the x-y coordinates of the points to search (x1,y1,x2,y2...)
#' @param num_loc, an integer containing the number of the point to be located
#' @param meshobj, the 2D mesh
#' @return a vector containing the index of the triangle where the point are.
#' @examples library(rgl)
#' library(PointLoc)
#' nodes = rbind(c(0,0), c(10,0), c(10,10), c(0,10),cbind(runif(20, min = 0, max = 10), runif(20, min = 0, max = 10)))
#' mesh = create.MESH.2D(nodes, segments = rbind(c(1,2),c(2,3),c(3,4),c(4,1)))
#' mesh_2 = refine.MESH.2D(mesh,maximum_area = 1.)
#' mesh_2$order = 1
#' plot(mesh_2)
#' num_nodes = length(mesh_2$nodes)/2
#' loc <- c(2,3)
#' res = Point.Locator.Naive(loc, 1, mesh_2)

Point.Locator.Naive<-function(locations, num_loc, meshobj)
{
  # Point.Locator Compute a solution for a Spatial Spline problem 
  #
  #     Arguments:
  #
  #     Output:

  # Indexes in C++ starts from 0, in R from 1, opportune transformation
  ##TO BE CHANGED SOON: LOW PERFORMANCES, IMPLIES COPY OF PARAMETERS
  meshobj$triangles = meshobj$triangles - 1
  meshobj$edges = meshobj$edges - 1
  meshobj$neighbors = meshobj$neighbors - 1
    
  ## Set propr type for correct C++ reading
  storage.mode(locations) <- "double"
  storage.mode(meshobj$points) <- "double"
  storage.mode(meshobj$triangles) <- "integer"
  storage.mode(meshobj$edges) <- "integer"
  storage.mode(meshobj$neighbors) <- "integer"
  storage.mode(meshobj$order) <- "integer"
  storage.mode(num_loc)<- "integer"
  
  ## Call C++ function
  bigsol <- .Call("PointLocNaive_c", locations, num_loc, meshobj, 
                  meshobj$order,
                  package = "PointLoc")
  
  ## Reset them correctly
  #fdobj$basis$params$mesh$triangles = fdobj$basis$params$mesh$triangles + 1
  #fdobj$basis$params$mesh$edges = fdobj$basis$params$mesh$edges + 1
  #fdobj$basis$params$mesh$neighbors = fdobj$basis$params$mesh$neighbors + 1
  return(bigsol)
}

#' Point.Locator.Walk
#' Point locator with walking algorithm, it works on 2D triangular mesh
#' @param locations, a vector containing the x-y coordinates of the points to search (x1,y1,x2,y2...)
#' @param num_loc, an integer containing the number of the point to be located
#' @param starts, a vector containing the the index of the starting triangles
#' @param num_starts, an integer containing the number of the starting triangles
#' @param meshobj, the 2D mesh
#' @return a vector containing the index of the triangle where the point are.
#' @examples library(rgl)
#' library(PointLoc)
#' nodes = rbind(c(0,0), c(10,0), c(10,10), c(0,10),cbind(runif(20, min = 0, max = 10), runif(20, min = 0, max = 10)))
#' mesh = create.MESH.2D(nodes, segments = rbind(c(1,2),c(2,3),c(3,4),c(4,1)))
#' mesh_2 = refine.MESH.2D(mesh,maximum_area = 1.)
#' mesh_2$order = 1
#' plot(mesh_2)
#' num_nodes = length(mesh_2$nodes)/2
#' loc <- c(2,3)
#' start <- c(2,4,8)
#' res = Point.Locator.Walk(loc, 1, start, 3, mesh_2)

Point.Locator.Walk<-function(locations, num_loc, starts, num_starts, meshobj)
{
  # Point.Locator Compute a solution for a Spatial Spline problem 
  #
  #     Arguments:
  #
  #     Output:

  # Indexes in C++ starts from 0, in R from 1, opportune transformation
  ##TO BE CHANGED SOON: LOW PERFORMANCES, IMPLIES COPY OF PARAMETERS
  starts = starts - 1
  meshobj$triangles = meshobj$triangles - 1
  meshobj$edges = meshobj$edges - 1
  meshobj$neighbors = meshobj$neighbors - 1
    
  ## Set propr type for correct C++ reading
  storage.mode(locations) <- "double"
  storage.mode(meshobj$points) <- "double"
  storage.mode(meshobj$triangles) <- "integer"
  storage.mode(meshobj$edges) <- "integer"
  storage.mode(meshobj$neighbors) <- "integer"
  storage.mode(meshobj$order) <- "integer"
  storage.mode(num_loc)<- "integer"
  
  ## Call C++ function
  bigsol <- .Call("PointLocWalk_c", locations, num_loc, starts, num_starts, meshobj, 
                  meshobj$order,
                  package = "PointLoc")
  
  ## Reset them correctly
  #fdobj$basis$params$mesh$triangles = fdobj$basis$params$mesh$triangles + 1
  #fdobj$basis$params$mesh$edges = fdobj$basis$params$mesh$edges + 1
  #fdobj$basis$params$mesh$neighbors = fdobj$basis$params$mesh$neighbors + 1
  return(bigsol)
}

