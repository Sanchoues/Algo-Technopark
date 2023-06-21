#include "matrixgraph.hpp"

MatrixGraph::MatrixGraph(int vertices_count): vertices_count(vertices_count) {
    graph.resize(vertices_count);
    for (int i = 0; i < vertices_count; i++) {
        graph[i].resize(vertices_count, 0);
    }
}


MatrixGraph::MatrixGraph(const IGraph& igraph) {
    std::vector<int> buff;
    vertices_count = igraph.VerticesCount();
    graph.resize(vertices_count);
    for (int i = 0; i < vertices_count; i++){
        buff = igraph.GetNextVertices(i);
        graph[i].resize(vertices_count, 0);
        for (int k = 0; k < buff.size(); k++) {
            graph[i][buff[k]] = 1;
        }
    }
} 

MatrixGraph::~MatrixGraph() {}


void MatrixGraph::AddEdge(int from, int to) {
    graph[from][to] = 1;
}


int MatrixGraph::VerticesCount() const {
    return vertices_count;
}


std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < vertices_count; i++) {
        if (graph[vertex][i] == 1) {
            result.push_back(i);
        }
    }

    return result;
}


std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < vertices_count; i++) {
        if (graph[i][vertex] == 1) {
            result.push_back(i);
        }
    } 

    return result;
}