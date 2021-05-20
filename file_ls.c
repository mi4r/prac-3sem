#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct item
{
    char * file;
    struct item * next;
}list;

list * add_to_list(list * head, char * word, int size)
{
    list * tmp = head;
    if (tmp == NULL)
    {
        tmp = malloc(sizeof(*tmp));
        tmp->file = strdup(word);
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
        tmp->file = strdup(word);
        tmp->next = NULL;
        return head;
    }
}

int main()
{
    list * head = NULL;
    int pipe_ends[2];
    int i;
    int j = 0;
    int flag = 1;
    int break_line = 1;
    char buf[20];
    char word[71];
    int fd;
    pipe(pipe_ends);
    int pid;
    pid = fork();
    if (pid == 0)
    {
        dup2(pipe_ends[1], 1);
        close(pipe_ends[0]);
        execlp("ls", "ls", "-l", NULL);
        perror("ls");
        exit(1);
    }
    close(pipe_ends[1]);
    while ((fd = read(pipe_ends[0], &buf, sizeof(buf))))
    {
        i = 0;
        while (i < fd)
        {
            if (buf[i] == '\n' && j > 0)
            {
                if (flag < 0)
                {
                    head = add_to_list(head, word, j);
                    flag *= -1;
                }
                memset(&word, 0, sizeof(word));
                j = 0;
                i++;
                if (buf[i] == '-')
                    flag *= -1;
            }
            else if (buf[i] == ' ')
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
    printf("All files:\n");
    while (head != NULL)
    {
        printf("%s\n", head->file);
        head = head->next;
    }
    close(pipe_ends[0]);
    wait(NULL);

    return 0;
}