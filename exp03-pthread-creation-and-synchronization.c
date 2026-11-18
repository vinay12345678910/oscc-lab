#include <pthread.h>
#include <stdio.h>
#include <string.h>

enum { THREAD_COUNT = 2, INCREMENTS = 5 };

static int counter;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *increment(void *argument)
{
    int id = *(int *)argument;
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        printf("Thread %d: Counter = %d\n", id, counter);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT] = {1, 2};
    int started = 0;

    for (; started < THREAD_COUNT; started++) {
        int error = pthread_create(&threads[started], NULL, increment,
                                   &ids[started]);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            break;
        }
    }

    for (int i = 0; i < started; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&lock);

    if (started != THREAD_COUNT) {
        return 1;
    }
    printf("Final Counter = %d\n", counter);
    return 0;
}
