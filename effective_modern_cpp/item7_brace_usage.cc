#include "common.h"
#include <initializer_list>

/*
    使用花括号构造时，编译器会优先匹配initializer构造函数。
    只有无法将参数转换成initializer时，才使用非initializer构造函数
*/

class Window
{
public:
    Window(int a, int b) { std::cout << "using #1 cstor\n";};
    Window(std::initializer_list<int> il){ std::cout << "using #2 initializer cstor\n";};
};

class Widget
{
public:
    Widget(int i, bool b){ std::cout << "using #1 cstor\n";};
    Widget(int i, double d){ std::cout << "using #2 cstor\n";};
    Widget(std::initializer_list<std::string> il){ std::cout << "using #3 initializer cstor\n";};
};

struct Box
{
    int length;
    int width;
    double height;
    void print(){ std::cout << "length=" << length << " width=" << width << " height=" << height << std::endl;};
};

int main()
{
    Window win1(1,2);
    Window win2{1,2};       //using initializer cstor

    Widget w1(10, true);
    Widget w2{10, true};    // cannot convert to initializer
    Widget w3(10, 5.0);
    Widget w4{10, 5.0};     // cannot convert

    Box box{10, 30, 22.3};
    box.print();
}

/*
    花括号初始化可以：
        1、禁止变窄转换
        2、免疫c++最令人头疼的解析
        3、通用的初始化

    作为类库作者：
        1、initializer_list函数会直接盖过其他重载函数；
        2、给一个类添加一个initializer_list构造函数，可能会导致已有的{}构造变为不想要的情况；

    作为类库使用者：
        1、小括号初始化和花括号初始化只使用一个
        2、vector的()初始化和{}初始化具有不同的效果，前者是构造N个M值，后者是给定一个数组。
    
    写给自己：
        1、对类进行初始化，调用正确的构造函数，通常使用()；
        2、只有确定的知道调用{}初始化时类的行为是什么，才使用{}，比如vector；
        3、C语言中，{}作为初始化列表，可以对数组及结构体进行初始化；
        3、在类中及结构体中，可以使用{}对成员进行初始化（实际是一种声明）；
*/
