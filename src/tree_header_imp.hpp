#ifndef TREE_HEADER_IMP_HPP_
#define TREE_HEADER_IMP_HPP_


template<class T>
Tree_Header<T>::Tree_Header(int const & ntree,int const & nk, Domain<T> const & d):
	tree_loc_(ntree), tree_lev_(0), ndimp_(T::dp()), nkey_(nk), ndimt_(T::dt()), nele_(0), iava_(1), iend_(1), tree_domain_(d) {

	std::vector<TreeNode<T> > foo;
	if(foo.max_size() < unsigned(tree_loc_ + 1))
		/* If there is no enough space to store the requested nodes and
		 * the tree head, a LocLengthError exception is thrown.
		 */
		throw(LocLengthError<T>(foo.max_size(), tree_loc_));
}

template<class T>
void Tree_Header<T>::stml(int const & nt) {
	std::vector<TreeNode<T> > foo;
	if(foo.max_size() < unsigned(nt + 1))
		/* If there is no enough space to store the requested nodes and
		 * the tree head, a LocLengthError exception is thrown.
		 */
		throw(LocLengthError<T>(foo.max_size(), nt));
	tree_loc_ = nt;
}

template<class T>
std::ostream & operator<<(std::ostream & ostr, Tree_Header<T> const & head) {
	ostr << std::endl << std::endl;
	ostr << "General informations about the tree" << std::endl;
	ostr << "----------------------------------" << std::endl << std::endl;
	ostr << "Tree memory locations: " << head.tree_loc_ << std::endl;
	ostr << "Number of tree levels: " << head.tree_lev_ << std::endl;
	ostr << "Number of physical space dimension: " << head.ndimp_ << std::endl;
	ostr << "Number of pieces of information carried by the tree: " << head.nkey_ << std::endl;
	ostr << "Number of dimensions used for the search: " << head.ndimt_ << std::endl;
	ostr << "Number of logical locations currently used in the tree: " << head.nele_;
	ostr << head.tree_domain_ << std::endl;

	return ostr;
}

template<class T>
Tree_Header<T> createtreeheader(int const & nt,int const & nk, Domain<T> const & d) {
	try {
		Tree_Header<T> hd(nt, nk, d);
		return hd;
	}
	catch(LocLengthError<T> lo) {
		std::cout << std::endl << std::endl;
		std::cout << "warning!	createtreeheader : max dimension exceeded" << std::endl;
		std::cout << "the limit is " << lo.getmaxtreeloc()-1
				<< " while needed at least " << lo.gettreeloc() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

template<class T>
void Tree_Header<T>::settreeloc(int const & nt) {
	try {
		stml(nt);
	}
	catch(LocLengthError<T> lo) {
		std::cout << std::endl << std::endl;
		std::cout << "warning!	settreeloc : max dimension exceeded" << std::endl;
		std::cout << "the limit is " << lo.getmaxtreeloc()-1
				<< " while requested " << lo.gettreeloc() << std::endl;
		std::cout << "increasing tree memory locations up to the limit" << std::endl;
		stml(lo.getmaxtreeloc()-1);
	}
}

#endif /* TREE_HEADER_IMP_HPP_ */
