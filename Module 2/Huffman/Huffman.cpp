#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>

#include "Huffman.h"


// Интерфейсы 
typedef unsigned char byte;


// Чтение бит
class BitReader {
public:
    BitReader(IInputStream& in_stream) : in(in_stream), buf(0), pos(0) {
    }

    bool ReadBit(byte& value) {
        if (pos == 0) {
            bool res = in.Read(buf);
            if (!res) {
                return false;
            }
        }

        value = (buf >> (8 - (++pos))) & 1;
        if (pos == 8) {
            pos = 0;
        }
        return true;
    }


private:
    IInputStream& in;
    byte buf;
    int pos;
};

// Запись бит
class BitWriter {
public:
    BitWriter(IOutputStream& out_stream) : out(out_stream), buf(0), pos(0) {
    }

    void WriterBit(byte value) {
        value &= 1;
        buf = buf | (value << (7 - pos++));
        if (pos == 8) {
            Flush();
        }
    }

    void Flush() {
        if (pos > 0) {
            out.Write(buf);
            buf = 0;
            pos = 0;
        }
    }
private:
    IOutputStream& out;
    byte buf;
    int pos;
};

// Кодирование и декодирования
const size_t kMaxSize = 8;

struct Node {
    Node(): right(nullptr), left(nullptr), weight(1) {
    };
    
    std::vector<byte> value;
    size_t weight;
    Node* right;
    Node* left;

};

struct NodeComparator {
    bool operator()(const Node* l, const Node* r) {
        return l->weight > r->weight;
    }
};

struct VectorHasher {
int operator()(const std::vector<byte>& vector) const {
    size_t hash = vector.size();
    size_t kPrimeNumber = 17;
    for (auto& i : vector) {
        hash = hash * kPrimeNumber + i;
    }
    return hash;
}
};

void CopyBits(byte *output, const byte *input) {
    for (size_t i = 0; i < kMaxSize; i++) {
        output[i] = input[i];
    }
}


Node* HuffmanTree(const std::unordered_map<std::vector<byte>, Node*, VectorHasher>& counter) {
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> queue;
    for (auto iter : counter) {
        queue.push(iter.second);
    }

    Node* first;
    Node* second;
    Node* buff;
    while (queue.size() != 1) {
        first = queue.top();
        queue.pop();
        second = queue.top();
        queue.pop();
        buff = new Node();
        buff->left = first;
        buff->right = second;
        buff->weight = first->weight + second->weight;
        queue.push(buff);
    }

    return queue.top();
}

void DeleteTree(Node* root) {
    std::stack<Node*> first;
    std::stack<Node*> second;
    Node* top;
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
        delete second.top();
        second.pop();
    }
}

void EncodeString(std::vector<byte>& encode_string, Node* tree) {
    if (tree->left == nullptr && tree->right == nullptr) {
        encode_string.push_back(0);
        for (size_t i = 0; i < tree->value.size(); i++) {
            encode_string.push_back(tree->value[i]);
        }
        return;
    }
    encode_string.push_back(1);
    EncodeString(encode_string, tree->left);
    EncodeString(encode_string, tree->right);

}

void EncodeTable(std::unordered_map<std::vector<byte>, std::vector<byte>, VectorHasher>& encode_table, std::vector<byte> current_byte, Node* tree) {
    if (tree->left == nullptr && tree->right == nullptr) {
        if (current_byte.empty()) {
            current_byte.push_back(0);
        }
        encode_table[tree->value] = current_byte;
        return;
    }

    std::vector<byte> current_byte_right = current_byte;
    current_byte.push_back(0);
    EncodeTable(encode_table, current_byte, tree->left);
    current_byte_right.push_back(1);
    EncodeTable(encode_table, current_byte_right, tree->right);
 }

std::vector<byte> IntToByte(size_t integer, size_t num_byte) {
    std::stack<byte> buffer;
    while (integer != 0) {
        buffer.push(integer & 1);
        integer = integer >> 1;
    }

    std::vector<byte> result(num_byte * 8 - buffer.size(), 0);
    while (!buffer.empty()) {
        result.push_back(buffer.top());
        buffer.pop();
    }

    return result;
}



// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed) {
    BitReader bits(original);
    byte bit = 0;
    std::unordered_map<std::vector<byte>,Node*, VectorHasher> counter;
    std::vector<byte> key;
    std::vector<byte> buff_stream;

    while (bits.ReadBit(bit)) {
        key.push_back(bit);
        buff_stream.push_back(bit);
        if (key.size() == kMaxSize) {
            if (counter.find(key) != counter.end()) {
                counter[key]->weight++;
            }
            else {
                counter[key] = new Node();
                counter[key]->value = key;
                counter[key]->weight = 1;
            }
            key.clear();
        }
    }

    if (key.size() != 0) {
        counter[key] = new Node();
        counter[key]->value = key;
        counter[key]->weight = 1;
    }
    
    Node* tree = HuffmanTree(counter);
    std::vector<byte> encode_string;
    EncodeString(encode_string, tree);
    std::unordered_map<std::vector<byte>, std::vector<byte>, VectorHasher> code_table;
    std::vector<byte> word;
    EncodeTable(code_table, word, tree);

    DeleteTree(tree);

    BitReader to_output(original);
    BitWriter bit_writer(compressed);
    std::vector<byte> encode_string_size;
    std::vector<byte> int_to_byte;

    encode_string_size = IntToByte(encode_string.size(), 4);

    std::vector<byte> buff;
    std::vector<byte> code;
    std::vector<byte> encoded_text;
    size_t encoded_length = 0;
    for (size_t i = 0; i < buff_stream.size(); i++) {
        buff.push_back(buff_stream[i]);
        if (code_table.find(buff) != code_table.end()) {
            code = code_table[buff];
            for (size_t i = 0; i < code.size(); i++) {
               //bit_writer.WriterBit(code[i]);
                encoded_text.push_back(code[i]);
            }

            encoded_length += code.size();
            buff.clear();
        }
    }

    if (encoded_length + encode_string.size() + 5 * 8 >= buff_stream.size()) {
        for (size_t i = 0; i < 8; i++) {
            bit_writer.WriterBit(1);
        }
        for (size_t i = 0; i < buff_stream.size(); i++) {
            bit_writer.WriterBit(buff_stream[i]);
        }
        return;
    }

    else {
        for (size_t i = 0; i < 8; i++) {
            bit_writer.WriterBit(0);
        }
    }

    for (size_t i = 0; i < 32; i++) {
        bit_writer.WriterBit(encode_string_size[i]);

    }

    for (size_t i = 0; i < encode_string.size(); i++) {
        bit_writer.WriterBit(encode_string[i]);
    }

    for (size_t i = 0; i < encoded_length; i++) {
        bit_writer.WriterBit(encoded_text[i]);

    }

    size_t last_byte_size = (encoded_length + encode_string.size()) % 8;

    for (size_t i = 0; i < 8 - last_byte_size; i++) {
        bit_writer.WriterBit(0);
    }

    std::vector<byte> length_last_word = IntToByte(last_byte_size, 1);
    for (size_t i = 0; i < 8; i++) {
        bit_writer.WriterBit(length_last_word[i]);
    }

}

size_t ByteToInt(const std::vector<byte>& bytes) {
    size_t result = 0;
    for (size_t i = bytes.size(); i > 0; i--) {
        result += (bytes[bytes.size() - i] & 1) << (i - 1);
    }
    return result;
}


void RecoverHuffmanTree(Node* tree, const std::vector<byte>& encode_string) {
    static size_t iter = 0;

    if (encode_string[iter] == 0) {
        for (size_t i = 1; i < kMaxSize + 1; i++) {
            tree->value.push_back(encode_string[iter + i]);
        }
        iter = iter + kMaxSize + 1;
        return;
    }
    iter++;
    tree->left = new Node();
    tree->right = new Node();
    if (iter != encode_string.size()) {
        RecoverHuffmanTree(tree->left, encode_string);
    }
    if (iter != encode_string.size()) {
        RecoverHuffmanTree(tree->right, encode_string);
    }
}

