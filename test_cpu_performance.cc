#include <chrono>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr, "usage : ./test_cpu_performance [number]\n");
        return 1;
    }
    int number = atoi(argv[1]);
    uint8_t *dst = new uint8_t[number];
    uint8_t *src = new uint8_t[number];
    memset(dst, 0, number);
    memset(src, 0xee, number);

    auto start = std::chrono::steady_clock::now();

    memcpy(dst, src, number);

    auto end = std::chrono::steady_clock::now();

    printf("memcpy %d bytes using time %ld us\n", number, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

    return 0;
}

