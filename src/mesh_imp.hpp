#ifndef MESH_IMP_HPP_
#define MESH_IMP_HPP_

template <UInt ORDER>
MeshHandler<ORDER>::MeshHandler(Real* points, UInt* edges, UInt* triangles, UInt* neighbors, UInt num_nodes, UInt num_edges, UInt num_triangles, bool flag):
			points_(points), edges_(edges), triangles_(triangles), neighbors_(neighbors), num_nodes_(num_nodes), num_edges_(num_edges), num_triangles_(num_triangles), flag_(flag) 
{
	if(flag_ == 1)
	{
		ADTree<Triangle<ORDER*3>> tmp(points_, triangles_, num_nodes_, num_triangles_);
		tree_ = tmp;
	}else{
		ADTree<Triangle<ORDER*3>> tmp;
		tree_ = tmp;
	}
};


#ifdef R_VERSION_
template <UInt ORDER>
MeshHandler<ORDER>::MeshHandler(SEXP mesh):flag_(0), tree_()
{
	mesh_ 		= mesh;
	points_ 	= REAL(VECTOR_ELT(mesh_, 0));
	edges_ 		= INTEGER(VECTOR_ELT(mesh_, 6));
	triangles_  = INTEGER(VECTOR_ELT(mesh_, 3));
	neighbors_  = INTEGER(VECTOR_ELT(mesh_, 8));
	
	num_nodes_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 0), R_DimSymbol))[0];
	num_edges_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 6), R_DimSymbol))[0];
	num_triangles_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 3), R_DimSymbol))[0];
	
}

template <UInt ORDER>
MeshHandler<ORDER>::MeshHandler(SEXP mesh, bool flag):flag_(flag)
{
	mesh_ 		= mesh;
	points_ 	= REAL(VECTOR_ELT(mesh_, 0));
	edges_ 		= INTEGER(VECTOR_ELT(mesh_, 6));
	triangles_  = INTEGER(VECTOR_ELT(mesh_, 3));
	neighbors_  = INTEGER(VECTOR_ELT(mesh_, 8));
	
	num_nodes_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 0), R_DimSymbol))[0];
	num_edges_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 6), R_DimSymbol))[0];
	num_triangles_ = INTEGER(getAttrib(VECTOR_ELT(mesh_, 3), R_DimSymbol))[0];

	if(flag_ == 1)
	{
		ADTree<Triangle<ORDER*3>> tmp(points_, triangles_, num_nodes_, num_triangles_);
		tree_ = tmp;
	}else{
		ADTree<Triangle<ORDER*3>> tmp;
		tree_ = tmp;
	}	
}
#endif

template <UInt ORDER>
Point MeshHandler<ORDER>::getPoint(Id id)
{
	Point point(id, Identifier::NVAL, points_[id], points_[num_nodes_+id]);
	return point;
}

template <UInt ORDER>
Edge MeshHandler<ORDER>::getEdge(Id id)
{
	Id id_start_point = edges_[id];
	Id id_end_point = edges_[num_edges_+id];
	Edge edge(id, Identifier::NVAL, Point(id_start_point, Identifier::NVAL, points_[id_start_point], points_[num_nodes_+id_start_point]), 
						Point(id_end_point, Identifier::NVAL, points_[id_end_point], points_[num_nodes_+id_end_point]));
	return edge;
}

template <UInt ORDER>
Triangle<ORDER * 3> MeshHandler<ORDER>::getTriangle(Id id) const
{
	std::vector<Point> triangle_points;
	triangle_points.resize(ORDER * 3);
	Id id_current_point;
	for (int i=0; i<ORDER * 3; ++i)
	{
		id_current_point = triangles_[i*num_triangles_ + id];
		triangle_points[i]= Point(id_current_point, Identifier::NVAL, points_[id_current_point],points_[num_nodes_+id_current_point]);
	}
	return Triangle<ORDER * 3>(id, triangle_points);
}

template <UInt ORDER>
Triangle<ORDER * 3> MeshHandler<ORDER>::getNeighbors(Id id_triangle, UInt number) const
{
	Id id_neighbour = neighbors_[number * num_triangles_ + id_triangle];
	if (id_neighbour == -1) return Triangle<ORDER * 3>(); //Triangle with NVAL ID
	
	return getTriangle(id_neighbour);
}


template <UInt ORDER>
Triangle<ORDER * 3> MeshHandler<ORDER>::findLocationNaive(Point point) const
{
	Triangle<ORDER * 3> current_triangle; 
	
	for(Id id=0; id < num_triangles_; ++id)
	{
		current_triangle = getTriangle(id);
		if(current_triangle.isPointInside(point)) 
			return current_triangle;
	}

	return Triangle<ORDER * 3>(); //default triangle with NVAL ID
}

