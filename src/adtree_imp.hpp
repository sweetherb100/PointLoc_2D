#ifndef ADTREE_IMP_HPP_
#define ADTREE_IMP_HPP_

template<class Shape>
ADTree<Shape>::ADTree(Tree_Header<Shape> const & header): header_(header) {
  /*
   * The first element in the tree nodes vector is the head.
   * It stores the address o the tree root (i.e. the first node in the tree).
   * If it stores 0 it means that the tree is empty.
   */
  data_.reserve(header_.gettreeloc()+1);

  // Id, obj, foo_k are arbitrary parameters. Remember that data_[0] is the head, not a tree node.
  std::vector<int> foo_k(header_.getnkey(), 0);
  Shape obj;
  Id id = std::numeric_limits<UInt>::max();
  data_.push_back(TreeNode<Shape>(id,obj, foo_k));

}

template<class Shape>
ADTree<Shape>::ADTree(Real const * const points, UInt const * const triangle, const UInt num_nodes, const UInt num_triangle) {
  if(typeid(Shape) != typeid(Triangle<3>) && typeid(Shape) != typeid(Triangle<6>)) {
    std::cout << std::endl << std::endl;
    std::cout << "error! ADTree<Shape>::ADTree(Real const * const points, Uint const * const triangle, Uint num_nodes, Uint num_triangle) : bad template parameter" << std::endl << std::endl;
    std::cout << "In order to build the tree associated to a 2D (triangle) mesh object" << std::endl;
    std::cout << "template parameter must be equal to 'Triangle<3>' or 'Triangle<6>' and not to " << typeid(Shape).name() << std::endl;
    std::exit(EXIT_FAILURE);
  } else {
    // Build the tree.
    std::vector<std::vector<Real> > vcoord;
    vcoord.resize(2);
    for (int i = 0; i < 2; i++)
    {
	vcoord[i].resize(num_nodes);
	for(int j = 0; j < num_nodes; j++)
	{
	   vcoord[i][j] = points[i*num_nodes + j];	
	}
    }
	
    Domain<Shape> mydom(vcoord);
    Tree_Header<Shape> myhead = createtreeheader<Shape>(num_triangle, 0, mydom);

    header_ = myhead;
    data_.reserve(header_.gettreeloc()+1);
    std::vector<int> foo_k(header_.getnkey(), 0);
    Shape obj;
    Id id = std::numeric_limits<UInt>::max();
    data_.push_back(TreeNode<Shape>(id,obj, foo_k)); //uso costruttore di default

    // Fill the tree.
      UInt idpt;
      std::vector<Real> point(6);
      for ( int i = 0; i < num_triangle; i++ )
      {
	idpt = triangle[0*num_triangle + i];
      	point[0] = points[idpt];
     	point[1] = points[idpt+num_nodes];
	idpt = triangle[1*num_triangle + i];
     	point[2] = points[idpt];
     	point[3] = points[idpt+num_nodes];
	idpt = triangle[2*num_triangle + i];
     	point[4] = points[idpt];
      	point[5] = points[idpt+num_nodes];

     	std::vector<int> key;

	this -> addtreenode(i ,point, key);
       }
  }
}


