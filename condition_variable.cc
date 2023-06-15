#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
 
/*
    为什么条件变量需要锁？

        锁既保护临界资源，又保护条件（条件也是临界资源）。保护临界资源不用赘述，这里主要说明保护条件的作用。

        条件变量使用流程：
        {
            lock();
            while(条件不满足){
                pthread_cond_wait(lock);
            }
            do_some_thing();        // 处理临界资源
            unlock();
        }
        
        而pthread_cond_wait的内部流程大致如下：
        {
            wait_and_unlock();
            lock();
        }
        其中wait_and_unlock()是一个原子操作，将线程放入等待队列的同时，解锁。
        使用锁和原子操作，保证了 上锁-判断-等待-解锁 是一个线程安全的过程。
        如果没有锁，则这个过程中可能插入notify。
        
        等待线程作为W Thread， 唤醒线程作为N Thread。
        当没有锁时，流程可能是：条件不满足(W) --> 条件满足(N) --> 唤醒(N) --> 等待(W)。那么等待线程会错过条件判断和唤醒，一直等待。

        加入锁后，流程只能是如下两种：
        1 先等待，然后被唤醒
            上锁(W) --> 条件不满足(W) --> 等待并解锁(W) --> 上锁(N) --> 满足条件(N) --> 解锁(N) --> 唤醒(N) --> 被唤醒(W) --> 上锁(W) --> 处理数据(W) --> 解锁(W)
        2 条件满足，不用等待
            上锁(N) --> 满足条件(N) --> 解锁(N) --> 上锁(W) --> 条件满足(W) --> 处理数据(W) --> 解锁(W)
*/

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;
 
void worker_thread()
{
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return ready;});
 
    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";
 
    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";
 
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}
 
int main()
{
    std::thread worker(worker_thread);
 
    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();
 
    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }
    std::cout << "Back in main(), data = " << data << '\n';
 
    worker.join();
}