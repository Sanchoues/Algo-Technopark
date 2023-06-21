#include <iostream>
#include "setgraph.hpp"
#include "listgraph.hpp"
#include "matrixgraph.hpp"
#include "arcgraph.hpp"

/*
Граф:
         |<-------------|
      |- 0 -> 1<-> 2 <-> 5
      |  ^ / |    ^
      |  3   |    |
      |  ^  \|    |
      |->4 <-6 ---- 
*/

int main() {

    ListGraph list_graph(7);
    list_graph.AddEdge(0,1);
    list_graph.AddEdge(0,4);

    list_graph.AddEdge(1,2);
    list_graph.AddEdge(1,3);
    list_graph.AddEdge(1,6);

    list_graph.AddEdge(2,1);
    list_graph.AddEdge(2,5);


    list_graph.AddEdge(3,0);
    list_graph.AddEdge(3,1);
    list_graph.AddEdge(3,6);

    list_graph.AddEdge(4,3);

    list_graph.AddEdge(5,2);
    list_graph.AddEdge(5,4);

    list_graph.AddEdge(6,1);
    list_graph.AddEdge(6,3);
    list_graph.AddEdge(6,4);

    MatrixGraph matrix_graph(list_graph);
    std::vector<int> tested; 
    std::vector<int> validated;
    for (int i = 0; i < matrix_graph.VerticesCount(); i++) {
        tested = matrix_graph.GetNextVertices(i);
        validated = list_graph.GetNextVertices(i);
        if (list_graph.GetNextVertices(i) != matrix_graph.GetNextVertices(i)) {
            std::cout << "[MatrixGraph] GetNextVertices not equal for vertex " << i <<std::endl;
        } 

        if (list_graph.GetPrevVertices(i) != matrix_graph.GetPrevVertices(i)) {
            std::cout << "[MatrixGraph] GetPrevVertices not equal for vertex " << i <<std::endl;
        } 
    }

    SetGraph set_graph(matrix_graph);
    for (int i = 0; i < set_graph.VerticesCount(); i++) {
        if (set_graph.GetNextVertices(i) != matrix_graph.GetNextVertices(i)) {
            std::cout << "[SetGraph] GetNextVertices not equal for vertex " << i <<std::endl;
        } 

        if (set_graph.GetPrevVertices(i) != matrix_graph.GetPrevVertices(i)) {
            std::cout << "[SetGraph] GetPrevVertices not equal for vertex " << i <<std::endl;
        } 
    }

    ArcGraph arc_graph(set_graph);
    for (int i = 0; i < matrix_graph.VerticesCount(); i++) {
        if (arc_graph.GetNextVertices(i) != set_graph.GetNextVertices(i)) {
            std::cout << "[ArcGraph] GetNextVertices not equal for vertex " << i <<std::endl;
        } 

        if (arc_graph.GetPrevVertices(i) != set_graph.GetPrevVertices(i)) {
            std::cout << "[ArcGraph] GetPrevVertices not equal for vertex " << i <<std::endl;
        } 
    }

    std::cout << "END" << std::endl;

}