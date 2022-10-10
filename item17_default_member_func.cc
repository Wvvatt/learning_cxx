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
    2、2个拷贝操作是独立的，显式声明其中一个不会限制编译器声明另一个。
    3、如果显式声明了某个移动函数，编译器不再生成另一个。因为这代表了“逐一移动成员变量”不适用于这个类。
    4、显式声明拷贝操作使编译器不会生成移动操作；显式声明移动操作使编译器不会生成拷贝操作。
    5、Rule of Three：如果声明了拷贝构造、拷贝赋值、析构三者其一，那么也应该声明其余两个 ——— 这代表用户接管了对自身资源的管理。
    6、显式声明析构，编译器不再生成移动操作。

注：显式的声明拷贝或者移动操作，编译器的逻辑都会认为此类不适合“逐一拷贝/移动成员变量”。

*/

// 声明了big three之后，编译器就不再生成移动操作了。
class TheBigThree
{
public:
    ~TheBigThree();
    TheBigThree(const TheBigThree& rhs);
    TheBigThree& operator=(const TheBigThree& rhs);
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
    NoMove() {};
    ~NoMove() { std::cout << "NoMove destruct\n";};
    NoMove(const NoMove& rhs) { std::cout << "copy construct\n";};
};

class Movable       // 声明了移动，不再有默认拷贝
{
public:
    Movable() {};
    ~Movable() { std::cout << "Movable destruct\n";};
    Movable(Movable&& rhs) { std::cout << "move construct\n";};
};

class Null
{
    // 一个空的类，自动生成了6个函数
};

int main()
{
    NoMove t1;
    NoMove t2(std::move(t1));       // copy construct 
    NoMove t3;              
    t3 = t1;            // copy=
    t3 = std::move(t1); // copy=

    Movable mv1;
    Movable mv2(std::move(mv1));    // move construct
    //Movable mv3(mv1); // error

    Null nl1;
    Null nl2(nl1);
    Null nl3(std::move(nl1));
    Null nl4;
    nl4 = nl2;
    Null nl5;
    nl5 = std::move(nl2); 
}