// Visibility walk algorithm which uses barycentric coordinate [Sundareswara et al]
//Starting triangles usually n^(1/3) points
template <UInt ORDER>
Triangle<ORDER * 3> MeshHandler<ORDER>::findLocationWalking(const Point& point, const vector<Triangle<ORDER * 3> >& starting_triangles) const
{
	
	//Real eps = 2.2204e-016,
	//	 tolerance = 10000 * eps;

	// Finding the nearest trianglefrom the proposed list
	UInt min_index = 0;
	Real distance;
	Real distance_old = (starting_triangles[0][0][0] - point[0])*(starting_triangles[0][0][0] - point[0]) + 
						(starting_triangles[0][0][1] - point[1])*(starting_triangles[0][0][1] - point[1]);
	for(UInt i=1; i < starting_triangles.size(); ++i)
	{
		distance = (starting_triangles[i][0][0] - point[0])*(starting_triangles[i][0][0] - point[0]) + 
				   (starting_triangles[i][0][1] - point[1])*(starting_triangles[i][0][1] - point[1]);
		if(distance < distance_old)
		{
			min_index = i;
			distance_old = distance;
		}
	}
	
	//Walking algorithm to the point
	Triangle<ORDER * 3> current_triangle = starting_triangles[min_index];
	
	int direction = current_triangle.getPointDirection(point);

	//Test for found Triangle, or out of border
	while(direction != -1 && current_triangle.getId() != Identifier::NVAL)
	{
		current_triangle = getNeighbors(current_triangle.getId(), direction);
		direction = current_triangle.getPointDirection(point);
	}	
	
	return current_triangle;
}


/*std::ostream & operator<<(std::ostream & out, MeshHandler const& m){
	out<< " ***** MESH  INFORMATION ******"<<std::endl;
	out<<" Num Points="<<m.num_nodes()<<" "<<" Num elements="<<m.num_triangles()<<" "
			<<"Num. edges="<<m.num_edges();
			//<<" "<<"Num Boundary Edges="<<m.num_bEdges()<<std::endl;
	out<< "POINTS:"<<std::endl;
	int oprec=out.precision(10);
	std::ios_base::fmtflags oflags=
			out.setf(std::ios_base::scientific,std::ios_base::floatfield);
	for (UInt i=0;i<m.num_nodes();++i){
		Point p=m.point(i);
		double x=p[0];
		double y=p[1];
		out<<i<<" "<<x<<" "<<y<<std::endl;
	}
	out<<" TRIANGLE CONNECTIVITY AND AREA:"<<std::endl;
	for (UInt i=0; i<m.num_elements();++i){
		Triangle t=m.triangle(i);
		out<<i<<" "<<t[0].id()<<" "<<t[1].id()<<" "<<t[2].id()<<
		  " "<<t.measure()<<"  Edge: "<<t.getEdges_id(0)<<" "<<t.getEdges_id(1)<<" "<<t.getEdges_id(2)<<std::endl;
	}
	out.precision(oprec);
	out.flags(oflags);
	return out;
}*/


template <UInt ORDER>
Triangle<ORDER * 3> MeshHandler<ORDER>::findLocationTree(const Point& point) const
{
	if(flag_ == 1){
		std::cout << "Hello! I'm using findLocationTree!" <<std::endl;
		std::vector<Real> region(4);
		bool result;
		std::set<int> found;
		int index;
		Triangle<ORDER * 3> tmp;
		region[0] = point[0];	
		region[1] = point[1];
		region[2] = point[0];
		region[3] = point[1];
	
		result = tree_.search(region, found);
		if(result == 0)
			return Triangle<ORDER * 3>();
		for (std::set<int>::iterator i = found.begin(); i != found.end(); i++) {
			index = *i;
			index = this -> tree_.pointId(index);
	  		tmp = this -> getTriangle(index);
			result = tmp.isPointInside(point);
			if(result == 1)
				return tmp;
		}
	}
	std::cout << std::endl;
	std::cout << " you need to create the tree to use this algorithm, put flag = 1 " <<std::endl;
	return Triangle<ORDER * 3>();
}

template <UInt ORDER>
void MeshHandler<ORDER>::printPoints(std::ostream & out)
{
	for(UInt i = 0; i < num_nodes_; ++i)
	{
		out<<"-"<< i <<"-"<<"("<<points_[i]<<","<<points_[num_nodes_+i]<<")"<<std::endl<<"------"<<std::endl;
	}	
}

template <UInt ORDER>
void MeshHandler<ORDER>::printEdges(std::ostream & out)
{
	
	out << "Numero lati: "<< num_edges_ <<std::endl;
	for (UInt i = 0; i < num_edges_; ++i ) 
	{
		out<<"Lato ("<<edges_[i]<<","<<edges_[num_edges_+i]<<")"<<std::endl;
	}
	
}

template <UInt ORDER>
void MeshHandler<ORDER>::printTriangles(std::ostream & out)
{
	
	out << "# Triangles: "<< num_triangles_ <<std::endl;
	for (UInt i = 0; i < num_triangles_; ++i ) 
	{
		out<<"-"<< i <<"- ";
		for( UInt k = 0; k < ORDER * 3; ++k)
			out<<triangles_[k*num_triangles_ + i]<<"   ";
		out<<std::endl;	
	}
	
}

template <UInt ORDER>
void MeshHandler<ORDER>::printNeighbors(std::ostream & out)
{
	
	out << "# Neighbors list: "<< num_triangles_ <<std::endl;
	for (UInt i = 0; i < num_triangles_; ++i ) 
	{
		out<<"-"<< i <<"- ";
		for( UInt k = 0; k < 3; ++k)
			out<<neighbors_[k*num_triangles_ + i]<<"   ";
		out<<std::endl;	
	}
	
}

template <UInt ORDER>
void MeshHandler<ORDER>::printTree(std::ostream & out)
{
	
	out << "# Tree characteristic: " <<std::endl;
	out << tree_ << std::endl;
	
}


#endif
