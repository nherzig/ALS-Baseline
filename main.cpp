// Copyright (c) 2016, The Regents of the University of California (Regents).
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:

//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.

//    2. Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.

//    3. Neither the name of the copyright holder nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// Please contact the author(s) of this library if you have any questions.
// Author: Michael Kellman   ( kellman@eecs.berkeley.edu )

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include <time.h>
#include "fftw3.h"
#include <memory.h>

#include "als.cpp"

using namespace Eigen;

typedef Triplet<double> T;
typedef SparseMatrix<double> SpMat;
typedef std::shared_ptr<fftw_complex> cp;

using namespace std;

int main(){
	// Baseline subtraction using Eigen attempt 2.0
	int N = 10000;
	// in data
	FILE * sfile;
	sfile = fopen("baselinesub","r+");
	double readtemp;
	fread(&N,4,1,sfile);
	cout << N << endl;
	cp in(new fftw_complex[N]);
	for (int ii = 0; ii < N; ++ii)
	{
		fread(&readtemp,8,1,sfile);
		in.get()[ii][0] = readtemp;
		in.get()[ii][1] = 0;
	}
	fclose(sfile);

	// parameters
	cp out(new fftw_complex[N]);
	double smooth = 10000000;
	double p = 0.01;
	int maxIter = 5;

	als(in,out,N,smooth,p,maxIter);

	//write output
	FILE * wfile;
	wfile = fopen("output","w+");
	fwrite(&N,4,1,wfile);
	for(int ii = 0; ii < N; ii++)
	{
		//cout << x[ii] << endl;
		readtemp = out.get()[ii][0];
		fwrite(&readtemp,8,1,wfile);
	}
	fclose(wfile);

	return 0;
}