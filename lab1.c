#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void f(int sig);
void f1(int sig);
void f2(int sig);
pid_t p1, p2;
int main()
{
    
    int count = 1;
    int p[2];
    char buf1[30], buf2[30];
    printf("pipe return:%d\n", pipe(p));
    p1 = fork();
    if(p1!=0)
        p2 = fork();
    if (p1 == 0)
    {
        //child process 1
        while(1)
        {
            signal(SIGINT, (void*)1);
            signal(SIGUSR1, f1);
            sprintf(buf1, "I send you %d times!", count++);
            write(p[1], buf1, 30);
            sleep(1);
        }
    }
    else if (p2 == 0)
    {
        //child process 2
        while(1)
        {
            signal(SIGINT, (void *)1);
            signal(SIGUSR2, f2);
            read(p[0], buf2, 30);
            printf("%s\n", buf2);
        }
    }

    {
        //parent process
        signal(SIGINT, f);
        waitpid(p1, NULL, 0);
        waitpid(p2, NULL, 0);
        close(p[0]);
        close(p[1]);
        printf("Parent process was killed!\n");
        exit(0);
    }
}
void f(int sig)
{
    printf("\nActivate ctrl-c!\n");
    kill(p1, SIGUSR1);
    kill(p2, SIGUSR2);
}
void f1(int sig)
{
    printf("Child process1 was killed by parent!\n");
    exit(0);
}
void f2(int sig)
{
    printf("Child process2 was killed by parent!\n");
    exit(0);
}