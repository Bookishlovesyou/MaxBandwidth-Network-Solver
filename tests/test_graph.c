#include <assert.h>
#include <stdio.h>

#include "../graph.h"

static void test_single_connection(void)
{
    Graph *graph = graph_create(2);
    assert(graph != NULL);
    assert(graph_add_connection(graph, 0, 1, 25));
    assert(graph_max_flow(graph, 0, 1) == 25);
    graph_destroy(graph);
}

static void test_two_paths(void)
{
    Graph *graph = graph_create(4);
    assert(graph != NULL);
    assert(graph_add_connection(graph, 0, 1, 10));
    assert(graph_add_connection(graph, 1, 3, 10));
    assert(graph_add_connection(graph, 0, 2, 5));
    assert(graph_add_connection(graph, 2, 3, 5));
    assert(graph_max_flow(graph, 0, 3) == 15);
    graph_destroy(graph);
}

static void test_bottleneck(void)
{
    Graph *graph = graph_create(4);
    assert(graph != NULL);
    assert(graph_add_connection(graph, 0, 1, 10));
    assert(graph_add_connection(graph, 1, 3, 4));
    assert(graph_add_connection(graph, 0, 2, 10));
    assert(graph_add_connection(graph, 2, 3, 6));
    assert(graph_add_connection(graph, 1, 2, 10));
    assert(graph_max_flow(graph, 0, 3) == 10);
    graph_destroy(graph);
}

static void test_parallel_connections(void)
{
    Graph *graph = graph_create(2);
    assert(graph != NULL);
    assert(graph_add_connection(graph, 0, 1, 7));
    assert(graph_add_connection(graph, 0, 1, 8));
    assert(graph_max_flow(graph, 0, 1) == 15);
    graph_destroy(graph);
}

int main(void)
{
    test_single_connection();
    test_two_paths();
    test_bottleneck();
    test_parallel_connections();
    puts("All graph tests passed.");
    return 0;
}