void DecodeTable(std::unordered_map<std::vector<byte>, std::vector<byte>, VectorHasher>& encode_table, std::vector<byte> current_byte, Node* tree) {
    if (tree->left == nullptr && tree->right == nullptr) {
        if (current_byte.empty()) {
            current_byte.push_back(0);
        }
        encode_table[current_byte] = tree->value;
        return;
    }

    std::vector<byte> current_byte_right = current_byte;
    current_byte.push_back(0);
    DecodeTable(encode_table, current_byte, tree->left);
    current_byte_right.push_back(1);
    DecodeTable(encode_table, current_byte_right, tree->right);
}


// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original) {
    BitReader bits(compressed);
    byte bit = 0;
    BitWriter bit_writer(original);

    std::vector<byte> length_code_string_byte;

    for (size_t i = 0; i < 8; i++) {
        bits.ReadBit(bit);
    }

    if (bit == 1) {
        while (bits.ReadBit(bit)) {
            bit_writer.WriterBit(bit);
        }
        return;
    }

    for (size_t i = 0; i < 32; i++) {
        bits.ReadBit(bit);
        length_code_string_byte.push_back(bit);
    }

    size_t length_code_string = ByteToInt(length_code_string_byte);
    std::vector<byte> encode_string;
    for (size_t i = 0; i < length_code_string; i++) {
        bits.ReadBit(bit);
        encode_string.push_back(bit);
    }

    Node* tree = new Node;
    RecoverHuffmanTree(tree, encode_string);

    std::vector<byte> current_byte;
    std::unordered_map<std::vector<byte>, std::vector<byte>, VectorHasher> decode_table;
    DecodeTable(decode_table, current_byte, tree);
    DeleteTree(tree);

    std::vector<byte> buff;

    while (bits.ReadBit(bit))
    {
        buff.push_back(bit);
    }

    std::vector<byte> last_byte_len;
    for (size_t i = 0; i < 8; i++) {
        last_byte_len.push_back(buff[buff.size() - 8 + i]);
    }

    size_t length_last_byte = ByteToInt(last_byte_len);
    std::vector<byte> encoded_byte;

    
    for (size_t i = 0; i < buff.size() - (8 - length_last_byte) - 8; i++) {
        encoded_byte.push_back(buff[i]);
        if (decode_table.find(encoded_byte) != decode_table.end()) {
            encoded_byte = decode_table[encoded_byte];
            for (size_t k = 0; k < encoded_byte.size(); k++) {
                bit_writer.WriterBit(encoded_byte[k]);
            }
            encoded_byte.clear();
        }
    }
    
}





int main() {
    std::vector<byte> vec_output;
  //  vec_output.push_back(02);
   // vec_output.push_back(1);
  //  vec_output.push_back(2);

    setlocale(LC_ALL, "Russian");

    //vec_output.push_back(100);
    //vec_output.push_back(100);
    //vec_output.push_back(100);
    //vec_output.push_back(51);

    size_t size = 100;
    byte frgr;
    char cha;
    //for (int i = 0; i < size; i++) {
    //    std::cin.get(cha);
    //    frgr = static_cast<byte>(cha);
    //    vec_output.push_back(frgr);
    //} 

    VectorInputStream vi_(vec_output);
    std::vector<byte> vec_ou;
    VectorOutputStream va_(vec_ou);

    BitWriter write(va_);
    int d;
    for (int i = 0; i < 3385786; i++) {
     //   d = rand() % 2;
     //  write.WriterBit('a');

        vec_ou.push_back(static_cast<byte>(std::cin.get()));
    }

    VectorInputStream vi(vec_ou);
    VectorOutputStream va(vec_output);


    Encode(vi, va);


    VectorInputStream compvec(vec_output);

    std::vector<byte> vec;
    VectorOutputStream ad(vec);

    Decode(compvec, ad);

    //for (int i = 0; i < size; i++) {
    //    std::cout << vec[i];

    //}
    bool res = vec == vec_ou;
    std::cout << res;

}

