#include <vector>
#include <iostream>
#include <stdlib.h>
#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <Eigen/FFT>

using namespace Eigen;

typedef Triplet<double> T;
typedef SparseMatrix<double> SpMat;

using namespace std;

int main(){
	// Baseline subtraction using Eigen attempt 2.0
	int N = 100;
	
	//Init weights
	vector<T> weights;
	weights.reserve(N);
	for (int ii = 0; ii < N; ++ii)
	{
		weights.push_back(T(ii,ii,1));
	}

	// Init Diff matrix


	return 0;
}