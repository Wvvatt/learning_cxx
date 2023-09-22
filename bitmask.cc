#include <iostream>

#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>

typedef int OpenMode;

static constexpr OpenMode kIn = (OpenMode)0x01;
static constexpr OpenMode kOut = (OpenMode)0x02;
static constexpr OpenMode kAte = (OpenMode)0x04;
static constexpr OpenMode kApp = (OpenMode)0x08;
static constexpr OpenMode kOpenMode = (OpenMode)0x1f;

static void test_openmode()
{
    OpenMode mode = kIn;

    mode |= kApp;       // set value
    mode &= ~kIn;       // clear value
    if(mode & kApp){
        std::cout << "mode set kApp\n";
    }
    if(mode & kIn){
        std::cout << "mode set kIn\n";
    }
}

enum IOState : int
{
    kBegin = 0x01,
    kEof = 0x02,
    kGood = 0x04,
    kError = 0x08,
    kIOState = 0x1f
};

static void test_iostate()
{
    int state = kBegin;

    state |= kGood;       // set value
    state &= ~kBegin;       // clear value
    if(state & kGood){
        std::cout << "mode set kGood\n";
    }
    if(state & kBegin){
        std::cout << "mode set kBegin\n";
    }
}


static void test_bitset()
{
    typedef std::size_t length_t, position_t; // the hints
 
    // constructors:
    constexpr std::bitset<4> b1;
    constexpr std::bitset<4> b2{0xA}; // == 0B1010
    std::bitset<4> b3{"0011"}; // can also be constexpr since C++23
    std::bitset<8> b4{"ABBA", length_t(4), /*0:*/'A', /*1:*/'B'}; // == 0B0000'0110
 
    // bitsets can be printed out to a stream:
    std::cout << "b1:" << b1 << "; b2:" << b2 << "; b3:" << b3 << "; b4:" << b4 << '\n';
 
    // bitset supports bitwise operations:
    b3 |= 0b0100; assert(b3 == 0b0111);
    b3 &= 0b0011; assert(b3 == 0b0011);
    b3 ^= std::bitset<4>{0b1100}; assert(b3 == 0b1111);
 
    // operations on the whole set:
    b3.reset(); assert(b3 == 0);
    b3.set(); assert(b3 == 0b1111);
    assert(b3.all() && b3.any() && !b3.none());
    b3.flip(); assert(b3 == 0);
 
    // operations on individual bits:
    b3.set(position_t(1), true); assert(b3 == 0b0010);
    b3.set(position_t(1), false); assert(b3 == 0);
    b3.flip(position_t(2)); assert(b3 == 0b0100);
    b3.reset(position_t(2)); assert(b3 == 0);
 
    // subscript operator[] is supported:
    b3[2] = true; assert(true == b3[2]);
 
    // other operations:
    assert(b3.count() == 1);
    assert(b3.size() == 4);
    assert(b3.to_ullong() == 0b0100ULL);
    assert(b3.to_string() == "0100");
}

int main()
{
    std::cout << "--- 1) test typedef\n";
    test_openmode();
    std::cout << "--- 2) test enum int\n";
    test_iostate();
    std::cout << "--- 3) test cppreference example bitset\n";
    test_bitset();
}