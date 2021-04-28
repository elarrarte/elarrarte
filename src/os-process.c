#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "../include/posix_exitcodes.h"
#include "../include/boolean.h"

bool_t loop = TRUE;

void sigusr1_handler(int sig, siginfo_t *siginfo, void *context)
{
    loop = FALSE;
}

#define SLEEP_TIME 1
int main()
{
    struct sigaction action_handler;

    // set action for SIGUSR1 signal
    memset(&action_handler, 0, sizeof(struct sigaction));
    action_handler.sa_sigaction = &sigusr1_handler;
    sigaction(SIGUSR1, &action_handler, NULL);

    while(loop)
    {
        sleep(SLEEP_TIME);
    }

    return (EXIT_SUCCESS);
}