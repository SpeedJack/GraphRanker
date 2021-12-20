#include "graphlist.h"
#include "io.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static graph_list graphs = { NULL, NULL };

static void string_trim(char **pstr)
{
	assert(pstr && *pstr);
	char *str = *pstr;
	while (isspace((int)*str)) str++;
	*pstr = str;

	if (*str == '\0')
		return;

	char *end;
	for (end = str + strlen(str) - 1; end >= str && isspace((int)*end); end--)
		;
	*(++end) = '\0';
}

static char *string_to_lower(char *str)
{
	assert(str);
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}

static int add_graph(uint32_t count)
{
	uint32_t *matrix = malloc(sizeof(uint32_t) * count * count);
	if (io_read_matrix(matrix, count) != 0)
		return -1;

	struct graph *graph = graph_create(matrix, count);
	graphlist_add(&graphs, graph);

	return 0;
}

static void print_top_k(uint32_t count)
{
	for (uint32_t i = 0; i < count; i++) {
		struct graph *graph = graphlist_next(&graphs);
		if (graph == NULL)
			break;
		printf((i == 0) ? "%u" : " %u", graph->index);
	}
	printf("\n");
	graphlist_reset_current(&graphs);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
	uint32_t d, k;
	char cmd[32];

	if (io_read_uint(&d) < 0)
		return -1;
	if (d == 0) {
		fputs("d must be at least 1.\n", stderr);
		return -1;
	}
	if (io_read_char(',') != 0) {
		fputs("Input error: expected ','.\n", stderr);
		return -1;
	}
	if (io_read_uint(&k) < 0)
		return -1;

	while (1) {
		if (io_read_line(cmd, 32) == 0)
			break; /* EOF */
		char *cmdstr = cmd;
		string_trim(&cmdstr);
		cmdstr = string_to_lower(cmdstr);
		if (strcmp(cmdstr, "aggiungigrafo") == 0) {
			if (add_graph(d) != 0)
				goto clean_exit_error;
		} else if (strcmp(cmdstr, "topk") == 0) {
			print_top_k(k);
		} else {
			fputs("Invalid command.\n", stderr);
			goto clean_exit_error;
		}
	}

	graphlist_destroy(graphs);
	return 0;

clean_exit_error:
	graphlist_destroy(graphs);
	return -1;
}
