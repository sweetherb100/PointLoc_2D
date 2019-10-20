## This script tests 
## Point.Locator()
## Point.Locator.Naive()
## Point.Locator.Walk
## with different number of point

library(rgl)
library(PointLoc)

nodes = rbind(c(0,0), c(10,0), c(10,10), c(0,10),cbind(runif(20, min = 0, max = 10), runif(20, min = 0, max = 10)))
mesh = create.MESH.2D(nodes, segments = rbind(c(1,2),c(2,3),c(3,4),c(4,1)))
mesh_2 <-refine.MESH.2D(mesh, maximum_area = 0.125)
mesh_2$order=1
plot(mesh_2)
num_nodes = length(mesh_2$nodes)/2
start <-c(2,4,8)

num_loc = 1
loc <- runif(2*num_loc, min = 0, max = 10)
res = Point.Locator(loc, num_loc, mesh_2)
res2 = Point.Locator.Naive(loc, num_loc, mesh_2)
res3 = Point.Locator.Walk(loc, num_loc,start,3, mesh_2)

points(loc[1], loc[2], lwd = 3, col = 'green')
points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")

num_loc = 10
loc <- runif(2*num_loc, min = 0, max = 10)
res = Point.Locator(loc, num_loc, mesh_2)
res2 = Point.Locator.Naive(loc, num_loc, mesh_2)
res3 = Point.Locator.Walk(loc, num_loc,start,3, mesh_2)
plot(mesh_2)

for(j in 1:10){
     points(loc[2*j-1], loc[2*j], lwd = 3, col = 'green')
     points(mesh_2$nodes[mesh_2$triangle[res[j],1],1], mesh_2$nodes[mesh_2$triangle[res[j],1],2], lwd = 3, col='blue')
     points(mesh_2$nodes[mesh_2$triangle[res[j],2],1], mesh_2$nodes[mesh_2$triangle[res[j],2],2], lwd = 3, col='blue')
     points(mesh_2$nodes[mesh_2$triangle[res[j],3],1], mesh_2$nodes[mesh_2$triangle[res[j],3],2], lwd = 3, col='blue')
     lines(c(mesh_2$nodes[mesh_2$triangle[res[j],1],1], mesh_2$nodes[mesh_2$triangle[res[j],2],1]), c(mesh_2$nodes[mesh_2$triangle[res[j],1],2],mesh_2$nodes[mesh_2$triangle[res[j],2],2]), lwd = 3 ,col = "blue")
     lines(c(mesh_2$nodes[mesh_2$triangle[res[j],3],1], mesh_2$nodes[mesh_2$triangle[res[j],2],1]), c(mesh_2$nodes[mesh_2$triangle[res[j],3],2],mesh_2$nodes[mesh_2$triangle[res[j],2],2]), lwd = 3 ,col = "blue")
     lines(c(mesh_2$nodes[mesh_2$triangle[res[j],3],1], mesh_2$nodes[mesh_2$triangle[res[j],1],1]), c(mesh_2$nodes[mesh_2$triangle[res[j],3],2],mesh_2$nodes[mesh_2$triangle[res[j],1],2]), lwd = 3 ,col = "blue")
     }
