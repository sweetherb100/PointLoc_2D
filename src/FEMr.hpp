#ifndef FEMR_HPP_
#define FEMR_HPP_

// Insert principal libraries
#ifdef R_VERSION_
#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h> 
#include <Rmath.h>
#endif

#include <stdint.h>

#include <cstdlib>
//#include <iomanip>
#include <limits>
#include <vector>
#include <chrono>

#include "Eigen/Eigen/Sparse"
#include "Eigen/Eigen/Dense"


#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <set>
#include <typeinfo>

typedef double Real;
typedef int UInt;

typedef Eigen::Matrix<Real,Eigen::Dynamic,Eigen::Dynamic> MatrixXr;
typedef Eigen::Matrix<Real,Eigen::Dynamic,1> VectorXr;
typedef Eigen::SparseMatrix<Real> SpMat;
typedef Eigen::SparseVector<Real> SpVec;
typedef Eigen::Triplet<Real> coeff;

#endif /* FEMR_HPP_ */
