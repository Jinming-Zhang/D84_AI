/*
  CSC D84 - Unit 2 - MiniMax search and adversarial games

  This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
  handout carefully - it describes the game, the data you will
  have to handle, and the search functions you must provide.

  Once you have read the handout carefully, implement your search
  code in the sections below marked with

  **************
  *** TO DO:
  **************

  Make sure to add it to your report.txt file - it will be marked!

  Have fun!

  DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
  management being done properly, and for memory leaks.

  Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta) {

  /////////////////////////////////////////
  // return:                             //
  //  minimax value (depends on agent) //
  //    minmax_cost (only for mouse)     //
  //    path                             //
  /////////////////////////////////////////
  // sleep(10);
  char prefix[depth * 4 + 1];
  for (int i = 0; i < depth * 4; i++) {
    prefix[i] = '.';
  }
  prefix[depth * 4] = '\0';
  if(mode == 2) printf("%sMiniMax depth: [%d]/[%d], agent: [%d]<<<<<<<<<<<<<\n", prefix, depth, maxDepth, agentId);
  // reached terminal
  if(checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses) || depth >= maxDepth) {
    double u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
    if(agentId == 0) {
      minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = u;
      if(mode == 2) printf("%s  reached terminal(%d %d): %f, agent: [%d]\n", prefix, mouse_loc[0][0], mouse_loc[0][1], u, agentId);
    }
    return u;
  }
  // for each child node, get the minmax value
  else {
    // update next agent id and depth used to pass recursively
    int next_agent = agentId;
    if(next_agent >= cats) {
      next_agent = 0;
      depth += 1;
    } else {
      next_agent += 1;
    }

    /*************************mouse move***************************************/
    if(agentId <= 0) {
      int next_mouse_loc[1][2];
      int mouse_x = mouse_loc[0][0];
      int mouse_y = mouse_loc[0][1];
      int mouse_index;
      xy_to_index(mouse_x, mouse_y, &mouse_index);

      char best_move[4] = {'t', 'r', 'b', 'l'};
      int child_xy[4][2];
      // top
      child_xy[0][0] = mouse_x;
      child_xy[0][1] = mouse_y - 1;
      // right
      child_xy[1][0] = mouse_x + 1;
      child_xy[1][1] = mouse_y;
      // bottom
      child_xy[2][0] = mouse_x;
      child_xy[2][1] = mouse_y + 1;
      // left
      child_xy[3][0] = mouse_x - 1;
      child_xy[3][1] = mouse_y;

      // double best_util = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
      double best_util = -999;
      if(depth == 0) {
        path[0][0] = mouse_x;
        path[0][1] = mouse_y;
      }
      if(depth == 0){
        printf("util at position %d %d is [%f]\n", mouse_x, mouse_y, best_util);
      }
      for (int i = 0; i < 4; i++) {
        // find utility for mouse moving around top/right/bottom/left
        if(gr[mouse_index][i] >= 1) {
          next_mouse_loc[0][0] = child_xy[i][0];
          next_mouse_loc[0][1] = child_xy[i][1];
          // // printf("%s  finding util for mouse moving top..\n", prefix);
          double util = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, next_mouse_loc, mode, utility, next_agent, depth, maxDepth, alpha, beta);
          if (depth == 0)
          {
            printf("util of %c is [%f]\n", best_move[i], util);
          }
          // // printf("%s  found util for mouse moving top: [%f]\n", prefix, util);
          if(best_util < util) {
            best_util = util;
            // best_move = 't';
            if(depth == 0) {
              path[0][0] = child_xy[i][0];
              path[0][1] = child_xy[i][1];
            }
          }
          // ab pruning
          if(mode == 1) {
            if(best_util > alpha) {
              alpha = best_util;
            }
            if(alpha > beta) {
              minmax_cost[mouse_x][mouse_y] = best_util;
              return best_util;
            }
          }// end of ab pruning
        }
      }
      minmax_cost[mouse_x][mouse_y] = best_util;
      return best_util;
    } // end of mouse

    /***************************cat move***************************************/
    else {
      int next_cat_loc[10][2];
      memcpy(next_cat_loc, cat_loc, sizeof(int) * 10 * 2);
      int cat_x = cat_loc[agentId - 1][0];
      int cat_y = cat_loc[agentId - 1][1];
      int cat_index;
      xy_to_index(cat_x, cat_y, &cat_index);

      int child_xy[4][2];
      // top
      child_xy[0][0] = cat_x;
      child_xy[0][1] = cat_y - 1;
      // right
      child_xy[1][0] = cat_x + 1;
      child_xy[1][1] = cat_y;
      // bottom
      child_xy[2][0] = cat_x;
      child_xy[2][1] = cat_y + 1;
      // left
      child_xy[3][0] = cat_x - 1;
      child_xy[3][1] = cat_y;

      double best_util = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);

      for (int i = 0; i < 4; i++) {
        if(gr[cat_index][i] >= 1) {
          next_cat_loc[agentId - 1][0] = child_xy[i][0];
          next_cat_loc[agentId - 1][1] = child_xy[i][1];
          // printf("%s  finding util for cat[%d]  moving top..\n", prefix, agentId - 1);
          double util = MiniMax(gr, path, minmax_cost, next_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agent, depth, maxDepth, alpha, beta);
          // printf("%s  found util for cat[%d] moving top: [%f]\n", prefix, agentId - 1, util);
          if(best_util > util) {
            best_util = util;
          }
          // ab pruning
          if(mode == 1) {
            if(best_util < beta) {
              beta = best_util;
            }
            if(alpha > beta) {
              return best_util;
            }
          }
        }
      }
      return best_util;
    }// end of cat
  }// end of none terminal stated
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]) {
  static int dijkstraed = 0;
  static int gr_min_dists[graph_size][graph_size];
  if(dijkstraed == 0) {
    // printf("dijkstra_ing the maze.... \n");
    for (int j = 0; j < graph_size; j++) {
      for (int i = 0; i < graph_size; ++i) {
        gr_min_dists[j][i] = 999999;
      }
      dijkstra(gr, j, gr_min_dists[j]);
    }
    // printf("dijkstra_ed maze:\n");
    // for (int j = 0; j < graph_size; j++) {
    //   printf("min distance from node %d....\n", j);
    //   int count = 0;
    //   for (int i = 0; i < graph_size; i++) {
    //     count += 1;
    //     printf("%d ", gr_min_dists[j][i]);
    //     if(count == size_X) {
    //       printf("\n");
    //       count = 0;
    //     }
    //   }
    // }
    dijkstraed = 1;
  }

  // simple utility function be the distance from mouse to closest cheese - distance between mouse to closest cat
  int min_dis_to_cheese = 99999;
  int min_cheese_ind;
  int mouse_ind;
  xy_to_index(mouse_loc[0][0], mouse_loc[0][1], &mouse_ind);
  for (int i = 0; i < cheeses; i++) {
    xy_to_index(cheese_loc[i][0], cheese_loc[i][1], &min_cheese_ind);
    min_dis_to_cheese = fmin(min_dis_to_cheese, gr_min_dists[mouse_ind][min_cheese_ind]);
  }

  int min_dis_to_cat = 99999;
  // int total_cat_dist = 0;
  int min_cat_ind;
  int min_cat;
  for (int i = 0; i < cats; i++) {
    xy_to_index(cat_loc[i][0], cat_loc[i][1], &min_cat_ind);
    if(min_dis_to_cat > gr_min_dists[mouse_ind][min_cat_ind]){
      min_dis_to_cat = gr_min_dists[mouse_ind][min_cat_ind];
      min_cat = min_cat_ind;
    }
    // min_dis_to_cat = fmin(min_dis_to_cat, gr_min_dists[mouse_ind][min_cat_ind]);
    // total_cat_dist += gr_min_dists[mouse_ind][min_cat_ind];
  }
  // min_dis_to_cat = min_dis_to_cat > 3 ? 2*min_dis_to_cat : 0.2 * min_dis_to_cat;
  // min_dis_to_cheese = min_dis_to_cheese < 3 ? 0.2 * min_dis_to_cheese : min_dis_to_cheese;
  // return  graph_size - (min_dis_to_cheese);
  // return  min_dis_to_cat < 5 ? -46 - min_dis_to_cat : (46 - min_dis_to_cheese);
  // return  graph_size - (min_dis_to_cat - min_dis_to_cheese);
  // if(min_dis_to_cat < 6){
  //   // int skip_path = 0;
  //   // for (int i = 0; i < graph_size; i++)
  //   // {
  //   //   if(skip_path < gr_min_dists[min_cat][i]){
  //       // skip_path = gr_min_dists[min_cat][i];
  //   //   }
  //   // }
  //   return gr_min_dists[min_cat][mouse_ind];
  // }else{
  //   return 46 - min_dis_to_cheese - depth;
  // }
  return gr_min_dists[min_cat][mouse_ind] - min_dis_to_cheese - depth;
}

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2], int cheese_loc[10][2], int cats, int cheeses) {
  /*
    This function determines whether a given configuration constitutes a terminal node.
    Terminal nodes are those for which:
      - A cat eats the mouse
      or
      - The mouse eats a cheese

    If the node is a terminal, the function returns 1, else it returns 0
  */

  // Check for cats having lunch
  for (int i = 0; i < cats; i++)
    if (mouse_loc[0][0] == cat_loc[i][0] && mouse_loc[0][1] == cat_loc[i][1]) return(1);

  // Check for mouse having lunch
  for (int i = 0; i < cheeses; i++)
    if (mouse_loc[0][0] == cheese_loc[i][0] && mouse_loc[0][1] == cheese_loc[i][1]) return(1);

  return(0);
}

