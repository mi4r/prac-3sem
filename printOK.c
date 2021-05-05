#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int k = 0;

void hdl1(int sign)
{
    signal(SIGINT,hdl1);
    k++;
    if (k == 1)
        alarm(1);
    if (k == 3)
        exit(0);
}

void hdl2(int sign)
{
    signal(SIGALRM,hdl2);
    k = 0;
}

int main()
{
    int c;
    signal(SIGINT, hdl1);
    signal(SIGALRM, hdl2);
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
            printf("OK\n");
    }
    printf("Goodbye\n");

    return 0;
}
