#include <set>
#include <climits>
#include <stack>
#include <algorithm>
#include "listgraph.hpp"

class MinimumSpanningTree {
public:

    MinimumSpanningTree(IGraph& igraph);

    void BuildTree(int start_vertex);
    IGraph* GetTree();
    int GetWeight();
    std::vector<int> GetWay();

private:
    void Relax(int u, const Pair& v);

    IGraph* graph;
    std::vector<int> previous;
    std::vector<float> min_e;
    std::set<Pair, PairComparator> pairs;
};