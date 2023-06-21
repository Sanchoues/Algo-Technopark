/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. 
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.
*/

#include <iostream>
#include <vector>

template <class Key> struct Hash;

template<> struct Hash<std::string> {
    size_t operator()(const std::string& key, size_t m, size_t i) {

        size_t hash = (FirstStringHasher(key) + i * SecondStringHasher(key, m)) % m;
        return hash;
    }

    size_t FirstStringHasher(const std::string& key) {
        size_t hash = 0;
        size_t kPrimeNumber = 17;
        for (char i: key) {
            hash = hash * kPrimeNumber + i;
        }
        return hash;
    }

    size_t SecondStringHasher(const std::string& key, size_t m) {
        size_t hash = 0;
        size_t kPrimeNumber = 19;
        for (char i : key) {
            hash = hash * kPrimeNumber + i;
        }
        return (hash * 2 + 1) % m;
    }

};

template <typename Key, typename Value, typename Hasher = Hash<Key> >
class HashTable {

    static const size_t kInitialSize = 8;

    struct Node {
        Key key;
        Value value;
        bool is_deleted;

        Node(const Key& key, const Value& val) : key(key), value(val), is_deleted(false) {
        }
    };

public:
    HashTable(Hasher hasher = Hasher()) : GetHash(hasher), buckets_count(kInitialSize),
        buckets(kInitialSize, nullptr), items_count(0), max_load_factor(kInitialSize * 3 / 4) {
    }

    ~HashTable() {
        for (size_t i = 0; i < buckets_count; i++) {
            delete buckets[i];
        }
    }

    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;

    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool Has(const Key& key) {
        size_t i = 0;
        size_t hash = GetHash(key, buckets_count, i++);
        while (buckets[hash] != nullptr) {
            if (buckets[hash]->key == key && !buckets[hash]->is_deleted) {
                return true;
            }
            hash = GetHash(key, buckets_count, i++);
        }
        return false;
    }

    bool Insert(const Key& key, const Value& value) {
        if (Has(key)) {
            return false;
        }

        if (items_count >= max_load_factor) {
            Grow();
        }

        size_t i = 0;
        size_t hash = GetHash(key, buckets_count, i++);
        size_t first_deleted = -1;
        while (buckets[hash] != nullptr) {
            if (buckets[hash]->is_deleted && first_deleted == -1) {
                first_deleted = hash;
            }
            hash = GetHash(key, buckets_count, i++);
        }

        if (first_deleted != -1) {
            buckets[first_deleted] = new Node(key, value);
        }
        else {
            items_count++;
            buckets[hash] = new Node(key, value);
        }

        return true;
    }

    bool Delete(const Key& key) {
        if (!Has(key)) {
            return false;
        }

        size_t i = 0;
        size_t hash = GetHash(key, buckets_count, i++);
        while (buckets[hash]->key != key || buckets[hash]->is_deleted) {
            hash = GetHash(key, buckets_count, i++);
        }

        buckets[hash]->is_deleted = true;
        return true;
    }

private:
    void Grow() {
        buckets_count *= 2;
        std::vector<Node*> buf = std::move(buckets);
        std::vector<Node*> new_buckets(buckets_count, nullptr);
        buckets = new_buckets;
        max_load_factor = max_load_factor * 2;
        items_count = 0;

        for (size_t i = 0; i < buckets_count / 2; i++) {
            if (buf[i] != nullptr) {
                if (!buf[i]->is_deleted) {
                    Insert(buf[i]->key, buf[i]->value);
                }
                delete buf[i];
            }
        }
  
    }

    std::vector<Node*> buckets;
    Hasher GetHash;
    size_t items_count;
    size_t buckets_count;
    size_t max_load_factor;
};


void Run(std::istream &in, std::ostream &out) {
    HashTable<std::string, std::string> table;
    std::string word = "";
    char command = 0;

    while (in >> command >> word) {
        switch(command) {
            case('+'):
                if (table.Insert(word, "")) {
                    out << "OK" << std::endl;
                }
                else {
                    out << "FAIL" << std::endl;
                }
                break;
            case('-'):
                if (table.Delete(word)) {
                    out << "OK" << std::endl;
                }
                else {
                    out << "FAIL" << std::endl;
                }
                break;
            case('?'):
                if (table.Has(word)) {
                    out << "OK" << std::endl;
                }
                else {
                    out << "FAIL" << std::endl;
                }
                break;
        }
    }
}

int main() {
    Run(std::cin, std::cout);
    return 0;
}
