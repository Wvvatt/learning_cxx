#include <iostream>
#include <string>

/*
    外观模式：

    为子系统中的一组接口提供一个统一的入口。外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。
    简化客户端与子系统之间的交互，方便客户端使用，降低系统耦合度。

    结构：
    - facade（外观角色）：外观角色可以知道多个相关子系统的功能，它将所有从客户端发来的请求委派给相应的子系统，传递给相应的子系统处理。
    - subsystem（子系统角色）：子系统是一个类，或者由多个类组成的类的集合，它实现子系统具体的功能。
*/

class SubSystem1
{
public:
    void method1(){
        std::cout << "sub system 1 method 1\n";  
    }
    void method2(){
        std::cout << "sub system 1 method 2\n";  
    }
};

class SubSystem2
{
public:
    void method1(){
        std::cout << "sub system 2 method 1\n";  
    }
    void method2(){
        std::cout << "sub system 2 method 2\n";  
    }
};

class Facade 
{
public:
    void facade_method_1()
    {
        std::cout << "---- facade method 1 ----\n";
        s1_.method1();
        s1_.method2();
        s2_.method2();
    }

    void facade_method_2()
    {
        std::cout << "---- facade method 2 ----\n";
        s2_.method1();
    }
private:
    SubSystem1 s1_;
    SubSystem1 s2_;
};

int main()
{
    Facade f;
    f.facade_method_1();
    f.facade_method_2();
}