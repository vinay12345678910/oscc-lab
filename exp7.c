#include <stdbool.h>
#include <stdio.h>

enum { PROCESSES = 3, RESOURCES = 3 };

static bool can_finish(const int allocation[], const int maximum[],
                       const int available[])
{
    for (int resource = 0; resource < RESOURCES; resource++) {
        if (maximum[resource] - allocation[resource] > available[resource]) {
            return false;
        }
    }
    return true;
}

int main(void)
{
    const int allocation[PROCESSES][RESOURCES] = {
        {1, 1, 0}, {2, 0, 1}, {1, 1, 1}
    };
    const int maximum[PROCESSES][RESOURCES] = {
        {2, 2, 1}, {3, 1, 2}, {2, 2, 2}
    };
    int available[RESOURCES] = {2, 1, 1};
    bool finished[PROCESSES] = {false};
    int sequence[PROCESSES];
    int count = 0;

    while (count < PROCESSES) {
        int previous_count = count;
        for (int process = 0; process < PROCESSES; process++) {
            if (finished[process] ||
                !can_finish(allocation[process], maximum[process], available)) {
                continue;
            }

            sequence[count++] = process;
            finished[process] = true;
            for (int resource = 0; resource < RESOURCES; resource++) {
                available[resource] += allocation[process][resource];
            }
        }

        if (count == previous_count) {
            puts("Unsafe state: no safe sequence exists.");
            return 0;
        }
    }

    printf("Safe sequence: P%d", sequence[0]);
    for (int i = 1; i < count; i++) {
        printf(" -> P%d", sequence[i]);
    }
    putchar('\n');
    return 0;
}
