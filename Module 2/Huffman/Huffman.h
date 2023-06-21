#pragma once
#define interface struct
typedef unsigned char byte;

interface IInputStream {
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

class VectorInputStream : public IInputStream {
public:
    VectorInputStream(const std::vector<byte>& input) : buf(input), pos(0) {
    };

    bool Read(byte& value) override {
        if (pos >= buf.size()) {
            return false;
        }
        value = buf[pos++];
        return true;
    }
private:
    const std::vector<byte>& buf;
    size_t pos;
};


class VectorOutputStream : public IOutputStream {
public:
    VectorOutputStream(std::vector<byte>& input) : buf(input) {
    }

    void Write(byte value) override {
        buf.push_back(value);
    }

private:
    std::vector<byte>& buf;

};