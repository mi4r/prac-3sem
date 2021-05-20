#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int gl_sum = 0;
int gl_flag = 1;

void hdl1(int sign)
{
    signal(SIGINT, hdl1);
    if (gl_flag == -1)
        exit(0);
    gl_sum = 0;
    gl_flag *= -1;
    alarm(2);
}

void hdl2(int sign)
{
    signal(SIGALRM, hdl2);
    gl_flag *= -1;
}

int main()
{
    signal(SIGINT,hdl1);
    signal(SIGALRM,hdl2);
    int c;
    int x = 0;
    char buf[20];
    int i = 0;
    while ((c = getchar()) != EOF)
    {
        if (c == '-' && i == 0)
        {
            buf[i++] = c;
        }
        else if (c >= '0' && c <= '9')
        {
            buf[i++] = c;
        }
        else if (c == ' '  || c == '\t' || c == '\n')
        {
            i = 0;
            if (buf[0] == '-')
                i++;
            while (buf[i] != '\0')
            {
                x = x*10 + (buf[i] - '0');
                i++;
            }
            i = 0;
            if (buf[0] == '-')
                x *= -1;
            gl_sum = gl_sum + x;
            memset(&buf, 0, sizeof(buf));
            x = 0;
        }
        if (c == '\n')
            printf("%d\n", gl_sum);
    }

    return 0;
}
