/*
2_3. Выведите элементы в порядке post-order (снизу вверх).
*/

#include <iostream>
#include <stack>

template<typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) {
        return l <= r;
    }
};

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
    Node(const T& value) : left(nullptr), right(nullptr), data(value) {
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class BinaryTree {
public:
    BinaryTree(Comparator comp = Comparator()):  root(nullptr){
    }

    ~BinaryTree() {
        PostOrder([](Node<T>* value) {
            delete value;
        });
    }

    void Insert(const T& value) {
        if (root == nullptr) {
            root = new Node<T>(value);
            return;
        }

        Node<T>* searcher = root;
        while (searcher != nullptr) {
            if (comp(searcher->data, value)) {
                if (searcher->right == nullptr) {
                    searcher->right = new Node<T>(value);
                    return;
                }
                else {
                    searcher = searcher->right;
                }
            }
            else {
                if (searcher->left == nullptr) {
                    searcher->left = new Node<T>(value);
                    return;
                }
                else {
                    searcher = searcher->left;
                }
            }
        }

    }

      void PostOrder(void (*func)(Node<T>*)) {

        std::stack<Node<T>*> first;
        std::stack<Node<T>*> second;
        Node<T>* top;
        first.push(root);
        while (!first.empty()) {
            top = first.top();
            first.pop();    
            if (top->left != nullptr) {
                first.push(top->left);
            }
            if (top->right != nullptr) {
                first.push(top->right);
            }
            second.push(top);
        }

        while (!second.empty()) {
            func(second.top());
            second.pop();
        }
    }

private:
    Node<T>* root;
    Comparator comp;
};

template<typename T>
void Print(Node<T>* value) {
    std::cout << value->data << std::endl;
}

void Run(int n) {
    BinaryTree<int, DefaultComparator<int>> tree;
    int data = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> data;
        tree.Insert(data);
    }

    tree.PostOrder(&Print);
}

int main() {
    int n;
    std::cin >> n;
    Run(n);
}