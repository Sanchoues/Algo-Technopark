#include "randomgraph.hpp"


RandomGraph::RandomGraph() : vertices_count(0), graph(nullptr) {
    srand(time(0));
}

RandomGraph::~RandomGraph() {
    delete graph;
}

std::pair<float, float> RandomGraph::Randomizer() {
    float u = rand() * (1.0 / RAND_MAX);
    float v = rand() * (1.0 / RAND_MAX);
    float x = std::cos(2 * 3.1415 * u) * std::sqrt(-2 * std::log(v));
    float y = std::sin(2 * 3.1415 * u) * std::sqrt(-2 * std::log(v));
    return std::pair<float, float>({ x,y });
}

void RandomGraph::GenerateGraph(int size) {
    if (graph != nullptr) {
        delete graph;
    }

    vertices_count = size;
    coordinates.resize(size);

    for (size_t i = 0; i < vertices_count; i++) {
        coordinates[i] = Randomizer();
    }

    graph = new ListGraph(vertices_count);
    float weight = 0;
    for (size_t i = 0; i < vertices_count; i++) {
        for (size_t k = i + 1; k < vertices_count; k++) {
            weight = Distance(coordinates[i], coordinates[k]);
            graph->AddEdge(i, k, weight);
        }
    }
    
}

IGraph* RandomGraph::GetGraph() {
    return graph;
}


std::vector<std::pair<float, float>> RandomGraph::GetCoordinates() {
    return coordinates;
}



float RandomGraph::Distance(std::pair<float, float> first_v, std::pair<float, float> second_v) {
    float result = std::sqrt(std::pow(first_v.first - second_v.first, 2) + std::pow(first_v.second - second_v.second, 2));
    return result;
}

