#include <stdbool.h>
#include <stdio.h>

static void simulate(const int pages[], size_t count, size_t capacity)
{
    int frames[5];
    size_t occupied = 0;
    size_t next = 0;
    size_t faults = 0;

    printf("\nFrames: %zu\n", capacity);
    for (size_t i = 0; i < count; i++) {
        bool hit = false;
        for (size_t frame = 0; frame < occupied; frame++) {
            if (frames[frame] == pages[i]) {
                hit = true;
                break;
            }
        }

        if (hit) {
            printf("Page hit: %d\n", pages[i]);
        } else {
            frames[next] = pages[i];
            next = (next + 1) % capacity;
            if (occupied < capacity) {
                occupied++;
            }
            faults++;
            printf("Page fault: %d\n", pages[i]);
        }
    }

    printf("Total page faults = %zu/%zu (%.1f%%)\n",
           faults, count, 100.0 * faults / count);
    if (faults > count / 2) {
        puts("High fault rate: thrashing may occur.");
    } else {
        puts("Fault rate is within the simulation threshold.");
    }
}

int main(void)
{
    const int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    size_t count = sizeof pages / sizeof pages[0];

    simulate(pages, count, 3);
    simulate(pages, count, 5);
    return 0;
}
