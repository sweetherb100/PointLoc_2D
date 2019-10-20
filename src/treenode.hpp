/**
 *	\file treenode.hpp
 *	\author Alessandra Cardani
 *	\author Pigoli Davide
 *  \author Prada Daniele
 */

#ifndef TREENODE_HPP_
#define TREENODE_HPP_

#include "FEMr.hpp"
//#include "mesh_objects.hpp"
//#include "bounding_box.hpp"

/**	\class TreeNode
 * 	\brief Class defining a tree node.
* \param Shape: template parameter, is the original shape, in the treenode are stored the bbox of the original object and an index that identify that object
 */
template<class Shape>
class TreeNode{
protected:
  /** Position of the father node.
   *
   * 	It's used by the algorithm for deleting a tree node.
   */
  int father_;
  /// Positions of left and right children.
  int children_[2];
  ///Bounding Box of the object to be stored
  Box<Shape::dp()> box_;
  /// The id (in the mesh) of the Triangle that create the BBox
  Id id_;
//dò per scontato che ci sia un oggetto mesh che contenga tutte le forme (salvate in qualche modo) e che le identifichi attraverso un id di tipo Uint!
//generalizzando si potrebbe usare un puntatore a Shape (parametro template della forma generica, può essere un triangolo, o l'id stesso se si vuole tornare al caso precedente), in teoria poi non devo distruggere la memoria perchè la forma è salvata in una struttra mesh che deve rimanere inalterata
//Shape * id_;
//un'altra possibilità è salvare la forma anche nella struttura mesh con un shared_ptr e mettere anche qui uno shared_ptr<Shape>
  /** Additional informations to be stored in the node.
   *
   * 	Maybe a more sophisticated object than a std::vector<int> can be more useful.
   */
  std::vector<int> keys_;
public:
  /**	Default constructor.
   *
   *	It's fundamental in creating a vector of TreeNode objects.
   */
  TreeNode(): father_(0), box_() {
    children_[0] = 0;
    children_[1] = 0;
    id_ = std::numeric_limits<UInt>::max();
  }
  /**	Another constructor.
   *
   * 	Shape is the shape from the id, it's need the constructor for box, from shape! it works with Triangle, or Box, it can be extended
   */
  TreeNode(Id const id, Shape shape,  std::vector<int> const & keys): father_(0), keys_(keys), box_(shape) {
    children_[0] = 0;
    children_[1] = 0;
    id_ = id;
  }
  /// Sets the father.
  inline void setfather(int const & ifth) { father_ = ifth; }
  /**	\brief Sets a child.
   *
   * 	\param[in] flag Index of the child to be set. \n
   * 					If:
   * 					<ul>
   * 					<li> flag = 0, set the left child
   * 					<li> flag = 1, set the right child
   * 					</ul>
   * 	\param[in] child The child node.
   */
  inline void setchild(short int const & flag, int const & child) { children_[flag] = child; }
  /// Returns the father.
  inline int getfather() const { return father_; }
  /**	\brief Returns a child.
   *
   * 	\param[in] flag Index of the child to be returned. \n
   * 					If:
   * 					<ul>
   * 					<li> flag = 0, return the left child
   * 					<li> flag = 1, return the right child
   * 					</ul>
   */
  inline int getchild(short int const & flag) const { return children_[flag]; }
  ///	Sets the coordinates of the bbox stored in the node.
  inline void setcoords(std::vector<Real> const & data) { box_.set(data); }
  /// Gets the i-th coordinate of the bounding box of the object stored in the node.
  inline Real getcoord(int const & i) const { return box_[i]; }
  ///	Sets the id stored in the node.
  inline void setid(Id id) { id_=id; }
  /// Gets id stored in the node.
  inline Id getid() const { return id_; }
  /// Gets the i-th coordinate value of the p-th vertex.
//non utile avendo usato bbox anzichè shape
//  inline Real getvertex(int const & p, int const & i) //{ return _shape.vertex(p, i); }
  /// Sets the additional informations to be stored in the node.
  inline void setkeys(std::vector<int> const & k) { keys_ = k; }
  /// Gets the additional informations stored in the node.
  inline std::vector<int> getkeys() const {
    std::vector<int> info(keys_);
    return info;
  }
  /// Returns a reference to box_.
  inline Box<Shape::dp()> & getbox() { return box_; }
  ///print information about the treenode
  void print(std::ostream & out) const;
};

#include "treenode_imp.hpp"

#endif /* TREENODE_HPP_ */
