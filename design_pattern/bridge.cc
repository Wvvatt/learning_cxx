#include <iostream>
#include <memory>

class Taste
{
public:
    virtual void Add() = 0; // 咖啡味道的抽象
};

class Milk : public Taste
{
public:
    void Add() final
    {
        std::cout << "milk\n";
    }
};

class Suger : public Taste
{
public:
    void Add() final
    {
        std::cout << "suger\n";
    }
};

class Coffee
{
public:
    explicit Coffee(std::shared_ptr<Taste> taste);
    void GetCupOfCoffee(); // 将两个继承体系桥接
protected:
    virtual void Capacity() = 0; // 咖啡容量的抽象
private:
    std::shared_ptr<Taste> taste_;
};

Coffee::Coffee(std::shared_ptr<Taste> taste):
    taste_(taste)
{

}

void Coffee::GetCupOfCoffee()
{
    Capacity();
    taste_->Add();
}

class BigCup : public Coffee
{
public:
    explicit BigCup(std::shared_ptr<Taste> taste);
protected:
    void Capacity() final;
};

BigCup::BigCup(std::shared_ptr<Taste> taste):
    Coffee(taste)
{

}

void BigCup::Capacity()
{
    std::cout << "get a big cup of coffee with ";
}

class SmallCup : public Coffee
{
public:
    explicit SmallCup(std::shared_ptr<Taste> taste);
protected:
    void Capacity() final;
};

SmallCup::SmallCup(std::shared_ptr<Taste> taste):
    Coffee(taste)
{

}

void SmallCup::Capacity()
{
    std::cout << "get a small cup of coffee with ";
}

int main()
{
    std::shared_ptr<Coffee> big_milk = std::make_shared<BigCup>(std::make_shared<Milk>());
    std::shared_ptr<Coffee> big_suger = std::make_shared<BigCup>(std::make_shared<Suger>());
    
    std::shared_ptr<Coffee> small_milk = std::make_shared<SmallCup>(std::make_shared<Milk>());
    std::shared_ptr<Coffee> small_suger = std::make_shared<SmallCup>(std::make_shared<Suger>());

    big_milk->GetCupOfCoffee();
    big_suger->GetCupOfCoffee();
    small_milk->GetCupOfCoffee();
    small_suger->GetCupOfCoffee();
}