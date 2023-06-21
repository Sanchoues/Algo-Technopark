#pragma once
#include <set>
#include "igraph.hpp"

struct SetGraph: public IGraph {
public:
    SetGraph(int vertices_count);
    SetGraph(const IGraph& igraph);

    virtual ~SetGraph();
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>*> graph;
    int vertices_count;
};