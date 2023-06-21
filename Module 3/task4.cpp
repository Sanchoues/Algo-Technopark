/*
Написать алгоритм для решения игры в “пятнашки”. 
Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 
задает пустую ячейку. 
Достаточно найти хотя бы какое-то решение. 
Число перемещений костяшек не обязано быть минимальным.
Если вам удалось найти решение, то в первой строке файла выведите число перемещений, которое 
требуется сделать в вашем решении. 
А во второй строке выведите соответствующую последовательность ходов: L означает, 
что в результате перемещения костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. 
Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.
*/

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <stack>
#include <climits>

static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

const FieldArray EMPTY_STATE = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
};

const FieldArray GOAL_STATE = {
    1,2,3,4,
    5,6,7,8,
    9,10,11,12,
    13,14,15,0
};

class FieldState {
public:
    FieldState() : zero_pos(0) {}

    FieldState(const FieldState& st) : state(st.state), zero_pos(st.zero_pos) {}

    FieldState(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == 0) {
                zero_pos = i;
            }
            state[i] = arr[i];
        }
    }

    ~FieldState() {
    }

    std::vector<std::pair<FieldState, char>> GetNextVertices() const {
        std::vector<std::pair<FieldState, char>> result;
        // UP
        if (zero_pos >= LINE_SIZE) {
            FieldState st(*this);
            st.MoveUp();
            result.push_back({ st , 'D' });
        }

        // DOWN
        if (zero_pos < LINE_SIZE * (LINE_SIZE - 1)) {
            FieldState st(*this);
            st.MoveDown();
            result.push_back({ st , 'U' });
        }

        // LEFT
        if (zero_pos % LINE_SIZE != 0) {
            FieldState st(*this);
            st.MoveLeft();
            result.push_back({ st , 'R' });
        }

        // RIGHT
        if ((zero_pos + 1) % LINE_SIZE != 0) {
            FieldState st(*this);
            st.MoveRight();
            result.push_back({ st , 'L' });
        }

        return result;
    }

    bool operator==(const FieldState& r) const {
        return state == r.state;
    }

    bool operator!=(const FieldState& r) const {
        return state != r.state;
    }

    bool operator<(const FieldState& r) const {
        return state < r.state;
    }

    int operator[](const int& i) const {
        return state[i];
    }

private:
    void MoveUp() {
        int new_zero_pos = zero_pos - LINE_SIZE;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }

    void MoveDown() {
        int new_zero_pos = zero_pos + LINE_SIZE;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }

    void MoveLeft() {
        int new_zero_pos = zero_pos - 1;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }

    void MoveRight() {
        int new_zero_pos = zero_pos + 1;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }

    FieldArray state;
    int zero_pos;
};


bool Parity(const FieldArray& field) {
    int N = 0;
    int K = 0;
    int zero_pos = 0;
    for (int i = 1; i < field.size(); i++) {
        if (field[i] == 0) {
            zero_pos = i;
            continue;
        }
        for (int k = 0; k < i; k++) {
            if (field[k] > field[i]) {
                N++;
            }
        }
    }

    K = zero_pos / LINE_SIZE + 1;
    if ((N + K) % 2 == 0) {
        return true;
    }
    else {
        return false;
    }
}


int ManhattanDistance(FieldState state) {
    float coef = 1;
    int state_x, state_y, goal_x, goal_y;
    int metric = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        state_x = i % LINE_SIZE;
        state_y = std::floor(i / LINE_SIZE);
        if (state[i] == 0) {
            goal_x = LINE_SIZE - 1;
            goal_y = LINE_SIZE - 1;
        }
        else {
            goal_x = (state[i] - 1) % LINE_SIZE;
            goal_y = std::floor((state[i] - 1) / LINE_SIZE);
        }


        metric = abs(state_x - goal_x) + abs(state_y - goal_y) + metric;
    }

    metric = metric * coef;
    return metric;
}


template<typename Key, typename Value>
struct Pair {
    Pair() : state(0), weight(0) {
    }

    Pair(Key id, Value value) : state(id), weight(value) {
    }

    Key state;
    Value weight;
};


template<typename Key, typename Value>
struct PairComparator {
    bool operator()(const Pair<Key, Value>& l, const  Pair<Key, Value>& r) const {
        if (l.weight == r.weight) {
            return l.state < r.state;
        }

        return l.weight < r.weight;
    }
};


void AStar(FieldState start) {
    std::map<FieldState, int> distance;
    std::map<FieldState, std::pair<FieldState, char>> previous;
    FieldState goal(GOAL_STATE);
    distance[start] = 0;
    std::set<Pair<FieldState, int>, PairComparator<FieldState, int>> pairs;
    std::set<Pair<FieldState, int>>::iterator current;

    pairs.insert({ start,0 });
    while (!pairs.empty()) {
        current = pairs.begin();

        if (current->state == goal) {
            break;
        }

        for (const auto& v : current->state.GetNextVertices()) {
            if (distance.count(v.first) == 0) {
                distance[v.first] = INT_MAX;
                previous[v.first] = { current->state, v.second };
                pairs.insert({ v.first, distance[v.first] });
            }
            if (distance[v.first] > distance[current->state] ) {
                pairs.erase(Pair<FieldState, int>(v.first, distance[v.first]));
                distance[v.first] = distance[current->state];
                previous[v.first] = { current->state, v.second };
                pairs.insert(Pair<FieldState, int>(v.first, distance[v.first] + ManhattanDistance(v.first)));
            }
        }

        pairs.erase(current);

    }

    if (pairs.empty()) {
        std::cout << -1 << std::endl;
    }
    else {
        std::stack<char> way;
        auto current = previous[goal];

        while (true) {
            way.push(current.second);
            if (current.first != start) {
                current = previous[current.first];
            }
            else {
                break;
            }

        }

        std::cout << way.size() << std::endl;

        while (!way.empty()) {
            std::cout << way.top();
            way.pop();
        }

    }

}



void Solution() {
    FieldArray start_array;
    int num;
    for (int i = 0; i < FIELD_SIZE; i++) {

        std::cin >> num;
        start_array[i] = num;
    }

    if (!Parity(start_array)) {
        std::cout << -1 << std::endl;
        return;
    }

    FieldState start_state(start_array);
    AStar(start_state);
}


int main()
{
    Solution();
    return 0;
}
