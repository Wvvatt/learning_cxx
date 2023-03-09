#include <queue>
#include <iostream>
#include <unordered_map>

/*
    构建RAII类来管理资源，需要注意深浅拷贝的问题，不然容易引发double free.
*/

class RaiiNoMove
{
public:
    RaiiNoMove();
    ~RaiiNoMove();
    RaiiNoMove(const RaiiNoMove &rhs);
    RaiiNoMove &operator=(const RaiiNoMove &rhs);
private:
    int *int_ptr_;
};

RaiiNoMove::RaiiNoMove():
    int_ptr_(new int(0))
{
    std::cout << "raii construct\n";
}

RaiiNoMove::~RaiiNoMove()
{
    delete int_ptr_;
}

RaiiNoMove::RaiiNoMove(const RaiiNoMove &rhs)
{
    std::cout << "raii copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);
}

RaiiNoMove &RaiiNoMove::operator=(const RaiiNoMove &rhs)
{
    std::cout << "raii = copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);

    return *this;
}

class RaiiWithMove
{
public:
    RaiiWithMove();
    ~RaiiWithMove();
    RaiiWithMove(const RaiiWithMove &rhs);
    RaiiWithMove &operator=(const RaiiWithMove &rhs);
    RaiiWithMove(RaiiWithMove &&rhs);
    RaiiWithMove &operator=(RaiiWithMove &&rhs);
private:
    int *int_ptr_;
};

RaiiWithMove::RaiiWithMove():
    int_ptr_(new int(0))
{
    std::cout << "raii construct\n";
}

RaiiWithMove::~RaiiWithMove()
{
    delete int_ptr_;
}

RaiiWithMove::RaiiWithMove(const RaiiWithMove &rhs)
{
    std::cout << "raii copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);
}

RaiiWithMove &RaiiWithMove::operator=(const RaiiWithMove &rhs)
{
    std::cout << "raii = copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);

    return *this;
}

RaiiWithMove::RaiiWithMove(RaiiWithMove &&rhs)
{
    std::cout << "raii move\n";
    int_ptr_ = rhs.int_ptr_;
    rhs.int_ptr_ = nullptr;
}

RaiiWithMove &RaiiWithMove::operator=(RaiiWithMove &&rhs)
{
    std::cout << "raii = move\n";
    int_ptr_ = rhs.int_ptr_;
    rhs.int_ptr_ = nullptr;

    return *this;
}

int main()
{
    std::queue<RaiiNoMove> q1;
    std::cout << "queue------ \n";
    std::cout << "raii no move : \n";
    q1.emplace();
    q1.emplace(RaiiNoMove());
    q1.push(RaiiNoMove());

    std::queue<RaiiWithMove> q2;
    std::cout << "raii with move : \n";
    q2.emplace();
    q2.emplace(RaiiWithMove());
    q2.push(RaiiWithMove());

    std::unordered_map<int, RaiiNoMove> m1;
    std::cout << "map------ \n";
    std::cout << "raii no move : \n";
    m1.emplace(1, RaiiNoMove());
    m1.emplace(std::make_pair(1, RaiiNoMove()));
    m1.insert(std::make_pair(1, RaiiNoMove()));
    
    std::unordered_map<int, RaiiWithMove> m2;
    std::cout << "raii with move : \n";
    m2.emplace(1, RaiiWithMove());
    m2.emplace(std::make_pair(1, RaiiWithMove()));
    m2.insert(std::make_pair(1, RaiiWithMove()));
}