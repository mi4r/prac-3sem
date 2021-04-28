#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

//chdir()

//execlp("cd")


int gl_counter = 0;

int gl_end = 0;

void handler (int sign)
{
    printf("go further..");
};

int main(int argc, char** argv)
{
    struct sigaction sact;
    memset(&sact, 0, sizeof(sact));
    sact.sa_handler = &handler;
    sigaction(SIGINT, &sact, 0);

    char ch;
    while(1)
    {
        ch = getchar();
        if (ch == EOF)
        {
            if (!feof(stdin))
            {
                printf("fake EOF");
                //ch = getchar();
            } else
            {
                printf("EOF");
                break;
            }
        } else
        {
            printf("Read(%c)", ch);
        }
    }
    
    return 0;
}
