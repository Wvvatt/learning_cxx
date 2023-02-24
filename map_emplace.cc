#include <map>
#include <iostream>
#include <stdio.h>
#include <string>

class CopyObj
{
public:
    CopyObj(int first, int second);
    ~CopyObj() = default;
    CopyObj(const CopyObj &rhs);
    CopyObj(CopyObj &&rhs) = delete;
    CopyObj &operator=(const CopyObj &rhs) = delete;
    CopyObj &operator=(CopyObj &&rhs) = delete;

    std::string print();
private:
    int first_;
    int second_;
};

CopyObj::CopyObj(int first, int second):
    first_(first),
    second_(second)
{

}

CopyObj::CopyObj(const CopyObj &rhs)
{
    printf("copy construct\n");
    first_ = rhs.first_;
    second_ = rhs.second_;
}

std::string CopyObj::print()
{
    char s[128];
    sprintf(s, "{ %d %d }", first_, second_);
    std::string str = s;
    return str;
}

class MoveObj
{
public:
    MoveObj(int first, int second);
    ~MoveObj() = default;
    MoveObj(const MoveObj &rhs);
    MoveObj(MoveObj &&rhs);
    MoveObj &operator=(const MoveObj &rhs) = delete;
    MoveObj &operator=(MoveObj &&rhs) = delete;

    std::string print();
private:
    int first_;
    int second_;
};

MoveObj::MoveObj(int first, int second):
    first_(first),
    second_(second)
{

}

MoveObj::MoveObj(const MoveObj &rhs)
{
    printf("copy construct\n");
    first_ = rhs.first_;
    second_ = rhs.second_;
}

MoveObj::MoveObj(MoveObj &&rhs)
{
    printf("move construct\n");
    first_ = rhs.first_;
    second_ = rhs.second_;
}

std::string MoveObj::print()
{
    char s[128];
    sprintf(s, "{ %d %d }", first_, second_);
    std::string str = s;
    return str;
}

int main()
{
    std::map<int, CopyObj> m_c;
    m_c.emplace(std::make_pair(1, CopyObj(10,20)));         // 没有move construct，只能拷贝
    for(auto iter = m_c.begin(); iter != m_c.end(); iter++){
        printf("index = %d, obj = %s\n", iter->first, iter->second.print().c_str());
    }

    std::map<int, MoveObj> m_m;
    m_m.emplace(std::make_pair(2, MoveObj(100,200)));       // 如果有move construct，会优先使用
    for(auto iter = m_m.begin(); iter != m_m.end(); iter++){
        printf("index = %d, obj = %s\n", iter->first, iter->second.print().c_str());
    }
}