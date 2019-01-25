#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 1024

int A[N][N];
int B[N][N];
int C[N][N];

void scenario1() {
	int i,j,k;
	struct timeval t0,t1;
	gettimeofday(&t0, 0);
	for(i=0;i<N;i++) {
		for(j=0;j<N;j++) {
			C[i][j] = 0;
			for(k=0;k<N;k++) {
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
	gettimeofday(&t1, 0);
	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("Time taken for scenario1 = %ld ms\n",elapsed/1000);
}

int main() {
	srand((unsigned int)time(NULL));
	int i;
	int j; 
	for(i=0;i<N;i++) {
		for(j=0;j<N;j++) {
			A[i][j] = rand()%10;
			B[i][j] = rand()%10;
		}
	}
	scenario1(A,B,C);
	return 0;
}
