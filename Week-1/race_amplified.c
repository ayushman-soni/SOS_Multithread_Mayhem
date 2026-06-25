#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ITERS 10000000

long counter = 0;

void *increment(void *arg) {
    (void)arg;

    for (long i = 0; i < ITERS; i++) {
        long temp = counter;
        usleep(1);
        temp = temp + 1;
        counter = temp;
    }

    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    long expected = 2L * ITERS;

    printf("Final counter: %ld\n", counter);
    printf("Expected: %ld\n", expected);
    printf("Difference: %ld\n", expected - counter);

    return 0;
}
