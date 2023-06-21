#include "listgraph.hpp"

ListGraph::ListGraph(int vertices_count): vertices_count(vertices_count) {
    graph.resize(vertices_count);
    reverse_graph.resize(vertices_count);
}


ListGraph::ListGraph(const IGraph& igraph) {
    vertices_count = igraph.VerticesCount();
    graph.resize(vertices_count);
    reverse_graph.resize(vertices_count);

    for (int i = 0; i < vertices_count; i++) {
        graph[i] = igraph.GetNextVertices(i);
        reverse_graph[i] = igraph.GetPrevVertices(i);
    }
} 


ListGraph::~ListGraph() {}


void ListGraph::AddEdge(int from, int to) {
    graph[from].push_back(to);
    reverse_graph[to].push_back(from);
}


int ListGraph::VerticesCount() const {
    return vertices_count;
}


std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    result.reserve(graph[vertex].size());

    for (int i = 0; i < graph[vertex].size(); i++) {
        result.push_back(graph[vertex][i]);
    }
    return result;
}


std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    result.reserve(reverse_graph[vertex].size());

    for (int i = 0; i < reverse_graph[vertex].size(); i++) {
        result.push_back(reverse_graph[vertex][i]);
    }
    return result;
}

