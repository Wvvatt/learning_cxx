#include <iostream>

class Base
{
public:
    Base() = default;
    virtual ~Base(){
        std::cout << "Base Destruct\n";
        Deinit();
    };

    virtual bool Init() {
        std::cout << "Base Init\n";
        return true;
    };

    virtual void Deinit();
};

void Base::Deinit()
{
    std::cout << "Base Deinit\n";
}

class Son : public Base
{
public:
    Son() = default;
    ~Son() override {
        std::cout << "Son Destruct\n";
        Deinit();
    };

    bool Init() override {
        std::cout << "Son Init\n";
        return true;
    };

    void Deinit() override{
        std::cout << "Son Deinit\n";
    };
};

int main()
{
    Base *p = new Son();
    p->Init();
    p->Deinit();
    std::cout << "base deinit ptr=" << &Son::Deinit << std::endl;
    delete p;
}