#include "common.h"

/*
    c++ 11 提供的特殊成员函数:

class Widget 
{
public:
    Widget();                           // 默认构造，无参数。如果声明了任意一个构造函数，或者编译器不再提供默认构造。
    ~Widget();                          // 默认析构，非虚的，默认noexcept。
    Widget(const Widget& rhs);          // 拷贝构造，逐一拷贝非static成员。
    Widget& operator=(Widget&& rhs);    // 拷贝赋值
    Widget(Widget&& rhs);               // 移动构造，默认的原理是“逐一移动成员变量”，这里的“移动”实际上是“移动请求”，如果没法移动就会拷贝
    Widget& operator=(Widget&& rhs);    // 移动赋值
}

    1、这些函数仅在代码中用到了时才会自动生成。
    2、Rule of Big Three：如果声明了拷贝构造、拷贝赋值、析构三者其一，那么也应该声明其余两个 ——— 这代表用户接管了对自身资源的管理。

关于默认移动构造和移动赋值运算符的规则：
    1、如果显式定义了Big Three其中之一，编译器都不会再生成默认的移动构造或者移动赋值。
    2、如果既没有显式定义移动，编译器也没有提供默认的，那么移动会转换为拷贝。因为移动实际上是请求移动，请求失败则构造。
    3、只有一个类没有显式定义Big Three、且每个非静态成员都可移动时，编译器才会生成默认的移动构造和移动赋值。
    4、如果显式声明移动操作，那么两个默认赋值不会再提供。

移动和赋值的不同：
    1、两个拷贝操作是独立的，显式声明其中一个不会限制编译器声明另一个。
    2、两个移动操作非独立，显式声明其中一个编译器不会再生成另一个，因为这代表了“逐一移动成员变量”不适用于这个类。


注：显式的声明拷贝或者移动操作，编译器的逻辑都会认为此类不适合“逐一拷贝/移动成员变量”，用户接管了对类资源的管理。

*/

// 声明了big three之后，编译器就不再生成移动操作了。
class TheBigThree
{
public:
    ~TheBigThree() = default;
    TheBigThree(const TheBigThree& rhs) = default;
    TheBigThree& operator=(const TheBigThree& rhs) = default;
};

// 多态基类的样式，big three是必须的
class Base
{
public:
    Base() = default;
    virtual ~Base() = default;
    Base(const Base&) = default;
    Base& operator=(const Base&) = default;
    // 如果需要有移动性，则使用default加上。
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;
};

class NoMove        // 声明了析构和拷贝，没有移动了
{
public:
    NoMove() { std::cout << "NoMove construct\n";};
    ~NoMove() { std::cout << "NoMove destruct\n";};
    NoMove(const NoMove& rhs) { std::cout << "NoMove copy construct\n";};
    NoMove &operator=(const NoMove& rhs) { 
        std::cout << "copy =\n";
        return *this;
    };
};

class Movable       // 声明了移动，不再有默认拷贝
{
public:
    Movable() { std::cout << "Movable construct\n"; };
    ~Movable() { std::cout << "Movable destruct\n";};
    Movable(Movable&& rhs) { std::cout << "Movable move construct\n";};
    Movable &operator=(Movable&& rhs) {
        std::cout << "move =\n";
        return *this;
    };
};

class Null
{
    // 一个空的类，自动生成了6个函数
};

int main()
{
    NoMove t1;                      // construct
    NoMove t2_1(t1);                // copy construct 
    NoMove t2_2(std::move(t1));     // copy construct, because without default move construct       
    NoMove t3 = t1;                 // transfer to copy construct
    NoMove t4;                      // construct
    t4 = t1;                        // copy=
    t4 = std::move(t1);             // copy=
    std::cout << "------\n";
    Movable mv1;                    // construct
    Movable mv2(std::move(mv1));    // move construct
    //Movable mv3(mv1);             // error, because without copy construct
    Movable mv3 = std::move(mv2);   // move construct
    Movable mv4;
    //mv4 = std::move(mv3);         // error, because without copy=
    mv4 = std::move(mv3);
    std::cout << "------\n";
    Null nl1;
    Null nl2(nl1);
    Null nl3(std::move(nl1));
    Null nl4;
    nl4 = nl2;
    Null nl5;
    nl5 = std::move(nl2); 
}