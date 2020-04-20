/*
  CSC D84 - Unit 1 - Search

  This file contains stubs for implementing the different search
  algorithms covered in the course. Please read the assignment
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

  Starter code: F.J.E., Jul. 15
  Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Charles Xu
% 2) Student Name: Jinming Zhang
%
% 1) Student number: 1004264271
% 2) Student number: 1000997503
%
% 1) UtorID: xucharle
% 2) UtorID: zhan2754
%
% We hereby certify that the work contained here is our own
%
% _Charles Xu_________             _Jinming Zhang_______
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"
#include <string.h>
#include <unistd.h>
void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{

  // fprintf(stdout, ">---------- Search ----------<\n");
  int queue[graph_size];        // list of nodes to expand
  int visit_his[graph_size];    // mark each node is visited or not
  int predesessors[graph_size]; // predecessor of each node as searching goes
  // initialize lists and variables
  memset(queue, -1, graph_size * sizeof(int));
  memset(visit_his, -1, graph_size * sizeof(int));
  memset(predesessors, -1, graph_size * sizeof(int));

  int queue_length = 0; // indicate current length of the list
  int visit_count = 1;  // for counting visit order

  int mouse_index = mouse_loc[0][1] * size_Y + mouse_loc[0][0];
  queue[0] = mouse_index; // add current mouse location to the list
  queue_length += 1;      // increment list length

  fprintf(stdout, "mouse index %d\n", mouse_index);
  fprintf(stdout, "mouse at: %d %d\n", mouse_loc[0][0], mouse_loc[0][1]);
  // bfs
  if (mode == 0)
  {
    fprintf(stdout, "----- BFS -----\n");
    BFS(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, queue, queue_length, visit_his, predesessors, 0);
    return;
  }
  // dfs
  else if (mode == 1)
  {
    fprintf(stdout, "----- DFS -----\n");
    DFS(gr, path, visit_order, cats, cat_loc, cheeses, cheese_loc, mouse_loc[0]);
    return;
  }
  // A*
  else if (mode == 2)
  {
    fprintf(stdout, "----- A* -----\n");
    int dist_from_mouse[graph_size]; // list for track each node's smallest distance from mouse
    memset(dist_from_mouse, -1, graph_size * sizeof(int));
    dist_from_mouse[mouse_index] = 0;
    Astar(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, queue, queue_length, visit_his, predesessors, dist_from_mouse, heuristic, 0);
    return;
  }
  // A* no kitties
  else if (mode == 3)
  {
    fprintf(stdout, "----- A* no kitties-----\n");
    int dist_from_mouse[graph_size];
    memset(dist_from_mouse, -1, graph_size * sizeof(int));
    dist_from_mouse[mouse_index] = 0;
    Astar(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, queue, queue_length, visit_his, predesessors, dist_from_mouse, heuristic, 0);
    return;
  }
  // unknown
  else
  {
    fprintf(stdout, "Unknown mode\n");
    return;
  }
  return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{

  // return the distance between location (x,y) and the closest cheese
  // distance between node and the closest cheese as heuristic value
  int h = size_X + size_Y;
  for (int i = 0; i < cheeses; i++)
  {
    int dist_x = cheese_loc[i][0] - x;
    int dist_y = cheese_loc[i][1] - y;
    h = (int)fmin(h, fabs(dist_x) + fabs(dist_y));
    // h = fmin(h, ceil(sqrt(pow(dist_x, 2) + pow(dist_y, 2))));
  }
  return h;
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
  /*
  This function computes and returns the heuristic cost for location x,y.
  As discussed in lecture, this means estimating the cost of getting from x,y to the goal.
  The goal is cheese.

  However - this time you want your heuristic function to help the mouse avoid being eaten.
  Therefore: You have to somehow incorporate knowledge of the cats' locations into your
  heuristic cost estimate. How well you do this will determine how well your mouse behaves
  and how good it is at escaping kitties.

  This heuristic *does not have to* be admissible.
  *After multiply all heuristic with a constant, does it still admissible?*
  Input arguments have the same meaning as in the H_cost() function above.
  */

  // int h_cheese = 9999999;
  // for (int i = 0; i < cheeses; i++)
  // {
  //   int dist_x = cheese_loc[i][0] - x;
  //   int dist_y = cheese_loc[i][1] - y;
  //   h_cheese = fmin(h_cheese, ceil(sqrt(pow(dist_x, 2) + pow(dist_y, 2))));
  // }

  // int h_cat = 99999999; // find the distance to closest cat
  // for (int i = 0; i < cats; i++)
  // {
  //   int dist_x = cat_loc[i][0] - x;
  //   int dist_y = cat_loc[i][1] - y;
  //   h_cat = fmin(h_cat, ceil(sqrt(pow(dist_x, 2) + pow(dist_y, 2))));
  // }
  // // if the closest cat from a node is further, then it's heuristic value should be lower
  // // achived by assuming the furtherest distance to a cat is graph_size 32*32
  // // the heuristic value for cats part will be the furtherest distance - closest cat distance
  // // so the further the cat is, the lower the heuristic is (more priority to be expanded)
  // return h_cheese + (graph_size - h_cat);

  int max_length = size_X + size_Y;

  // This is the shortest distance to a cheese.
  int h_cheese = max_length;
  for (int i = 0; i < cheeses; i++)
  {
    int dist_x = cheese_loc[i][0] - x;
    int dist_y = cheese_loc[i][1] - y;
    h_cheese = (int)fmin(h_cheese, fabs(dist_x) + fabs(dist_y));
  }

  // This is the shortest distance to a cat.
  // int h_cat = max_length;
  // for (int i = 0; i < cats; i++)
  // {
  //   int dist_x = cat_loc[i][0] - x;
  //   int dist_y = cat_loc[i][1] - y;
  //   h_cat = (int)fmin(h_cat, fabs(dist_x) + fabs(dist_y));
  // }

  // double danger = ((double) h_cat) / ((double) max_length);
  // return h_cheese / danger;

  // We calculate a "danger" value based on how cloose we are to a cat, relative to max length.
  // double danger = 1.0 - ((double) h_cat) / ((double) max_length);
  // return h_cheese * danger;

  double danger = 1.0;

  for (int i = 0; i < cats; i++)
  {
    int dist_x = cat_loc[i][0] - x;
    int dist_y = cat_loc[i][1] - y;
    double cat_danger = (fabs(dist_x) + fabs(dist_y)) / ((double)max_length);
    danger = fmin(danger, cat_danger);
  }

  // [0, 1] -> [dangerous, not dangerous]

  return h_cheese / danger;
}

