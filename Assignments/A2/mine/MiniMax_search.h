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
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<unistd.h>
#include "board_layout.h"

// Function prototypes for D84 - Unit 2 - MiniMax assignment solution
double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta);

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2], int cheese_loc[10][2], int cats, int cheeses);

// If you need to add any function prototypes yourself, you can do so *below* this line.
//////////////////////
// helper functions //
//////////////////////

void dijkstra(double gr[graph_size][4], int src_index, int min_dist_from_src[graph_size]);

/**
 * convert [x y] coordinates to index and store in @index
 * index = -1 if [x y] out of graph range
 * @param x     x
 * @param y     y
 * @param index converted index
 */
inline void xy_to_index(int x, int y, int *index) {
  if(0 <= x && x < size_X && 0 <= y && y < size_Y) {
    *index = x + (y * size_X);
  } else {
    *index = -1;
  }
}

/**
 * convert index to [x y] coordinates
 * @param x     x
 * @param y     y
 * @param index index to convert
 */
inline void index_to_xy(int *x, int *y, int index) {
  *x = index % size_X;
  *y = index / size_Y;
}

/**
 *
 */
inline int has_cat(int index, int cat_loc[10][2], int cats) {
  int x = index % size_X;
  int y = index / size_Y;
  for (int i = 0; i < cats; i++) {
    if (x == cat_loc[i][0] && y == cat_loc[i][1]) {
      return 1;
    }
  }
  return 0;
}

/**
 *
 */
inline int has_cheese(int index, int cheese[10][2], int cheeses) {
  int x = index % size_X;
  int y = index / size_Y;
  for (int i = 0; i < cheeses; i++) {
    if (x == cheese[i][0] && y == cheese[i][1]) {
      return 1;
    }
  }
  return 0;
}

#endif

