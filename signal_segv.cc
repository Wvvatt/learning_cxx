#include "common.h"

int *p_int = new int(5);

static void segv_handler(int signum)
{
    void *buffer[256];
    signal(signum, SIG_DFL);
    int fd = open("backtrace", O_RDWR | O_CREAT | O_TRUNC);
    if (fd > 0)
    {
        int size = backtrace(buffer, 256);
        backtrace_symbols_fd(buffer, size, fd);
        close(fd);
    }

    exit(1);
}

static void thread1_entry()
{
    while(true)
    {
        std::cout << "thread1 loop..." << *p_int << std::endl;
        sleep(1);
    }
}

int main()
{
    signal(SIGSEGV, segv_handler);
    std::thread t1 = std::thread(thread1_entry);

    while(true){
        std::cout << "main loop..." << std::endl;
        sleep(1);
        p_int = nullptr;
    }

    return 0;
}