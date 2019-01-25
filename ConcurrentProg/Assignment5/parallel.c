/*
 * Skeleton by: Krishna.
 * Fill in your code here.
 */
#include<stdio.h>

int distributed_delete_forest(int forest_size, int **forest){
	
  // Return 1, if you do the memory deallocation in a distributed manner.
  return 0;
}
int distributed_allocate(int ***forest, int forest_size, double **percent_burned, int n_probs){
	
  // Return 1, if you do the memory allocatation in a distributed manner.
  return 0;
}
void do_parallel(int n_probs, double *prob_spread, double *percent_burned, int forest_size, int **forest, int n_trials){}

void print_time(double tm){
    printf("elapsed time = %f seconds\n", tm);
}
