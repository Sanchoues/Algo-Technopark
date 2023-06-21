#include "arcgraph.hpp"

ArcGraph::ArcGraph(int vertices_count): vertices_count(vertices_count) {
    graph.resize(vertices_count);
}

ArcGraph::ArcGraph(const IGraph& igraph) {
    vertices_count = igraph.VerticesCount();
    std::vector<int> buff;
    for (int i = 0; i < vertices_count; i++) {
        buff = igraph.GetNextVertices(i);
        for (int k = 0; k < buff.size(); k++) {
            Pair pair(i, buff[k]);
            graph.push_back(pair);
        }
    }
} 

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to) {
    Pair pair(from, to);
    graph.push_back(pair);
}

int ArcGraph::VerticesCount() const {
    return vertices_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < graph.size(); i++) {
        if (graph[i].source == vertex) {
            result.push_back(graph[i].destination);
        }
    }
    
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < graph.size(); i++) {
        if (graph[i].destination == vertex) {
            result.push_back(graph[i].source);
        }
    }
    
    return result;
}