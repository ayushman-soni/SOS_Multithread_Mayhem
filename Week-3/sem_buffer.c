#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SENTINEL -1

typedef struct
{
    int *data;
    int capacity;
    int in;
    int out;

    sem_t empty;
    sem_t full;

    pthread_mutex_t mutex;

} buffer_t;

buffer_t buf;

void buffer_init(int capacity)
{
    buf.data = malloc(sizeof(int) * capacity);

    buf.capacity = capacity;
    buf.in = 0;
    buf.out = 0;

    sem_init(&buf.empty, 0, capacity);
    sem_init(&buf.full, 0, 0);

    pthread_mutex_init(&buf.mutex, NULL);
}

void buffer_put(int item)
{
    sem_wait(&buf.empty);

    pthread_mutex_lock(&buf.mutex);

    buf.data[buf.in] = item;
    buf.in = (buf.in + 1) % buf.capacity;

    pthread_mutex_unlock(&buf.mutex);

    sem_post(&buf.full);
}

int buffer_get(void)
{
    sem_wait(&buf.full);

    pthread_mutex_lock(&buf.mutex);

    int item = buf.data[buf.out];
    buf.out = (buf.out + 1) % buf.capacity;

    pthread_mutex_unlock(&buf.mutex);

    sem_post(&buf.empty);

    return item;
}

void buffer_destroy(void)
{
    free(buf.data);

    sem_destroy(&buf.empty);
    sem_destroy(&buf.full);

    pthread_mutex_destroy(&buf.mutex);
}

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 1000

typedef struct
{
    int id;
    long produced_sum;

} producer_args_t;

typedef struct
{
    int id;
    long consumed_sum;
    int items_consumed;

} consumer_args_t;

void *producer(void *arg)
{
    producer_args_t *p = (producer_args_t *)arg;

    long sum = 0;

    for(int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        int item = p->id * 100000 + i;

        buffer_put(item);

        sum += item;
    }

    p->produced_sum = sum;

    return NULL;
}

void *consumer(void *arg)
{
    consumer_args_t *c = (consumer_args_t *)arg;

    long sum = 0;
    int count = 0;

    while(1)
    {
        int item = buffer_get();

        if(item == SENTINEL)
        {
            break;
        }

        sum += item;
        count++;
    }

    c->consumed_sum = sum;
    c->items_consumed = count;

    return NULL;
}

void run_test(int capacity)
{
    printf("Buffer Capacity = %d\n", capacity);

    buffer_init(capacity);

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    producer_args_t pargs[NUM_PRODUCERS];
    consumer_args_t cargs[NUM_CONSUMERS];

    for(int i = 0; i < NUM_PRODUCERS; i++)
    {
        pargs[i].id = i + 1;

        pthread_create(
            &producers[i],
            NULL,
            producer,
            &pargs[i]
        );
    }

    for(int i = 0; i < NUM_CONSUMERS; i++)
    {
        cargs[i].id = i;

        pthread_create(
            &consumers[i],
            NULL,
            consumer,
            &cargs[i]
        );
    }

    for(int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for(int i = 0; i < NUM_CONSUMERS; i++)
    {
        buffer_put(SENTINEL);
    }

    for(int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    long total_produced = 0;
    long total_consumed = 0;

    int total_items_consumed = 0;
    int total_items_produced =
        NUM_PRODUCERS * ITEMS_PER_PRODUCER;

    for(int i = 0; i < NUM_PRODUCERS; i++)
    {
        total_produced += pargs[i].produced_sum;
    }

    for(int i = 0; i < NUM_CONSUMERS; i++)
    {
        total_consumed += cargs[i].consumed_sum;
        total_items_consumed += cargs[i].items_consumed;
    }

    printf("Items Produced : %d\n",
           total_items_produced);

    printf("Items Consumed : %d\n",
           total_items_consumed);

    printf("Produced Sum   : %ld\n",
           total_produced);

    printf("Consumed Sum   : %ld\n",
           total_consumed);

    if(total_items_produced == total_items_consumed &&
       total_produced == total_consumed)
    {
        printf("PASS\n\n");
    }
    else
    {
        printf("FAIL\n\n");
    }

    buffer_destroy();
}

int main()
{
    run_test(1);
    run_test(16);

    return 0;
}