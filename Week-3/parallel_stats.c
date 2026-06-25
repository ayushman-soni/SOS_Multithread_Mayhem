#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DATA_SIZE 10000000

typedef struct
{
    long start;
    long end;

    long sum;

    int min;
    int max;

} thread_args_t;

int *data;

void *worker(void *arg)
{
    thread_args_t *curr = (thread_args_t *)arg;

    long sum = 0;

    int min = data[curr->start];
    int max = data[curr->start];

    for(long i = curr->start; i < curr->end; i++)
    {
        int value = data[i];

        sum += value;

        if(value < min)
        {
            min = value;
        }

        if(value > max)
        {
            max = value;
        }
    }

    curr->sum = sum;
    curr->min = min;
    curr->max = max;

    return NULL;
}

void run_test(int num_threads)
{
    pthread_t threads[num_threads];
    thread_args_t args[num_threads];

    long chunk = DATA_SIZE / num_threads;

    for(int i = 0; i < num_threads; i++)
    {
        args[i].start = i * chunk;

        if(i == num_threads - 1)
        {
            args[i].end = DATA_SIZE;
        }
        else
        {
            args[i].end = (i + 1) * chunk;
        }

        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for(int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    long total_sum = 0;

    int global_min = args[0].min;
    int global_max = args[0].max;

    for(int i = 0; i < num_threads; i++)
    {
        total_sum += args[i].sum;

        if(args[i].min < global_min)
        {
            global_min = args[i].min;
        }

        if(args[i].max > global_max)
        {
            global_max = args[i].max;
        }
    }

    double average = (double)total_sum / DATA_SIZE;

    printf("Threads : %d\n", num_threads);
    printf("Sum     : %ld\n", total_sum);
    printf("Min     : %d\n", global_min);
    printf("Max     : %d\n", global_max);
    printf("Average : %.2f\n\n", average);
}

int main()
{
    data = malloc(DATA_SIZE * sizeof(int));

    if(data == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    srand(42);

    for(long i = 0; i < DATA_SIZE; i++)
    {
        data[i] = rand() % 1000;
    }

    int thread_counts[] = {1, 2, 4, 8};

    int tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    for(int i = 0; i < tests; i++)
    {
        run_test(thread_counts[i]);
    }

    free(data);

    return 0;
}