#include <memory>
#include <unistd.h>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <iostream>

template <typename T>
auto RawPtr(T &&ptr) -> decltype(std::addressof(*ptr))
{
    // ptr is a forwarding reference to support Ts with non-const operators.
    return (ptr != nullptr) ? std::addressof(*ptr) : nullptr;
}
inline std::nullptr_t RawPtr(std::nullptr_t) { return nullptr; }

class Block
{
public:
    explicit Block(size_t size = 0);
    ~Block() = default;
    // shared
    Block(const Block &) = default;
    Block &operator=(const Block &) = default;
    // move
    Block(Block &&) = default;
    Block &operator=(Block &&) = default;
    // copy
    Block DeepCopy() const;

    const uint8_t *DataConst() const;
    size_t Size() const { return buf_ ? buf_->size() : 0; };

    void Write(const uint8_t *src, size_t len, size_t pos = 0);
    size_t Read(uint8_t *dst, size_t len, size_t pos = 0) const;

private:
    uint8_t *Data();
    std::shared_ptr<std::vector<uint8_t>> buf_;
};

Block::Block(size_t size)
{
    buf_ = std::make_shared<std::vector<uint8_t>>(size, 0);
}

Block Block::DeepCopy() const
{
    Block b;
    b.buf_ = std::make_shared<std::vector<uint8_t>>(Size());
    std::memcpy(b.Data(), DataConst(), Size());

    return b;
}

void Block::Write(const uint8_t *src, size_t len, size_t pos)
{
    if(!src){
        return;
    }
    buf_->resize(len + pos);
    std::memcpy(Data(), src, len);
}

size_t Block::Read(uint8_t *dst, size_t len, size_t pos) const
{
    if(pos >= Size()){
        return 0;
    }
    size_t readlen = (pos + len > Size()) ? Size() - pos : len;
    std::memcpy(dst, DataConst() + pos, readlen);

    return readlen;
}

const uint8_t *Block::DataConst() const
{
    return RawPtr(buf_) == nullptr ? nullptr : &((*buf_)[0]);
}

uint8_t *Block::Data()
{
    return RawPtr(buf_) == nullptr ? nullptr : &((*buf_)[0]);
}

void PrintBuf(const Block &b){
    std::cout << b.Size() << "    " << (void *)b.DataConst() << std::endl;
}

int main()
{
    std::vector<uint8_t> tmp;
    tmp.resize(1000, 0xff);
    Block buf1, buf2, buf3;
    buf1.Write(&tmp[0], 1000);

    PrintBuf(buf1);
    PrintBuf(buf2);
    PrintBuf(buf3);

    std::cout << "-------- shared" << std::endl;

    buf2 = buf1;

    PrintBuf(buf1);
    PrintBuf(buf2);
    PrintBuf(buf3);
    
    std::cout << "-------- move" << std::endl;

    buf3 = std::move(buf1);

    PrintBuf(buf1);
    PrintBuf(buf2);
    PrintBuf(buf3);
}