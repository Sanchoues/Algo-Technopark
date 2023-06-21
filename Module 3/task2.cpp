/*
Дан невзвешенный неориентированный граф. 
В графе может быть несколько кратчайших путей между какими-то вершинами. 
Найдите количество различных кратчайших путей между заданными вершинами. 
Требуемая сложность O(V+E).
*/


#include <iostream>
#include <vector>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

};


struct ListGraph: public IGraph {
public:
    ListGraph(int vertices_count): vertices_count(vertices_count) {
        graph.resize(vertices_count);
        reverse_graph.resize(vertices_count);
        
    }

    ListGraph(const IGraph& igraph) {
        vertices_count = igraph.VerticesCount();
        graph.resize(vertices_count);
        reverse_graph.resize(vertices_count);

        for (int i = 0; i < vertices_count; i++) {
            graph[i] = igraph.GetNextVertices(i);
            reverse_graph[i] = igraph.GetPrevVertices(i);
        }

    } 

    virtual ~ListGraph() {}

    virtual void AddEdge(int from, int to) {
        graph[from].push_back(to);
        reverse_graph[to].push_back(from);
    }

    virtual int VerticesCount() const override {
        return vertices_count;
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> result;
        result.reserve(graph[vertex].size());

        for (int i = 0; i < graph[vertex].size(); i++) {
            result.push_back(graph[vertex][i]);
        }
        return result;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        result.reserve(reverse_graph[vertex].size());

        for (int i = 0; i < reverse_graph[vertex].size(); i++) {
            result.push_back(reverse_graph[vertex][i]);
        }
        return result;
    }

private:
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> reverse_graph;
    int vertices_count;
};



std::vector<int> GetPathCount(const IGraph& graph, int source) {
    std::vector<int> dist(graph.VerticesCount());
    std::vector<int> path_count(graph.VerticesCount()); 
    std::vector<bool> visited(graph.VerticesCount(), false); 
    std::vector<int> children; 
    std::queue<int> queue;
    int current_vertex;
    path_count[source] = 1;
    
    queue.push(source);
        
    while (!queue.empty()) { 
        current_vertex = queue.front();
        queue.pop();
        children = graph.GetNextVertices(current_vertex);

        for (int child: children) {
            if (dist[child] == 0 || dist[child] > dist[current_vertex] + 1) {
                dist[child] = dist[current_vertex] + 1;
                path_count[child] = path_count[current_vertex];
            } 
            else if (dist[child] == dist[current_vertex] + 1) {
                path_count[child] += path_count[current_vertex];
            }

            if (!visited[child]) {
                visited[child] = true;
                queue.push(child);
            }

        }

    }

    return path_count;
}

void Run(int vertex_num, int edge_num) {
    ListGraph graph(vertex_num);
    int first_edge, second_edge, source, dest;
    for (int i = 0; i < edge_num; i++) {
        std:: cin >> first_edge >> second_edge;
        graph.AddEdge(first_edge, second_edge);
        graph.AddEdge(second_edge, first_edge);

    }

    std::cin >> source >> dest;
    std::vector<int> result = GetPathCount(graph,source);
    std::cout << result[dest];
    
}

int main() {
    int vertex_num, edge_num;
    std::cin >> vertex_num >> edge_num;
    Run(vertex_num, edge_num);
    return 0;
}