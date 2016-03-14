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
	// signal
	VectorXd sig = VectorXd::Random(N,1);


	//Init weights
	vector<T> weights;
	VectorXd vweights(N);
	weights.reserve(N);
	for (int ii = 0; ii < N; ++ii)
	{
		weights.push_back(T(ii,ii,1));
		vweights[ii] = 1;
	}

	// Init Diff matrix
	vector<T> difftriplet;
	difftriplet.reserve(N);
	for (int ii = 0; ii < N-2; ++ii)
	{
		difftriplet.push_back(T(ii,ii,1));
		difftriplet.push_back(T(ii,ii+1,-2));
		difftriplet.push_back(T(ii,ii+2,1));
	}

	SpMat D(N,N);
	SpMat W(N,N);
	D.setFromTriplets(difftriplet.begin(),difftriplet.end());

	SpMat Dset(N,N);
	double lambda = 100, p = 0.01; 
	Dset = lambda*D.transpose()*D;
	cout << "I have set up Dset" << endl;
	int maxIter = 5;
	SpMat temp(N,N);


  	// VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side
	for(int ii = 0; ii < maxIter; ii++)
	{
		W.setFromTriplets(weights.begin(),weights.end());
		SimplicialCholesky<SpMat> chol(W+Dset);
		chol.solve(sig.cwiseProduct(vweights));
	


	}

	return 0;
}