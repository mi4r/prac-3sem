#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>

enum situation {s_eof, s_symb, s_space, s_newline, s_error, s_null};

typedef struct item
{
    char *s;
    struct item *next;
}mylist;

char* ft_strcpy(char *buf, int p)
{
    char *s;
    int len = 0;
    int k = 0;
    while (buf[len] != '\0')
        len++;
    s = malloc(sizeof(char)*(len+1));
    s[len] = '\0';
    while (k < len)
    {
        /*if ((p == 2) && (k == 0))
        {
            k++;
            continue;
        }*/
        s[k] = buf[k];
        k++;
    }
    return s;
}

mylist* ft_crt_node(char *buf, int p)
{
    mylist *new_node;
    new_node = malloc(sizeof(*new_node));
    (*new_node).s = ft_strcpy(buf,p);
    (*new_node).next = NULL;
    return new_node;
}

mylist* ft_add_node(char *buf, mylist *l, int p)
{
    mylist *head;
    head = NULL;
    if (l == NULL)
        head = ft_crt_node(buf,p);
    else
    {
        head = l;
        while ((*l).next != NULL)
            l = (*l).next;
        (*l).next = ft_crt_node(buf,p);
    }
    return head;
}

void ft_listout(mylist *p)
{
    mylist *l=NULL;
    l = p;
    while (l != NULL)
    {
        printf("%s\n",(*l).s);
        l = (*l).next;
    }
}

void ft_dislist(mylist *p)
{
    mylist *w;
    while (p != NULL)
    {
        w = (*p).next;
        free((*p).s);
        free(p);
        p = w;
    }
}

void ft_buf_copy(char *src_buf, char *dst_buf)
{
    int i = 0;
    /*if (!src_buf || !dst_buf)
    {
        return NULL;
    }*/
    while (src_buf[i] != '\0')
    {
        dst_buf[i] = src_buf[i];
        i++;
    }
    free(src_buf);
    //return dst_buf;
}

void ft_initarr(int len, char *buf) //filling array '\0'
{
    while (len > 0)
    {
        buf[len-1] = '\0';
        len--;
    }
}


void ft_mkarr(char **buf, const int len) //make buffer
{
    char *tmp = NULL;
    if (*buf == NULL)
    {
        *buf = (char *)malloc(sizeof(char)*(len+1));
        ft_initarr(len+1,*buf);
        return;
    }
    tmp = (char *)malloc(sizeof(char)*(len+1));
    ft_initarr(len+1,tmp);
    ft_buf_copy(*buf,tmp);
    *buf = tmp;
    return;
}

int ft_newline(int f)
{
    if (f < 0)
            {
                printf("Input error. Incorrect quotation.\n");
                return s_error;
            }
            return s_newline;
}

int ft_insert(char **buf) //buffer input
{
    int n = 10;
    int k = 1;
    int i = 0;
    int f = 1;
    int c = 0;
    if (*buf == NULL)
        ft_mkarr(buf, n*k);
    while (1)
    {
        if (i >= (n*k-1))
        {
            k++;
            ft_mkarr(buf, n*k);
        }
        if ((c = getchar()) == EOF)
            return s_eof;
        if (c == '"')
        {
            f = f*(-1);
            continue;
        }
        if (c == '\n')
        {
            if ((*buf)[0] == '\0')
                return s_null;
            return ft_newline(f);
        }
        if ((c != ' ') && (f > 0))
            (*buf)[i++] = (char)c;
        else if (f < 0)
            (*buf)[i++] = (char)c;
        else break;
    }
    if ((*buf)[0] == '\0')
        return s_space;
    return s_symb;
    //return(((*buf)[0] != '\0')?:1,2);
}

int ft_listnumber(mylist *list)
{
    int k = 0;
    while (list != NULL)
    {
        k++;
        list = (*list).next;
    }
    return k;
}

int ft_snumber(char *s)
{
    int k = 0;
    while (s[k] != '\0')
    {
        k++;
    }
    return k;
}

char** ft_list2arg(mylist *list)
{
    char **arg;
    int w_num,smb_num,i,j;
    mylist *tmp;
    tmp = list;
    w_num = ft_listnumber(list);
    arg = malloc((w_num+1) * sizeof(*arg));
    for (i = 0; i < w_num; i++)
    {
        smb_num = ft_snumber((*tmp).s);
        arg[i] = malloc((smb_num+1)*sizeof(**arg));
        arg[i][smb_num] = '\0';
        for (j = 0; j < smb_num; j++)
        {
            arg[i][j] = ((*tmp).s)[j];
        }
        tmp = (*tmp).next;
    }
    arg[w_num] = NULL;
    return arg;
}

void ft_execution(char ***arg)
{
    int pid;
    const char *homedir;
    if (strcmp(**arg,"cd") == 0)
    {
        if ((*arg)[1] == NULL)
        {
            if ((homedir = getenv("HOME")) == NULL) 
                homedir = getpwuid(getuid())->pw_dir;
            chdir(homedir);
        }
        else if (chdir((*arg)[1]) == -1)
                perror(**arg);
    }
    else 
    {
        pid = fork();
        if (pid == 0)
        {    
            execvp(**arg,*arg);
            perror(**arg);
            exit(1);
        }
        wait(NULL);
    }
    int i = 0;
    while ((*arg)[i] != NULL)
    {
        free((*arg)[i]);
        i++;
    }
    free(*arg);
}


int main()
{
    mylist *list;
    char *buf;
    char **args;
    int k = s_symb;
    int p = 0;
    while (k != s_eof)
    {
        printf(">>");
        k = s_symb;
        list = NULL;
        while ((k > s_eof) && (k != s_newline) && (k != s_error) && (k != s_null))
        {   
            buf = NULL;
            k = ft_insert(&buf);
            if ((k != s_space) && (k != s_null))
            {
                p++;
                list = ft_add_node(buf,list,p);
                free(buf);
            }
        }
        if ((k != s_eof) && (k != s_null))
        {
            args = ft_list2arg(list);
            ft_execution(&args);
        }
        if (k == s_eof)
            printf("\n");
        //if (k != s_error && k != s_eof)
        //    ft_listout(list);
        ft_dislist(list);
    }
    return 0;
}