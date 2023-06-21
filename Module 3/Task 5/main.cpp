/*
Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1.
Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. 
Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.

Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима.

*/

#include <iostream>
#include "travellingsalesman.hpp"
#include "randomgraph.hpp"

void Test() {
    RandomGraph random_graph;
    IGraph* graph;
    std::pair<int, int> range({2, 10});
    std::vector<std::pair<float, float>> coordinates;
    std::vector<float> approximation_ratio;
    float standart_deviation, arithmetic_mean_deviation;
    float sum = 0;
    float diff_sum = 0;
    int count = 10;

    for (int i = range.first ; i < range.second; i++) {
        approximation_ratio.clear();
        for (int k = 0; k < count; k++) {
            random_graph.GenerateGraph(i);
            graph = random_graph.GetGraph();
            coordinates = random_graph.GetCoordinates();
            TravellingSalesman salesman(*graph, coordinates);
            approximation_ratio.push_back(salesman.ApproximationRatio());

        }

        for (const auto& it : approximation_ratio) {
            sum += it;
        }
        arithmetic_mean_deviation = sum / count;

        for (const auto& it : approximation_ratio) {
            diff_sum += std::pow(it - arithmetic_mean_deviation, 2);
        }

        standart_deviation = std::sqrt(diff_sum / count);
        std::cout << "N = " << i << std::endl; 
        std::cout << "Arithmetic mean deviation: " << arithmetic_mean_deviation << std::endl;
        std::cout << "Standart deviation: " << standart_deviation << std::endl << std::endl ;

        sum = 0;
        diff_sum = 0;
    }

}

// Граф из лекции для проверки, что веса путей вычисляется верно
void LectureTest() {
    std::vector<std::pair<float, float>> coordinates;
    coordinates.push_back({ 2, 5 });
    coordinates.push_back({ 2, 3 });
    coordinates.push_back({ 1, 2 });
    coordinates.push_back({ 4, 5 });
    coordinates.push_back({ 5, 4 });
    coordinates.push_back({ 4, 3 });
    coordinates.push_back({ 6, 3 });
    coordinates.push_back({ 3, 1 });

    IGraph* graph = new ListGraph(8);
    float weight = 0;
    for (size_t i = 0; i < coordinates.size(); i++) {
        for (size_t k = i + 1; k < coordinates.size(); k++) {
            weight = std::sqrt(std::pow(coordinates[k].first - coordinates[i].first, 2) + std::pow(coordinates[k].second - coordinates[i].second, 2));
            graph->AddEdge(i, k, weight);
        }
    }

    TravellingSalesman salesman(*graph, coordinates);

    std::cout << "Lecture example: " << std::endl;
    std::cout << "Optimal: " << salesman.GetOptimalWeight() << std::endl;
    std::cout << "MST: " << salesman.GetMSTWeight() << std::endl;

    delete graph;
}

int main()
{
    Test();
    LectureTest();

    return 0;
}
