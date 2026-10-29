#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int destination;
    int capacity;
    struct Edge *next;
    struct Edge *reverse;
} Edge;

struct Graph {
    int node_count;
    Edge **adjacency;
};

static Edge *create_edge(int destination, int capacity)
{
    Edge *edge = malloc(sizeof(*edge));

    if (edge == NULL) {
        return NULL;
    }

    edge->destination = destination;
    edge->capacity = capacity;
    edge->next = NULL;
    edge->reverse = NULL;
    return edge;
}

static int add_directed_edge(Graph *graph, int start, int end, int capacity)
{
    Edge *forward = create_edge(end, capacity);
    Edge *reverse = create_edge(start, 0);

    if (forward == NULL || reverse == NULL) {
        free(forward);
        free(reverse);
        return 0;
    }

    forward->reverse = reverse;
    reverse->reverse = forward;

    forward->next = graph->adjacency[start];
    graph->adjacency[start] = forward;
    reverse->next = graph->adjacency[end];
    graph->adjacency[end] = reverse;
    return 1;
}

Graph *graph_create(int node_count)
{
    Graph *graph;
    int i;

    if (node_count < 2) {
        return NULL;
    }

    graph = malloc(sizeof(*graph));
    if (graph == NULL) {
        return NULL;
    }

    graph->node_count = node_count;
    graph->adjacency = calloc((size_t)node_count, sizeof(*graph->adjacency));
    if (graph->adjacency == NULL) {
        free(graph);
        return NULL;
    }

    for (i = 0; i < node_count; ++i) {
        graph->adjacency[i] = NULL;
    }

    return graph;
}

int graph_add_connection(Graph *graph, int start, int end, int bandwidth)
{
    if (graph == NULL || start < 0 || end < 0 ||
        start >= graph->node_count || end >= graph->node_count ||
        start == end || bandwidth <= 0) {
        return 0;
    }

    /*
     * A connection can carry bandwidth in both directions. Each direction
     * gets its own residual pair so parallel connections remain independent.
     */
    if (!add_directed_edge(graph, start, end, bandwidth)) {
        return 0;
    }
    if (!add_directed_edge(graph, end, start, bandwidth)) {
        return 0;
    }

    return 1;
}

static int find_augmenting_path(const Graph *graph, int source, int destination,
                                Edge **parent)
{
    int *visited = calloc((size_t)graph->node_count, sizeof(*visited));
    int *queue = malloc((size_t)graph->node_count * sizeof(*queue));
    int front = 0;
    int rear = 0;

    if (visited == NULL || queue == NULL) {
        free(visited);
        free(queue);
        return -1;
    }

    visited[source] = 1;
    queue[rear++] = source;

    while (front < rear) {
        int current = queue[front++];
        Edge *edge = graph->adjacency[current];

        while (edge != NULL) {
            if (!visited[edge->destination] && edge->capacity > 0) {
                visited[edge->destination] = 1;
                parent[edge->destination] = edge;
                queue[rear++] = edge->destination;
                if (edge->destination == destination) {
                    free(visited);
                    free(queue);
                    return 1;
                }
            }
            edge = edge->next;
        }
    }

    free(visited);
    free(queue);
    return 0;
}

int graph_max_flow(Graph *graph, int source, int destination)
{
    Edge **parent;
    int max_flow = 0;

    if (graph == NULL || source < 0 || destination < 0 ||
        source >= graph->node_count || destination >= graph->node_count ||
        source == destination) {
        return 0;
    }

    parent = calloc((size_t)graph->node_count, sizeof(*parent));
    if (parent == NULL) {
        return 0;
    }

    for (;;) {
        int path_found = find_augmenting_path(graph, source, destination, parent);

        if (path_found < 0) {
            fprintf(stderr, "Failed to allocate memory while searching for a path.\n");
            free(parent);
            return -1;
        }
        if (!path_found) {
            break;
        }

        int path_flow = INT_MAX;
        int current = destination;

        while (current != source) {
            Edge *edge = parent[current];
            if (edge->capacity < path_flow) {
                path_flow = edge->capacity;
            }
            current = edge->reverse->destination;
        }

        current = destination;
        while (current != source) {
            Edge *edge = parent[current];
            edge->capacity -= path_flow;
            edge->reverse->capacity += path_flow;
            current = edge->reverse->destination;
        }

        max_flow += path_flow;
    }

    free(parent);
    return max_flow;
}

void graph_destroy(Graph *graph)
{
    int i;

    if (graph == NULL) {
        return;
    }

    for (i = 0; i < graph->node_count; ++i) {
        Edge *edge = graph->adjacency[i];
        while (edge != NULL) {
            Edge *next = edge->next;
            free(edge);
            edge = next;
        }
    }

    free(graph->adjacency);
    free(graph);
}
