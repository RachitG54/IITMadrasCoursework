/*
 * =====================================================================================
 *
 *       Filename:  simulate.c
 *
 *    Description:  Code to simulate Ocean currents.
 *
 *        Version:  1.0
 *        Created:  03/03/2018 09:59:42 IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ?
 *
 * =====================================================================================
 */
#include<math.h>
#include<stdlib.h>
#include<string.h>

int simulate_ocean_currents(double **A, int n, double tol){
	int done = 0;
	double diff; 
	double old;
	int iter = 0;

	double **B, **C;
	B = (double **) malloc(n*sizeof(double*));
	for (int k = 0; k < n; k++){
		B[k]=(double *) malloc(n*sizeof(double));
		memcpy(B[k], A[k], n*sizeof(double));
	}

	while (!done){
		iter ++;
		diff = 0;      /* init */
		for (int i=1;i<n-1; ++i){ /* skip border elems */
			for (int j=1; j<n-1; ++j){ /* skip border elems */
				old = A[i][j];  
				B[i][j] = (A[i][j] + A[i][j-1] + A[i-1][j] + A[i][j+1] + A[i+1][j])/5.0; /*average */
				diff += fabs(B[i][j] - old);
			}
		}
		C = A; A = B; B = C; // exchange.
		if (diff/(n*n) < tol) done = 1;
	}
	return iter;

}
int simulate_ocean_currents_parallel(double **A, int dim, double tol, int procs){
	/* Write your code here. */
	int done = 0;
	double diff; 
	double old;
	int iter = 0;

	double **B, **C;
	B = (double **) malloc(dim*sizeof(double*));
	for (int k = 0; k < dim; k++){
		B[k]=(double *) malloc(dim*sizeof(double));
		memcpy(B[k], A[k], dim*sizeof(double));
	}

	while (!done){
		iter ++;
		diff = 0;      /* init */
		if(dim>1) {
			#pragma omp parallel for num_threads(procs) reduction(+:diff) private(old)
			for(long long int k = 0; k<(dim-2)*(dim-2); k++) {
				int i = k/(dim-2)+1;int j = k%(dim-2)+1;
				//printf("%d::%d\n", i,j);
					old = A[i][j];  
					B[i][j] = (A[i][j] + A[i][j-1] + A[i-1][j] + A[i][j+1] + A[i+1][j])/5.0; /*average */
					diff += fabs(B[i][j] - old);
				
			}
			C = A; A = B; B = C; // exchange.
			if (diff/(dim*dim) < tol) done = 1;
		}
		else {
			break;
		}
	}	
	return iter;
}
