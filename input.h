#ifndef INPUT_H
#define INPUT_H

#include "graph.h"

Graph *read_graph(int *node_count);
int read_request(int *source, int *destination, int *connection_count);
int read_connections(Graph *graph, int connection_count);

#endif
