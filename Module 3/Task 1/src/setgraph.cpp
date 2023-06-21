#include "setgraph.hpp"

SetGraph::SetGraph(int vertices_count): vertices_count(vertices_count) {
    graph.resize(vertices_count);
    for (int i = 0; i < vertices_count; i++) {
        graph[i] = new std::set<int>();
    }
}

SetGraph::SetGraph(const IGraph& igraph) {
    vertices_count = igraph.VerticesCount();
    std::vector<int> buff;
    graph.resize(vertices_count);
    for (int i = 0; i < vertices_count; i++) {
        buff = igraph.GetNextVertices(i);
        graph[i] = new std::set<int>();
        for (int k = 0; k < buff.size(); k++) {
            graph[i]->insert(buff[k]);
        }
    }
} 

SetGraph::~SetGraph() {
    for (int i = 0; i < vertices_count; i++) {
        delete graph[i];
    }
}

void SetGraph::AddEdge(int from, int to) {
    graph[from]->insert(to);
}

int SetGraph::VerticesCount() const {
    return vertices_count;
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (auto iter: *graph[vertex]) {
        result.push_back(iter);
    }

    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < vertices_count; i++) {
        if (graph[i]->find(vertex) != graph[i]->end()) {
            result.push_back(i);
        }
    }

    return result;
}