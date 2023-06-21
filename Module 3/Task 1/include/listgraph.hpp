#include "igraph.hpp"

struct ListGraph: public IGraph {
public:
    ListGraph(int vertices_count);
    ListGraph(const IGraph& igraph);

    virtual ~ListGraph();
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> reverse_graph;

    int vertices_count;
};