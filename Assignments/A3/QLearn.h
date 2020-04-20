/*
	CSC D84 - Unit 3 - Reinforcement Learning

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in QLearn.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jan. 2016
*/

#ifndef __QLearn_header

#define __QLearn_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>

#define alpha .01			// Learning rate for standard Q-Learning
#define lambda .5			// Discount rate for future rewards
#define max_graph_size 32*32

#define numFeatures 5			// UPDATE THIS to be the number of features you have

#define save_dijkstra 0      // we compute the dijkstras' graph on the maze, set this to one to make the program save the computed dijkstras' graph

// Function prototypes for D84 - Unit 3 - Reinforcement Learning
void QLearn_update(int s, int a, double r, int s_new, double *QTable);
int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size);
double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);
int feat_QLearn_action(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size);
void evaluateFeatures(double gr[max_graph_size][4], double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);
double Qsa(double weights[25], double features[25]);
void maxQsa(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA);

void expensiveFeature1(double gr[max_graph_size][4], int path[max_graph_size][2], int start_x, int start_y, int (*goalCheck)(int x, int y, int pos[5][2]), int pos[5][2], int s_type, int *l, int size_X);
int checkForGoal(int x, int y, int pos[5][2]);

// If you need to add any function prototypes yourself, you can do so *below* this line.
//////////////////////
// helper functions //
//////////////////////

/**
 * Get the state index of given variables in the graph
 * @param  i      mouse x
 * @param  j      mouse y
 * @param  k      cat x
 * @param  l      cat y
 * @param  m      cheese x
 * @param  n      cheese y
 * @param  size_X height/width of the square maze
 * @return        the state index based on given variables
 */
inline int get_state(int i, int j, int k, int l, int m, int n, int size_X, int graph_size) {
    return (i + (j * size_X)) + ((k + (l * size_X)) * graph_size) + ((m + (n * size_X)) * graph_size * graph_size);
}


void dijkstra(double gr[max_graph_size][4], int src_index, int min_dist_from_src[max_graph_size], int size_X, int graph_size);

/**
 * convert [x y] coordinates to index and store in @index
 * index = -1 if [x y] out of graph range
 * @param x     x
 * @param y     y
 * @param index converted index
 */
inline int xy_to_index(int x, int y, int size_X) {
    if(0 <= x && x < size_X && 0 <= y && y < size_X) {
        return  x + (y * size_X);
    } else {
        return -1;
    }
}

/**
 * convert index to [x y] coordinates
 * @param x     x
 * @param y     y
 * @param index index to convert
 */
inline void index_to_xy(int *x, int *y, int index, int size_X) {
    *x = index % size_X;
    *y = index / size_X;
}

#endif

