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
    int tmp_res, tmp_status;
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
    res = wait4(-1, &status, WNOHANG, NULL);
    do
    {
        tmp_res = wait4(-1, &tmp_status, WNOHANG, NULL);
        if (tmp_res > 0)
        {
            res = tmp_res;
            status = tmp_status;
        }
    } while (tmp_res > -1);
    name = find_name(head, res);
    printf("last cmd: %s\tsignal: %d\n",arr[name], status);
}

int main(int argc, char **argv)
{
    int i;
    char ** arr = malloc((argc+1)*sizeof(*arr));
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i],"---") == 0)
            arr[i] = NULL;
        else arr[i] = argv[i];
    }
    arr[argc] = NULL;
    launcher(argc, arr);

    return 0;
}









/*
    int pid;
    char **arr = NULL;
    int i;
    for (i = 1; i <= argc; i++)
    {
        arr = malloc(argc);
        while((strcmp(argv[i],"---") != 0))
        {
            //arr[i-1] = malloc(sizeof(argv[i]));
            arr[i-1] = argv[i];
            i++;
        }
        pid = fork();
        if (pid == 0)
        {
            execvp(*arr,arr);
            perror(*arr);
            exit(1);
        }
        free(arr);
        wait(NULL);
    }
    */