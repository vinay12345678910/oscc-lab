#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

enum { MESSAGE_SIZE = 256 };

int main(void)
{
    char input[MESSAGE_SIZE];
    printf("Enter message: ");
    fflush(stdout);
    if (fgets(input, sizeof input, stdin) == NULL) {
        fprintf(stderr, "No message received\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    int id = shmget(IPC_PRIVATE, MESSAGE_SIZE, IPC_CREAT | 0600);
    if (id == -1) {
        perror("shmget");
        return 1;
    }

    char *shared_message = shmat(id, NULL, 0);
    if (shared_message == (void *)-1) {
        perror("shmat");
        shmctl(id, IPC_RMID, NULL);
        return 1;
    }

    if (shmctl(id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        shmdt(shared_message);
        return 1;
    }

    strcpy(shared_message, input);
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        shmdt(shared_message);
        return 1;
    }
    if (child == 0) {
        printf("Message from shared memory: %s\n", shared_message);
        if (shmdt(shared_message) == -1) {
            perror("shmdt");
            return 1;
        }
        return 0;
    }

    int detach_result = shmdt(shared_message);
    if (detach_result == -1) {
        perror("shmdt");
    }
    int status;
    pid_t finished;
    do {
        finished = waitpid(child, &status, 0);
    } while (finished == -1 && errno == EINTR);

    if (finished == -1) {
        perror("waitpid");
        return 1;
    }
    return (detach_result == -1 || !WIFEXITED(status)) ? 1 : WEXITSTATUS(status);
}
