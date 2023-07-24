#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstring>
using namespace std;

long fibonacci(unsigned n)
{
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}
 
static void print_ctime(const std::string &name, ::time_t time)
{
    std::cout << "--- " << name << " ---" << std::endl;
    std::cout << " | time_t     | " << time << std::endl;
    std::cout << " | std::ctime | " << std::ctime(&time);
    std::cout << " | localtime  | " <<  std::put_time(std::localtime(&time), "%c %Z") << std::endl;
    std::cout << " | UTC        | " <<  std::put_time(std::gmtime(&time), "%c %Z") << std::endl;
    std::cout << std::endl;
}

int main()
{
//system_clock，起点是unix时间戳的起点
    std::cout << "====== system clock ======\n";
    // 声明系统时间time_point
    const std::chrono::time_point<std::chrono::system_clock> epoch_sys{};       
    const auto p1 = std::chrono::system_clock::now();
    const auto p2 = p1 - std::chrono::hours(24);
    
    // 转换成c的time_t，只有system_clock有。epoch time既是unix时间戳开始的时间，1970-1-1 00:00:00 UTC。
    // std::ctime 转换出来的时间包含时区
    time_t epoch_time = std::chrono::system_clock::to_time_t(epoch_sys);
    print_ctime("epoch_time", epoch_time);

    std::time_t today_time = std::chrono::system_clock::to_time_t(p1);
    print_ctime("today", today_time);
    
    // time_since_epoch()获得一个duration，表示从epoch time开始的时长
    std::cout << "now() seconds since epoch : " << std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count() << " s\n";
    std::cout << "now() hours since epoch : " << std::chrono::duration_cast<std::chrono::hours>(p1.time_since_epoch()).count() << " h\n";
    std::cout << "now() yesterday, hours since epoch : " << std::chrono::duration_cast<std::chrono::hours>(p2.time_since_epoch()).count() << " h\n";

// steady_clock，起点一般是系统启动时间
    std::cout << "====== steady clock ======\n";
    const std::chrono::time_point<std::chrono::steady_clock> epoch_steady{};
    const auto start = std::chrono::steady_clock::now();
    std::cout << "steady epoch(boot time) : " << std::chrono::duration_cast<std::chrono::seconds>(epoch_steady.time_since_epoch()).count() << std::endl;;
    std::cout << "now() seconds since epoch : " << std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count() << std::endl;
     // 验证steady_time不会随着系统时间的变化而变化
    uint32_t count = 0;
    while(1){
        std::cout << "fibonacci(42) : " << fibonacci(42) << std::endl;
        auto end = std::chrono::steady_clock::now();
        std::cout << "fibonacci " << count++ << " cost time : " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " (seconds)\n";
    }
}