#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

int gl_flag = 1;
int gl_k = 0;
int gl_pid;

void hdl1(int sign)
{
    signal(SIGALRM, hdl1);
    gl_k = 0;
}

void hdl2(int sign)
{
    signal(SIGINT, hdl2);
    gl_k++;
    gl_flag *= -1;
    if (gl_k == 1)
        alarm(1);
    if (gl_k == 2)
        kill(gl_pid, SIGKILL);
    if (gl_flag > 0)
        kill(gl_pid, SIGSTOP);
    else
        kill(gl_pid, SIGCONT);
}

int main(int argc, char **argv)
{
    signal(SIGALRM, hdl1);
    signal(SIGINT, hdl2);

    int fd0, fd1;
    fd0 = open(argv[1],O_RDONLY);
    fd1 = open(argv[2], O_WRONLY);
    gl_pid = fork();
    if (gl_pid == 0)
    {
        signal(SIGINT,SIG_IGN);
        if (fd0 == -1 || fd1 == -1)
            printf("ERROR: file isn't opened\n");
        else
        {
            dup2(fd0, STDIN_FILENO);
            dup2(fd1, STDOUT_FILENO);
            execvp(argv[3], argv+3);
            perror(argv[3]);
            exit(1);
        }
    }
    wait(NULL);
    close(fd0);
    close(fd1);

    return 0;
}