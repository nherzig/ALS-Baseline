// baseline.cpp : Defines the entry point for the console application.
//Forked from the code: https://github.com/kellman/ALS-Baseline-Subtraction/blob/master/als.cpp
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <vector>

using namespace std;


using namespace Eigen;
typedef Triplet<double> T;
typedef SparseMatrix<double> SpMat;

vector<double> baselineals(vector<double> in, double lambda, double p, int maxIter)
{
	int N(in.size());
	// Initialize weights
	vector<Triplet<double>> weights;
	VectorXd vweights(N);
	VectorXd sig(N);
	vector<double> out;
	weights.reserve(N);
	for (int ii = 0; ii < N; ++ii)
	{
		weights.push_back(Triplet<double>(ii,ii,1));
		vweights[ii] = 1;
		
		//setup the in vector
		sig[ii] = in[ii];
	}

	// Initialize Diff matrix
	vector<Triplet<double>> difftriplet;
	difftriplet.reserve(N);
	for (int ii = 0; ii < N-2; ++ii)
	{
		difftriplet.push_back(Triplet<double>(ii,ii,1));
		difftriplet.push_back(Triplet<double>(ii,ii+1,-2));
		difftriplet.push_back(Triplet<double>(ii,ii+2,1));
	}

	SparseMatrix<double> D(N,N);
	SparseMatrix<double> W(N,N);
	D.setFromTriplets(difftriplet.begin(),difftriplet.end());
	SparseMatrix<double> Dset(N,N);
	Dset = lambda*D.transpose()*D;

	SparseMatrix<double> temp(N,N);

  	VectorXd x(N);

  	for(int ii = 0; ii < maxIter; ii++)
	{
		cout << "Iteration: " << ii+1 << endl;
		// decomposition
		W.setFromTriplets(weights.begin(),weights.end());
		SimplicialCholesky<SparseMatrix<double>> chol(W+Dset);
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

	for (int ii = 0; ii < N; ++ii)
	{
		out.push_back(x[ii]);
	}
	return out;

}


int main()
{
  std::vector<double> x_unfilt,x_filt;
  std::ifstream ifs;
  double num(0.0);
  //Loading data to filter
	ifs.open("x_unfilt.txt");
	if(!ifs){
		std::cout<<"x_unfilt.txt file not loaded"<<std::endl;
	}
	while (ifs >> num) {
        x_unfilt.push_back(num);
		}
	//Filtering
	std::cout<<"start filtering"<<std::endl;
	x_filt=baselineals(x_unfilt,100000000000,0.999,10);


	//write x_filt in a file
	std::ofstream myfile ("x_filt.txt");
  if (myfile.is_open())
  {
	for(int i(0);i<x_filt.size();i++){
		myfile << x_filt[i]<<std::endl;
	}
	myfile.close();
  }
  else std::cout << "Unable to open file";

  std::cout<<"end filtering"<<std::endl;

	std::getchar();

}