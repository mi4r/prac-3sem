#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int gl_max = 0;
int gl_min = 0;
int gl_flag = 1;
int flag_go = 1;

void hdl1(int sign)
{
    signal(SIGINT, hdl1);
    if (gl_flag < 0)
        exit(0);
    gl_max = 0;
    gl_min = 0;
    gl_flag *= -1;
    flag_go = 1;
    alarm(2);
}

void hdl2(int sign)
{
    signal(SIGALRM, hdl2);
    gl_flag *= -1;
}

int main()
{
    int c;
    int i = 0;
    int x = 0;
    char buf[20];
    signal(SIGINT, hdl1);
    signal(SIGALRM, hdl2);
    while((c = getchar()) != EOF)
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
            if (flag_go > 0 && buf[0] != '\0' && !(buf[0] == '-' && buf[1] == '\0'))
            {
                gl_max = x;
                gl_min = x;
                flag_go = -1;
            }
            else
            {
                if (x > gl_max && buf[0] != '\0' && !(buf[0] == '-' && buf[1] == '\0'))
                    gl_max = x;
                if (x < gl_min && buf[0] != '\0' && !(buf[0] == '-' && buf[1] == '\0'))
                    gl_min = x;
            }
            x = 0;
            memset(&buf, 0, sizeof(buf));
        }
        if (c == '\n')
        {
            printf("max = %d\n",gl_max);
            printf("min = %d\n", gl_min);
        }
    }

    return 0;
}