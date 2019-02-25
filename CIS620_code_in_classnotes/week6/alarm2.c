#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>  /* global go to */

void onintr() {

    printf("3-sec violation! \n");
    alarm(3);
}

int main() {

    signal(SIGALRM, onintr);
    alarm(3);  /* start timer, 3 seconds */
    for(;;)
        pause();

    return 0;
}
