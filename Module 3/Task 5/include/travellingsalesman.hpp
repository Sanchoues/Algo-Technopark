#include "listgraph.hpp"
#include "minimumspanningtree.hpp"

class TravellingSalesman {
public:
    TravellingSalesman(IGraph& igraph, const std::vector<std::pair<float, float>>& coord);

    float GetOptimalWeight();
    float GetMSTWeight();
    float ApproximationRatio();

private:

    float CalculationWay(std::vector<int> verticles);

    IGraph* graph;
    std::vector<std::pair<float, float>> coordinates;
};