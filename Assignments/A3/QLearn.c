/*
  CSC D84 - Unit 3 - Reinforcement Learning

  This file contains stubs for implementing the Q-Learning method
  for reinforcement learning as discussed in lecture. You have to
  complete two versions of Q-Learning.

  * Standard Q-Learning, based on a full-state representation and
    a large Q-Table
  * Feature based Q-Learning to handle problems too big to allow
    for a full-state representation

  Read the assignment handout carefully, then implement the
  required functions below. Sections where you have to add code
  are marked

  **************
  *** TO DO:
  **************

  If you add any helper functions, make sure you document them
  properly and indicate in the report.txt file what you added.

  Have fun!

  DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
  management being done properly, and for memory leaks.

  Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable) {
    /*
      This function implementes the Q-Learning update as stated in Lecture. It
      receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.

      Your work here is to calculate the required update for the Q-table entry
      for state s, and apply it to the Q-table

      The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not
      have to change their values. Use them as they are.

      Details on how states are used for indexing into the QTable are shown
      below, in the comments for QLearn_action. Be sure to read those as well!
    */
    int cur_state_action_entry = s * 4 + a; // this is a QTable_entry
    int res_state_entry = s_new * 4;
    double diff = -9999.00;
    // diff = lambda * max(Q(s', a'))
    for (int i = 0; i < 4; i++) {
        if((QTable[res_state_entry + i] - QTable[cur_state_action_entry]) > diff) {
            diff = (QTable[res_state_entry + i] - QTable[cur_state_action_entry]);
            // diff = (QTable[res_state_entry + i] > diff) ? QTable[res_state_entry + i] : diff;
        }
    }
    // Q(s, a) = alpha * (r + lambda * max(Q(s', a')) - Q(S, a))
    QTable[cur_state_action_entry] = QTable[cur_state_action_entry] + alpha * (r + lambda * diff);
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size) {
    // to loop through possible mouse move directions
    int next_mouse_move[4][2];
    // top
    next_mouse_move[0][0] = mouse_pos[0][0];
    next_mouse_move[0][1] = mouse_pos[0][1] - 1;
    // right
    next_mouse_move[1][0] = mouse_pos[0][0] + 1;
    next_mouse_move[1][1] = mouse_pos[0][1];
    // bottom
    next_mouse_move[2][0] = mouse_pos[0][0];
    next_mouse_move[2][1] = mouse_pos[0][1] + 1;
    // left
    next_mouse_move[3][0] = mouse_pos[0][0] - 1;
    next_mouse_move[3][1] = mouse_pos[0][1];

    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

    /********************* get current state s *********************/
    int s = get_state(mouse_pos[0][0], mouse_pos[0][1], cats[0][0], cats[0][1], cheeses[0][0], cheeses[0][1], size_X, graph_size);

    /********************* get action a /*********************/
    int a = 0;
    // choose valid action randomly
    double dice =  rand() % 101 / 100.0;
    if(dice > pct) {
        a = rand() % 4;
        while(gr[mouse_index][a] < 1) {
            a = rand() % 4;
        }
    } else {
        double opt = -999999;
        for (int i = 0; i < 4; i++) {
            if(gr[mouse_index][i] >= 1) {
                if(QTable[s * 4 + i] > opt) {
                    opt = QTable[s * 4 + i];
                    a = i;
                }
            }
        }
    }
    /********************* get reward r *********************/
    double r = QLearn_reward(gr, mouse_pos, cats, cheeses, size_X, graph_size);

    /********************* get result state s' *********************/
    int s_new = get_state(next_mouse_move[a][0], next_mouse_move[a][1], cats[0][0], cats[0][1], cheeses[0][0], cheeses[0][1], size_X, graph_size);

    // update QTable based on sars
    QLearn_update(s, a, r, s_new, QTable);
    return a;

    /* two component that interact with each other
     * process for development, code review
     * how to keep work on time
     * cool/bad mat
     *
       This function decides the action the mouse will take. It receives as inputs
       - The graph - so you can check for walls! The mouse must never move through walls
       - The mouse position
       - The cat position
       - The chees position
       - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
         for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
         the remaining 75% of the time it chooses randomly among the available actions.

       Remember that the training process involves random exploration initially, but as training
       proceeds we use the QTable more and more, in order to improve our QTable values around promising
       actions.

       The value of pct is controlled by QLearn_core_GL, and increases with each round of training.

       This function *must return* an action index in [0,3] where
          0 - move up
          1 - move right
          2 - move down
          3 - move left

       QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
       the mouse leave the map - this should not happen. If you see a warning, fix the code in this
       function!

     The Q-table has been pre-allocated and initialized to 0. The Q-table has
     a size of

          graph_size^3 x 4

     This is because the table requires one entry for each possible state, and
     the state is comprised of the position of the mouse, cat, and cheese.
     Since each of these agents can be in one of graph_size positions, all
     possible combinations yield graph_size^3 states.

     Now, for each state, the mouse has up to 4 possible moves (up, right,
     down, and left). We ignore here the fact that some moves are not possible
     from some states (due to walls) - it is up to the QLearn_action() function
     to make sure the mouse never crosses a wall.

     So all in all, you have a big table.

     For example, on an 8x8 maze, the Q-table will have a size of

         64^3 x 4  entries

         with

         size_X = 8   <--- size of one side of the maze
         graph_size = 64    <--- Total number of nodes in the graph

     Indexing within the Q-table works as follows:

       say the mouse is at   i,j
           the cat is at     k,l
           the cheese is at  m,n

       state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
       ** Make sure you undestand the state encoding above!

       Entries in the Q-table for this state are

       *(QTable+(4*state)+a)      <-- here a is the action in [0,3]

       (yes, it's a linear array, no shorcuts with brackets!)

       NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
     */
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
    static int dijkstraed = 0;
    static int dijkstra_graph[max_graph_size][max_graph_size];
    if(dijkstraed == 0) {
        printf("Qlearning..initializing dijkstra's algorithm on current maze..please wait...........\n");
        for (int i = 0; i < graph_size; i++) {
            memset(dijkstra_graph[i], 999999, sizeof(int) * graph_size);
            dijkstra(gr, i, dijkstra_graph[i], size_X, graph_size);
        }
        printf("Finished dijkstra's algorithm on current maze... thanks for the patience!!\n");
        if(save_dijkstra) {
            printf("Saving dijkstra result to file....\n");
            FILE *dijk = fopen("./dijkstra_table.txt", "w");
            for (int j = 0; j < graph_size; j++) {
                fprintf(dijk, "min distance from node %d....\n", j);
                int count = 0;
                for (int i = 0; i < graph_size; i++) {
                    count += 1;
                    fprintf(dijk, "%d ", dijkstra_graph[j][i]);
                    if(count == size_X) {
                        fprintf(dijk, "\n");
                        count = 0;
                    }
                }
            }
            fclose(dijk);
            printf("Saved dijkstra result to file....\n");
        }
        dijkstraed = 1;
    }

    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
    int cat_index = xy_to_index(cats[0][0], cats[0][1], size_X);
    int cheese_index = xy_to_index(cheeses[0][0], cheeses[0][1], size_X);

    if(dijkstra_graph[mouse_index][cheese_index] <= 0) {
        return 1;
    } else if (dijkstra_graph[mouse_index][cat_index] <= 0) {
        return -1;
    } else {
        return 0;
    }
    /*
      This function computes and returns a reward for the state represented by the input mouse, cat, and
      cheese position.

      You can make this function as simple or as complex as you like. But it should return positive values
      for states that are favorable to the mouse, and negative values for states that are bad for the
      mouse.

      I am providing you with the graph, in case you want to do some processing on the maze in order to
      decide the reward.

      This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.
     */
}

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

    double maxU = -999999;
    int a;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &a);

    // features' value at current state
    double current_features_value[25];
    evaluateFeatures(gr, current_features_value, mouse_pos, cats, cheeses, size_X, graph_size);
    double current_state_value = Qsa(weights, current_features_value);
    // update weight of each feature
    for (int f = 0; f < numFeatures; f++) {
        weights[f] = weights[f] + alpha * (reward + lambda * maxU - current_state_value) * current_features_value[f];
    }
    /*
      This function performs the Q-learning adjustment to all the weights associated with your features
      you receive the current state (mouse, cats, and cheese potisions), and the reward
      associated with this action (this is called immediately after the mouse makes a move,
      so implicit in this is the mouse having selected some action)

      Your code must then evaluate the update and apply it to the weights in the weight array.
     */

}

