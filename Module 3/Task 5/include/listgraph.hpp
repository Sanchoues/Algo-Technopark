#pragma once
#include "igraph.hpp"

struct ListGraph: public IGraph {
public:
    ListGraph(int vertices_count);
    ListGraph(const IGraph& igraph);

    virtual ~ListGraph();
    virtual void AddEdge(int from, int to, float weight) override;
    virtual int VerticesCount() const override;
    virtual std::vector<Pair> GetNextVertices(int vertex) const override;

private:
    std::vector<std::vector<Pair>> graph;
    int vertices_count;
};