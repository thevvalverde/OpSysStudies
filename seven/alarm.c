#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static int count = 0;

void timer_handler(int signum)
{
    printf("timer expired %d times\n", ++count);
    if (count==3) exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
    struct sigaction sa;
    struct itimerval timer;
    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);
    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;
    /* Start a virtual timer.
It counts down whenever this process is executing. */
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    /* Do busy work. */
    for (;;)
        ;
}