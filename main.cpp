#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <unordered_map>
#include <vector>

#include "graph.h"

using namespace std;

vector<Edge> boruvka(Graph &&graph) {  // Boruvka's algorithm
    using CompId = int;
    using Component = struct {
        vector<NodeId> nodes;
        optional<Edge> min_edge;
    };

    vector component_of = vector<CompId>(graph.num_nodes);  // List to save component number of each node
    iota(component_of.begin(), component_of.end(), 0);

    unordered_map components = unordered_map<CompId, Component>(graph.num_nodes);  // List to keep track of remaining components
    for (NodeId node_id = 0; node_id < graph.num_nodes; ++node_id)
        components.emplace(node_id, Component{.nodes = vector{node_id}, .min_edge = optional<Edge>()});  // Create singleton components for each node

    vector mst_edges = std::vector<Edge>();  // Edges of the minimal spanning tree returned at the end

    while (components.size() > 1) {
        erase_if(graph.edges, [&](const Edge &edge) { return component_of[edge.end_nodes[0]] == component_of[edge.end_nodes[1]]; });  // Remove edges with nodes in same component for faster iteration
#ifdef DO_NOT_TRUST_ANYONE
        if (graph.edges.empty())
            throw runtime_error("Boruvka cannot handle disconnected graphs");
#endif

        for (const Edge &edge : graph.edges) {       // Find minimal outgoing edge for each component
            for (NodeId node_id : edge.end_nodes) {  // by iterating over each edge and checking component of both end nodes
                optional<Edge> &min_edge = components[component_of[node_id]].min_edge;
                if (!min_edge.has_value() || min_edge->weight > edge.weight)
                    min_edge = edge;
            }
        }

        for (auto component = components.begin(); component != components.end();) {  // Merge components by their minimal edges
            if (!component->second.min_edge.has_value()) {                           // No value means component was already merged in a previous iteration of this loop
                ++component;
                continue;
            }

            mst_edges.push_back(*component->second.min_edge);  // Add edge to the edges of MST

            auto other =  // Determine the other component, where the minimal outgoing edge leads to
                (component_of[component->second.min_edge->end_nodes[1]] == component->first) ? components.find(component_of[component->second.min_edge->end_nodes[0]])
                                                                                             : components.find(component_of[component->second.min_edge->end_nodes[1]]);
            bool less = component->second.nodes.size() < other->second.nodes.size();  // Compare sizes of component and other measured as number of nodes
            const auto &smaller = less ? component : other;
            const auto &bigger = !less ? component : other;

            for (NodeId node_id : smaller->second.nodes) {  // Relabel nodes in smaller component to now belong to the bigger one
                component_of[node_id] = bigger->first;
            }
            bigger->second.nodes.insert(bigger->second.nodes.end(), smaller->second.nodes.begin(), smaller->second.nodes.end());  // Add nodes from smaller to bigger

            if (less) {  // If component was smaller and thus absorbed, then re-validate iterator
                component = components.erase(component);
            } else {  // If other was smaller and thus absorbed, then remember minimal outgoing edge to also perform a merge on that one
                component->second.min_edge = other->second.min_edge;
                components.erase(other);
            }

            if (bigger->second.min_edge.has_value() && component_of[bigger->second.min_edge->end_nodes[0]] == component_of[bigger->second.min_edge->end_nodes[1]])  // If both components requested this merge, remove the link also from the other component as nothing is left to do
                bigger->second.min_edge.reset();
        }
    }

    return mst_edges;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        Graph g(argv[1]);  // Construct Graph from filename

        auto mst_edges = boruvka(move(g));  // Calculate MST with boruvka and its weight
        int weight = accumulate(mst_edges.cbegin(), mst_edges.cend(), 0, [](int weight, const Edge &rhs) { return weight + rhs.weight; });

        for (Edge &edge : mst_edges) {  // Conform to specified output format
            if (edge.end_nodes[0] > edge.end_nodes[1])
                swap(edge.end_nodes[0], edge.end_nodes[1]);
        }
        sort(mst_edges.begin(), mst_edges.end());

        cout << weight << endl;  // Print weight and edges of MST
        for (const Edge &edge : mst_edges) {
            cout << edge.end_nodes[0] << " " << edge.end_nodes[1] << endl;
        }
    }
}
