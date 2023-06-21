#include "travellingsalesman.hpp"

TravellingSalesman::TravellingSalesman(IGraph& igraph, const std::vector<std::pair<float, float>>& coord) : 
            graph(&igraph), coordinates (coord) {}


float TravellingSalesman::GetOptimalWeight() {
    std::vector<int> combination;

    float weight = 0;
    float min_weight = -1;
    for (int i = 0; i < graph->VerticesCount(); i++) {
        combination.push_back(i);
    }

    do {
        weight = CalculationWay(combination);
        if (weight < min_weight || min_weight == -1) {
            min_weight = weight;
        }
    } 
    while (std::next_permutation(combination.begin(), combination.end()));

    return min_weight;
}


float TravellingSalesman::GetMSTWeight() {
    MinimumSpanningTree tree(*graph);
    std::vector<int> verticles;
    verticles.reserve(graph->VerticesCount());
    verticles = tree.GetWay();
    return CalculationWay(verticles);
}


float TravellingSalesman::ApproximationRatio() {
    float mst = GetMSTWeight();
    float optimal = GetOptimalWeight();
    return std::max( mst / optimal, optimal / mst);
}


float TravellingSalesman::CalculationWay(std::vector<int> verticles) {
    float result = 0;
    std::vector<Pair> next_verticles = graph->GetNextVertices(verticles[0]);
    // Вычисляем путь из последней вершины в первую
    if (verticles.size() != 2) {
        for (int i = 0; i < next_verticles.size(); i++) {
            if (next_verticles[i].vertex_number == verticles[graph->VerticesCount() - 1]) {
                result += next_verticles[i].weight;
                break;
            }
        }        
    }

    for (size_t i = 1; i < graph->VerticesCount(); i++) {
        next_verticles = graph->GetNextVertices(verticles[i]);
        for (size_t k = 0; k < next_verticles.size(); k++) {
            if (next_verticles[k].vertex_number == verticles[i-1]) {
                result += next_verticles[k].weight;
                break;
            }
        }
    }

    return result;
}
