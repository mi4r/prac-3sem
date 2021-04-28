#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int gl_sum = 0;
int gl_flag = 1;

void hdl1(int sign)
{
    gl_flag *= -1;
}

void hdl2(int sign)
{
    if (gl_flag < 0)
    {
        exit(0);
    }
    else
    {
        gl_sum = 0;
        alarm(2);
        gl_flag *= -1;
    }
}

void ft_initarr(char *buf)
{
    int i = 0;
    for (i = 0; i < 100; i++)
    {
        buf[i] = '\0';
    }
}

int main()
{
    int c;
    int x = 0;
    char buf[100];
    int i = 0;

    struct sigaction sact;    
    memset(&sact, 0, sizeof(sact));
    sact.sa_handler = &hdl1;
    sigaction(SIGALRM,&sact,0);

    struct sigaction sact2;
    memset(&sact2, 0, sizeof(sact2));
    sact2.sa_handler = &hdl2;
    sigaction(SIGINT,&sact2,0);

    while (1)
    {
        c = getchar();
        ft_initarr(buf);
        i = 0;
        if (!feof(stdin))
        {
        }
        else if (c == EOF)
            break;

        while((c != ' ') && (c != EOF) && (c != '\n'))
        {
            switch (c)
            {
                case '-':
                    if (i==0)
                    {
                        buf[i] = c;
                    } else
                    {
                        buf[i] = 0;
                    }
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    buf[i] = c;
                    break;
                default:
                    buf[i] = 0;
                    break;
            }
            /*
            if (c == '-' && i==0)
            {
            } else if ((c >= '0') && (c <= '9'))
            {
                buf[i] = c;
                //printf("DBG:%i\n", i);
            } else
            {
                buf[i] = 0;
            }
            */
            c = getchar();
            i++;
        }
        i = 0;
        int sign = 1;
        // Parse string
        if (buf[0]=='-')
        {
            sign = -1;
            ++i;            
        }
        while (buf[i] != 0)
        {
            x = x * 10 + (buf[i] - '0');
            i++;
        }
        x *= sign;

        gl_sum = gl_sum + x;
        x = 0;
        if (c == '\n')
            printf("%d\n",gl_sum);
    }

    // regular expression: [-]?[0-9]{1,19}
    // libPCRE

    return 0;
}