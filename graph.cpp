// graph.cpp (Implementation of Class Graph)

#include "graph.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Graph::Graph(char const *filename) : num_nodes(0)
{
    std::ifstream file(filename);
    if (not file)
    {
        throw std::runtime_error("Cannot open file.");
    }

    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> num_nodes;
    if (not ss)
    {
        throw std::runtime_error("Invalid file format.");
    }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        NodeId head, tail;
        ss >> tail >> head;
        if (not ss)
        {
            throw std::runtime_error("Invalid file format.");
        }
        int weight = 1;
        ss >> weight;
        if (tail != head)
        {
            add_edge(Edge{.end_nodes{tail, head}, .weight=weight});
        }
        else
        {
            throw std::runtime_error("Invalid file format: loops not allowed.");
        }
    }
}

void Graph::add_edge(Edge edge) {
    for (NodeId id : edge.end_nodes)
        if (id >= num_nodes or id < 0) {
            throw std::runtime_error(
                "Edge cannot be added due to undefined endpoint.");
        }
    edges.push_back(std::move(edge));
}
