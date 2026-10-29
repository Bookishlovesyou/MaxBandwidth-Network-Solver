#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "input.h"

int main(void)
{
    int node_count;
    int source;
    int destination;
    int connection_count;
    Graph *graph = read_graph(&node_count);

    if (graph == NULL) {
        return EXIT_FAILURE;
    }

    if (!read_request(&source, &destination, &connection_count) ||
        source < 1 || destination < 1 ||
        source > node_count || destination > node_count ||
        connection_count < 0) {
        fprintf(stderr, "Invalid source, destination, or connection count.\n");
        graph_destroy(graph);
        return EXIT_FAILURE;
    }

    if (!read_connections(graph, connection_count)) {
        graph_destroy(graph);
        return EXIT_FAILURE;
    }

    {
        int max_flow = graph_max_flow(graph, source - 1, destination - 1);
        if (max_flow < 0) {
            graph_destroy(graph);
            return EXIT_FAILURE;
        }

        printf("\n\nNetwork 1\n");
        printf("The bandwidth is %d\n", max_flow);
    }

    graph_destroy(graph);
    return EXIT_SUCCESS;
}
