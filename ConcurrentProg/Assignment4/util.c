#include"kempe.h"

struct node** stack;
int stack_top;

void stack_init (int n){
	stack = (struct node**)calloc(n, sizeof(struct node*));
}
void push(struct node* n){
	stack[stack_top++] = n;
}
struct node* pop(){
	return stack[--stack_top];
}
int live_nodes_present(struct node **B, int n, int col){
	for (int i=0;i<n; i++){
		if (B[i]->deleted) continue;
		return i;
	}
	return -1;
}
int non_critical_nodes_present(struct node **B, int n, int col){
	for (int i=0;i<n; i++){
		if (B[i]->deleted) continue;
		if (B[i]->deg <  col) return 1;
	}
	return 0;
}
void restore_incident_edges(struct node *x){
	for (int j=0;j<x->nncnt;++j){
		x->neighbors[j]->deg ++;
	}
}
void remove_incident_edges(struct node *x){
	for (int j=0;j<x->nncnt;++j){
		x->neighbors[j]->deg --;
	}
}
