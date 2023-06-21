#include "igraph.hpp"

struct ArcGraph: public IGraph {
public:
    ArcGraph(int vertices_count);
    ArcGraph(const IGraph& igraph);

    virtual ~ArcGraph();
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:

    struct Pair {
        Pair(): source(0), destination(0) {}  
        Pair(int from, int to): source(from), destination(to) {}  

        int source;
        int destination;
    };

    std::vector<Pair> graph;
    int vertices_count;
};
