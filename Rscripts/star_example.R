## This script tests 
## Point.Locator()
## Point.Locator.Naive()
## Point.Locator.Walk
## with star mesh

library(rgl)
library(PointLoc)

 nodes <-rbind(c(0,0),c(1,2),c(0,3),c(1,3),c(2,4),c(3,3),c(4,3),c(3,2),c(4,0),c(2,1),c(2,2))

mesh <- create.MESH.2D(nodes, segments=rbind(c(1,2),c(2,3),c(3,4),c(4,5),c(5,6),c(6,7),c(7,8),c(8,9),c(9,10),c(10,1)))
mesh_2 <-refine.MESH.2D(mesh, maximum_area = 0.025)
mesh_2$order=1
plot(mesh_2)
points(3.4,0.4, lwd = 3, col = 'green')

res = Point.Locator(c(3.4,0.4),1,mesh_2)

points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")
 


