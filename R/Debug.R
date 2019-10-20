#' Test for class Point:
#' Test of some funtionalities on default point (users can't modify them), I test all the method I use or I modified: constructor, print, operator [], physical and search dimension
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_Point<- function(n){
	result<-.C("Test_Point_c", as.integer(n))
	result
}

#' Test for class Triangle:
#' Test of some funtionalities on default triangle (users can't modify them), I test all the method I use or I modified: constructor, print, operator [], physical and search dimension, isPointInside
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_Triangle<- function(n){
	result<-.C("Test_Triangle_c", as.integer(n))
	result
}


#' Test for class Box:
#' Test of some funtionalities on default box (users can't modify them), I test all the method I use or I modified: constructor, print, operator [], physical, search and vector dimension, set coordinates
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_Box<- function(n){
	result<-.C("Test_Box_c", as.integer(n))
	result
}

#' Test for class Domain
#' Test of some funtionalities on default domain (users can't modify them) with different shape, I test all method I use or I modified: constructor, operator <<, origin, scalingfactors, set and modify tolerance/mindiff
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_Domain<- function(n){
	result<-.C("Test_Domain_c", as.integer(n))
	result
}

#' Test for class TreeNode
#' Test of some funtionalities on default treenode (users can't modify them), I test all method I use or I modified: constructor, print, set/get father/child/box/id/keys
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_TreeNode<- function(n){
	result<-.C("Test_TreeNode_c", as.integer(n))
	result
}


#' Test for class Tree_Header
#' Test of some funtionalities on default Tree_Header (users can't modify them), I test all method I use or I modified: constructor, operator <<, get/change information on the TreeHeader
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_TreeHeader<- function(n){
	result<-.C("Test_TreeHeader_c", as.integer(n))
	result
}

#' Test for class ADTree
#' Test of some funtionalities on default ADTree (users can't modify them), I test all method I use or I modified: constructor, addtreenode, deltrenode, search, visualization of information 
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_ADTree<- function(n){
	result<-.C("Test_ADTree_c", as.integer(n))
	result
}

#' Test for class MeshHandler
#' Test of some funtionalities on default Mesh (users can't modify them), I test the constructor with the new flag and the search algorithm from ADTree
#' @param n is an int and is the input integer (n != 0)
#' @return An int with value:
#'         \item{0}{if it's all ok} 
#'         \item{1}{if there's some problem} 

Test_MeshHandler<- function(n){
	result<-.C("Test_MeshHandler_c", as.integer(n))
	result
}


