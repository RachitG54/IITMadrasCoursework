/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file to simulate ocean currents. 
 *    		    DO NOT MODIFY THIS FILE.
 *
 *         Author:  V. Krishna Nandivada (Krishna), nvk@cse.iitm.ac.in
 *        Company:  IIT Madras.
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

/* Functions to be supplied in simulator.c */
extern int simulate_ocean_currents(double **, int, double);
extern int simulate_ocean_currents_parallel(double **, int, double, int);

/* read input from the standard input, after allocating the array */
double ** read_input (int n){
	double **X;
	X = (double **)malloc(n*sizeof(double*));
	for (int i=0;i<n;++i){
		X[i]=(double *)malloc(n*sizeof(double));
		for (int j=0;j<n;++j)
			scanf("%lf",&X[i][j]);
	}

	return X;
}

/* output the final grid. */
void print_output(double **A, int n, int niter){

	printf("Number of iterations = %d\n", niter);

	for (int i=0;i<n;++i){
		for (int j=0;j<n;++j)
			printf("%lf ",A[i][j]);
		printf("\n");
	}
	printf("\n");

}

/* Print the time statistics */
void print_statistics(struct timeval start_time,struct timeval end_time)
{
    printf("Start time:\t%lf \n", start_time.tv_sec+(start_time.tv_usec/1000000.0));
    printf("End time:\t%lf\n", end_time.tv_sec+(end_time.tv_usec/1000000.0));
    printf("Total time: \t%lf (s)\n", end_time.tv_sec - start_time.tv_sec + ((end_time.tv_usec - start_time.tv_usec)/1000000.0));
}

/* Error in command line arguments. Print usage and exit. */
void print_usage_and_exit(char *prog){
	fprintf(stderr, "Usage: %s <nprocs> <tol> <-serial|-parallel>\n", prog);
	exit(1);
}


int main(int argc, char **argv){
	struct timeval start_time, end_time; 
	int num_iter = 0;
	double tol;
	double **A;
	int procs;
	int dim;
	if (argc != 4){
		print_usage_and_exit(argv[0]);
	}
	sscanf(argv[1],"%d",&procs);
	sscanf(argv[2],"%lf",&tol);
	char *option = argv[3];

	if (option == NULL || (strcmp(option,"-serial") != 0 &&
		strcmp(option,"-parallel") != 0 ))
			print_usage_and_exit(argv[0]);

	printf("Options: Procs = %d, Tol = %lf, Execution%s\n\n",procs, tol, option);

	// printf("Dimensions = ");
	scanf("%d", &dim);
	A = read_input(dim);
	
	// Calculate start time 
	gettimeofday(&start_time, NULL);
	
	if (strcmp(option,"-serial") == 0)
		num_iter=simulate_ocean_currents(A, dim, tol);
	else 
		num_iter=simulate_ocean_currents_parallel(A, dim, tol, procs);
	
	// Calculate end time 
	gettimeofday(&end_time, NULL);
	// Print Statistics
	print_output(A, dim, num_iter);
	print_statistics(start_time,end_time);
		
}
