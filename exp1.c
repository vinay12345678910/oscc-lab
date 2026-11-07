#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t result = fork();
    if (result < 0)
    {
        puts("fork failed");
        return 1;
    }
    else if (result == 0)
    {
        execl("/bin/ls", "ls", (char *)NULL);
        perror("execl");
        return 1;
    }
    else
    {
        wait(NULL);
        puts("I am the parent");
    }
    exit(EXIT_SUCCESS);
}
