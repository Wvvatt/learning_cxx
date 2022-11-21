#include "includes/common.h"

struct Interface
{
    int(*eat)();
    int(*run)();
};

struct Parent
{
    Interface *funcs_ptr;       // 可视为 虚函数指针
};

int parent_eat()
{
    std::cout << "parent eat\n";
    return 1;
}

int parent_run()
{
    std::cout << "parent run\n"; 
    return 1;
}

struct Child
{
    Interface *funcs_ptr;
};

int child_eat()
{
    std::cout << "child eat\n";
    return 1;
}

// 2张虚函数表，子类重写了父类的eat()方法，继承了run()方法
static Interface parent_funcs = {parent_eat, parent_run};
static Interface child_funcs = {child_eat, parent_run};

int main()
{
    Parent *p = new Parent{&parent_funcs};
    p->funcs_ptr->eat();
    p->funcs_ptr->run();
    delete p;

    // Parent *p = new Child;
    Child *c = new Child{&child_funcs};
    p = (Parent *)c;
    
    p->funcs_ptr->eat();
    p->funcs_ptr->run();
    delete p;
}