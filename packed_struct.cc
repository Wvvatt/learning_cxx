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

struct MyGnuPackedStruct
{
    void Print()
    {
        std::cout << "MyGnuPackedStruct: a= " << (void *)&a << ", b= " << &b << ", c= " << &c << std::endl;
    }
    char a;
    int b;
    short c;
} __attribute__((packed));

#pragma pack(1)
struct MyPragmaPackedStruct
{
    void Print()
    {
        std::cout << "MyPragmaPackedStruct: a= " << (void *)&a << ", b= " << &b << ", c= " << &c << std::endl;
    }
    char a;
    int b;
    short c;
};
#pragma pack()

int main()
{   
    MyStruct my_s;
    MyGnuPackedStruct my_gnu_ps;
    MyPragmaPackedStruct my_pragma_ps;
    my_s.Print();
    my_gnu_ps.Print();
    my_pragma_ps.Print();
}