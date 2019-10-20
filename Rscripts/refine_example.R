
## This script tests 
## Point.Locator()
## Point.Locator.Naive()
## Point.Locator.Walk
## on different mesh

library(rgl)
library(PointLoc)

nodes = rbind(c(0,0), c(10,0), c(10,10), c(0,10),cbind(runif(20, min = 0, max = 10), runif(20, min = 0, max = 10)))
mesh = create.MESH.2D(nodes, segments = rbind(c(1,2),c(2,3),c(3,4),c(4,1)))
mesh_2 = mesh
plot(mesh_2)
num_nodes = length(mesh_2$nodes)/2
loc <- c(2,3)
start<- c(2,4,8)
res = Point.Locator(loc, 1, mesh_2)
res2 = Point.Locator.Naive(loc, 1, mesh_2)
res3 = Point.Locator.Walk(loc, 1,start,3, mesh_2)

points(loc[1],loc[2], lwd = 3, col = 'green')
points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")

mesh_2 = refine.MESH.2D(mesh,maximum_area = 0.5)
mesh_2$order = 1
plot(mesh_2)
num_nodes = length(mesh_2$nodes)/2
res = Point.Locator(loc, 1, mesh_2)
res2 = Point.Locator.Naive(loc, 1, mesh_2)
res3 = Point.Locator.Walk(loc, 1,start,3, mesh_2)


points(loc[1],loc[2], lwd = 3, col = 'green')
points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")



