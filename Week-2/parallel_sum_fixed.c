#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 10000000
#define NUM_THREADS 4

int *array;
long total = 0;

pthread_mutex_t total_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int thread_id;
    long start_index;
    long end_index;
} thread_arg_t;

void *partial_sum(void *arg)
{
    thread_arg_t *t = (thread_arg_t *)arg;

    for(long i = t->start_index; i < t->end_index; i++)
    {
        pthread_mutex_lock(&total_lock);

        total += array[i];

        pthread_mutex_unlock(&total_lock);
    }

    return NULL;
}

int main(void)
{
    array = malloc(ARRAY_SIZE * sizeof(int));

    if(array == NULL)
    {
        perror("malloc");
        return 1;
    }

    for(long i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = 1;
    }

    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    long chunk = ARRAY_SIZE / NUM_THREADS;

    for(int i = 0; i < NUM_THREADS; i++)
    {
        args[i].thread_id = i;
        args[i].start_index = i * chunk;

        if(i == NUM_THREADS - 1)
        {
            args[i].end_index = ARRAY_SIZE;
        }
        else
        {
            args[i].end_index = (i + 1) * chunk;
        }

        pthread_create(&threads[i], NULL, partial_sum, &args[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Computed sum: %ld\n", total);
    printf("Expected: %d\n", ARRAY_SIZE);

    if(total == ARRAY_SIZE)
    {
        printf("Correct result. Race condition fixed.\n");
    }
    else
    {
        printf("Something is still wrong.\n");
    }

    pthread_mutex_destroy(&total_lock);

    free(array);

    return 0;
}