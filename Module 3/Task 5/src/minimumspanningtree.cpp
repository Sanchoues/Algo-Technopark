#include "minimumspanningtree.hpp"


MinimumSpanningTree::MinimumSpanningTree(IGraph& igraph) : graph(&igraph),
            previous(igraph.VerticesCount(), -1), min_e(igraph.VerticesCount(), INT_MAX) {
    BuildTree(0);
}

void MinimumSpanningTree::BuildTree(int start_vertex = 0) {
    min_e[start_vertex] = 0;
    pairs.insert(Pair(start_vertex, 0));
    std::set<Pair>::iterator u;
    while (!pairs.empty()) {
        u = pairs.begin();
        for (const auto& v : graph->GetNextVertices(u->vertex_number)) {
            if (min_e[v.vertex_number] == INT_MAX) {
                min_e[v.vertex_number] = v.weight;
                previous[v.vertex_number] = u->vertex_number;
                pairs.insert({ v.vertex_number, v.weight });
            }
            else {
                Relax(u->vertex_number, v);
            }
        }

        pairs.erase(u);
    }
}


void MinimumSpanningTree::Relax(int u, const Pair& v) {
    if (min_e[v.vertex_number] > v.weight &&
                pairs.find(Pair(v.vertex_number, min_e[v.vertex_number])) != pairs.end()) { 
        pairs.erase(Pair(v.vertex_number, min_e[v.vertex_number]));
        min_e[v.vertex_number] = v.weight;
        previous[v.vertex_number] = u;
        pairs.insert(Pair(v.vertex_number, min_e[v.vertex_number]));

    }
}


IGraph* MinimumSpanningTree::GetTree() {
    IGraph* tree = new ListGraph(previous.size());
    float weight = 0;
    for (size_t i = 0; i < previous.size(); i++) {
        if (previous[i] != -1) {
            weight = min_e[i];
            tree->AddEdge(previous[i], i, weight);
        }
    }

    return tree;
}

int MinimumSpanningTree::GetWeight() {
    int result = 0;
    for (int i = 0; i < min_e.size(); i++) {
        result += min_e[i];
    }

    return result;
}

// Обход в глубину
std::vector<int> MinimumSpanningTree::GetWay() {
    IGraph* tree = GetTree();
    std::vector<bool> visited(tree->VerticesCount(), false);
    std::vector<int> result;
    std::stack<int> stack;
    std::vector<Pair> next_verticles;
    int vertex = 0;
    for (int i = 0; i < tree->VerticesCount(); i++) {
        if (!visited[i]) {
            visited[i] = true;
            stack.push(i);
            while (!stack.empty()) {
                vertex = stack.top();
                result.push_back(vertex);
                stack.pop();
                next_verticles = tree->GetNextVertices(vertex);
                std::reverse(next_verticles.begin(), next_verticles.end());
                for (const auto& v : next_verticles) {
                    if (!visited[v.vertex_number]) {
                        visited[v.vertex_number] = true;
                        stack.push(v.vertex_number);
                    }
                }
            }
        }
    }

    delete tree;
    return result;
}


