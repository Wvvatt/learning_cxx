#include <iostream>
#include <string>

/*
    decorator : 装饰器模式
    参与者：
        Component
        ——定义一个对象接口，可以给这些对象动态的添加职责。

        ConcreteComponent
        ——定义一个对象，可以给这个对象添加一些职责。

        Decorator
        ——维持一个指向Component对象的指针，并定义一个与Component接口一致的接口。

        ConcreteDecorator
        ——向组件添加职责。

    适合对一个类进行多层装饰，Decorator的作用是提供一个对具体对象进行装饰的方法实现，同时复用接口。
    为什么装饰器要继承基类？只有这样才能无限的进行装饰层的嵌套，不然装饰器只能传入待装饰对象，而无法传入已装饰对象。
    装饰器模式最大的特点就是可以无限装饰，自由组合。
*/


/*
    以下例子以RPG游戏中的玩家角色为例，可携带的装备数无限，能够自由组合
    角色类型：soldier, mage
    武器装备：sword, shield, spear
*/
class PlayerBase
{
public:
    virtual void play() = 0;
};

class Soldier : public PlayerBase
{
public:
    Soldier() : name_("soldier"){};
    virtual void play() override{
         std::cout << "\nI am a " << name_ << " ";
    };
private:
    std::string name_;
};

class Mage : public PlayerBase
{
public:
    Mage() : name_("mage"){};
    virtual void play() override{
         std::cout << "\nI am a " << name_ << " ";
    };
private:
    std::string name_;
};

class Decorator : public PlayerBase
{
public:
    Decorator(PlayerBase *player) : player_(player){};
    virtual void play() override {
        player_->play();
    };
    
private:
    PlayerBase *player_;
};

class Sword : public Decorator
{
public:
    Sword(PlayerBase *p) : Decorator(p){};
    virtual void play() override{
        Decorator::play();
        std::cout << "playing a sword\t";;
    };
};

class Shield : public Decorator
{
public:
    Shield(PlayerBase *p) : Decorator(p){};
    virtual void play() override{
        Decorator::play();
        std::cout << "playing a shield\t";
    };
};


int main()
{
    Soldier s;
    Mage m;
    Sword sword_s(&s);
    sword_s.play();
    Shield shield_s(&s);
    shield_s.play();
    Shield shield_sword_s(&sword_s);
    shield_sword_s.play();
    Shield shield_sword_m(&m);
    shield_sword_m.play();
};