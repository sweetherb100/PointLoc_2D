#ifndef DOMAIN__IMP_HPP_
#define DOMAIN__IMP_HPP_

template<class T>
Real Domain<T>::tolerance_ = 1.e-3;

template<class T>
Real Domain<T>::mindiff_ = std::numeric_limits<Real>::min();

template<class T>
Domain<T>::Domain(){
	origin_.resize(T::dt());
	std::vector<Real> tmp(T::dt(),1);
	scalingfactors_ = tmp;
	}

template<class T>
Domain<T>::Domain(std::vector<std::vector<Real> > const & coord) {
	int ndimp = T::dp();
	origin_.resize(T::dt());
	scalingfactors_.resize(T::dt());

	/* Find geometric limits.
	 *
	 * If loops are put outside for loops in order to improve performance.
	 */
	if(ndimp == int(coord.size())) {
		// T is equal to Point<NDIMP>, Triangle<NDIMP> or Tetrahedron.
		//***************I am not convinced about Tetrahedron
		//**************Why do you write <NDIMP> at end of Point and Triangle?
		if(T::dp() == T::dt()) {
			// T is equal to Point<NDIMP>
			for(int i = 0; i < ndimp; ++i) {
				origin_[i] = *(std::min_element(coord[i].begin(), coord[i].end()));
				scalingfactors_[i] = *(std::max_element(coord[i].begin(), coord[i].end()));

				// Add the tolerance.
				Real delta = scalingfactors_[i] - origin_[i];
				origin_[i] -= delta*gettolerance();
				scalingfactors_[i] += delta*gettolerance();

				delta = scalingfactors_[i] - origin_[i];
				scalingfactors_[i] = 1./std::max(delta, getmindiff());
				//*******************it comes out as 1./2*(max-min)*tolerance?!?!
				//**********shouldn't it be 1./(max(coord(*,1:number of points)) - min(coord(*,1:number of points)))????
			}
		} else {
			// T is equal to Triangle<NDIMP> or Tetrahedron.
			for(int i = 0; i < ndimp; ++i) {
				origin_[i] = *(std::min_element(coord[i].begin(), coord[i].end()));
				scalingfactors_[i] = *(std::max_element(coord[i].begin(), coord[i].end()));

				// Add the tolerance.
				double delta = scalingfactors_[i] - origin_[i];
				//************shoudn't this be double like above??
				origin_[i] -= delta*gettolerance();
				scalingfactors_[i] += delta*gettolerance();

				delta = scalingfactors_[i] - origin_[i];
				scalingfactors_[i] = 1./std::max(delta, getmindiff());
				//*******************it comes out as 1./2*(max-min)*tolerance?!?!
				//**********shouldn't it be 1./(max(coord(*,1:number of points)) - min(coord(*,1:number of points)))????

				/* Repeat the limits because tree dimension is in fact 2 * physical space dimension
				 * because the tree contains triangle or tetrahedron bounding boxes.
				 */
				origin_[i + ndimp] = origin_[i];
				scalingfactors_[i + ndimp] = scalingfactors_[i];
			}
		}
	} else {
		// T is equal to Box<NDIMP>.
		//***************but box should have the same dimension if it is 2-dimen??
		for(int i = 0; i < ndimp; ++i) {
			origin_[i] = *(std::min_element(coord[i].begin(), coord[i].end()));

			/* This statement is necessary when representing a rectangle with the corner having
			 * minimum coordinates and the opposite one.
			 */
			scalingfactors_[i] = *(std::max_element(coord[i+ndimp].begin(), coord[i+ndimp].end()));

			// Add the tolerance.
			double delta = scalingfactors_[i] - origin_[i];
			//************shoudn't this be double like above??
			origin_[i] -= delta*gettolerance();
			scalingfactors_[i] += delta*gettolerance();

			delta = scalingfactors_[i] - origin_[i];
			scalingfactors_[i] = 1./std::max(delta, getmindiff());

			/* Repeat the limits because tree dimension is in fact 2 * physical space dimension
			 * because because a box is defined by two points.
			 */
			origin_[i + ndimp] = origin_[i];
			scalingfactors_[i + ndimp] = scalingfactors_[i];
		}
	}
}




template<class T>
std::ostream & operator<<(std::ostream & ostr, Domain<T> const & d) {
	ostr << std::endl << std::endl;
	ostr << "Domain" << std::endl;
	ostr << "------" << std::endl << std::endl;

	int dimp = T::dp();
	for(int i = 0; i < dimp; ++i)
		ostr << "x" << i+1 << "_min = " << d.origin_[i] << std::endl;
	ostr << std::endl;

	for(int i = 0; i < dimp; ++i)
		ostr << "x" << i+1 << "_max = " << d.origin_[i]+1./d.scalingfactors_[i] << std::endl;
	ostr << std::endl;

	return ostr;
}




#endif /* DOMAIN_IMP_HPP_ */
