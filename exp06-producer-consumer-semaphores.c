#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { CAPACITY = 3, ITEM_COUNT = 8 };

static int buffer[CAPACITY];
static sem_t empty_slots;
static sem_t full_slots;
static sem_t mutex;

static void take(sem_t *semaphore)
{
    while (sem_wait(semaphore) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
    }
}

static void *produce(void *argument)
{
    (void)argument;
    int position = 0;
    for (int item = 1; item <= ITEM_COUNT; item++) {
        take(&empty_slots);
        take(&mutex);
        buffer[position] = item;
        position = (position + 1) % CAPACITY;
        printf("Produced: %d\n", item);
        sem_post(&mutex);
        sem_post(&full_slots);
    }
    return NULL;
}

static void *consume(void *argument)
{
    (void)argument;
    int position = 0;
    for (int i = 0; i < ITEM_COUNT; i++) {
        take(&full_slots);
        take(&mutex);
        printf("Consumed: %d\n", buffer[position]);
        position = (position + 1) % CAPACITY;
        sem_post(&mutex);
        sem_post(&empty_slots);
    }
    return NULL;
}

static void start(pthread_t *thread, void *(*work)(void *))
{
    int error = pthread_create(thread, NULL, work, NULL);
    if (error != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(error));
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    pthread_t producer, consumer;
    if (sem_init(&empty_slots, 0, CAPACITY) == -1 ||
        sem_init(&full_slots, 0, 0) == -1 ||
        sem_init(&mutex, 0, 1) == -1) {
        perror("sem_init");
        return 1;
    }

    start(&producer, produce);
    start(&consumer, consume);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    sem_destroy(&mutex);
    sem_destroy(&full_slots);
    sem_destroy(&empty_slots);
    return 0;
}