/**
 *
 */

void BFS(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int queue[graph_size], int queue_length, int visit_his[graph_size], int predesessors[graph_size], int visit_count)
{

  // fprintf(stdout, "----------BFS  %d--------\n", visit_count);
  // queue empty, no path to cheese
  if (queue_length <= 0)
  {
    path[0][0] = mouse_loc[0][0];
    path[0][1] = mouse_loc[0][1];
    path[1][0] = mouse_loc[0][0];
    path[1][1] = mouse_loc[0][1];
    fprintf(stdout, "Can't find path, staying\n");
    return;
  }
  else
  {
    // extract next node to expand
    int node_index = queue[0];
    int node_x = node_index % size_X;
    int node_y = node_index / size_Y;
    // fprintf(stdout, "node: %d, (%d, %d)\n", node_index, node_x, node_y);
    visit_order[node_x][node_y] = visit_count;
    visit_his[node_index] = 1;
    // update queue
    for (int l = 1; l < queue_length; l++)
    {
      queue[l - 1] = queue[l];
    }
    queue_length -= 1;
    // found cheese, return path
    if (has_cheese(node_index, cheese_loc, cheeses))
    {
      fprintf(stdout, "found cheese (%d %d) at count %d\n", node_x, node_y, visit_count);
      int path_length = 0;
      int predesessor_index = node_index;
      // find the path length
      while (predesessors[predesessor_index] >= 0)
      {
        path_length += 1;
        predesessor_index = predesessors[predesessor_index];
      }
      // fprintf(stdout, "path to cheese: ");
      // store the path into path variable
      while (predesessors[node_index] >= 0)
      {
        path[path_length][0] = node_index % size_X;
        path[path_length][1] = node_index / size_Y;
        // fprintf(stdout, "(%d, %d) ->", node_index % size_X, node_index / size_Y);
        // fprintf(stdout, "%d ", path_length);
        node_index = predesessors[node_index];
        path_length -= 1;
        // fprintf(stdout, "(%d, %d) ->", path[path_length][0], path[path_length][1]);
      }
      // fprintf(stdout, "\n");

      path[0][0] = mouse_loc[0][0];
      path[0][1] = mouse_loc[0][1];
      return;
    }
    // else expand next node to the queue
    else
    {
      // top
      int top_node_index = (node_y - 1) * size_X + node_x;
      // if it's not visited and it's connected and it doesn't have a cat
      if (visit_his[top_node_index] < 0 && gr[node_index][0] == 1 && has_cat(top_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = top_node_index;
        queue_length += 1;
        predesessors[top_node_index] = node_index;
      }
      // right
      int right_node_index = node_y * size_X + node_x + 1;
      if (visit_his[right_node_index] < 0 && gr[node_index][1] == 1 && has_cat(right_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = right_node_index;
        queue_length += 1;
        predesessors[right_node_index] = node_index;
      }
      // bottom
      int bottom_node_index = (node_y + 1) * size_X + node_x;
      if (visit_his[bottom_node_index] < 0 && gr[node_index][2] == 1 && has_cat(bottom_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = bottom_node_index;
        queue_length += 1;
        predesessors[bottom_node_index] = node_index;
      }
      // left
      int left_node_index = node_y * size_X + (node_x - 1);
      if (visit_his[left_node_index] < 0 && gr[node_index][3] == 1 && has_cat(left_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = left_node_index;
        queue_length += 1;
        predesessors[left_node_index] = node_index;
      }

      // recursively expand next node
      visit_count += 1;
      return BFS(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, queue, queue_length, visit_his, predesessors, visit_count);
    }
  }
}

void DFS(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_count, int cat_loc[10][2], int cheese_count, int cheese_loc[10][2], int mouse_loc[2])
{

  printf("Currently at: (%d, %d)\n", mouse_loc[0], mouse_loc[1]);
  // Graph size.
  int memory_size = size_X * size_Y * sizeof(int);

  // Create a new stack for DFS.
  int *stack = (int *)malloc(memory_size);
  int stack_size = 0;

  // Create a set of predecessors.
  int *pred = (int *)malloc(memory_size);

  // Cleans the default data.
  for (int x = 0; x < size_X; x++)
  {
    for (int y = 0; y < size_Y; y++)
    {
      visit_order[x][y] = -1;
      pred[x + (y * size_X)] = -2;
    }
  }

  // Add the first node onto the stack.
  int mouse_pos = to_coordinate(mouse_loc[0], mouse_loc[1]);
  stack[stack_size++] = mouse_pos;

  // This are the amount of nodes visited.
  int visit_count = 1;

  // Set the initial visit.
  pred[mouse_pos] = -1;
  visit_order[mouse_loc[0]][mouse_loc[1]] = visit_count++;

  int dest = -1;

  while (stack_size > 0)
  {
    stack_size--;

    // This is the coordinate at the top of the stack.
    int loc = stack[stack_size];

    // This is the position that the coordinate references to.
    int x = loc % 32;
    int y = loc / 32;

    // Check if we've found the cheese.
    if (has_cheese(loc, cheese_loc, cheese_count))
    {
      dest = loc;
      break;
    }

    // Visit the top neighbour, if it's connected AND we haven't queued it yet AND there is no cat.
    int top_coords = to_coordinate(x, y - 1);
    if (gr[loc][0] == 1 && pred[top_coords] == -2 && has_cat(top_coords, cat_loc, cat_count) == 0)
    {
      pred[top_coords] = loc;
      visit_order[x][y - 1] = visit_count++;
      stack[stack_size++] = top_coords;
    }

    // Visit the right neighbour, if it's connected AND we haven't queued it yet AND there is no cat.
    int right_coords = to_coordinate(x + 1, y);
    if (gr[loc][1] == 1 && pred[right_coords] == -2 && has_cat(right_coords, cat_loc, cat_count) == 0)
    {
      pred[right_coords] = loc;
      visit_order[x + 1][y] = visit_count++;
      stack[stack_size++] = right_coords;
    }

    // Visit the bottom neighbour, if it's connected AND we haven't queued it yet AND there is no cat.
    int bot_coords = to_coordinate(x, y + 1);
    if (gr[loc][2] == 1 && pred[bot_coords] == -2 && has_cat(bot_coords, cat_loc, cat_count) == 0)
    {
      pred[bot_coords] = loc;
      visit_order[x][y + 1] = visit_count++;
      stack[stack_size++] = bot_coords;
    }

    // Visit the right neighbour, if it's connected AND we haven't queued it yet AND there is no cat.
    int left_coords = to_coordinate(x - 1, y);
    if (gr[loc][3] == 1 && pred[left_coords] == -2 && has_cat(left_coords, cat_loc, cat_count) == 0)
    {
      pred[left_coords] = loc;
      visit_order[x - 1][y] = visit_count++;
      stack[stack_size++] = left_coords;
    }
  }
  if (dest == -1)
  { // No cheese was found.
    printf("No cheese found.\n");
    path[0][1] = mouse_loc[0];
    path[0][2] = mouse_loc[1];
  }
  else
  { // A cheese was found at the destination.
    printf("Cheese found at %d -> (%d, %d)\n", dest, dest % 32, dest / 32);
    int curr_loc = dest;
    int curr_index = 0;

    while (curr_loc > -1)
    { // Iterates through all the predecessors.
      int curr_x = curr_loc % size_X;
      int curr_y = curr_loc / size_Y;

      path[curr_index][0] = curr_x;
      path[curr_index][1] = curr_y;

      curr_loc = pred[curr_loc];
      curr_index++;
    }

    // We have a path from Cheese -> Mouse. However, we want a path from Mouse -> Cheese.
    int head = 0;
    int tail = curr_index - 1;

    while (tail > head)
    {
      int tail_x = path[tail][0];
      int tail_y = path[tail][1];

      path[tail][0] = path[head][0];
      path[tail][1] = path[head][1];

      path[head][0] = tail_x;
      path[head][1] = tail_y;

      head++;
      tail--;
    }

    int max_index = visit_count - 1;
    for (int i = curr_index; i < max_index; i++)
    {
      path[i][0] = dest % 32;
      path[i][1] = dest / 32;
      // printf("Iteration %3d: (%d, %d)\n", i, path[i][0], path[i][1]);
    }
  }

  free(pred);
  free(stack);
}

/**
 *
 */
void Astar(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int queue[graph_size], int queue_length, int visit_his[graph_size], int predesessors[graph_size], int dist_from_mouse[graph_size], int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]), int visit_count)
{
  // fprintf(stdout, "----------Astar  %d--------\n", visit_count);
  // queue empty, no path to cheese
  if (queue_length <= 0)
  {
    path[0][0] = mouse_loc[0][0];
    path[0][1] = mouse_loc[0][1];
    path[1][0] = mouse_loc[0][0];
    path[1][1] = mouse_loc[0][1];
    fprintf(stdout, "Can't find path, staying\n");
    return;
  }
  else
  {
    // extract next node to expand
    int node_index = queue[0];
    int node_x = node_index % size_X;
    int node_y = node_index / size_Y;
    int cost = dist_from_mouse[node_index] + heuristic(node_x, node_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
    int qidx = 0;
    // find the next node to expand depand on the heuristic value (smallest one)
    for (int i = 0; i < queue_length; i++)
    {
      int next_node_index = queue[i];
      int next_node_x = next_node_index % size_X;
      int next_node_y = next_node_index / size_Y;
      int h_next = heuristic(next_node_x, next_node_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
      if (dist_from_mouse[next_node_index] + h_next < cost)
      {
        node_index = next_node_index;
        node_x = next_node_x;
        node_y = next_node_y;
        cost = dist_from_mouse[next_node_index] + h_next;
        qidx = i;
      }
    }

    // fprintf(stdout, "node: %d, (%d, %d)\n", node_index, node_x, node_y);
    visit_order[node_x][node_y] = visit_count;
    visit_his[node_index] = 1;
    // update queue
    for (int l = qidx + 1; l < queue_length; l++)
    {
      queue[l - 1] = queue[l];
    }
    queue_length -= 1;
    // found cheese, return path
    if (has_cheese(node_index, cheese_loc, cheeses))
    {
      fprintf(stdout, "found cheese (%d %d) at count %d\n", node_x, node_y, visit_count);
      int path_length = 0;
      int predesessor_index = node_index;
      // find the path length
      while (predesessors[predesessor_index] >= 0)
      {
        path_length += 1;
        predesessor_index = predesessors[predesessor_index];
      }
      fprintf(stdout, "path to cheese: ");
      // store the path into path variable
      while (predesessors[node_index] >= 0)
      {
        path[path_length][0] = node_index % size_X;
        path[path_length][1] = node_index / size_Y;
        // fprintf(stdout, "(%d, %d) ->", node_index % size_X, node_index / size_Y);
        // fprintf(stdout, "%d ", path_length);
        node_index = predesessors[node_index];
        path_length -= 1;
        // fprintf(stdout, "(%d, %d) ->", path[path_length][0], path[path_length][1]);
      }
      // fprintf(stdout, "\n");

      path[0][0] = mouse_loc[0][0];
      path[0][1] = mouse_loc[0][1];
      return;
    }
    // else expand next node to the queue
    else
    {
      // top
      int top_node_index = (node_y - 1) * size_X + node_x;
      // if it's not visited and it's connected and it doesn't have a cat
      if (visit_his[top_node_index] < 0 && gr[node_index][0] == 1 && has_cat(top_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = top_node_index;
        queue_length += 1;
        predesessors[top_node_index] = node_index;
        dist_from_mouse[top_node_index] = dist_from_mouse[node_index] + 1;
      }
      // right
      int right_node_index = node_y * size_X + node_x + 1;
      if (visit_his[right_node_index] < 0 && gr[node_index][1] == 1 && has_cat(right_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = right_node_index;
        queue_length += 1;
        predesessors[right_node_index] = node_index;
        dist_from_mouse[right_node_index] = dist_from_mouse[node_index] + 1;
      }
      // bottom
      int bottom_node_index = (node_y + 1) * size_X + node_x;
      if (visit_his[bottom_node_index] < 0 && gr[node_index][2] == 1 && has_cat(bottom_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = bottom_node_index;
        queue_length += 1;
        predesessors[bottom_node_index] = node_index;
        dist_from_mouse[bottom_node_index] = dist_from_mouse[node_index] + 1;
      }
      // left
      int left_node_index = node_y * size_X + (node_x - 1);
      if (visit_his[left_node_index] < 0 && gr[node_index][3] == 1 && has_cat(left_node_index, cat_loc, cats) == 0)
      {
        queue[queue_length] = left_node_index;
        queue_length += 1;
        predesessors[left_node_index] = node_index;
        dist_from_mouse[left_node_index] = dist_from_mouse[node_index] + 1;
      }

      // recursively expand next node
      visit_count += 1;
      return Astar(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, queue, queue_length, visit_his, predesessors, dist_from_mouse, heuristic, visit_count);
    }
  }
}

/***************************** end of code ************************************/

/*
    This function is the interface between your solution for the assignment and the driver code. The driver code
    in AI_search_core_GL will call this function once per frame, and provide the following data

    Board and game layout:

  The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
  constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
  purposes, the grid of locations is represented by a graph with one node per grid location, so given
  the 32x32 cells, the graph has 1024 nodes.

  To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
  grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
  and b) there are loops.

  Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
  top, right, bottom, and left directions respectively:

      node at (i,j-1)
        ^
        |
  (node at i-1, j) <- node at (i,j) -> node at (i+1, j)
        |
        v
      node at (i,j+1)

  The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
  graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
  possible neighbours in the order towp, right, bottom, left (clockwise from top).

  Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
  1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

  graph[i][0] = 0
  graph[i][1] = 1
  graph[i][2] = 0
  graph[i][3] = 1

  then node i is connected to the right and left neighbours, but not to top or bottom.

  The index in the graph for the node corresponding to grid location (x,y) is

  index = x + (y*size_X)    or in this case   index = x + (y*32)

  Conversely, if you have the index and want to figure out the grid location,

  x = index % size_X    or in this case   x = index % 32
  y = index / size_Y    or in this case   y = index / 32

  (all of the above are *integer* operations!)

  A path is a sequence of (x,y) grid locations. We store it using an array of size
  1024 x 2 (since there are 1024 locations, this is the maximum length of any
  path that visits locations only once).

  Agent locations are coordinate pairs (x,y)

    Arguments:
    gr[graph_size][4]   - This is an adjacency list for the maze
    path[graph_size][2] - An initially empty path for your code to fill.
              In this case, empty means all entries are initially -1
    visit_order[size_X][size_Y] - An array in which your code will store the
              *order* in which grid locations were
              visited during search. For example, while
              doing BFS, the initial location is the
              start location, it's visit order is 1.
              Then the search would expand the immediate
              neighbours of the initial node in some order,
              these would get a visit order of 2, 3, 4, and
              5 respectively, and so on.

              This array will be used to display the search
              pattern generated by each search method.

    cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
           but there can be fewer). Only valid cat locations are 0 to (cats-1)
    cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
               but possibly fewer). Valid locations are 0 to (cheeses-1)
    mouse_loc[1][2] - Mouse location - there can be only one!
    mode - Search mode selection:
          mode = 0  - BFS
          mode = 1  - DFS
          mode = 2  - A*

    (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
        - This is a pointer to one of the heuristic functions you will implement, either H_cost()
          or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
          depending on what search the user wants to run.

          If the mode is 0 or 1, this pointer is NULL

          * How to call the heuristic function from within this function : *
          - Like any other function:
            h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

    Return values:
    Your search code will directly update data passed-in as arguments:

    - path[graph_size][2] : Your search code will update this array to contain the path from
            the mouse to one of the cheese chunks. The order matters, so
            path[0][:] must be the mouse's current location, path[1][:]
            is the next move for the mouse. Each successive row will contain
            the next move toward the cheese, and the path ends at a location
            whose coordinates correspond to one of the cheese chunks.
            Any entries beyond that must remain set to -1
    - visit_order[size_X][size_Y]   :  Your search code will update this array to contain the
               order in which each location in the grid was expanded
               during search. This means, when that particular location
               was checked for being a goal, and if not a goal, had its
               neighbours added as candidates for future expansion (in
               whatever order is dictated by the search mode).

               Note that since there are 1024 locations, the values in
               this array must always be in [0, 1023]. The driver code
               will then display search order as a yellow-colored
               brightness map where nodes expanded earlier will look
               brighter.

    * Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
          or the location of the mouse - if you try, the driver code will know it *

    That's that, now, implement your solution!
  */

/********************************************************************************************************
  *
  * TO DO:  Implement code to carry out the different types of search depending on the
  *   mode.
  *
  *   You can do this by writing code within this single function (and being clever about it!)
  *   Or, you can use this function as a wrapper that calls a different search function
  *    (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
  *         to inform your A* function somehow of what heuristic it's supposed to use.
  *
  *   Visiting Order: When adding the neighbours of a node to your list of candidates for
  *       expansion, do so in the order top, right, bottom, left.
  *
  *   NOTE: Your search functions should be smart enough to not choose a path that goes
  *         through a cat! this is easily done without any heuristics.
  *
  *   How you design your solution is up to you. But:
  *
  *   - Document your implementation by adding concise and clear comments in this file
  *   - Document your design (how you implemented the solution, and why) in the report
  *
  ********************************************************************************************************/