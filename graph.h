#ifndef GRAPH_H
#define GRAPH_H

typedef struct Graph Graph;

Graph *graph_create(int node_count);
int graph_add_connection(Graph *graph, int start, int end, int bandwidth);
int graph_max_flow(Graph *graph, int source, int destination);
void graph_destroy(Graph *graph);

#endif
