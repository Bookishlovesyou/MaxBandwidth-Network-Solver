# MaxBandwidth Network Solver

> This project was first made while I was in my second year. I have improved it since then by fixing errors, separating the code into files, and adding tests.

This program finds the maximum bandwidth that can travel from a source node
to a destination node in an undirected network.

It reads:

1. The number of nodes.
2. The source node, destination node, and number of connections.
3. Each connection as `start end bandwidth`.

Example:

```text
4
1 4 4
1 2 10
2 4 10
1 3 5
3 4 5
```

The result is the maximum bandwidth from node 1 to node 4.

## Example calculation

The source is node `1` and the destination is node `4`. There are two
different routes:

```text
Route 1: 1 -> 2 -> 4
Capacity: 10

Route 2: 1 -> 3 -> 4
Capacity: 5
```

Both routes can be used at the same time, so the total bandwidth is:

```text
10 + 5 = 15
```

The program therefore prints:

```text
Network 1
The bandwidth is 15
```

The answer cannot be greater than `15` because the connections leaving node
`1` have capacities `10` and `5`. Together, they provide only `15` units of
bandwidth. The program uses a maximum-flow calculation to find this result.

## Build and run

With GCC:

```text
gcc -std=c11 -Wall -Wextra -pedantic main.c graph.c input.c -o max_bandwidth
max_bandwidth < input.txt
```

To run the tests:

```text
gcc -std=c11 -Wall -Wextra -pedantic tests/test_graph.c graph.c -o graph_tests
graph_tests
```
