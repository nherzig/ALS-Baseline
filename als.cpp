#include <vector>
#include <iostream>
#include <stdlib.h>
#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <time.h>

#include "fftw3.h"
#include <memory.h>

using namespace std;
typedef std::shared_ptr<fftw_complex> cp;

using namespace Eigen;
typedef Triplet<double> T;
typedef SparseMatrix<double> SpMat;

void als(cp in, cp out, int N, double smooth, double p, int maxIter)
{

	clock_t start;
	start = clock();

	// Initialize weights
	vector<T> weights;
	VectorXd vweights(N);
	VectorXd sig(N);
	weights.reserve(N);
	for (int ii = 0; ii < N; ++ii)
	{
		weights.push_back(T(ii,ii,1));
		vweights[ii] = 1;
		
		//setup the in vector
		sig[ii] = in.get()[ii][0];
	}

	// Initialize Diff matrix
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
	Dset = smooth*D.transpose()*D;


	double t = (clock() - start)/(double)CLOCKS_PER_SEC;
	cout << "setup time: " << t << endl;
	start = clock();

	SpMat temp(N,N);

  	VectorXd x(N);

  	for(int ii = 0; ii < maxIter; ii++)
	{
		cout << "Iteration: " << ii+1 << endl;
		// decomposition
		W.setFromTriplets(weights.begin(),weights.end());
		SimplicialCholesky<SpMat> chol(W+Dset);
		x = chol.solve(sig.cwiseProduct(vweights));
		// update
		for(int jj = 0; jj < N; jj++)
		{
			if(sig[jj] > x[jj])
			{
				vweights[jj] = p;
			}
			else
			{
				vweights[jj] = 1-p;
			}
			weights[jj] =  T(jj,jj,vweights[jj]);
		}
	}

	// subtract the baseline
	for(int ii = 0;ii < N; ii++)
	{
		out.get()[ii][0] = in.get()[ii][0] - x[ii];
		out.get()[ii][1] = 0; // This should remain 0.
		// cout << in.get()[ii][0] << " : " << out.get()[ii][0] << " : " << x[ii] << endl;
		// cout << out.get()[ii][0] << " : " << x[ii] << endl;	
	}

	t = (clock() - start)/(double)CLOCKS_PER_SEC;
	cout << "compute time: " << t << endl;

}