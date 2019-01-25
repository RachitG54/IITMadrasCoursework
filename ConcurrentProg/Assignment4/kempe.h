/*
 * =====================================================================================
 * 
 *       Filename:  kempe.h
 * 
 *    Description:  Header file for Kempes heuristic
 * 
 *        Version:  1.0
 *        Created:  03/12/2018 22:09:49 IST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  V. Krishna Nandivada (Krishna), nvk@cse.iitm.ac.in
 *        Company:  IIT Madras.
 * 
 * =====================================================================================
 */

#ifndef  KEMPE_FILE_HEADER_INC
#define  KEMPE_FILE_HEADER_INC

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

struct node {
	struct node **neighbors;
	int deg;
	int nncnt;
	int deleted;
	int color;
};

/* defined in util.c */
extern struct node** stack;
extern int stack_top;
extern void stack_init (int n);
extern void push(struct node* n);
extern struct node* pop();
extern int live_nodes_present(struct node **B, int n, int col);
extern int non_critical_nodes_present(struct node **B, int n, int col);
extern void restore_incident_edges(struct node *x);
extern void remove_incident_edges(struct node *x);
extern void do_color(struct node **B, int n, int col);

/* Functions to be supplied in kempe.c */
extern void kempe_serial(struct node **, int, int);
extern void kempe_parallel(struct node **A, int n, int col, int nproc);


#endif   /* ----- #ifndef KEMPE_FILE_HEADER_INC  ----- */
