#include <cmath>
#include <random>
#include "listgraph.hpp"

class RandomGraph {
public:
    RandomGraph();    
    ~RandomGraph();

    std::pair<float, float> Randomizer();
    void GenerateGraph(int size);
    IGraph* GetGraph();
    std::vector<std::pair<float, float>> GetCoordinates();

private:

    float Distance(std::pair<float, float> first_v, std::pair<float, float> second_v);
    size_t vertices_count;
    IGraph* graph;
    std::vector<std::pair<float, float>> coordinates;
};
