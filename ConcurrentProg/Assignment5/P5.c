/* P5.c 
 * Original code: David Joiner - firestarter
 * Usage: P5 [forestSize(20)] [numTrials(5000)]  [numProbabilities(101)] 
 * Modified by: Krishna
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

#define true 1
#define false 0
#define NO_X11

typedef int boolean;

extern void seed_by_time(int);
extern int ** allocate_forest(int);
extern void initialize_forest(int, int **);
extern double get_percent_burned(int, int **);
extern void delete_forest(int, int **, int);
extern void light_tree(int, int **,int,int);
extern boolean forest_is_burning(int, int **);
extern void forest_burns(int, int **,double);
extern int burn_until_out(int,int **,double,int,int);
extern void print_forest(int, int **);
void do_parallel(int n_probs, double *prob_spread, double *percent_burned, int forest_size, int **forest, int n_trials);
void do_serial(int n_probs, double *prob_spread, double *percent_burned, int forest_size, int **forest, int n_trials);
void storage_allocate(int ***forest, int forest_size, double **percent_burned, int n_probs, int is_parallel);
int distributed_allocate(int ***forest, int forest_size, double **percent_burned, int n_probs);
int distributed_delete_forest(int forest_size, int **forest);

void print_time(double tm);


int main(int argc, char ** argv) {
    // initial conditions and variable definitions
    int forest_size=20;
    double * prob_spread;
    int **forest;
    double * percent_burned;
    int n_trials=5000;
    int n_probs=101;

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // check command line arguments
    if (argc != 5) {
	    if (rank == 0)
		    printf("Usage: %s [forest_size] [n_trials] [n_probs] [-serial|-parallel]\n", argv[0]);
	    exit (1);
    }

    sscanf(argv[1],"%d",&forest_size);
    sscanf(argv[2],"%d",&n_trials);
    sscanf(argv[3],"%d",&n_probs);
    int is_parallel = strcmp(argv[4], "-serial");

    // setup problem
    seed_by_time(101);
    prob_spread = (double *) malloc (n_probs*sizeof(double));
    storage_allocate(&forest, forest_size, &percent_burned, n_probs, is_parallel);


    double t1,t2;
    t1 = MPI_Wtime();
    if (!is_parallel){
	    do_serial(n_probs, prob_spread, percent_burned, forest_size, forest, n_trials);
    } else {
	    do_parallel(n_probs, prob_spread, percent_burned, forest_size, forest, n_trials);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    if(rank == 0)
      print_time(t2-t1);

    // clean up
    delete_forest(forest_size,forest, is_parallel);
    free(prob_spread);
    free(percent_burned);
    MPI_Finalize();

    return 0;
}

void seed_by_time(int offset) {
    srand(offset);
}
void do_serial(int n_probs, double *prob_spread, double *percent_burned, int forest_size, int **forest, int n_trials){
    // for a number of probabilities, calculate
    // average burn and output
    double prob_step;
    double prob_min=0.0;
    double prob_max=1.0;
    int i_trial;
    int i_prob;
    prob_step = (prob_max-prob_min)/(double)(n_probs-1);
    printf("Probability of fire spreading, Average percent burned\n");
    for (i_prob = 0 ; i_prob < n_probs; i_prob++) {
        //for a number of trials, calculate average
        //percent burn
        prob_spread[i_prob] = prob_min + (double)i_prob * prob_step;
        percent_burned[i_prob]=0.0;
        for (i_trial=0; i_trial < n_trials; i_trial++) {
            //start fire and burn until fire is gone
            burn_until_out(forest_size,forest,prob_spread[i_prob],
                forest_size/2,forest_size/2);
	    double d = get_percent_burned(forest_size,forest);
            percent_burned[i_prob]+=d;
        }
        percent_burned[i_prob]/=n_trials;

        // print output
        printf("%lf , %lf\n",prob_spread[i_prob], percent_burned[i_prob]);
    }
}

int burn_until_out(int forest_size,int ** forest, double prob_spread,
    int start_i, int start_j) {
    int count;

    initialize_forest(forest_size,forest);
    light_tree(forest_size,forest,start_i,start_j);

    // burn until fire is gone
    count = 0;
    while(forest_is_burning(forest_size,forest)) {
        forest_burns(forest_size,forest,prob_spread);
        count++;
    }

    return count;
}

double get_percent_burned(int forest_size,int ** forest) {
    int i,j;
    int total = forest_size*forest_size-1;
    int sum=0;

    // calculate pecrent burned
    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNT) {
                sum++;
            }
        }
    }

    // return percent burned;
    return ((double)(sum-1)/(double)total);
}


int ** allocate_forest(int forest_size) {
    int i;
    int ** forest;

    forest = (int **) malloc (sizeof(int*)*forest_size);
    for (i=0;i<forest_size;i++) {
        forest[i] = (int *) malloc (sizeof(int)*forest_size);
    }

    return forest;
}

void initialize_forest(int forest_size, int ** forest) {
    int i,j;

    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            forest[i][j]=UNBURNT;
        }
    }
}

void delete_forest(int forest_size, int ** forest, int is_parallel) {
    int i, flag = 0;

    if (is_parallel) 
	flag = distributed_delete_forest(forest_size, forest); 
    if (!flag) {
	    for (i=0;i<forest_size;i++) {
		free(forest[i]);
	    }
	    free(forest);
    }
}

void light_tree(int forest_size, int ** forest, int i, int j) {
    forest[i][j]=SMOLDERING;
}

boolean fire_spreads(double prob_spread) {
    if ((double)rand()/(double)RAND_MAX < prob_spread) 
        return true;
    else
        return false;
}

void forest_burns(int forest_size, int **forest,double prob_spread) {
    int i,j;
    extern boolean fire_spreads(double);

    //burning trees burn down, smoldering trees ignite
    for (i=0; i<forest_size; i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNING) forest[i][j]=BURNT;
            if (forest[i][j]==SMOLDERING) forest[i][j]=BURNING;
        }
    }

    //unburnt trees catch fire
    for (i=0; i<forest_size; i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNING) {
                if (i!=0) { // North
                    if (fire_spreads(prob_spread)&&forest[i-1][j]==UNBURNT) {
                        forest[i-1][j]=SMOLDERING;
                    }
                }
                if (i!=forest_size-1) { //South
                    if (fire_spreads(prob_spread)&&forest[i+1][j]==UNBURNT) {
                        forest[i+1][j]=SMOLDERING;
                    }
                }
                if (j!=0) { // West
                    if (fire_spreads(prob_spread)&&forest[i][j-1]==UNBURNT) {
                        forest[i][j-1]=SMOLDERING;
                    }
                }
                if (j!=forest_size-1) { // East
                    if (fire_spreads(prob_spread)&&forest[i][j+1]==UNBURNT) {
                        forest[i][j+1]=SMOLDERING;
                    }
                }
            }
        }
    }
}

boolean forest_is_burning(int forest_size, int ** forest) {
    int i,j;

    for (i=0; i<forest_size; i++) {
        for (j=0; j<forest_size; j++) {
            if (forest[i][j]==SMOLDERING||forest[i][j]==BURNING) {
                return true;
            }
        }
    }
    return false;
}

void print_forest(int forest_size,int ** forest) {
    int i,j;

    for (i=0;i<forest_size;i++) {
        for (j=0;j<forest_size;j++) {
            if (forest[i][j]==BURNT) {
                printf(".");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
}

void storage_allocate(int ***forest, int forest_size, double **percent_burned, int n_probs, int is_parallel){
    int flag = 0;
    if (is_parallel) 
	flag = distributed_allocate(forest, forest_size, percent_burned, n_probs); 
    if (!flag){
	    *forest=allocate_forest(forest_size);
	    *percent_burned = (double *) malloc (n_probs*sizeof(double));
    }
}
