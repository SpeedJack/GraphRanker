#include "graph.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

static unsigned long long compute_metric(struct graph *graph)
{
	int visited[graph->size];
	unsigned long long dist[graph->size];

	for (uint32_t vertex = 0; vertex < graph->size; vertex++) {
		visited[vertex] = 0;
		dist[vertex] = ULLONG_MAX;
	}
	dist[0] = 0;

	while (1) {
		unsigned long long mindist = ULLONG_MAX;
		uint32_t nearest = 0;
		int allvisited = 1;
		for (uint32_t vertex = 0; vertex < graph->size; vertex++) {
			if (visited[vertex] != 0)
				continue;
			allvisited = 0;
			if (dist[vertex] < mindist) {
				mindist = dist[vertex];
				nearest = vertex;
			}
		}
		if (allvisited != 0)
			break;

		visited[nearest] = 1;

		for (uint32_t neighbor = 0; neighbor < graph->size; neighbor++) {
			uint32_t distance = graph->adjacency_matrix[nearest * graph->size + neighbor];
			if (neighbor == nearest || visited[neighbor] != 0 || distance == 0)
				continue;
			unsigned long long newdist = dist[nearest] + distance;
			if (newdist < dist[neighbor])
				dist[neighbor] = newdist;
		}
	}
	unsigned long long metric = 0;
	for (uint32_t vertex = 0; vertex < graph->size; vertex++) {
		if (dist[vertex] == ULLONG_MAX) /* unreachable */
			dist[vertex] = 0;
		if (ULLONG_MAX - metric < dist[vertex]) { /* overflow */
			metric = ULLONG_MAX; /* set max metric */
			break;
		}
		metric += dist[vertex];
	}

	return metric;
}

struct graph *graph_create(uint32_t *matrix, uint32_t size)
{
	assert(matrix);
	static unsigned int idx = 0;

	struct graph *graph = malloc(sizeof(struct graph));
	graph->index = idx;
	graph->adjacency_matrix = matrix;
	graph->size = size;
	graph->metric = compute_metric(graph);

	idx++;
	return graph;
}

void graph_destroy(struct graph *graph)
{
	assert(graph && graph->adjacency_matrix);
	free(graph->adjacency_matrix);
	free(graph);
}
