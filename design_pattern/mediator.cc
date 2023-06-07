#include <iostream>
#include <string>
#include <vector>
#include <memory>

/*
    中介者模式（调停者模式）

    中介者（Mediator）模式的定义：
    定义一个中介对象来封装一系列对象之间的交互，使原有对象之间的耦合松散，且可以独立地改变它们之间的交互。中介者模式又叫调停模式，它是迪米特法则的典型应用。
    
    中介者模式是一种对象行为型模式。

    其主要优点是：
    - 类之间各司其职，符合迪米特法则。
    - 降低了对象之间的耦合性，使得对象易于独立地被复用。
    - 将对象间的一对多关联转变为一对一的关联，提高系统的灵活性，使得系统易于维护和扩展。
    
    其主要缺点是：
    - 中介者模式将原本多个对象直接的相互依赖变成了中介者和多个同事类的依赖关系。当同事类越多时，中介者就会越臃肿，变得复杂且难以维护。

    结构：
    - 抽象中介者（Mediator）角色：它是中介者的接口，提供了同事对象注册与转发同事对象信息的抽象方法。
    - 具体中介者（Concrete Mediator）角色：实现中介者接口，定义一个 List 来管理同事对象，协调各个同事角色之间的交互关系，因此它依赖于同事角色。
    - 抽象同事类（Colleague）角色：定义同事类的接口，保存中介者对象，提供同事对象交互的抽象方法，实现所有相互影响的同事类的公共功能。
    - 具体同事类（Concrete Colleague）角色：是抽象同事类的实现者，当需要与其他同事对象交互时，由中介者对象负责后续的交互。
*/
class Colleague;
class Mediator{
public:
    virtual void Register(Colleague *colleague) = 0;
    virtual void Relay(Colleague *, const std::string &message) = 0;
};

class Colleague{
public:
    Colleague(const std::string &name) : name_(name){};
    void SetMediator(Mediator *mediator){
        mediator_ = mediator;
    };

    virtual void Send() = 0;
    virtual void Receive(const std::string &message) = 0;

    const std::string &name() const{
        return name_;
    }
protected:
    Mediator *mediator_;
    std::string name_;
};

class ConcreteMediator : public Mediator
{
public:
    void Register(Colleague * colleague) override{
        colleagues_.push_back(colleague);
        colleague->SetMediator(this);
    };
    void Relay(Colleague *sender, const std::string &message) override{
        for(auto c : colleagues_)
        {
            // 传给其他同事
            if(c->name() != sender->name()){
                c->Receive(message);
            }
        }
    };
private:
    std::vector<Colleague *> colleagues_;
};

class ConcreteColleague : public Colleague
{
public:
    ConcreteColleague(const std::string &name) : Colleague(name){};
    void Send() override{
        std::string message;
        message.append(name_).append(" is calling you");
        mediator_->Relay((Colleague *)this, message);
    }
    void Receive(const std::string &message) override{
        std::cout << name_ << " receive a message : [ " << message << " ]" << std::endl;
    }
};

int main()
{
    ConcreteMediator mediator;
    ConcreteColleague c1("watt");
    ConcreteColleague c2("dennis ritchie");
    ConcreteColleague c3("ken thompson");

    mediator.Register(&c1);
    mediator.Register(&c2);
    mediator.Register(&c3);

    c1.Send();
    c2.Send();
    c3.Send();
}