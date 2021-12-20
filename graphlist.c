#include "graphlist.h"
#include <assert.h>
#include <stdlib.h>

static int compare_graphs(struct graph *first, struct graph *second)
{
	assert(first && second);
	int diff = second->metric - first->metric;
	if (diff == 0)
		diff = second->index - first->index;
	return diff;
}

void graphlist_add(graph_list *list, struct graph *new)
{
	assert(list && new);
	struct list_entry *entry = malloc(sizeof(struct list_entry));
	entry->graph = new;
	entry->next = NULL;
	if (list->head == NULL) {
		list->head = entry;
	} else if (compare_graphs(list->head->graph, entry->graph) < 0) {
		entry->next = list->head;
		list->head = entry;
	} else {
		struct list_entry *prev = list->head;
		struct list_entry *cur = list->head->next;
		while (1) {
			if (cur == NULL || compare_graphs(cur->graph, entry->graph) < 0) {
				entry->next = cur;
				prev->next = entry;
				break;
			}
			prev = cur;
			cur = cur->next;
		}
	}
	graphlist_reset_current(list);
}

void graphlist_reset_current(graph_list *list)
{
	list->current = list->head;
}

struct graph *graphlist_next(graph_list *list)
{
	assert(list);
	if (list->current == NULL) {
		list->current = list->head;
		return NULL;
	}
	struct graph *current = list->current->graph;
	list->current = list->current->next;
	return current;
}

void graphlist_destroy(graph_list list)
{
	struct list_entry *entry = list.head;
	while (entry != NULL) {
		graph_destroy(entry->graph);
		struct list_entry *tmp = entry;
		entry = entry->next;
		free(tmp);
	}
}
