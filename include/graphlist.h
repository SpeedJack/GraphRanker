#ifndef GRAPHRANKER_GRAPHLIST_H
#define GRAPHRANKER_GRAPHLIST_H

#include "graph.h"

struct list_entry {
	struct graph *graph;
	struct list_entry *next;
};

typedef struct {
	struct list_entry *head, *current;
} graph_list;

extern void graphlist_add(graph_list *list, struct graph *new);
extern struct graph *graphlist_next(graph_list *list);
extern void graphlist_reset_current(graph_list *list);
extern void graphlist_destroy(graph_list list);

#endif /* GRAPHRANKER_GRAPHLIST_H */