//////////////////////
// helper functions //
//////////////////////

/**
 * Given a graph and index of the source node, find the length of the shortest
 * path from src node to the rest nodes of the graph
 */
void dijkstra(double gr[graph_size][4], int src_index, int min_dist_from_src[graph_size]) {
  // initialize variables
  int visit_his[graph_size];
  memset(visit_his, -1, sizeof(int) * graph_size);

  min_dist_from_src[src_index] = 0;

  /* check for termination, when the min distance in min_dist_from_src is inf,
     this means we have checked all the nodes that can be reached from src node,
     the algorithm ends here
   */
  int traced_all = 999999;
  for (int i = 0; i < graph_size; i++) {
    if(visit_his[i] == -1) {
      traced_all = fmin(min_dist_from_src[i], traced_all);
    }
  }

  while(traced_all < 999999) {
    // next node to expand is the unvisited node with the smallest distance to src
    int node = src_index;
    int min_dist = 999999;
    for (int i = 0; i < graph_size; i++) {
      if(visit_his[i] == -1 && min_dist_from_src[i] < min_dist) {
        node = i;
        min_dist = min_dist_from_src[i];
      }
    }
    // get x,y coordinate of the node, use it to get the index of the neighbor nodes
    int node_x, node_y;
    index_to_xy(&node_x, &node_y, node);
    int neighbor_ind;
    // check the neighbors of next node
    if(gr[node][0] >= 1) {
      xy_to_index(node_x, node_y - 1, &neighbor_ind);
      min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
    }
    // right neighbor
    if(gr[node][1] >= 1) {
      xy_to_index(node_x + 1, node_y, &neighbor_ind);
      min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
    }
    // bottom neighbor
    if(gr[node][2] >= 1) {
      xy_to_index(node_x, node_y + 1, &neighbor_ind);
      min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
    }
    // left neighbor
    if(gr[node][3] >= 1) {
      xy_to_index(node_x - 1, node_y, &neighbor_ind);
      min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
    }

    // mark this node as visited
    visit_his[node] = 1;
    // check for termination.....
    traced_all = 999999;
    for (int i = 0; i < graph_size; i++) {
      if(visit_his[i] == -1) {
        traced_all = fmin(min_dist_from_src[i], traced_all);
      }
    }

  }// end while
}
