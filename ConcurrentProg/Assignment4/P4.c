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

#include"kempe.h"


/* read input from the standard input, after allocating the array. For each
 * node, read the degree, followed by the indices of the neighbors. 
 */
struct node ** read_input (int n){
	struct node ** nodes;
	nodes = (struct node**)malloc(n*sizeof(struct node*));
	for (int i=0;i<n;++i){
		nodes[i] = (struct node*)calloc(1, sizeof(struct node));
	}
	for (int i=0;i<n;++i){
		scanf("%d",&(nodes[i]->deg));
		 nodes[i]->nncnt = nodes[i]->deg;
		nodes[i]->neighbors=(struct node**)malloc(nodes[i]->deg*sizeof(struct node*));
		for (int j=0;j<nodes[i]->deg;++j){
			int indx;
			scanf("%d",&indx);
			nodes[i]->neighbors[j]=nodes[indx];
		}
	}
	return nodes;

}

/* output the final colors. */
void print_output(struct node **A, int n){

	for (int i=0;i<n;++i){
			printf("%d %d\n",i, A[i]->color);
	}

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
	fprintf(stderr, "Usage: %s <nprocs> <col> <-serial|-parallel>\n", prog);
	exit(1);
}


int main(int argc, char **argv){
	struct timeval start_time, end_time; 
	int num_iter = 0;
	int col;
	struct node **A;
	int *outC; // output color outC[i] gives the color of node i
	int procs;
	int n;
	if (argc != 4){
		print_usage_and_exit(argv[0]);
	}
	sscanf(argv[1],"%d",&procs);
	sscanf(argv[2],"%d",&col);
	char *option = argv[3];

	if (option == NULL || (strcmp(option,"-serial") != 0 &&
		strcmp(option,"-parallel") != 0 ))
			print_usage_and_exit(argv[0]);

	printf("Options: Procs = %d, Colors = %d, Execution%s\n\n",procs, col, option);

	printf("Nodes = ");
	scanf("%d", &n);
	A = read_input(n);
	outC = (int *)calloc(n, sizeof(int));
	
	// Calculate start time 
	gettimeofday(&start_time, NULL);
	
	if (strcmp(option,"-serial") == 0)
		kempe_serial(A, n, col);
	else 
		kempe_parallel(A, n, col, procs);
	
	// Calculate end time 
	gettimeofday(&end_time, NULL);
	// Print Statistics
	print_output(A, n);
	print_statistics(start_time,end_time);
		
}
