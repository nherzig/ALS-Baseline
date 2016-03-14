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
	int N;
	// signal
	FILE * sfile;
	sfile = fopen("baselinesub","r+");
	double readtemp;
	fread(&N,4,1,sfile);
	cout << N << endl;
	VectorXd sig(N);

	for (int ii = 0; ii < N; ++ii)
	{
		fread(&readtemp,8,1,sfile);
		sig[ii] = readtemp;
		cout << sig[ii] << endl;
	}
	fclose(sfile);


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
	cout << "I have finished setup" << endl;


	int maxIter = 5;
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
	FILE * wfile;
	wfile = fopen("output","w+");
	fwrite(&N,4,1,wfile);
	for(int ii = 0; ii < N; ii++)
	{
		cout << x[ii] << endl;
		readtemp = x[ii];
		fwrite(&readtemp,8,1,wfile);
	}
	fclose(wfile);
	return 0;
}