int feat_QLearn_action(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size) {
    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

    /********************* get action a /*********************/
    int a = 0;
    double dice =  rand() % 101 / 100.0; // dice = [0.00, 1.00]
    // choose a valid action randomly
    if(dice > pct) {
        a = rand() % 4;
        while(gr[mouse_index][a] < 1) {
            a = rand() % 4;
        }
    }
    // choose the optimal action at current state
    else {
        double maxU;
        maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &a);
    }
    return a;
    /*
      Similar to its counterpart for standard Q-learning, this function returns the index of the next
      action to be taken by the mouse.

      Once more, the 'pct' value controls the percent of time that the function chooses an optimal
      action given the current policy.

      E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
      the optimal action. The remaining 85% of the time, a random action is chosen.

      As before, the mouse must never select an action that causes it to walk through walls or leave
      the maze.
     */
}

void evaluateFeatures(double gr[max_graph_size][4], double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
    /**
     *      Features
     *  features[0]: Path length to closest cheese [0,1)
     *  features[1]: Total path length from cheeses to mouse
     *  features[2]: Path length to closest cat (-1,0]
     *  features[3]: Total path length from cats to mouse
     *  features[4]: Path length from closest cheese to it's closest cat
     */
    static int dijkstraed = 0;
    static int dijkstra_graph[max_graph_size][max_graph_size];
    if(dijkstraed == 0) {
        printf("Featured Qlearning..initializing dijkstra's algorithm on current maze..please wait...........\n");
        for (int i = 0; i < graph_size; i++) {
            memset(dijkstra_graph[i], 999999, sizeof(int) * graph_size);
            dijkstra(gr, i, dijkstra_graph[i], size_X, graph_size);
        }
        printf("Finished dijkstra's algorithm on current maze... thanks for the patience!!\n");
        if(save_dijkstra) {
            printf("Saving dijkstra result to file....\n");
            FILE *dijk = fopen("./dijkstra_table.txt", "w");
            for (int j = 0; j < graph_size; j++) {
                fprintf(dijk, "min distance from node %d....\n", j);
                int count = 0;
                for (int i = 0; i < graph_size; i++) {
                    count += 1;
                    fprintf(dijk, "%d ", dijkstra_graph[j][i]);
                    if(count == size_X) {
                        fprintf(dijk, "\n");
                        count = 0;
                    }
                }
            }
            fclose(dijk);
            printf("Saved dijkstra result to file....\n");
        }
        dijkstraed = 1;
    }
    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
    // find closest cheese
    int closest_cheese_index = xy_to_index(cheeses[0][0], cheeses[0][1], size_X);
    features[1] = dijkstra_graph[mouse_index][closest_cheese_index]; // total distances from cheeses to mouse
    for (int c = 0; c < 5; c++) {
        int next_cheese_index = xy_to_index(cheeses[c][0], cheeses[c][1], size_X);
        if(next_cheese_index >= 0) {
            features[1] += dijkstra_graph[mouse_index][next_cheese_index];
            closest_cheese_index = (dijkstra_graph[mouse_index][next_cheese_index] < dijkstra_graph[mouse_index][closest_cheese_index]) ? next_cheese_index : closest_cheese_index;
        } else {
            break; // no more cheeses
        }
    }

    // find closest cat
    int closest_cat_index = xy_to_index(cats[0][0], cats[0][1], size_X);
    features[3] = dijkstra_graph[mouse_index][closest_cat_index]; // total distances from cats to mouse
    for (int c = 1; c < 5; c++) {
        int next_cat_index = xy_to_index(cats[c][0], cats[c][1], size_X);
        if(next_cat_index >= 0) {
            features[3] += dijkstra_graph[mouse_index][next_cat_index];
            closest_cat_index = (dijkstra_graph[mouse_index][next_cat_index] < dijkstra_graph[mouse_index][closest_cat_index]) ?  next_cat_index : closest_cat_index;
        } else {
            break; // no more cats
        }
    }

    // evaluate features
    features[0] = 1 - ((double) dijkstra_graph[mouse_index][closest_cheese_index] / (2.0 * size_X));//graph_size);
    features[1] = 1 - ((double) features[1] / (double)(size_X * 3.0));

    features[2] = (double) dijkstra_graph[mouse_index][closest_cat_index] / (2.0 * size_X) - 1;
    features[3] = (double) features[3] / (double) (size_X * 3.0) - 1;

    features[4] = 999999.00; // path length from closest cheese to its closest cat
    for (int i = 0; i < 5; ++i) {
        int cat_index = xy_to_index(cats[i][0], cats[i][1], size_X);
        if(cat_index >= 0) {
            features[4] = fmin(features[4], dijkstra_graph[closest_cheese_index][cat_index]);
        } else {
            break;
        }
    }

    features[4] = (double) features[4] / (double) (2.0 * size_X);

    /*
     This function evaluates all the features you defined for the game configuration given by the input
     mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
     evaluate each, and return all the feature values in the features[] array.

     Take some time to think about what features would be useful to have, the better your features, the
     smarter your mouse!

     Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
     the arrays themselves will tell you what are valid cat/cheese locations.

     You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
     will have a value of -1 - check this when evaluating your features!
    */
}

