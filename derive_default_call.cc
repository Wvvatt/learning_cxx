#include <iostream>

class B
{
public:
    B() : base_int_(0){
        std::cout << "  B construct\n";  
    };
    ~B(){
        std::cout << "  B destruct\n";  
    }
    B(const B &rhs) {
        base_int_ = rhs.base_int_;
        std::cout << "  B copy construct\n";  
    }
    B& operator=(const B &rhs){
        base_int_ = rhs.base_int_;
        std::cout << "  B operator=\n";
        return *this;
    }

    void set_base_int(int rhs){
        base_int_ = rhs;
    }
    int base_int() const {
        return base_int_;
    };

private:
    int base_int_;
};

class DDefault : public B
{
public:
    void set_ddef_int(int rhs){
        ddef_int_ = rhs;
    };
    int ddef_int() const {
        return ddef_int_;
    }
private:
    int ddef_int_;
};

class D : public B
{
public:
    D(){
        derive_int_ = 0;
        std::cout << "  D construct\n";  
    }
    ~D(){
        std::cout << "  D destruct\n";  
    }
    D(const D &rhs) {
        std::cout << "  D copy construct\n";  
    }
    D& operator=(const D &rhs){
        derive_int_ = rhs.derive_int_;
        std::cout << "  D operator=\n";
        return *this;
    }

    void set_derive_int(int rhs){
        derive_int_ = rhs;
    }
    int derive_int() const {
        return derive_int_;
    }

private:
    int derive_int_;
};

int main()
{
    std::cout << "Default ------\n";
    {
        std::cout << "1) construct\n";
        DDefault d1;
        DDefault d2;
        std::cout << "2) copy construct\n";
        DDefault d3(d1);
        std::cout << "3) operator=\n";
        d2 = d1;
        std::cout << "4) destruct\n";
    }
    std::cout << "User Define ------\n";
    {
        std::cout << "1) construct\n";
        D d1;
        D d2;
        std::cout << "2) copy construct\n";
        D d3(d1);
        std::cout << "3) operator=\n";
        d2 = d1;
        std::cout << "4) destruct\n";
    }

    std::cout << "Default operator= test ------\n";
    {
        DDefault d1;
        d1.set_base_int(200);
        d1.set_ddef_int(300);
        DDefault d2;
        d2.set_base_int(0);
        d2.set_ddef_int(0);
        
        d2 = d1;

        std::cout << "after d2 = d1:\n";
        std::cout << "  d1.base_int() " << d1.base_int() << std::endl;
        std::cout << "  d1.ddef_int() " << d1.ddef_int() << std::endl;
        std::cout << "  d2.base_int() " << d2.base_int() << std::endl;
        std::cout << "  d2.ddef_int() " << d2.ddef_int() << std::endl;
    }

    std::cout << "User Define operator= test ------\n";
    {
        D d1;
        d1.set_base_int(200);
        d1.set_derive_int(300);
        D d2;
        d2.set_base_int(10);
        d2.set_derive_int(0);

        d2 = d1;

        std::cout << "after d2 = d1:\n";
        std::cout << "  d1.base_int() " << d1.base_int() << std::endl;
        std::cout << "  d1.derive_int() " << d1.derive_int() << std::endl;
        std::cout << "  d2.base_int() " << d2.base_int() << std::endl;
        std::cout << "  d2.derive_int() " << d2.derive_int() << std::endl;
    }

    return 0;
}