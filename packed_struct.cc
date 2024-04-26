#include <iostream>

struct MyStruct
{
    void Print()
    {
        std::cout << "MyStruct: a= " << (void *)&a << ", b= " << &b << ", c= " << &c << std::endl;
    }
    char a;
    int b;
    short c;
};

struct MyPackedStruct
{
    void Print()
    {
        std::cout << "MyPackedStruct: a= " << (void *)&a << ", b= " << &b << ", c= " << &c << std::endl;
    }
    char a;
    int b;
    short c;
} __attribute__((packed));


int main()
{   
    MyStruct my_s;
    MyPackedStruct my_ps;
    my_s.Print();
    my_ps.Print();
}