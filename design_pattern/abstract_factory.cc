#include <iostream>

/*
    抽象工厂：
    - 增加产品族中的产品品类很困难
    - 产品品类不变，增加一条新的产品线（品牌），很简单
*/

class Phone{
public:
    virtual ~Phone() = default;
    virtual void Run() = 0;
};

class Iphone : public Phone
{
public:
    void Run() override {
        std::cout << "welcome to iphone\n";
    }
};

class Hphone : public Phone 
{
public:
    void Run() override {
        std::cout << "welcome to hphone\n";
    }
};

class Watch
{
public:
    virtual ~Watch() = default;
    virtual void Time() = 0;
};

class Iwatch : public Watch
{
public:
    void Time() override {
        std::cout << "it's America time\n";
    }
};

class Hwatch : public Watch
{
public:
    void Time() override {
        std::cout << "it's China time\n";
    }
};

class Factory
{
public:
    virtual ~Factory() = default;
    virtual Phone *CreatePhone() = 0;
    virtual Watch *CreateWatch() = 0;
};

class AppleFactory : public Factory
{
public: 
    Phone *CreatePhone() override {
        return new Iphone();
    }
    Watch *CreateWatch() override {
        return new Iwatch();
    }
};

class HuaweiFactory : public Factory
{
public: 
    Phone *CreatePhone() override {
        return new Hphone();
    }
    Watch *CreateWatch() override {
        return new Hwatch();
    }
};

void test(Factory *factory)
{
    Phone *p = factory->CreatePhone();
    Watch *w = factory->CreateWatch();

    p->Run();
    w->Time();

    delete p;
    delete w;
}

int main()
{
    std::cout << "----- apple factory -----\n";
    Factory *apple = new AppleFactory();
    test(apple);
    delete apple;

    std::cout << "----- hw factory -----\n";
    Factory *huawei = new HuaweiFactory();
    test(huawei);
    delete huawei;
}