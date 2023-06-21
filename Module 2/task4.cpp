/*
4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
 Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
 а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
 в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для
 каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.

Требования: скорость выполнения команды - O(log n).
Формат входных данных.
Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой следующей строке содержится описание команды:
 число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y если солдата,
 стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.
Формат выходных данных.
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен встать этот солдат
(все стоящие за ним двигаются назад).
*/

#include <iostream>
#include <stack>

template<typename T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) {
        if (l < r) {
            return -1;
        }
        else if (l > r) {
            return 1;
        }
        return 0;
    }
};


template<typename Key, typename Value, typename Comparator = DefaultComparator<Key>>
class AVLTree {
    struct Node {
        Key key;
        Value value;

        Node* left;
        Node* right;
        uint8_t heigth;
        // Количество узлов в поддереве
        int nodes_num;
        
        Node(const Key& key, const Value& value) :
            left(nullptr), right(nullptr), key(key), value(value), heigth(1), nodes_num(1) {
        }

        ~Node() {
            delete left;
            delete right;
        }
    };

public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), items_count(0), comp(comp) {}

    ~AVLTree() {
        delete root;
    }

    size_t Size() const {
        return items_count;
    }

    Value* Find(const Key& key) {
        return FindAux(key, root);
    }

    void Insert(const Key& key, const Value& value) {
        root = InsertAux(key, value, root);
    }

    void Erase(const Key& key) {
        root = EraseAux(key, root);
    }

    Key FindByIndex(int index) {
        Node* searcher = root;
        Key result;
        int cmp_res;

        while (searcher != nullptr) {
            //Если идем по правой ветке, то индекс не меняется.
            //Если идем по левой ветке, то пересчитываем индекс.

            cmp_res = comp(NodesNum(searcher->right), index);
            result = searcher->key;

            if (cmp_res == 0) {
                break;
            }
            else if (cmp_res == 1) {
                searcher = searcher->right;
            }
            else {
                index = index - NodesNum(searcher->right) - 1;
                searcher = searcher->left;
            }
        }
        return result;
    }

    int Position(const Key& key) {

        Node* searcher = root;
        int position = NodesNum(searcher);
        int cmp_res;

        while (searcher != nullptr) {
            // Если ключ меньше, чем ключ в вершине поддерева, идем в левое поддерево.
            // Если ключ больше ключа вершины правого поддерева, то идем вправо и пересчитываем возможную позицию.
            // Повторяем, пока не null. 
            cmp_res = comp(searcher->key, key);

            if (cmp_res == 0) {
                break;
            }
            else if (cmp_res == 1) {
                searcher = searcher->left;
            }
            else if (cmp_res == -1) {
                position = position - NodesNum(searcher->left) - 1;
                searcher = searcher->right;
            }
        }
        return position;
    }

private:
    Value* FindAux(const Key& key, Node* node) {
        if (!node) {
            return nullptr;
        }

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) {
            return FindAux(key, node->left);
        }
        else if (cmp_res == 1) {
            return FindAux(key, node->right);
        }

        return &node->value;
    }

    Node* InsertAux(const Key& key, const Value& value, Node* node) {
        if (!node) {
            items_count++;
            return new Node(key, value);
        }

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) {
            node->left = InsertAux(key, value, node->left);
        }
        else if (cmp_res == 1) {
            node->right = InsertAux(key, value, node->right);
        }
        return Balance(node);
    }

    Node* EraseAux(const Key& key, Node* node) {
        if (!node) {
            return nullptr;
        }

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) {
            node->left = EraseAux(key, node->left);
        }
        else if (cmp_res == 1) {
            node->right = EraseAux(key, node->right);
        }
        else {
            Node* left = node->left;
            Node* right = node->right;
            node->right = nullptr;
            node->left = nullptr;

            delete node;
            if (!right) {
                return left;
            }

            Node* min_node = FindAndRemoveMinNode(right);
            min_node->left = left;
            return Balance(min_node);
        }

        return Balance(node);
    }

    Node* FindAndRemoveMinNode(Node* node) {
        if (node->left == nullptr) {
            return node;
        }

        std::stack<Node*> buff;
        while (node->left->left) {
            buff.push(node);
            node = node->left;
        }

        Node* min_node = node->left;
        node->left = node->left->right;

        Node* new_node;
        Node* balanced_node = Balance(node);
        while (!buff.empty()) {
            new_node = buff.top();
            new_node->left = balanced_node;
            balanced_node = Balance(new_node);
            buff.pop();
        }

        min_node->right = balanced_node;
        return min_node;
    }

    Node* Balance(Node* node) {
        FixHeight(node);

        int bf = BFactor(node);
        if (bf == 2) {
            if (BFactor(node->right) < 0) {
                node->right = RotateRight(node->right);
            }
            return RotateLeft(node);
        }
        if (bf == -2) {
            if (BFactor(node->left) > 0) {
                node->left = RotateLeft(node->left);
            }
            return RotateRight(node);
        }
        return node;
    }

    Node* RotateRight(Node* node) {
        Node* local_root = node->left;
        node->left = local_root->right;
        local_root->right = node;
        FixHeight(node);
        FixHeight(local_root);
        return local_root;
    }

    Node* RotateLeft(Node* node) {
        Node* local_root = node->right;
        node->right = local_root->left;
        local_root->left = node;
        FixHeight(node);
        FixHeight(local_root);
        return local_root;
    }

    void FixHeight(Node* node) {
        node->heigth = std::max(Height(node->left), Height(node->right)) + 1;
        node->nodes_num = NodesNum(node->left) + NodesNum(node->right) + 1;
    }

    uint8_t Height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->heigth;
    }

    int NodesNum(Node* node) {
        if (!node) {
            return 0;
        }
        return node->nodes_num;
    }

    int BFactor(Node* node) {
        return Height(node->right) - Height(node->left);
    }

    Node* root;
    size_t  items_count;
    Comparator comp;
};

void Run(int num) {
    AVLTree<int, int, DefaultComparator<int>> soldiers;
    int command, key;
    for (int i = 0; i < num; i++) {
        std::cin >> command >> key;
        if (command == 1) {
            std::cout << soldiers.Position(key) << std::endl;
            soldiers.Insert(key, 0);
        }
        if (command == 2) {
            soldiers.Erase(soldiers.FindByIndex(key));
        }
    }
}

int main() {
    int num;
    std::cin >> num;
    Run(num);
    return 0;
}