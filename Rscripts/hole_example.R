## This script tests 
## Point.Locator()
## Point.Locator.Naive()
## Point.Locator.Walk
## on a mesh with a hole

library(rgl)
 library(PointLoc)

nodes=rbind(c(0, 0), c(0, 1/4), c(0, 1/2),  c(0, 3/4), c(0, 1),c(1/4, 0), c(1/4, 1/4), c(1/4, 1/2),  c(1/4, 3/4), c(1/4, 1),c(1/2, 0), c(1/2, 1/4),  c(1/2, 3/4), c(1/2, 1),c(3/4, 0), c(3/4, 1/4), c(3/4, 1/2),  c(3/4, 3/4), c(3/4, 1),  c(1, 0), c(1, 1/4), c(1, 1/2),  c(1, 3/4), c(1, 1))
mesh_hole<-create.MESH.2D(nodes, segments=rbind(c(1,2), c(2,3), c(3,4),c(4,5), c(5, 10), c(10,14), c(14,19), c(19,24), c(24,23), c(23,22), c(22,21), c(21,20), c(20,15),c(15,11), c(11,6), c(6,1), c(7,8), c(8,9), c(9,13),c(13,18),c(18,17), c(17,16), c(16,12),c(12,7)), holes = rbind(c(0.3,0.3),c(0.3,0.6),c(0.6,0.6),c(0.6,0.3) ))   
plot(mesh_hole)
mesh_2<-refine.MESH.2D(mesh_hole,maximum_area = 0.002)
plot(mesh_2)
mesh_2$order = 1

points(0.8,0.5, lwd = 3, col = 'green')
res = Point.Locator(c(0.8,0.5),1,mesh_2)
points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")
