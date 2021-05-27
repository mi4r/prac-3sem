#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void func(int argc, int ind, char ** arr)
{
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    if (fork() == 0)
    {
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        dup2(fd1[1], 1);
        close(fd1[1]);
        execvp(arr[1], arr+1);
        perror(arr[1]);
        exit(1);
    }
    if (fork() == 0)
    {
        int c;
        int evenline = 1;
        close(fd1[1]);
        close(fd2[0]);
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]);
        close(fd2[1]);
        while ((c = getchar()) != EOF)
        {
            if (evenline < 0)
            {
                printf("%c",c);
            }
            if (c == '\n')
            {
                evenline *= -1;
            }
        }
    }
    if (fork() == 0)
    {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        dup2(fd2[0], 0);
        close(fd2[0]);
        execvp(arr[ind+1], &arr[ind+1]);
        perror(arr[ind+1]);
        exit(1);
    }
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
}

int main(int argc, char ** argv)
{
    char ** arr = malloc((argc+1)* sizeof(*arr));
    int i;
    int ind;
    for (i = 0; i < argc; i++)
    {
        if (strcmp("---", argv[i]) == 0)
        {
            ind = i;
            arr[i] = NULL;
        }
        else
            arr[i] = argv[i];
    }
    arr[argc] = NULL;
    func(argc, ind, arr);

    return 0;
}