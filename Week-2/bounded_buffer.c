#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 2

#define ITEMS_PER_PRODUCER 1000

int *buffer;

int capacity;
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;

long produced_sum = 0;
long consumed_sum = 0;

int total_produced = 0;
int total_consumed = 0;

void buffer_init(int cap)
{
    capacity = cap;

    buffer = malloc(capacity * sizeof(int));

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
}

void buffer_put(int item)
{
    pthread_mutex_lock(&lock);

    while (count == capacity)
    {
        pthread_cond_wait(&not_full, &lock);
    }

    buffer[in] = item;
    in = (in + 1) % capacity;
    count++;

    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&lock);
}

int buffer_get(void)
{
    pthread_mutex_lock(&lock);

    while (count == 0)
    {
        pthread_cond_wait(&not_empty, &lock);
    }

    int item = buffer[out];

    out = (out + 1) % capacity;
    count--;

    pthread_cond_signal(&not_full);

    pthread_mutex_unlock(&lock);

    return item;
}

void buffer_destroy(void)
{
    free(buffer);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
}

void *producer(void *arg)
{
    int id = *(int *)arg;

    int start = id * 1000;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        int value = start + i;

        buffer_put(value);

        __sync_fetch_and_add(&produced_sum, value);
        __sync_fetch_and_add(&total_produced, 1);
    }

    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;

    int items_to_consume =
        (NUM_PRODUCERS * ITEMS_PER_PRODUCER) / NUM_CONSUMERS;

    for (int i = 0; i < items_to_consume; i++)
    {
        int value = buffer_get();

        __sync_fetch_and_add(&consumed_sum, value);
        __sync_fetch_and_add(&total_consumed, 1);
    }

    return NULL;
}

int main()
{
    buffer_init(BUFFER_SIZE);

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    int ids[NUM_PRODUCERS];

    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        ids[i] = i;

        pthread_create(
            &producers[i],
            NULL,
            producer,
            &ids[i]
        );
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_create(
            &consumers[i],
            NULL,
            consumer,
            NULL
        );
    }

    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    printf("Produced Items : %d\n", total_produced);
    printf("Consumed Items : %d\n", total_consumed);

    printf("Produced Sum   : %ld\n", produced_sum);
    printf("Consumed Sum   : %ld\n", consumed_sum);

    if (total_produced == total_consumed &&
        produced_sum == consumed_sum)
    {
        printf("Test Passed\n");
    }
    else
    {
        printf("Test Failed\n");
    }

    buffer_destroy();

    return 0;
}