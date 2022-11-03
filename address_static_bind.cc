#include <iostream>
#include <string>

class Gamer
{
public:
    void Play();
private:
    int game_;
};

void Gamer::Play()
{
    std::cout << "I can play " << game_ << std::endl;
}

void on_handle_something(void *hdl)
{   
    Gamer *gamer = (Gamer *)hdl;
    gamer->Play();              // 静态绑定，也称作地址早绑定，在编译时就知道调用哪个函数
                                // 此处调用Gamer的Play()，其内存分布只占一个int，因此把int强转后，game_的位置也是100
}

int main()
{
    int *hacker = new int(100);
    on_handle_something(hacker);
}