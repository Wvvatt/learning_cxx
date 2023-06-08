#include <iostream>

class Product
{
public:
    virtual ~Product() = default;
    virtual void Name() = 0; 
};

class Shoe : public Product
{
public:
    void Name() override {
        std::cout << "This is a shoe\n";
    }
};

class Shirt : public Product
{
public:
    void Name() override {
        std::cout << "This is a shirt\n";
    }
};

class Factory
{
public:
    virtual Product *Create() = 0;
};

class ShoeFactory : public Factory
{
public:
    Product *Create(){
        return new Shoe;
    }
};

class ShirtFactory : public Factory
{
public:
    Product *Create(){
        return new Shirt;
    }
};

int main()
{
    ShoeFactory shoe_factory;
    ShirtFactory shirt_factory;

    Product *shoe = shoe_factory.Create();
    Product *shirt = shirt_factory.Create();

    shoe->Name();
    shirt->Name();

    delete shoe;
    delete shirt;
}