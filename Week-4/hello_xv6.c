#include "kernel/types.h"
#include "user/user.h"

#define NUM_SYSCALLS 25

int
main(int argc, char *argv[])
{
    printf("Hello, xv6! This is Ayushman Soni.\n");

    printf("Number of arguments: %d\n", argc - 1);

    for(int i = 1; i < argc; i++){
        printf("arg %d: %s\n", i, argv[i]);
    }

    printf("xv6 knows %d system calls.\n", NUM_SYSCALLS);

    exit(0);
}