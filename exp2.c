#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child = fork();

    if (child == -1) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        printf("Child process: PID = %ld, parent PID = %ld\n",
               (long)getpid(), (long)getppid());
        return 0;
    }

    int status;
    pid_t finished;
    do {
        finished = wait(&status);
    } while (finished == -1 && errno == EINTR);

    if (finished == -1) {
        perror("wait");
        return 1;
    }

    printf("Parent process: PID = %ld, reaped child PID = %ld\n",
           (long)getpid(), (long)finished);

    if (!WIFEXITED(status)) {
        fprintf(stderr, "Child terminated abnormally\n");
        return 1;
    }

    printf("Child exit status = %d\n", WEXITSTATUS(status));
    return WEXITSTATUS(status);
}