template<class Shape>
int ADTree<Shape>::adtrb(Id shapeid, std::vector<Real> const & coords, std::vector<int> const & keys) {
  /*
   * We will trasverse the tree in preorder fashion.
   * ipoi and ifth will store the location of the current node
   * and that of the father node.
   *
   * A value 0 for the left_link or the right_link indicates
   * a void sibling address.
   * Therefore, if left_link = right_link = 0 the node is a leaf.
   *
   * In the algorithm, when ipoi = 0 we have
   * reached a void sibling, where we can add a new node.
   */
  int nele = header_.getnele();
  int iava = header_.getiava();
  int iend = header_.getiend();
  int dimt = header_.getndimt();
  std::vector<Real> x;
  x.reserve(dimt);

  // At the start ipoi is set to the address of the root and ifth to 0.
  int ipoi = data_[0].getchild(0);
  int ifth = 0;

  /* We scale the dimension of the "bounding box" of the Shape object
   * with coordinate values given by coords.
   */
  Shape shapeobj(coords);
  Box<Shape::dp()> shapebox(shapeobj);
  for(int i = 0; i < header_.getndimt(); ++i) {
    Real orig = header_.domainorig(i);
    Real scal = header_.domainscal(i);
    Real val  = (shapebox[i]-orig)*scal;
    if( (val<0.) || (val>1.) )
      // Object out of domain.
      throw(TreeDomainError<Shape>(nele+1, Shape::coordsize(), coords));
    x.push_back(val);
  }
  /* x now stores the scaled coordinated of the object's bounding box: x_i \in (0,1)
   * If Shape = Point<NDIMP> or Box<NDIMP>, the object and its bounding box coincide.
   */

  /*
   * The variable currentlev will contain the current level.
   * We start from level 0.
   */
  int currentlev = 0;

  short int edge = 0;
  while(ipoi != 0) {
    // Get the current dimension.
    int id = searchdim(currentlev, dimt);

    /*
     * We take advantage of the fact that we are only descending
     * the tree. Then we recursively multiply by 2 the coordinate.
     */
    x[id] *= 2.;
    ifth = ipoi;

    if(x[id] < 1.)
      // Go to the left.
      edge = 0;
    else {
      // Go to the right.
      edge = 1;
      --x[id];
    }
    // Next level.
    ++currentlev;
    ipoi = data_[ifth].getchild(edge);
  }

  if( iava == iend ) {
    /*
     * The list of available node is empty, so we have to put the new node
     * in the yet unassigned portion of the vector storing the tree.
     */
    data_.push_back(TreeNode<Shape>(shapeid, shapeobj, keys));
  }
  else {
    std::vector<Real> bcoords = {shapebox[0], shapebox[1], shapebox[2], shapebox[3]};
    data_[iava].setcoords(bcoords);
    data_[iava].setkeys(keys);
  }

  int neletmp = nele;
  // Add the node in the next available location.
  int ipoitmp = iava;
  // iava is the next available location.
  int iavatmp = data_[ipoitmp].getchild(0);
  ++neletmp;
  if(iavatmp == 0) {
    if( iend > header_.gettreeloc() ) {
      // Not enough space.
      throw TreeAlloc<Shape>();
    }
  }



  // Add the node in the next available location.
  ipoi = iava;
  // Set the father link to the new node.
  data_[ifth].setchild(edge, ipoi);
  // iava is the next available location.
  iava = data_[ipoi].getchild(0);
  ++nele;
  if(iava == 0) {
    if( iend > header_.gettreeloc() ) {
      // Not enough space.
      throw TreeAlloc<Shape>();
    }
    ++iend;
    iava = iend;
  }

  /*
   * Set left_link and right link of ipoi equal to 0: this node
   * is a leaf. This is already done by the constructor of TreeNode class
   * when we put the node in the "free store".
   */
  data_[ipoi].setchild(0, 0);
  data_[ipoi].setchild(1, 0);
  data_[ipoi].setfather(ifth);

  // Store back header informations.
  header_.setiend(iend);
  header_.setiava(iava);
  header_.setnele(nele);
  if(currentlev > header_.gettreelev()) {
    header_.settreelev(currentlev);
    if(currentlev > LevRuntimeError<Shape>::getmaxtreelev())
      // Current maximum number of tree levels exceeded.
      throw LevRuntimeError<Shape>();
  }

  return ipoi;
}

template<class Shape>
int ADTree<Shape>::handledomerr(Id shapeid, std::vector<Real> const & coords, std::vector<int> const & keys) {
  try {
    int iloc = adtrb(shapeid, coords, keys);
    return iloc;
  }
  catch(TreeDomainError<Shape> de) {
    // Handle a TreeDomainError exception.
    std::cout << "error!	" << de.getnelep1() << "-th object which is to be inserted into the tree is out of domain"
	      << std::endl;
    std::cout << "Coordinates" << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << de;
    std::exit(EXIT_FAILURE);
  }
}

