/*
 * =====================================================================================
 *
 *       Filename:  kempe.c
 *
 *    Description:  Code to do coloring using Kempes heuristic
 *
 *        Version:  1.0
 *        Created:  12/03/2018 09:59:42 IST
 *       Revision:  none
 *       Compiler:  gcc -std=c99
 *
 *         Author:  ?
 *
 * =====================================================================================
 */
#include<math.h>
#include<stdlib.h>
#include<strings.h>
#include"kempe.h"


/* *
Graph coloring - Kempe's heuristic Algorithm dating back to 1879.
Input : G - the interference graph, K - number of colors
repeat
	repeat
		Remove a node n and all its edges from G , such that degree of n is less than K;
		Push n onto a stack;
	until G has no node with degree less than K;
	// G is either empty or all of its nodes have degree >= K
	if G is not empty then
		Take one node m out of G;
		push m onto the stack;
	end
until G is empty;
Take one node at a time from the stack and assign a non conflicting color (if possible, else spill).
 *
 */

void kempe_serial(struct node **B, int n, int col){
	stack_init(n);
	int ln_index;
	do {
		int flag;
		do {
			for (int i=0;i<n;++i){
				if (B[i]->deleted) continue;
				if (B[i]->deg < col) {
					push(B[i]);
					B[i]->deleted = 1;
					remove_incident_edges(B[i]);
				}
			}
			flag = non_critical_nodes_present(B, n, col);
		} while (flag);

		ln_index = live_nodes_present(B, n, col);
		if (ln_index != -1) {
			push(B[ln_index]);
			B[ln_index]->deleted = 1;
			B[ln_index]->color = -1; // spilled
			remove_incident_edges(B[ln_index]);
		} 
	} while (ln_index != -1);
	do_color(B, n, col);
}

