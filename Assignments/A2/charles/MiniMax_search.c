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

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
	// No alpha-beta pruning
	if (mode == 0)
	{
		int entity_count = cats + 1;
		int next_entity = (agentId + 1) % entity_count;

		// We've reached max depth.
		if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses))
		{
			double result = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
			return result;
		}

		// This is the mouse. Maximize.
		else if (agentId == 0)
		{
			int mouse_index = translateToIndex(mouse_loc[0][0], mouse_loc[0][1]);

			int loc_up[1][2] = {{mouse_loc[0][0], mouse_loc[0][1] - 1}};
			double val_up;
			if (validArr(loc_up) && gr[mouse_index][0] == 1)
				val_up = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_up, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			else
				val_up = INT_MIN;

			int loc_right[1][2] = {{mouse_loc[0][0] + 1, mouse_loc[0][1]}};
			double val_right;
			if (validArr(loc_right) && gr[mouse_index][1] == 1)
				val_right = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_right, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			else
				val_right = INT_MIN;

			int loc_down[1][2] = {{mouse_loc[0][0], mouse_loc[0][1] + 1}};
			double val_down;
			if (validArr(loc_down) && gr[mouse_index][2] == 1)
				val_down = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_down, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			else
				val_down = INT_MIN;

			int loc_left[1][2] = {{mouse_loc[0][0] - 1, mouse_loc[0][1]}};
			double val_left;
			if (validArr(loc_left) && gr[mouse_index][3] == 1)
				val_left = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_left, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			else
				val_left = INT_MIN;

			if (depth == 0)
			{
				printf("%.2f %.2f %.2f %.2f\n", val_up, val_right, val_down, val_left);
			}

			// No possible moves.
			if (val_up == INT_MIN && val_up == val_right && val_up == val_down && val_up == val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = INT_MIN;
				return INT_MIN;
			}

			// Upwards direction is the best.
			else if (val_up >= val_right && val_up >= val_down && val_up >= val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1] - 1;
				minmax_cost[path[0][0]][path[0][1]] = val_up;
				return val_up;
			}

			// Rightwards direction is the best.
			else if (val_right >= val_down && val_right >= val_left)
			{
				path[0][0] = mouse_loc[0][0] + 1;
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = val_right;
				return val_right;
			}

			// Downwards direction is the best.
			else if (val_down >= val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1] + 1;
				minmax_cost[path[0][0]][path[0][1]] = val_down;
				return val_down;
			}

			// Leftwards direction is the best.
			else
			{
				path[0][0] = mouse_loc[0][0] - 1;
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = val_left;
				return val_left;
			}
		}

		// This is the cat. Minimize.
		else
		{
			int loc_up[10][2];
			int loc_right[10][2];
			int loc_down[10][2];
			int loc_left[10][2];

			for (int i = 0; i < cats; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					loc_up[i][j] = cat_loc[i][j];
					loc_right[i][j] = cat_loc[i][j];
					loc_down[i][j] = cat_loc[i][j];
					loc_left[i][j] = cat_loc[i][j];
				}
			}

			int cat_id = agentId - 1;

			int cat_x = cat_loc[cat_id][0];
			int cat_y = cat_loc[cat_id][1];
			int cat_index = translateToIndex(cat_x, cat_y);

			double val_up;
			if (validIndex(cat_x, cat_y - 1) && gr[cat_index][0] == 1)
			{
				loc_up[cat_id][0] = cat_x;
				loc_up[cat_id][1] = cat_y - 1;
				val_up = MiniMax(gr, path, minmax_cost, loc_up, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			}
			else
				val_up = INT_MAX;

			double val_right;
			if (validIndex(cat_x + 1, cat_y) && gr[cat_index][1] == 1)
			{
				loc_right[cat_id][0] = cat_x + 1;
				loc_right[cat_id][1] = cat_y;
				val_right = MiniMax(gr, path, minmax_cost, loc_right, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			}
			else
				val_right = INT_MAX;

			double val_down;
			if (validIndex(cat_x, cat_y + 1) && gr[cat_index][2] == 1)
			{
				loc_down[cat_id][0] = cat_x;
				loc_down[cat_id][1] = cat_y + 1;
				val_down = MiniMax(gr, path, minmax_cost, loc_down, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			}
			else
				val_down = INT_MAX;

			double val_left;
			if (validIndex(cat_x - 1, cat_y) && gr[cat_index][3] == 1)
			{
				loc_left[cat_id][0] = cat_x - 1;
				loc_left[cat_id][1] = cat_y;
				val_left = MiniMax(gr, path, minmax_cost, loc_left, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, alpha, beta);
			}
			else
				val_left = INT_MAX;

			// Upwards direction is the best.
			if (val_up <= val_right && val_up <= val_down && val_up <= val_left)
			{
				return val_up;
			}

			// Rightwards direction is the best.
			else if (val_right <= val_down && val_right <= val_left)
			{
				return val_right;
			}

			// Downwards direction is the best.
			else if (val_down <= val_left)
			{
				return val_down;
			}

			// Leftwards direction is the best.
			else
			{
				return val_left;
			}
		}
	}

	// Alpha-beta pruning
	else if (mode == 1)
	{
		int entity_count = cats + 1;
		int next_entity = (agentId + 1) % entity_count;

		// We've reached max depth.
		if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses))
		{
			return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		}

		// This is the mouse. Maximize.
		else if (agentId == 0)
		{
			double current_alpha = alpha;
			int mouse_index = translateToIndex(mouse_loc[0][0], mouse_loc[0][1]);

			int loc_up[1][2] = {{mouse_loc[0][0], mouse_loc[0][1] - 1}};
			double val_up;
			if (validArr(loc_up) && gr[mouse_index][0] == 1)
				val_up = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_up, mode, utility, next_entity, depth + 1, maxDepth, current_alpha, INT_MAX);
			else
				val_up = INT_MIN;

			if (val_up > current_alpha)
			{
				current_alpha = val_up;
				if (current_alpha >= beta)
				{
					path[0][0] = mouse_loc[0][0];
					path[0][1] = mouse_loc[0][1] - 1;
					minmax_cost[path[0][0]][path[0][1]] = current_alpha;
					return current_alpha;
				}
			}

			int loc_right[1][2] = {{mouse_loc[0][0] + 1, mouse_loc[0][1]}};
			double val_right;
			if (validArr(loc_right) && gr[mouse_index][1] == 1)
				val_right = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_right, mode, utility, next_entity, depth + 1, maxDepth, current_alpha, INT_MAX);
			else
				val_right = INT_MIN;

			if (val_right > current_alpha)
			{
				current_alpha = val_right;
				if (current_alpha >= beta)
				{
					path[0][0] = mouse_loc[0][0] + 1;
					path[0][1] = mouse_loc[0][1];
					minmax_cost[path[0][0]][path[0][1]] = current_alpha;
					return current_alpha;
				}
			}

			int loc_down[1][2] = {{mouse_loc[0][0], mouse_loc[0][1] + 1}};
			double val_down;
			if (validArr(loc_down) && gr[mouse_index][2] == 1)
				val_down = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_down, mode, utility, next_entity, depth + 1, maxDepth, current_alpha, INT_MAX);
			else
				val_down = INT_MIN;

			if (val_down > current_alpha)
			{
				current_alpha = val_down;
				if (current_alpha >= beta)
				{
					path[0][0] = mouse_loc[0][0];
					path[0][1] = mouse_loc[0][1] + 1;
					minmax_cost[path[0][0]][path[0][1]] = current_alpha;
					return current_alpha;
				}
			}

			int loc_left[1][2] = {{mouse_loc[0][0] - 1, mouse_loc[0][1]}};
			double val_left;
			if (validArr(loc_left) && gr[mouse_index][3] == 1)
				val_left = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, loc_left, mode, utility, next_entity, depth + 1, maxDepth, current_alpha, INT_MAX);
			else
				val_left = INT_MIN;

			// No possible moves.
			if (val_up == INT_MIN && val_up == val_right && val_up == val_down && val_up == val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = INT_MIN;
				return INT_MIN;
			}

			// Upwards direction is the best.
			else if (val_up >= val_right && val_up >= val_down && val_up >= val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1] - 1;
				minmax_cost[path[0][0]][path[0][1]] = val_up;
				return val_up;
			}

			// Rightwards direction is the best.
			else if (val_right >= val_down && val_right >= val_left)
			{
				path[0][0] = mouse_loc[0][0] + 1;
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = val_right;
				return val_right;
			}

			// Downwards direction is the best.
			else if (val_down >= val_left)
			{
				path[0][0] = mouse_loc[0][0];
				path[0][1] = mouse_loc[0][1] + 1;
				minmax_cost[path[0][0]][path[0][1]] = val_down;
				return val_down;
			}

			// Leftwards direction is the best.
			else
			{
				path[0][0] = mouse_loc[0][0] - 1;
				path[0][1] = mouse_loc[0][1];
				minmax_cost[path[0][0]][path[0][1]] = val_left;
				return val_left;
			}
		}

		// This is the cat. Minimize.
		else
		{
			int loc_up[10][2];
			int loc_right[10][2];
			int loc_down[10][2];
			int loc_left[10][2];

			for (int i = 0; i < cats; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					loc_up[i][j] = cat_loc[i][j];
					loc_right[i][j] = cat_loc[i][j];
					loc_down[i][j] = cat_loc[i][j];
					loc_left[i][j] = cat_loc[i][j];
				}
			}

			double current_beta = beta;
			int cat_id = agentId - 1;

			int cat_x = cat_loc[cat_id][0];
			int cat_y = cat_loc[cat_id][1];
			int cat_index = translateToIndex(cat_loc[cat_id][0], cat_loc[cat_id][1]);

			double val_up;
			if (validIndex(cat_x, cat_y - 1) && gr[cat_index][0] == 1)
			{
				loc_up[cat_id][0] = cat_x;
				loc_up[cat_id][1] = cat_y - 1;
				val_up = MiniMax(gr, path, minmax_cost, loc_up, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, INT_MIN, current_beta);
			}
			else
				val_up = INT_MAX;

			if (val_up < current_beta)
			{
				current_beta = val_up;
				if (current_beta <= alpha)
				{
					return current_beta;
				}
			}

			double val_right;
			if (validIndex(cat_x + 1, cat_y) && gr[cat_index][1] == 1)
			{
				loc_right[cat_id][0] = cat_x + 1;
				loc_right[cat_id][1] = cat_y;
				val_right = MiniMax(gr, path, minmax_cost, loc_right, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, INT_MIN, current_beta);
			}
			else
				val_right = INT_MAX;

			if (val_right < current_beta)
			{
				current_beta = val_right;
				if (current_beta <= alpha)
				{
					return current_beta;
				}
			}

			double val_down;
			if (validIndex(cat_x, cat_y + 1) && gr[cat_index][2] == 1)
			{
				loc_down[cat_id][0] = cat_x;
				loc_down[cat_id][1] = cat_y + 1;
				val_down = MiniMax(gr, path, minmax_cost, loc_down, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, INT_MIN, current_beta);
			}
			else
				val_down = INT_MAX;

			if (val_down < current_beta)
			{
				current_beta = val_down;
				if (current_beta <= alpha)
				{
					return current_beta;
				}
			}

			double val_left;
			if (validIndex(cat_x - 1, cat_y) && gr[cat_index][3] == 1)
			{
				loc_left[cat_id][0] = cat_x - 1;
				loc_left[cat_id][1] = cat_y;
				val_left = MiniMax(gr, path, minmax_cost, loc_left, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_entity, depth + 1, maxDepth, INT_MIN, current_beta);
			}
			else
				val_left = INT_MAX;

			// Cat has no valid movement.
			if (val_up == INT_MAX && val_up == val_right && val_up == val_down && val_up == val_left)
			{
				return 0;
			}

			// Upwards direction is the best.
			else if (val_up <= val_right && val_up <= val_down && val_up <= val_left)
			{
				return val_up;
			}

			// Rightwards direction is the best.
			else if (val_right <= val_down && val_right <= val_left)
			{
				return val_right;
			}

			// Downwards direction is the best.
			else if (val_down <= val_left)
			{
				return val_down;
			}

			// Leftwards direction is the best.
			else
			{
				return val_left;
			}
		}
	}

	// Mouse stands still.
	else
	{
		path[0][0] = mouse_loc[0][0];
		path[0][1] = mouse_loc[0][1];

		return (0.0);
	}
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
	// Check for cats having lunch
	for (int i = 0; i < cats; i++)
		if (mouse_loc[0][0] == cat_loc[i][0] && mouse_loc[0][1] == cat_loc[i][1])
		{
			return INT_MIN;
		}

	// Check for mouses having lunch
	for (int i = 0; i < cheeses; i++)
		if (mouse_loc[0][0] == cheese_loc[i][0] && mouse_loc[0][1] == cheese_loc[i][1])
			return -depth;

	if (!validIndex(mouse_loc[0][0], mouse_loc[0][1]))
	{
		return 0;
	}

	int *predecessors = (int *)calloc(graph_size, sizeof(int));

	linked_list queue;
	init_linked_list(&queue);

	int start_index = translateToIndex(mouse_loc[0][0], mouse_loc[0][1]);

	queue_item(&queue, start_index);
	predecessors[start_index] = 1;

	double min_cheese_distance = graph_size;

	while (queue.size > 0)
	{
		int current_node = pop_item(&queue);
		int current_x, current_y;
		translateFromIndex(current_node, &current_x, &current_y);

		int current_weight = predecessors[current_node] - 1;

		for (int i = 0; i < cats; i++)
		{
			if (cat_loc[i][0] == current_x && cat_loc[i][1] == current_y)
			{
				continue;
			}
		}

		int found_cheese = 0;

		for (int i = 0; i < cheeses; i++)
		{
			if (cheese_loc[i][0] == current_x && cheese_loc[i][1] == current_y)
			{
				if (current_weight < min_cheese_distance)
				{
					min_cheese_distance = current_weight;
					found_cheese = 1;
				}
			}
		}

		if (found_cheese)
		{
			break;
		}

		if (gr[current_node][0] == 1)
		{
			int next_x = current_x;
			int next_y = current_y - 1;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0 && validIndex(next_x, next_y))
			{
				predecessors[next_node] = current_weight + 2;
				queue_item(&queue, next_node);
			}
		}

		if (gr[current_node][1] == 1)
		{
			int next_x = current_x + 1;
			int next_y = current_y;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0 && validIndex(next_x, next_y))
			{
				predecessors[next_node] = current_weight + 2;
				queue_item(&queue, next_node);
			}
		}

		if (gr[current_node][2] == 1)
		{
			int next_x = current_x;
			int next_y = current_y + 1;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0 && validIndex(next_x, next_y))
			{
				predecessors[next_node] = current_weight + 2;
				queue_item(&queue, next_node);
			}
		}

		if (gr[current_node][3] == 1)
		{
			int next_x = current_x - 1;
			int next_y = current_y;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0 && validIndex(next_x, next_y))
			{
				predecessors[next_node] = current_weight + 2;
				queue_item(&queue, next_node);
			}
		}
	}

	del_linked_list(&queue);
	free(predecessors);

	if (min_cheese_distance == graph_size)
	{
		return INT_MIN;
	}
	else
	{
		return 0 - min_cheese_distance - depth;
	}

	// printf("Closest Distance: %.2f tiles\n", min_cheese_distance);
	// return total_cat_distance - min_cheese_distance;
}

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2], int cheese_loc[10][2], int cats, int cheeses)
{
	// Check for cats having lunch
	for (int i = 0; i < cats; i++)
		if (mouse_loc[0][0] == cat_loc[i][0] && mouse_loc[0][1] == cat_loc[i][1])
			return (1);

	// Check for mouse having lunch
	for (int i = 0; i < cheeses; i++)
		if (mouse_loc[0][0] == cheese_loc[i][0] && mouse_loc[0][1] == cheese_loc[i][1])
			return (1);

	return (0);
}