template<class Shape>
int ADTree<Shape>::handletreealloc(Id shapeid, std::vector<Real> const & coords, std::vector<int> const & keys) {
  try {
    int iloc = handledomerr(shapeid, coords, keys);
    return iloc;
  }
  catch(TreeAlloc<Shape>) {
    // Handle a TreeAlloc exception.
    std::cout << "warning! not enough space" << std::endl;
    std::cout << "increasing tree memory locations up to 1.5 * number of current locations..." << std::endl;
    int locv = header_.gettreeloc();
    int delta = int(locv/2);
    header_.settreeloc(locv+delta);
    if(locv == header_.gettreeloc()) {
      std::cout << "error! no more space to add a new node" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    data_.resize(header_.gettreeloc()+1);
    int iloc = handledomerr(shapeid, coords, keys);
    return iloc;
  }
}

template<class Shape>
int ADTree<Shape>::handleleverr(Id shapeid, std::vector<Real> const & coords, std::vector<int> const & keys) {
  try {
    int iloc = handletreealloc(shapeid, coords, keys);
    return iloc;
  }
  catch(LevRuntimeError<Shape>) {
    // Handle a LevRuntimeError exception.
    std::cout << "warning! maximum number of tree levels exceeded" << std::endl;
    std::cout << "the limit is " << LevRuntimeError<Shape>::getmaxtreelev() << std::endl;
    std::cout << "setting the new limit to" << int(LevRuntimeError<Shape>::getmaxtreelev() * 1.1) << std::endl;
    LevRuntimeError<Shape>::setmaxtreelev(int(LevRuntimeError<Shape>::getmaxtreelev() * 1.1));

    int iloc = handletreealloc(shapeid, coords, keys);
    return iloc;
  }
}

template<class Shape>
int ADTree<Shape>::addtreenode(Id shapeid, std::vector<Real> const & coords, std::vector<int> const & keys) {
  return handleleverr(shapeid, coords, keys);
}

template<class Shape>
void ADTree<Shape>::gettri(int const & loc, std::vector<int> & info, std::vector<Real> & coord, Id & id) {
  info = data_[loc].getkeys();
  coord.clear();
  coord.reserve(Shape::dt());
  for (int i = 0; i< Shape::dt(); ++i) {
    coord.push_back(data_[loc].getcoord(i));
  }
   id = data_[loc].getid();
}

template<class Shape>
bool ADTree<Shape>::search(std::vector<Real> const & region, std::set<int> & found) const {
  // Start preorder traversal at level 0 (root).
  int ipoi = data_[0].getchild(0);
  int ipoiNext = 0;
  int dimp = header_.getndimp();
  int dimt = header_.getndimt();

  // xl is the origin point for searching.
  std::vector<Real> xl(dimt, 0);

  std::vector<Real> box(2*dimp, 0);
  std::vector<Real> xel(2*dimp, 0);

  std::stack<std::vector<Real> > _stack;
  found.clear();

  int lev = 0;

  // Check if the region intersects the domain.
  for(int i = 0; i < dimp; ++i) {
    double orig = header_.domainorig(i);
    double scal = header_.domainscal(i);
    if(region[i] > orig+(1./scal)) {
      return 0;
    }
    if(region[i+dimp] < orig) {
      return 0;
    }
  }

  // Rescale the region.
  for(int i = 0; i < dimp; ++i) {
    Real orig = header_.domainorig(i);
    Real scal = header_.domainscal(i);
    box[i] = (region[i]-orig)*scal;
    box[i+dimp] = (region[i+dimp]-orig)*scal;
  }

  /*
   * To traverse a non-empty binary tree in preorder,
   * perform the following operations recursively at each node, starting with the root node:
   * > visit the root;
   * > traverse the left subtree;
   * > traverse the right subtree.
   */
  while(ipoi != 0) {
    do {
      for(int i = 0; i < dimt; ++i) {
	Real orig = header_.domainorig(i);
	Real scal = header_.domainscal(i);
	xel[i] = (data_[ipoi].getcoord(i)-orig)*scal;
      }

      if(dimp == dimt) {
	/*
	 * This function works when we have either points or boxes.
	 * In the first case we have to repeat the object's coordinates.
	 */
	for(int i = 0; i < dimp; ++i) xel[i+dimp] = xel[i];
      }

      // Does the element intersect box?
      int flag = 0;
      for(int i = 0; i < dimp; ++i) {
	if(xel[i] > box[i+dimp]) { flag = 1; }
	if(xel[i+dimp] < box[i]) { flag = 1; }
      }

      if(flag == 0) {
	found.insert(ipoi);
	/*
	 * Put here all the action needed when an element is found which
	 * intersects the box.
	 */
      }

      // Traverse left subtree.
      ipoiNext = data_[ipoi].getchild(0);
		    
      // Check if subtree intersects box.
      if(ipoiNext != 0) 
	{
	  int id = searchdim(lev, dimt);
	  double amov = delta(lev, dimt);
			
	  if (id < dimp) 
	    {
	      if(xl[id] > box[id+dimp]) 
		{
		  ipoiNext = 0;
		}
	    }
	  else {
	    if(xl[id]+amov < box[id-dimp]) {
	      ipoiNext = 0;
	    }
	  }
	}
		    
      /*
       * Left subtree is neither null nor 'external'.
       * Push ipoi onto the stack.
       */
      if (ipoiNext != 0) {
	std::vector<Real> stackele;
	stackele.reserve(dimt+2);
	stackele.push_back(ipoi);
	for (int i = 0; i < dimt; ++i) {
	  stackele.push_back(xl[i]);
	}
	stackele.push_back(lev);
	_stack.push(stackele);
	ipoi = ipoiNext;
	++lev;
      }
    } while(ipoiNext != 0);

    // Traverse right subtree.
    ++lev;
    ipoi = data_[ipoi].getchild(1);
    do {
      while (ipoi == 0) {
	// If right_link is null we have to get the point from the stack.
	if(_stack.empty()) {
	  return !found.empty();
	}
	std::vector<Real> stackele(_stack.top());
	_stack.pop();
	ipoi = data_[stackele[0]].getchild(1);
	for(int i = 0; i < dimt; ++i) {
	  xl[i] = stackele[i+1];
	}
	lev = stackele[dimt+1]+1;
      }

      /*
       * Check if the subtree intersects box. Otherwise set right_link = 0,
       * pop new node from the stack and adjourn level.
       *
       * lev-1 is the level of the parent node, which directs the search.
       */
      int id = searchdim(lev-1, dimt);
      Real amov = delta(lev-1, dimt);

      // Reset xl (we have just traversed the tree to the right).
      xl[id] = xl[id]+amov;

      // Check if the subtree intersects box.
      Real x1 = xl[id];
      Real x2 = xl[id]+amov;
      if(id < dimp) {
	if(x1 > box[id+dimp]) {
	  ipoi=0;
	}
      }
      else {
	if(x2 < box[id-dimp]) {
	  ipoi=0;
	}
      }
    } while(ipoi == 0);

  }

  return !found.empty();
}

template<class Shape>
void ADTree<Shape>::deltreenode(int const & index) {
  if(index < header_.getiend()) {
    int nele = header_.getnele();
    int ipoi = index;
    int iava = header_.getiava();
    int ifth = data_[ipoi].getfather();

    --nele;
    int lchild = data_[ipoi].getchild(0);
    int rchild = data_[ipoi].getchild(1);
    int whichchild;

    if(data_[ifth].getchild(0) == ipoi) {
      whichchild = 0;
    }
    else {
      whichchild = 1;
    }

    if( (lchild == 0) && (rchild == 0) ) {
      // If the location is already a leaf, we can just release it.
      data_[ifth].setchild(whichchild, 0);
    }
    else {
      int ipoiNext = ipoi;
      int flag = 0;
      int il = data_[ipoiNext].getchild(0);
      int ir = data_[ipoiNext].getchild(1);
      // Traverse the tree to find an empty leaf.
      while( (il != 0) || (ir != 0) ) {
	il = data_[ipoiNext].getchild(0);
	ir = data_[ipoiNext].getchild(1);
	if(il != 0) {
	  flag = 0;
	  ipoiNext = il;
	}
	else if(ir != 0) {
	  flag = 1;
	  ipoiNext = ir;
	}
      }
      data_[ifth].setchild(whichchild, ipoiNext);
      int foo_f = data_[ipoiNext].getfather();
      data_[foo_f].setchild(flag, 0);
      data_[ipoiNext].setfather(ifth);
      if(ipoiNext != lchild) {
	data_[ipoiNext].setchild(0, lchild);
	data_[lchild].setfather(ipoiNext);
      }
      if(ipoiNext != rchild) {
	data_[ipoiNext].setchild(1, rchild);
	data_[rchild].setfather(ipoiNext);
      }
    }

    // Add the erased location to iava
    data_[ipoi].setchild(0, iava);
    data_[ipoi].setchild(1, 0);

    // Store back header informations.
    header_.setiava(ipoi);
    header_.setnele(nele);
  }
}

template<class S>
std::ostream & operator<<(std::ostream & ostr, ADTree<S> const & myadt) {
  ostr << myadt.header_;

  return ostr;
}


#endif /* ADTREE_IMP_HPP_ */
