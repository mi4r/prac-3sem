#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int ended = 0;

void hdl(int sign)
{
    signal(SIGCHLD, hdl);
    ended = 1;
}

int main(int argc, char **argv)
{
    char * words[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon",
                      "Zeta", "Eta", "Theta", "Iota", "Kappa", "Lambda"};
    int pipe_ends[2];
    int pid1, pid2;
    int status;
    int i = 0, j = 0;
    signal(SIGCHLD, hdl);
    pipe(pipe_ends);
    pid1 = fork();
    if (pid1 == 0)
    {
        close(pipe_ends[1]);
        dup2(pipe_ends[0], 0);
        close(pipe_ends[0]);
        execvp(argv[1], argv+1);
        perror(argv[1]);
        exit(1);
    }
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipe_ends[0]);
        dup2(pipe_ends[1], 1);
        close(pipe_ends[1]);
        for (i = 0; i < 100000; i++)
        {
            if (!ended)
            {
                for (j = 0; j < 11; j++)
                {
                    if (!ended)
                    {
                        printf("%s\n", words[j]);
                    }
                    else break;
                }
            }
            else break;
        }
    }
    else
    {
        close(pipe_ends[0]);
        close(pipe_ends[1]);
        wait4(pid1, &status, 0, 0);
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        }
    }

    return 0;
}