double Qsa(double weights[25], double features[25]) {
    /*
      Compute and return the Qsa value given the input features and current weights
     */
    double res = 0;
    for (int i = 0; i < numFeatures; i++) {
        res = res + (weights[i] * features[i]);
    }
    return res;
}

void maxQsa(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA) {
    /*
      Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
      the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
      and the index of the action corresponding to this value is returned in maxA.

      You should make sure the function does not evaluate moves that would make the mouse walk through a
      wall.
     */
    int next_mouse_move[4][2];
    // top
    next_mouse_move[0][0] = mouse_pos[0][0];
    next_mouse_move[0][1] = mouse_pos[0][1] - 1;
    // right
    next_mouse_move[1][0] = mouse_pos[0][0] + 1;
    next_mouse_move[1][1] = mouse_pos[0][1];
    // bottom
    next_mouse_move[2][0] = mouse_pos[0][0];
    next_mouse_move[2][1] = mouse_pos[0][1] + 1;
    // left
    next_mouse_move[3][0] = mouse_pos[0][0] - 1;
    next_mouse_move[3][1] = mouse_pos[0][1];

    int mouse_index = xy_to_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

    *maxU = -999999;
    for (int a = 0; a < 4; a++) {
        if(gr[mouse_index][a] >= 1) {
            int next_mouse_pos[1][2];
            next_mouse_pos[0][0] = next_mouse_move[a][0];
            next_mouse_pos[0][1] = next_mouse_move[a][1];
            double features_value[25];
            evaluateFeatures(gr, features_value, next_mouse_pos, cats, cheeses, size_X, graph_size);
            double neighbor_state_value = Qsa(weights, features_value);
            if(neighbor_state_value > *(maxU)) {
                *maxU = neighbor_state_value;
                *maxA = a;
            }
        }
    }
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
//////////////////////
// helper functions //
//////////////////////

/**
 * Given a graph and index of the source node, find the length of the shortest
 * path from src node to the rest nodes of the graph
 * Result is stored in @min_dist_from_src
 * ie. min_dist_from_src[src_index] = 0
 */
void dijkstra(double gr[max_graph_size][4], int src_index, int min_dist_from_src[max_graph_size], int size_X, int graph_size) {
    // initialize variables
    int visit_his[graph_size];
    memset(visit_his, -1, sizeof(int) * graph_size);
    memset(min_dist_from_src, 999999, sizeof(int) * graph_size);
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

    int node = src_index;
    while(traced_all < 999999) {
        // next node to expand is the unvisited node with the smallest distance to src
        int min_dist = 999999;
        for (int i = 0; i < graph_size; i++) {
            if(visit_his[i] == -1 && min_dist_from_src[i] < min_dist) {
                node = i;
                min_dist = min_dist_from_src[i];
            }
        }
        // get x,y coordinate of the node, use it to get the index of the neighbor nodes
        int node_x, node_y;
        index_to_xy(&node_x, &node_y, node, size_X);

        // check the neighbors of next node
        if(gr[node][0] >= 1) {
            int neighbor_ind = xy_to_index(node_x, node_y - 1, size_X);
            min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
        }
        // right neighbor
        if(gr[node][1] >= 1) {
            int neighbor_ind = xy_to_index(node_x + 1, node_y, size_X);
            min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
        }
        // bottom neighbor
        if(gr[node][2] >= 1) {
            int neighbor_ind = xy_to_index(node_x, node_y + 1, size_X);
            min_dist_from_src[neighbor_ind] = fmin(min_dist_from_src[neighbor_ind], min_dist_from_src[node] + 1);
        }
        // left neighbor
        if(gr[node][3] >= 1) {
            int neighbor_ind = xy_to_index(node_x - 1, node_y, size_X);
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
