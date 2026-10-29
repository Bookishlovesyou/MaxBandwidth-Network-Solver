#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

Graph *read_graph(int *node_count)
{
    int count;
    Graph *graph;

    if (scanf("%d", &count) != 1 || count < 2 || count > MAX_NODES) {
        fprintf(stderr, "Invalid node count. Enter a value from 2 to 100.\n");
        return NULL;
    }

    graph = graph_create(count);
    if (graph == NULL) {
        fprintf(stderr, "Failed to create the graph.\n");
        return NULL;
    }

    *node_count = count;
    return graph;
}

int read_request(int *source, int *destination, int *connection_count)
{
    return scanf("%d %d %d", source, destination, connection_count) == 3;
}

int read_connections(Graph *graph, int connection_count)
{
    int i;

    for (i = 0; i < connection_count; ++i) {
        int start;
        int end;
        int bandwidth;

        if (scanf("%d %d %d", &start, &end, &bandwidth) != 3) {
            fprintf(stderr, "Invalid connection format.\n");
            return 0;
        }
        if (start == 0 || end == 0 || bandwidth == 0) {
            break;
        }
        if (start < 1 || end < 1 || start > MAX_NODES ||
            end > MAX_NODES || bandwidth < 0 ||
            !graph_add_connection(graph, start - 1, end - 1, bandwidth)) {
            fprintf(stderr, "Invalid connection. Check node numbers and bandwidth.\n");
            return 0;
        }
    }

    return 1;
}
