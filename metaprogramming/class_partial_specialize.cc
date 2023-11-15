#include <iostream>
#include <type_traits>
#include <vector>

// 主模板（通用情况）
template <typename T, typename = void>
class MyClass {
public:
    void print() {
        std::cout << "Generic Template: " << typeid(T).name() << std::endl;
    }
};

// 对自定义类进行通用偏特化
template <typename T>
class MyClass<T, typename std::enable_if<std::is_class<T>::value>::type> {
public:
    void print() {
        std::cout << "Specialized Template for Custom Class: " << typeid(T).name() << std::endl;
    }
};
// 自定义类
class MyCustomClassA {};
class MyCustomClassB {};
class SpecialClass {};

// 对特定类进行特殊处理
template <>
class MyClass<SpecialClass> {
public:
    void print() {
        std::cout << "Specialized Template for SpecialClass" << std::endl;
    }
};

int main() {
    MyClass<double> obj1;
    obj1.print(); // 使用主模板

    MyClass<int> obj2;
    obj2.print(); // 使用主模板

    MyClass<MyCustomClassA> obj3;
    obj3.print(); // 使用对自定义类的通用偏特化模板

    MyClass<MyCustomClassB> obj4;
    obj4.print(); // 使用对自定义类的通用偏特化模板

    MyClass<SpecialClass> obj5;
    obj5.print(); // 使用对特定类的特殊处理模板

    return 0;
}