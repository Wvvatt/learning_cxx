#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

constexpr int kLoopCount = 100;

void producer() {
    for (int i = 0; i < kLoopCount; ++i) {
        std::unique_lock<std::mutex> lck(mtx);
        q.push(i);
        std::cout << "Produced: " << i << std::endl;
        cv.notify_one();
    }
}

void consumer() {
    for (int i = 0; i < kLoopCount; ++i) {
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{ return !q.empty(); });
        std::cout << "Consumed: " << q.front() << std::endl;
        q.pop();
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