void do_color(struct node **B, int n, int col){
	int *c = (int *)calloc (col, sizeof(int));
	for (int i=0;i<n;++i){
		struct node* nd = pop();
		if (nd->deg == 0) { // special case.
			nd->color = 0; 
			nd->deleted = 0;
			restore_incident_edges(nd);
			continue;
		}
		int j;
		for (j = 0; j < nd->nncnt; ++j){ // note which colors are taken by the neighbors.
			if (nd->neighbors[j]->deleted) continue;
			c[nd->neighbors[j]->color] = 1;
		}
		for (j=0;j<col; ++j){ // find the available color.
			if (c[j] == 0) break;
		}
		if (j == col) {nd->deleted = -1; nd->color = -1;} // spilled
		else {nd->deleted = 0; nd->color = j; }
		restore_incident_edges(nd);
		bzero(c, n*sizeof(int));
	}
}
void do_color_parallel(struct node **B, int n, int col, int nproc){
	int *c = (int *)calloc (col, sizeof(int));
	for (int i=0;i<n;++i){
		struct node* nd = pop();
		if (nd->deg == 0) { // special case.
			nd->color = 0; 
			nd->deleted = 0;

			#pragma omp parallel for num_threads(nproc)
			for (int j=0;j<nd->nncnt;++j){
				nd->neighbors[j]->deg --;
			}
			continue;
		}
		int j;
		for (j = 0; j < nd->nncnt; ++j){ // note which colors are taken by the neighbors.
			if (nd->neighbors[j]->deleted) continue;
			c[nd->neighbors[j]->color] = 1;
		}
		for (j=0;j<col; ++j){ // find the available color.
			if (c[j] == 0) break;
		}
		if (j == col) {nd->deleted = -1; nd->color = -1;} // spilled
		else {nd->deleted = 0; nd->color = j; }
		#pragma omp parallel for num_threads(nproc)
		for (int j=0;j<nd->nncnt;++j){
			nd->neighbors[j]->deg --;
		}
		bzero(c, n*sizeof(int));
	}
}
void kempe_parallel(struct node **B, int n, int col, int nproc){
	stack_init(n);
	int ln_index;
	do {
		int flag;
		do {
			for (int i=0;i<n;++i){
				if (B[i]->deleted) continue;
				if (B[i]->deg < col) {
					push(B[i]);
					B[i]->deleted = 1;
					#pragma omp parallel for num_threads(nproc)
					for (int j=0;j<B[i]->nncnt;++j){
						B[i]->neighbors[j]->deg --;
					}
				}
			}
			int flag = 0;
			#pragma omp parallel for num_threads(nproc)
			for (int i=0;i<n; i++){
				if (B[i]->deg <  col) flag = 1;
			}
		} while (flag);

		ln_index = live_nodes_present(B, n, col);
		if (ln_index != -1) {
			push(B[ln_index]);
			B[ln_index]->deleted = 1;
			B[ln_index]->color = -1; // spilled
			#pragma omp parallel for num_threads(nproc)
			for (int j=0;j<B[ln_index]->nncnt;++j){
				B[ln_index]->neighbors[j]->deg --;
			}
		} 
	} while (ln_index != -1);
	do_color_parallel(B, n, col,nproc);
}
/*
int givecount(struct node **order, int n, struct node* ptr) {
	for(int i =0;i<n;i++) {
		if(order[i] == ptr) {
			return i;
		}
	}
	exit(1);
	return -1;
}
int givecount2(struct node *ptr,struct node **B) {
	int sizediff = (long int)B[1] - (long int)B[0];
	return ((long int)ptr - (long int)B[0])/sizediff;
}
void do_color_parallel(struct node **stackorder, struct node **B, int n, int col, int nproc) {
	int *proposenode = (int *)calloc (n, sizeof(int));
	int flagcontinue = n;
	//#pragma omp parallel num_threads(nproc)
	//{
		//#pragma omp for
		for (int i=0;i<n;++i) {
			struct node* nd = B[i];
			nd->deg = nd->nncnt;
			proposenode[i] = 0;
		}
		//#pragma omp for
		while(flagcontinue>0) {
			for (int i=0;i<n;++i){
				struct node* nd = B[i];
				if(nd->deleted == 0) continue;
				if (nd->deg == 0) { // special case.
					nd->color = 0;
					flagcontinue--;
					nd->deleted = 0; 
					continue;
				}
				int colorpropose = proposenode[i];
				proposenode[i]++;
				if(proposenode[i] == n+1) {
					nd->deleted = -1; nd->color = -1;
					flagcontinue--;	
				}
				int flag = 1;
				int c1 = givecount(stackorder,n,nd);
				//printf("%d is count\n",c1);
				for (int k = 0; k < nd->nncnt; ++k){
					if(nd->neighbors[k]->deleted == 0 && nd->neighbors[k]->color == colorpropose) {
						int c2 = givecount(stackorder,n,nd->neighbors[k]);
						//printf("%d is count\n",c2);
						if(c1>c2) {
							nd->neighbors[k]->deleted = 1;
							flagcontinue++;
							//printf("Delete something\n");
							continue;
						}
						else {
							flag = -1;
							break;
						}
					}
				}
				if(flag==1) {
					nd->deleted = 0;
					flagcontinue--;
					nd->color = colorpropose;
				}
			}
		}
	//}
}
void kempe_parallel(struct node **B, int n, int col, int nproc){
	// Write your code here. 
	stack_init(n);
	int ln_index;
	do {
		int* order = (int*)calloc(n+1,sizeof(int));
		for(int i =0;i<n;i++) order[i] = -1;
		int flag;
		int iter = 0;
		int count = 0;
		#pragma omp parallel private(count) num_threads(nproc) 
		{
			count = 0;	
			int* orderlocal = (int*)calloc(n+1,sizeof(int));
			int* iterlocal = (int*)calloc(n+1,sizeof(int));
			do {
				#pragma omp for
				for (int i=0;i<n;i++){
					//printf("%d::%d::%d\n",omp_get_thread_num(),i,B[i]->deleted);
					if (B[i]->deleted) continue;
					if (B[i]->deg < col) {
						//push(B[i]);
						if(count==n)continue;
						orderlocal[count] = i;
						iterlocal[count] = iter;
						count++;
						B[i]->deleted = 1;
						remove_incident_edges(B[i]);
					}
				}
				#pragma omp single
				{   
					iter += 1;
					flag = non_critical_nodes_present(B,n,col);
				}
			}while(flag);

			for(int i = 0;i < count; i++) {
				order[orderlocal[i]] = iterlocal[i];
			}
		}
		int curriter = 0;
		do{
			for(int i = 0; i<n; i++) {
				if(order[i]==curriter) {
					struct node* nd = B[i];
					int deg = nd->deg;
					for(int k = 0; k < nd->nncnt; k++) {
						int currindex = givecount2(nd->neighbors[k]);
						int currorder = order[currindex];
						if(currorder!=-1 && (currorder<order[i] || (currorder == order[i] && currindex < i))) {
							deg--;
						}
					}
					if(deg < col) {
						p
					}
				}
			}
		}while()
		//rank(order);
		ln_index = live_nodes_present(B, n, col);
		if (ln_index != -1) {
			push(B[ln_index]);
			B[ln_index]->deleted = 1;
			B[ln_index]->color = -1; // spilled
			remove_incident_edges(B[ln_index]);
		} 
	} while (ln_index != -1);
	//do_color(B, n, col);
	return ;
}*/