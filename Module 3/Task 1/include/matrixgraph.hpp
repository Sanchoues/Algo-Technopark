#include "igraph.hpp"

struct MatrixGraph: public IGraph {
public:
    MatrixGraph(int vertices_count);
    MatrixGraph(const IGraph& igraph);

    virtual ~MatrixGraph();
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> graph;
    int vertices_count;
};
