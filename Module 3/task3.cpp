/*
Требуется отыскать самый выгодный маршрут между городами.
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.

*/


#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <map>

// Можно было бы воспользоваться std::pair, но полями first и second не удобно пользоваться. 
template<typename Key, typename Value>
struct Pair {
    Pair() : vertex_number(0), weight(0) {
    }

    Pair(Key id, Value value) : vertex_number(id), weight(value) {
    }

    Key vertex_number;
    Value weight;
};


template<typename Key, typename Value>
struct PairComparator {
    bool operator()(const Pair<Key, Value>& l, const  Pair<Key, Value>& r) const {
        if (l.weight == r.weight) {
            return l.vertex_number < r.vertex_number;

        }
        
        return l.weight < r.weight;
    }
};

// Неориентированный взвешенный граф


struct MapGraph {
public:
    MapGraph(int vertices_count) : vertices_count(vertices_count) {
        graph.resize(vertices_count);
    }

    virtual void AddEdge(int from, int to, int value) {
        if (graph[from].count(to) == 0 || (graph[from].count(to) == 1 && graph[from][to] > value)) {
            graph[from][to] = value;
            graph[to][from] = value;

        }
    }

    virtual int VerticesCount() const {
        return vertices_count;
    }

    virtual std::vector<Pair<int,int>> GetNextVertices(int vertex) const {
        std::vector<Pair<int, int>> result;
        for (const auto& iter : graph[vertex]) {
            result.push_back(Pair<int,int>(iter.first, iter.second));
        }

        return result;
    }


private:
    std::vector<std::map<int, int>> graph;
    int vertices_count;
};



class Dijkstra {
public:
    Dijkstra(const size_t& size) : graph(size), d(size, INT_MAX), pi(size, -1) {
        
    }

    void InsertEdge(int source, int dest, int value) {
        graph.AddEdge(source, dest, value);
    }

    void Relax(int u, const Pair<int, int>& v) {
        if (d[v.vertex_number] > d[u] + v.weight) {
            pairs.erase(Pair<int, int>(v.vertex_number, d[v.vertex_number]));
            d[v.vertex_number] = d[u] + v.weight;
            pi[v.vertex_number] = u;
            pairs.insert(Pair<int, int>(v.vertex_number, d[v.vertex_number]));
        }
    }

    int FindWay(int source, int dest) {
        d[source] = 0;
        pairs.insert(Pair<int, int>(source, 0));
        std::set<Pair<int, int>>::iterator u;
        while (!pairs.empty()) {
            u = pairs.begin();
            for (auto v : graph.GetNextVertices(u->vertex_number)) {
                if (d[v.vertex_number] == INT_MAX) {
                    d[v.vertex_number] = u->weight + v.weight;
                    pi[v.vertex_number] = u->vertex_number;
                    pairs.insert(Pair<int, int>(v.vertex_number, d[v.vertex_number]));
                }
                else {
                    Relax(u->vertex_number, v);
                }
            }

            pairs.erase(u);

        }

        return d[dest];
    }


private:
    MapGraph graph;
    std::vector<int> d;
    std::vector<int> pi;
    std::set<Pair<int,int>, PairComparator<int, int>> pairs;
    
};


void Run(int vertices_count, int edges_count) {
    Dijkstra searcher(vertices_count);
    int from, to, weight, source, dest;
    for (int i = 0; i < edges_count; i++) {
        std::cin >> from >> to >> weight;
        searcher.InsertEdge(from, to, weight);
    }
    
    std::cin >> source >> dest;
    std::cout << searcher.FindWay(source, dest) << std::endl;
}

int main()
{
    int vertices_count, edges_count;
    std::cin >> vertices_count >> edges_count;
    Run(vertices_count, edges_count);
    return 0;
}

