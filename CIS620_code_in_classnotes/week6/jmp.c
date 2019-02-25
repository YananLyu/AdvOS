#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

jmp_buf env;

void onintr() {

    printf("3-second violation!\n");
    longjmp(env, 1);  /* longjmp jump back to where setjmp() */
}

int main() {

    setjmp(env);
    signal(SIGALRM, onintr);
    alarm(3);
    for(;;)
        pause();

    return 0;
}