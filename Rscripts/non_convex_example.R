## This script tests 
## Point.Locator()
## Point.Locator.Naive()
## Point.Locator.Walk
## with non convex mesh


 library(rgl)
 library(PointLoc)
 nodes = rbind(c(0,0),c(0,1),c(0.5,0.5),c(1,1),c(1,0))
 mesh = create.MESH.2D(nodes, segments=rbind(c(1,2),c(2,3),c(3,4),c(4,5),c(5,1)))
 plot(mesh)
 mesh_2 = refine.MESH.2D(mesh, maximum_area = 0.005)
 plot(mesh_2)
 mesh_2$order = 1
points(0.91,0.82, lwd = 3, col = 'green')
 res = Point.Locator(c(0.91,0.82),1,mesh_2)
 points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
 points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
 points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
 lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
 lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
 lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")
 
mesh_2 = refine.MESH.2D(mesh, maximum_area = 0.001)
 mesh_2$order = 1
 plot(mesh_2)
points(0.91,0.82, lwd = 3, col = 'green')
 res = Point.Locator(c(0.91,0.82),1,mesh_2)
 points(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,1],2], lwd = 3, col='blue')
 points(mesh_2$nodes[mesh_2$triangle[res,2],1], mesh_2$nodes[mesh_2$triangle[res,2],2], lwd = 3, col='blue')
 points(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,3],2], lwd = 3, col='blue')
 lines(c(mesh_2$nodes[mesh_2$triangle[res,1],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,1],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
 lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,2],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,2],2]), lwd = 3 ,col = "blue")
 lines(c(mesh_2$nodes[mesh_2$triangle[res,3],1], mesh_2$nodes[mesh_2$triangle[res,1],1]), c(mesh_2$nodes[mesh_2$triangle[res,3],2],mesh_2$nodes[mesh_2$triangle[res,1],2]), lwd = 3 ,col = "blue")

