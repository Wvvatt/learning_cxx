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
    std::cout << " raii-no-move construct\n";
}

RaiiNoMove::~RaiiNoMove()
{
    std::cout << " raii-no-move destruct\n";
    delete int_ptr_;
}

RaiiNoMove::RaiiNoMove(const RaiiNoMove &rhs)
{
    std::cout << " raii-no-move copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);
}

RaiiNoMove &RaiiNoMove::operator=(const RaiiNoMove &rhs)
{
    std::cout << " raii-no-move = copy\n";
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
    std::cout << " raii-with-move construct\n";
}

RaiiWithMove::~RaiiWithMove()
{
    std::cout << " raii-with-move destruct\n";
    delete int_ptr_;
}

RaiiWithMove::RaiiWithMove(const RaiiWithMove &rhs)
{
    std::cout << " raii-with-move copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);
}

RaiiWithMove &RaiiWithMove::operator=(const RaiiWithMove &rhs)
{
    std::cout << " raii-with-move = copy\n";
    int_ptr_ = new int(*rhs.int_ptr_);

    return *this;
}

RaiiWithMove::RaiiWithMove(RaiiWithMove &&rhs)
{
    std::cout << " raii-with-move move\n";
    int_ptr_ = rhs.int_ptr_;
    rhs.int_ptr_ = nullptr;
}

RaiiWithMove &RaiiWithMove::operator=(RaiiWithMove &&rhs)
{
    std::cout << " raii-with-move = move\n";
    int_ptr_ = rhs.int_ptr_;
    rhs.int_ptr_ = nullptr;

    return *this;
}

int main()
{
    std::queue<RaiiNoMove> q1;
    std::cout << "queue------ \n";
    std::cout << "raii no move : \n\n";
    q1.emplace();               // 元素是RaiiNoMove，只构造
    std::cout << "\n";
    q1.emplace(RaiiNoMove());   // 退化为push，先构造，再拷贝构造
    std::cout << "\n";
    q1.push(RaiiNoMove());      // 先构造，再拷贝构造
    std::cout << "\n";

    std::queue<RaiiWithMove> q2;
    std::cout << "raii with move : \n\n";
    q2.emplace();
    std::cout << "\n";
    q2.emplace(RaiiWithMove());
    std::cout << "\n";
    q2.push(RaiiWithMove());
    std::cout << "\n";

    std::unordered_map<int, RaiiNoMove> m1;
    std::cout << "map------ \n";
    std::cout << "raii no move : \n\n";
    m1.emplace();                       // 元素是pair，构造pair，此时调用RaiiNoMove的默认构造
    std::cout << "\n";
    m1.emplace(1, RaiiNoMove());        // 构造RaiiNoMove -> 构造pair(RaiiNoMove拷贝构造)
    std::cout << "\n";
    m1.emplace(std::make_pair(2, RaiiNoMove()));        // 构造RaiiNoMove -> 构造pair(RaiiNoMove拷贝构造) -> 拷贝pair(RaiiNoMove拷贝构造)
    std::cout << "\n";
    m1.insert(std::make_pair(3, RaiiNoMove()));         // 与上述一致
    std::cout << "\n";
    
    std::unordered_map<int, RaiiWithMove> m2;
    std::cout << "raii with move : \n\n";
    m1.emplace();
    std::cout << "\n";
    m2.emplace(1, RaiiWithMove());
    std::cout << "\n";
    m2.emplace(std::make_pair(2, RaiiWithMove()));
    std::cout << "\n";
    m2.insert(std::make_pair(3, RaiiWithMove()));
    std::cout << "\n";
}