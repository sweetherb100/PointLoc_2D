#ifndef MESH_HPP_
#define MESH_HPP_

#include "FEMr.hpp"
#include "mesh_objects.hpp"
#include "bounding_box.hpp"
#include "tree_header.hpp"
#include "domain.hpp"
#include "treenode.hpp"
#include "exception_handling.hpp"
#include "adtree.hpp"


using std::vector;

//!  This class gives an object-oriented reading interface to the output of the library Triangle (Jonathan Richard Shewchuk).
/*!
 * The template parameters specify the order of its elemnts.
 * The aim of this class is to do not introduce any initialization overhead,
 * beacuse it will be called many time during the execution of a R script
*/
template <UInt ORDER>
class MeshHandler {
public:
	typedef int UInt;
	//! A constructor.
    /*!
      * The constructor permits the initialization of the mesh from an R object
      * constructed with the TriLibrary (our R wrapper for the Triangle library)
    */
    
    MeshHandler(Real* points, UInt* edges, UInt* triangles, UInt* neighbors, UInt num_nodes, UInt num_edges, UInt num_triangles):
			points_(points), edges_(edges), triangles_(triangles), neighbors_(neighbors), num_nodes_(num_nodes), num_edges_(num_edges), num_triangles_(num_triangles), flag_(0), tree_() 
{};
	//! Another constructor.
    /*!
      * The constructor permits the initialization of the mesh from an R object
      * constructed with the TriLibrary (our R wrapper for the Triangle library)
      * This constructor permit to put the flag_ = 1, so the ADTree associated to the mesh will be built and it's possible to use ADTree algorithm for point location
    */
    MeshHandler(Real* points, UInt* edges, UInt* triangles, UInt* neighbors, UInt num_nodes, UInt num_edges, UInt num_triangles, bool flag); //se flag =1 costruisco adtree!
    
    #ifdef R_VERSION_
	MeshHandler(SEXP Rmesh);
	//! Another constructor.
    /*!
      * The constructor permits the initialization of the mesh from an R object
      * constructed with the TriLibrary (our R wrapper for the Triangle library)
      * This constructor permit to put the flag_ = 1, so the ADTree associated to the mesh will be built and it's possible to use ADTree algorithm for point location
    */
	MeshHandler(SEXP Rmesh, bool flag);
    #endif
	
	~MeshHandler(){};
	
	//! A normal member returning an unsigned integer value.
    /*!
      \return The number of nodes in the mesh
    */    
    UInt num_nodes() const {return num_nodes_;}

	//! A normal member returning an unsigned integer value.
    /*!
      \return The number of nodes in the mesh
    */    
    UInt num_triangles() const {return num_triangles_;}
    
    //! A normal member returning an unsigned integer value.
    /*!
      \return The number of edges in the mesh
    */ 
    UInt num_edges() const {return num_edges_;}
    
    //! A normal member returning a Point
    /*!
     * \param id an Id argument 
      \return The point with the specified id
    */ 
    Point getPoint(Id id);
    
    //! A normal member returning an Edge
    /*!
     * \param id an Id argument 
      \return The edge with the specified id
    */ 
    Edge getEdge(Id id);
    
    //! A normal member returning a Triangle
    /*!
     * \param id an Id argument 
      \return The triangle with order coerent to that of the mesh with the specified id
    */ 
    Triangle<ORDER * 3>  getTriangle(Id id) const;
    
    //The "number" neighbor of triangle i is opposite the "number" corner of triangle i
    //! A normal member returning the Neighbors of a triangle
    /*!
     * \param id the id of the triangle
     * \param number the number of the vertex
      \return The triangle that has as an edge the one opposite to the specified
      vertex
    */ 
    Triangle<ORDER * 3> getNeighbors(Id id_triangle, UInt number) const;

    //! A normal member returning the ADTree
    /*!
     *  \return The ADTree, the nodes contain the index of the triangle in the mesh
    */ 
    const ADTree<Triangle<ORDER * 3>> &  getTree() const {return tree_;};
     
    void printPoints(std::ostream & out);
    void printEdges(std::ostream & out);
    void printTriangles(std::ostream & out);
    void printNeighbors(std::ostream & out);
    void printTree(std::ostream & out);
    
     //! A normal member returning the triangle on which a point is located
    /*!
     * This method implements a simply research between all triangle of the mesh
     * \param point the point we want to locate
      \return The triangle that contains the point
    */ 
    Triangle<ORDER * 3> findLocationNaive(Point point) const;
    
     //! A normal member returning the triangle on which a point is located
    /*!
     * This method implements a Visibility Walk Algorithm (further details in: Walking in a triangulation, Devillers et al)
     * \param point the point we want to locate
     * \param starting_triangles a vector of points that specifies the poposed starting 
     * points for the walking algorithm
      \return The triangle that contains the point
    */ 
    Triangle<ORDER * 3> findLocationWalking(const Point& point, const vector<Triangle<ORDER * 3> >& starting_triangles) const;

     //! A normal member returning the triangle on which a point is located
    /*!
     * This method implements a ADTree algorithm
     * \param point the point we want to locate
      \return The triangle that contains the point
    */ 
    Triangle<ORDER * 3> findLocationTree(const Point& point) const;
    
    //int readMesh(std::string const & file);
	//double measure()const;
	//bool checkmesh()const;
private:
	#ifdef R_VERSION_
	SEXP mesh_;
	#endif
	Real *points_;
	UInt *edges_;
	UInt *triangles_;
	UInt *neighbors_;
	
	UInt *border_edges; //contiene lista id_edges al bordo
	UInt num_nodes_, num_edges_, num_triangles_;
	/// flag_ = 1 -> adtree is built, flag_ = 0 -> adtree is empty
	bool flag_;  //se flag_=1 l'adtree contiene tutte le info della mesh
	/// is the adtree associated to the mesh
	ADTree<Triangle<ORDER*3>> tree_; //contiene l'albero, se inizializzato si può usare per Point_Location
	
};

#include "mesh_imp.hpp"

#endif
