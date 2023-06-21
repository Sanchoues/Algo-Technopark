#include "listgraph.hpp"

ListGraph::ListGraph(int vertices_count): vertices_count(vertices_count) {
    graph.resize(vertices_count);        
}

ListGraph::ListGraph(const IGraph& igraph) {
    vertices_count = igraph.VerticesCount();
    graph.resize(vertices_count);

    for (int i = 0; i < vertices_count; i++) {
        graph[i] = igraph.GetNextVertices(i);

    }

} 

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, float weight) {
    graph[from].push_back(Pair(to, weight));
    graph[to].push_back(Pair(from, weight));

}

int ListGraph::VerticesCount() const {
    return vertices_count;
}

std::vector<Pair> ListGraph::GetNextVertices(int vertex) const {
    std::vector<Pair> result;
    result.reserve(graph[vertex].size());

    for (int i = 0; i < graph[vertex].size(); i++) {
        result.push_back(graph[vertex][i]);
    }
    return result;
}