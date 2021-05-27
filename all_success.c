#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct item
{
    int i;
    int pid;
    struct item * next;
}list;

list * add_to_list(list * head, int pid, int i)
{
    list * tmp = malloc(sizeof(*tmp));
    tmp->pid = pid;
    tmp->i = i;
    tmp->next = head;
    return tmp;
}

int next_command(char ** arr, int i)
{
    int k;
    for (k = i; arr[k] != NULL; k++)
        ;
    return k + 1;
}

int find_name(list * head, int res)
{
    int name = 0;
    list * tmp = head;
    while (tmp != NULL)
    {
        if (tmp->pid == res)
            name = tmp->i;
        tmp = tmp->next;
    }
    return name;
}

void launcher(int argc, char ** arr)
{
    list * head = NULL;
    int res, status;
    int name;
    int i = 1;
    int pid;
    while (i < argc)
    {
        pid = fork();
        if (pid == 0)
        {
            execvp(arr[i], arr+i);
            perror(arr[i]);
            exit(1);
        }
        else head = add_to_list(head, pid, i);
        i = next_command(arr, i);
    }
    while ((res = wait4(-1, &status, 0, NULL)) > 0)
    {
        if (status == 0)
        {
            name = find_name(head, res);
            printf("cmd: %s\n",arr[name]);
        }
    }
}

int main(int argc, char **argv)
{
    int i;
    char ** arr = malloc((argc+1)*sizeof(*arr));
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i],";;") == 0)
            arr[i] = NULL;
        else arr[i] = argv[i];
    }
    arr[argc] = NULL;
    launcher(argc, arr);

    return 0;
}