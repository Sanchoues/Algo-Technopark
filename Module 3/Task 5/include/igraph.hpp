#pragma once
#include <vector>

struct Pair {
    Pair() : vertex_number(0), weight(0) {
    }

    Pair(int id, float value) : vertex_number(id), weight(value) {
    }

    int vertex_number;
    float weight;
};

struct PairComparator {
    bool operator()(const Pair& l, const  Pair& r) const {
        if (l.weight == r.weight) {
            return l.vertex_number < r.vertex_number;

        }

        return l.weight < r.weight;
    }
};

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, float value) = 0;
    virtual int VerticesCount() const = 0;
    virtual std::vector<Pair> GetNextVertices(int vertex) const = 0;

};

