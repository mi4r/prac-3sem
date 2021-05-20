#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct item
{
    char * dir;
    struct item * next;
}list;

list * add_to_list(list * subdirs, char * word, int size)
{
    list * tmp = subdirs;
    if (tmp == NULL)
    {
        tmp = malloc(sizeof(*tmp));
        tmp->dir = strdup(word);
        tmp->next = NULL;
        return tmp;
    }
    else
    {
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(*tmp));
        tmp = tmp->next;
        tmp->dir = strdup(word);
        tmp->next = NULL;
        return subdirs;
    }
    /*
    list * tmp = subdirs;
    tmp = malloc(sizeof(*tmp));
    tmp->dir = strdup(word);
    tmp->next = subdirs;
    return tmp;
    */
}

int main()
{
    int i;
    int j = 0;
    int pipe_ends[2];
    list * subdirs = NULL;
    char word[71];
    int fd;
    char buf[20];
    pipe(pipe_ends);
    int pid;
    pid = fork();
    if (pid == 0)
    {
        dup2(pipe_ends[1], 1);
        close(pipe_ends[0]);
        execlp("ls", "ls", "-F", NULL);
        perror("ls");
        exit(1);
    }
    close(pipe_ends[1]);
    while ((fd = read(pipe_ends[0], &buf, sizeof(buf))))
    {
        i = 0;
        while (i < fd)
        {
            if (buf[i] == '/' && j > 0)
            {
                subdirs = add_to_list(subdirs, word, j);
                memset(&word, 0, sizeof(word));
                j = 0;
            }
            else if (buf[i] == '\n')
            {
                memset(&word, 0, sizeof(word));
                j = 0;
            }
            else if (j < 70)
            {
                word[j] = buf[i];
                j++;
            }
            i++;
        }
    }
    printf("All subdirs:\n");
    while (subdirs != NULL)
    {
        printf("%s\n", subdirs->dir);
        subdirs = subdirs->next;
    }
    close(pipe_ends[0]);
    wait(NULL);

    return 0;
}