int translateToIndex(int x, int y)
{
	return x + (y * size_X);
}

void translateFromIndex(int index, int *x, int *y)
{
	(*x) = index % size_X;
	(*y) = index / size_X;
}

int validIndex(int x, int y)
{
	return x >= 0 && y >= 0 && x < size_X && y < size_Y;
}

int validArr(int loc[1][2])
{
	return validIndex(loc[0][0], loc[0][1]);
}

void init_linked_list(linked_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void queue_item(linked_list *list, int val)
{
	linked_node *node = (linked_node *)malloc(sizeof(linked_node));
	node->val = val;
	node->next = NULL;

	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
		list->size = 1;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
		list->size++;
	}
}

int pop_item(linked_list *list)
{
	int size = list->size;
	if (size <= 0)
	{
		return -1;
	}

	int val = list->head->val;

	if (size == 1)
	{
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	else
	{
		linked_node *old_node = list->head;
		list->head = list->head->next;

		free(old_node);
		list->size = size - 1;
	}

	return val;
}

void del_linked_list(linked_list *list)
{
	linked_node *current = list->head;
	linked_node *next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

int is_empty(linked_list *list)
{
	return list->size == 0;
}

/*
	-1			invalid
	0	up		(x, y - 1)
	1	right	(x + 1, y)
	2	down	(x, y + 1)
	3	left	(x - 1, y)
*/
void findMovement(double gr[graph_size][4], int startX, int startY, int endX, int endY, int *return_direction, int *return_distance)
{
	if ((!validIndex(startX, startY)) || (!validIndex(endX, endY)))
	{
		(*return_direction) = -1;
		(*return_distance) = -1;
		return;
	}
	else if (startX == endX && startY == endY)
	{
		(*return_direction) = 0;
		(*return_distance) = 0;
		return;
	}

	int *predecessors = (int *)calloc(graph_size, sizeof(int));

	linked_list queue;
	init_linked_list(&queue);

	int start_index = translateToIndex(startX, startY);

	queue_item(&queue, start_index);
	predecessors[start_index] = -1;

	while (queue.size > 0)
	{
		int current_node = pop_item(&queue);
		int current_x, current_y;
		translateFromIndex(current_node, &current_x, &current_y);

		if (gr[current_node][0] == 1)
		{
			int next_x = current_x;
			int next_y = current_y - 1;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0)
			{
				predecessors[next_node] = current_node + 1;
				if (next_x == endX && next_y == endY)
				{
					break;
				}
				else
				{
					queue_item(&queue, next_node);
				}
			}
		}

		if (gr[current_node][1] == 1)
		{
			int next_x = current_x + 1;
			int next_y = current_y;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0)
			{
				predecessors[next_node] = current_node + 1;
				if (next_x == endX && next_y == endY)
				{
					break;
				}
				else
				{
					queue_item(&queue, next_node);
				}
			}
		}

		if (gr[current_node][2] == 1)
		{
			int next_x = current_x;
			int next_y = current_y + 1;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0)
			{
				predecessors[next_node] = current_node + 1;
				if (next_x == endX && next_y == endY)
				{
					break;
				}
				else
				{
					queue_item(&queue, next_node);
				}
			}
		}

		if (gr[current_node][3] == 1)
		{
			int next_x = current_x - 1;
			int next_y = current_y;
			int next_node = translateToIndex(next_x, next_y);

			if (predecessors[next_node] == 0)
			{
				predecessors[next_node] = current_node + 1;
				if (next_x == endX && next_y == endY)
				{
					break;
				}
				else
				{
					queue_item(&queue, next_node);
				}
			}
		}
	}

	int end_index = translateToIndex(endX, endY);
	int result = -1;
	int distance = -1;

	if (predecessors[end_index] != 0)
	{
		int current_index = end_index;
		int parent;

		distance = 0;

		while (1)
		{
			distance++;
			parent = predecessors[current_index] - 1;
			if (predecessors[parent] <= 0)
			{
				int current_x, current_y;
				translateFromIndex(current_index, &current_x, &current_x);

				if (current_x > startX)
				{
					result = 1;
				}
				else if (current_x < startX)
				{
					result = 3;
				}
				else if (current_y > startY)
				{
					result = 2;
				}
				else if (current_y < startY)
				{
					result = 0;
				}

				break;
			}
			else
			{
				current_index = parent;
			}
		}
	}

	del_linked_list(&queue);
	free(predecessors);

	(*return_direction) = result;
	(*return_distance) = distance;
}
