#ifndef GRAPHRANKER_GRAPH_H
#define GRAPHRANKER_GRAPH_H

#include <stdint.h>

struct graph {
	unsigned int index;
	uint32_t *adjacency_matrix;
	uint32_t size;
	unsigned long long metric;
};

extern struct graph *graph_create(uint32_t *matrix, uint32_t size);
extern void graph_destroy(struct graph *graph);

#endif /* GRAPHRANKER_GRAPH_H */
