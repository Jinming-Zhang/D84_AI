/*
	CSC D84 - Unit 2 - MiniMax and adversarial games

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in MiniMax_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Script by: F.J.E., Sep. 2015
*/

#ifndef __MiniMax_search_header

#define __MiniMax_search_header

// Generally needed includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "board_layout.h"

#define INT_MAX 2147483647
#define INT_MIN -2147483648

// Function prototypes for D84 - Unit 2 - MiniMax assignment solution
double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta);

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2], int cheese_loc[10][2], int cats, int cheeses);
// If you need to add any function prototypes yourself, you can do so *below* this line.

typedef struct linked_node
{
	int val;
	struct linked_node *next;
} linked_node;

typedef struct linked_list
{
	linked_node *head;
	linked_node *tail;
	int size;
} linked_list;

int translateToIndex(int x, int y);
void translateFromIndex(int index, int *x, int *y);
int validIndex(int x, int y);
int validArr(int loc[1][2]);
void init_linked_list(linked_list *list);
void queue_item(linked_list *list, int val);
int pop_item(linked_list *list);
void del_linked_list(linked_list *list);
int is_empty(linked_list *list);
void findMovement(double gr[graph_size][4], int startX, int startY, int endX, int endY, int *return_direction, int *return_distance);

#endif
