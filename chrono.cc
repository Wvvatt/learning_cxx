#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    // time_since_epoch()获得一个duration，表示unix时间戳，即从1970-1-1 00:00:00到现在的时间
    const auto p0 = std::chrono::time_point<std::chrono::system_clock>{};       
    const auto p1 = std::chrono::system_clock::now();
    const auto p2 = p1 - std::chrono::hours(24);
    
    // 转换成c的time_t
    std::time_t epoch_time = std::chrono::system_clock::to_time_t(p0);
    std::cout << "epoch(unix时间戳开始的时间): " << std::ctime(&epoch_time);
    std::time_t today_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "today(time_t为" << today_time << "): " << std::ctime(&today_time);
    
    // 打印duration
    std::cout << "seconds since epoch: " << std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count() << " s\n";
    std::cout << "hours since epoch: " << std::chrono::duration_cast<std::chrono::hours>(p1.time_since_epoch()).count() << " h\n";
    std::cout << "yesterday, hours since epoch: " << std::chrono::duration_cast<std::chrono::hours>(p2.time_since_epoch()).count() << " h\n";


}