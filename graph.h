#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using NodeId = int;
struct Edge {
    NodeId end_nodes[2];
    int weight;
    auto operator<=>(const Edge &) const = default;
};

struct Graph {  // Actually only a list of pairs of nodes
    Graph(char const *filename);
    void add_edge(Edge edge);
    std::vector<Edge> edges;
    int num_nodes;
};

#endif  // GRAPH_H
