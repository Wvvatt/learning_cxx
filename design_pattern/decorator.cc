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

    装饰器的特点：拥有一个基类成员，同时自己也继承自基类。有点类似于积木，自己有一个榫，有一个卯，可以插在一起。
            __________                   __________
            |        |                   |        |
            |__      |__                 |__      |__
            __|      __|       --->      __|      __|
            |        |                   |        |
            |________|                   |________|
    装饰器模式最大的特点就是可以无限装饰，自由组合。
    装饰器虽然比继承灵活，但也有一个很大的缺陷，就是装饰后的对象无法继承component中不存在的接口，比如ConcreteComponent中独有的接口。所有的装饰对象都必须有同样的接口。
*/


/*
    以下例子是ffmpeg中URLProtocol结构的变体。
    传输层协议tcp、udp是基底，通过装饰可以完成最上层协议的功能。
*/
static void print_split(const std::string &name)
{
    std::cout << "--------------\n";
    std::cout << "---  " << name << "  ---\n";
    std::cout << "--------------\n";
}

class UrlProtocol
{
public:
    virtual ~UrlProtocol() = default;
    virtual void url_open() = 0;
    virtual void url_read() = 0;
    virtual void url_close() = 0;
};

class TcpProtocol : public UrlProtocol
{
public:
    void url_open() override {
        std::cout << "tcp open\n";
    }
    void url_read() override {
        std::cout << "tcp read\n";
    }
    void url_close() override{
        std::cout << "tcp close\n";
    }
};

class UdpProtocol : public UrlProtocol
{
public:
    void url_open() override {
        std::cout << "udp open\n";
    }
    void url_read() override {
        std::cout << "udp read\n";
    }
    void url_close() override{
        std::cout << "udp close\n";
    }
};

// 协议装饰器
class ProtDecorator : public UrlProtocol
{
public:
    ProtDecorator(UrlProtocol *component) : component_(component){}
    ~ProtDecorator(){
        delete component_;
    }
    void url_open() override {
        component_->url_open();
    }
    void url_read() override {
        component_->url_read();
    }
    void url_close() override {
        component_->url_close();
    }
private:
    UrlProtocol *component_;    
};  

class RtmpProtocol : public ProtDecorator
{
public:
    RtmpProtocol(UrlProtocol *component) : ProtDecorator(component){};
    void url_open() override {
        ProtDecorator::url_open();
        std::cout << "rtmp open\n";
    }
    void url_read() override {
        ProtDecorator::url_read();
        std::cout << "rtmp read\n";
    }
    void url_close() override{
        ProtDecorator::url_close();
        std::cout << "rtmp close\n";
    }
private:
    std::string name_;      
};

class HttpProtocol : public ProtDecorator
{
public:
    HttpProtocol(UrlProtocol *component) : ProtDecorator(component){};
    void url_open() override {
        ProtDecorator::url_open();
        std::cout << "http open\n";
    }
    void url_read() override {
        ProtDecorator::url_read();
        std::cout << "http read\n";
    }
    void url_close() override{
        ProtDecorator::url_close();
        std::cout << "http close\n";
    }
private:
    std::string name_;      
};

class RtpProtocol : public ProtDecorator
{
public:
    RtpProtocol(UrlProtocol *component) : ProtDecorator(component){};
    void url_open() override {
        ProtDecorator::url_open();
        std::cout << "rtp open\n";
    }
    void url_read() override {
        ProtDecorator::url_read();
        std::cout << "rtp read\n";
    }
    void url_close() override{
        ProtDecorator::url_close();
        std::cout << "rtp close\n";
    }
private:
    std::string name_;      
};

class RistProtocol : public ProtDecorator
{
public:
    RistProtocol(UrlProtocol *component) : ProtDecorator(component){};
    void url_open() override {
        ProtDecorator::url_open();
        std::cout << "rist open\n";
    }
    void url_read() override {
        ProtDecorator::url_read();
        std::cout << "rist read\n";
    }
    void url_close() override{
        ProtDecorator::url_close();
        std::cout << "rist close\n";
    }
private:
    std::string name_;      
};

int main()
{   
    UrlProtocol *p = nullptr;
    print_split("rtmp");
    p = new RtmpProtocol(new TcpProtocol());
    p->url_open();
    p->url_read();
    p->url_close(); 

    print_split("http");
    p = new HttpProtocol(new TcpProtocol());
    p->url_open();
    p->url_read();
    p->url_close();
    
    print_split("rist");
    p = new RistProtocol(new RtpProtocol(new TcpProtocol()));
    p->url_open();
    p->url_read();
    p->url_close();
}   