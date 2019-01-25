#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include<sys/time.h>

#define NO_X11
int neighbours(int **board, int i, int j, int n, int m) {
    int sum = 0;
    if(i!=0) {
        if(j!=0) sum += board[i-1][j-1];
        sum += board[i-1][j];
        if(j!=m-1) sum += board[i-1][j+1];
    }

    if(j!=0) sum += board[i][j-1];
    if(j!=m-1) sum += board[i][j+1];

    if(i!=n-1) {
        if(j!=0) sum += board[i+1][j-1];
        sum += board[i+1][j];
        if(j!=m-1) sum += board[i+1][j+1];
    }
    return sum;
}

/* Print the time statistics */
void print_statistics(struct timeval start_time,struct timeval end_time)
{
    printf("Start time:\t%lf \n", start_time.tv_sec+(start_time.tv_usec/1000000.0));
    printf("End time:\t%lf\n", end_time.tv_sec+(end_time.tv_usec/1000000.0));
    printf("Total time: \t%lf (s)\n", end_time.tv_sec - start_time.tv_sec + ((end_time.tv_usec - start_time.tv_usec)/1000000.0));
}

int main(int argc, char ** argv) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // check command line arguments

    if (argc != 6 && argc != 7) {
	    if (rank == 0)
		    printf("Usage: %s <threads per proc> [Seed] [Rows] [Columns] [Generations] [opt-filename]\n", argv[0]);
	    exit (1);
    }
    int t, seed, n, m, gen;
    char* filename;
    sscanf(argv[1],"%d",&t);
    sscanf(argv[2],"%d",&seed);
    sscanf(argv[3],"%d",&n);
    sscanf(argv[4],"%d",&m);
    sscanf(argv[5],"%d",&gen);
    if(rank==0)
    // printf("%d %d %d %d %d\n",t,seed,n,m,gen);
    // printf("rank is %d\n",argc);
    srand(seed);

    int **board = (int**)malloc(sizeof(int*) * n);
    int **boardnew = (int**)malloc(sizeof(int*) * n);

    if(argc == 7) {
        filename = argv[6];
        FILE* fp;
        fp = fopen(filename,"r");
        for(int i = 0;i<n;i++) {
            board[i] = (int*)malloc(sizeof(int)*m);
            for(int j = 0;j<m;j++) {
                fscanf(fp,"%d",&board[i][j]);
            }
        }
        fclose(fp);
    }
    else {
        for(int i = 0;i<n;i++) {
            board[i] = (int*)malloc(sizeof(int)*m);
            for(int j = 0;j<m;j++) {
                board[i][j] = rand()%2;
            }
        }
    }

    for(int i = 0;i<n;i++) {
        boardnew[i] = (int*)malloc(sizeof(int)*m);
    }


    struct timeval start_time, end_time; 

    gettimeofday(&start_time, NULL);
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rowsperproc = (n+size-1)/size;

    for(int g = 0; g < gen; g++) {
        for(int i = rank*rowsperproc;(i<n) && (i < (rank+1)*rowsperproc);i++) {
            //printf("%d::%d\n",rank,i);
            #pragma omp parallel for num_threads(t)
            for(int j = 0;j<m;j++) {
                int status = neighbours(board,i,j,n,m);
                if(board[i][j]==1) {
                    if(status<2) boardnew[i][j] = 0;
                    else if(status >3) boardnew[i][j] = 0;
                    else boardnew[i][j] = 1;
                }
                else {
                    if(status == 3) boardnew[i][j] = 1;
                    else boardnew[i][j] = 0;
                }
            }
        }
        //printf("%d\n",numberofrows);
        MPI_Request requests[size*rowsperproc];
        for(int i = 0; i < size; i++ ) {
            // MPI_Ibcast(&forest[i-1][j],1, MPI_INT, 0, MPI_COMM_WORLD,&requests[0]);
            // int* placeat = *(boardnew + rank*rowsperproc);
            
            // if(rank==i) {
            //     for(int j = 0; j < numberofrows; j++) {
            //         for(int k = 0; k < m; k++) {
            //             board[j+i*rowsperproc][k] = boardnew[j+i*rowsperproc][k];
            //         }
            //     }
            // }
            int tillrow = n - i*rowsperproc;
            if(rowsperproc < tillrow) tillrow = rowsperproc;

            for(int j = 0; j < tillrow;j++) {
                MPI_Ibcast(&boardnew[i*rowsperproc+j][0],m, MPI_INT, i, MPI_COMM_WORLD,&requests[i*rowsperproc+j]);
            }
            // for(int j = 0; j < numberofrows; j++) {
            //     for(int k = 0; k < m; k++) {
            //         boardnew[j+i*rowsperproc][k] = board[j+i*rowsperproc][k];
            //     }
            // }
            // MPI_Barrier(MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        int **temp; 
        temp = board;
        board = boardnew;
        boardnew = temp;
    }


    // Calculate end time 
    gettimeofday(&end_time, NULL);
    // if(rank==0)
    // print_statistics(start_time,end_time);

    if(rank==0) {
        for(int i = 0;i<n;i++) {
            for(int j = 0;j<m;j++) {
                printf ("%d ",board[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // if(rank==1) {
    //     for(int i = 0;i<n;i++) {
    //         for(int j = 0;j<m;j++) {
    //             printf ("%d ",board[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }

    // MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}


