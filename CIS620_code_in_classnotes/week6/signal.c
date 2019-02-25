#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cnt() {

    static int count = 0;  /* just initialize at first time */
    printf("Interrupt received %d\n", count++);
}

int main() {

    signal(SIGINT, cnt);
    printf("Begin ... \n");
    for( ; ; ) {   /* inifinite loop. equal to while(1) */

    }

    return 0;
}