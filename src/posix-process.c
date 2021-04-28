#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EGENERAL -1

extern char **environ;

int main(int argc, char *argv[])
{
    pid_t pid;
    uid_t uid;
    char *home;

    uid = getuid();
    printf("uid             : %d\n", uid);
    
    printf("argc            : %d\n", argc);
    printf("argv[0]         : %s\n", argv[0]);

    home = getenv("HOME");
    printf("home            : %s\n", home);

    pid = fork();
    switch(pid)
    {
        case EGENERAL:
            printf("fork() error!\n");
            break;

        case 0:
            printf("child here!\n");
            execlp("ls", "ls", "-1", NULL);
            break;

        default:
            printf("parent here!\n");
            printf("child pid       : %d\n", pid);
    }

    pid = getpid();
    printf("pid             : %d\n", pid);

    exit(0);
}
