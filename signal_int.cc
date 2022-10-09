#include "common.h"

sem_t sem;
int *p_int = new int(5);
std::atomic<bool> thread_running;

static void int_handler(int signum)
{
    signal(SIGINT, SIG_DFL);
    sem_post(&sem);
}

static void thread1_entry()
{
    while(thread_running)
    {
        std::cout << "thread1 loop..." << std::endl;
        sleep(1);
    }
}

int main()
{
    signal(SIGINT, int_handler);
    thread_running = true;
    std::thread t1 = std::thread(thread1_entry);
    sem_wait(&sem);
    thread_running = false;
    if(t1.joinable()){
        t1.join();
    }
    return 0;
}